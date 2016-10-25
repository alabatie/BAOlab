;+ 
; NAME: 
;       ICOSMOPS_GRID
;
; PURPOSE: 
;      Generate binned correlation function with the iCosmo package on a Omegam h^2
;      and alpha grid. Can be used to create the models
;      input_files/model/xi_lrg_BAO,
;      input_files/model/xi_lrg_nowiggles and
;      input_files/model/xi_lrg_no baryons
;
; CALLING SEQUENCE: 
;   ICOSMOPS_GRID
;
; INPUTS: 
;   none.
;
; OPTIONAL INPUT PARAMETERS: 
;   none.
;
; KEYED OUTPUT: 
;   none.
;
; OUTPUTS: 
;   none.
;
; MODIFICATION HISTORY: 
;    1-Nov-2012 A.Labatie
;-

;======================================


function bias_function,r
  ;scale dependent galaxy bias 
  if r lt 55.5 then return,0.88+(r/20.0)^2.9/160.0 ;fitting formula found for Las Damas catalogues
  return,1.0
end

pro icosmoPS_grid

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;                    Parameters to set                    ;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  ;Omega_m h^2 parameter table
  omegamh2_min=0.08 
  omegamh2_max=0.18
  
  omegamh2_table=double(findgen(101)/100.0*(omegamh2_max-omegamh2_min)+omegamh2_min) 
  n_line=n_elements(omegamh2_table)

  ;alpha parameter table 
  alpha_min=0.8 
  alpha_max=1.2
  alpha_table=double(findgen(101)/100.0*(alpha_max-alpha_min)+alpha_min) 
  n_line2=n_elements(alpha_table)

  ;value of Omega_b h^2, h, w0, n, tau, sigma8
  omegabh2=0.02227d
  h=0.7d
  w0=-1.0d  
  n=0.966d
  tau=0.085d
  sigma8=0.81d
  
  ;linear bias
  linear_bias=2.5

  ;catalogue redshift
  z_cat=0.3

  ;non linear BAO smearing size in Mpc/h for true cosmo
  nl_radius=9.5d    

  ;binning of output correlation function
  x=(indgen(18)+0.5)*10.0+20.0
  nx=n_elements(x)
  dx=x(1)-x(0)

  ;Folders to work in 
  CD, Current=name_idl_folder ;Get current folder
  name_saving_folder=name_idl_folder+'/../output_files/ps_transform/'
  name_program_folder=name_idl_folder+'/../bin/'

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;     power spectrums as a function of \Omega_m h^2    ;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  for i=0L,n_line-1 do begin 
     print,i+1,'/',n_line 
     omega_m=omegamh2_table(i)/h^2.0 
     omega_b=omegabh2/h^2.0 
     fid_BAO=set_fiducial(cosmo_in={h:h,omega_b:omega_b,omega_m:omega_m,omega_l:1.0-omega_m,w0:w0,n:n,tau:tau,sigma8:sigma8},calc_in={fit_nl:2,fit_tk:1,n_k:700,k_ran:[0.0001,1000],nz_fn:100},expt_in={sv1_n_zbin:2,sv1_zerror:0.02d}) 
     fid_nowiggles=set_fiducial(cosmo_in={h:h,omega_b:omega_b,omega_m:omega_m,omega_l:1.0-omega_m,w0:w0,n:n,tau:tau,sigma8:sigma8},calc_in={fit_nl:2,fit_tk:0,n_k:700,k_ran:[0.0001,1000],nz_fn:100},expt_in={sv1_n_zbin:2,sv1_zerror:0.02d}) 
     fid_nobaryons=set_fiducial(cosmo_in={h:h,omega_b:0.00001d,omega_m:omega_m,omega_l:1.0-omega_m,w0:w0,n:n,tau:tau,sigma8:sigma8},calc_in={fit_nl:2,fit_tk:1,n_k:700,k_ran:[0.0001,1000],nz_fn:100},expt_in={sv1_n_zbin:2,sv1_zerror:0.02d}) 

     cosmo_BAO=mk_cosmo(fid_BAO) 
     cosmo_nowiggles=mk_cosmo(fid_nowiggles) 
     cosmo_nobaryons=mk_cosmo(fid_nobaryons) 

     pk_BAO=get_pk(cosmo_BAO, z=z_cat) 
     pk_BAO_l=pk_BAO.pk_l 
     pk_nowiggles=get_pk(cosmo_nowiggles, z=z_cat)
     pk_nowiggles_l=pk_nowiggles.pk_l 
     pk_nowiggles_nl=pk_nowiggles.pk 
     pk_nobaryons=get_pk(cosmo_nobaryons, z=z_cat)
     pk_nobaryons_nl=pk_nobaryons.pk


     ;Save pk's
     n_pk=N_ELEMENTS(pk_BAO_l) 
     nameout_BAO=name_saving_folder+'input_pk_BAO/input_pk_lrg'+STRTRIM(i,2)+ '.dat' 
     nameout_nowiggles=name_saving_folder+'input_pk_nowiggles/input_pk_lrg'+STRTRIM(i,2)+ '.dat' 
     nameout_nobaryons=name_saving_folder+'input_pk_nobaryons/input_pk_lrg'+STRTRIM(i,2)+ '.dat' 

     openw, unit, nameout_BAO, /get_lun 	
     for j=0L, n_pk-1 do begin 
        k=pk_BAO.k(j) 
        kernel=exp(-0.5*(k*nl_radius)^2.0) ;kernel for the non linear degradation of BAO feature
        correction_nl=pk_nowiggles_nl(j)/pk_nowiggles_l(j)  ;non linear scale invariant effect
        printf, unit, k, (pk_nowiggles_l(j)+(pk_BAO_l(j)- pk_nowiggles_l(j))*kernel)*correction_nl  
     endfor 
     free_lun, unit 

     openw, unit, nameout_nowiggles, /get_lun 	
     for j=0L, n_pk-1 do begin 
        printf, unit, pk_nowiggles.k(j), pk_nowiggles_nl(j) 
     endfor 
     free_lun, unit 

     openw, unit, nameout_nobaryons, /get_lun 	
     for j=0L, n_pk-1 do begin 
        printf, unit, pk_nobaryons.k(j), pk_nobaryons_nl(j) 
     endfor 
     free_lun, unit 

  endfor

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;               convert power spectrums to                  ;;;;;
    ;;;;;     correlation functions by Hankel transform (FFTLog)    ;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  cd,name_program_folder
  for i=0L,n_line-1 do begin
     spawn,'./ps_transform -v -z 200 '+name_saving_folder+'input_pk_BAO/input_pk_lrg'+STRTRIM(i,2)+'.dat'
     spawn,'cp '+name_saving_folder+'output_xi.dat '+name_saving_folder+'output_xi_BAO/output_xi_lrg'+STRTRIM(i,2)+'.dat'
     spawn,'./ps_transform -v -z 200 '+name_saving_folder+'input_pk_nowiggles/input_pk_lrg'+STRTRIM(i,2)+'.dat'
     spawn,'cp '+name_saving_folder+'output_xi.dat '+name_saving_folder+'output_xi_nowiggles/output_xi_lrg'+STRTRIM(i,2)+'.dat'
     spawn,'./ps_transform -v -z 200 '+name_saving_folder+'input_pk_nobaryons/input_pk_lrg'+STRTRIM(i,2)+'.dat'
     spawn,'cp '+name_saving_folder+'output_xi.dat '+name_saving_folder+'output_xi_nobaryons/output_xi_lrg'+STRTRIM(i,2)+'.dat'
  endfor
  cd,name_saving_folder

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;                   add dilation parameter                  ;;;;;
    ;;;;;                   and scale-dependent bias                ;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  for i=0L,n_line-1 do begin 
     print,i+1,'/',n_line 
     name_BAO='output_xi_BAO/output_xi_lrg'+STRTRIM(i,2)+'.dat' 
     name_nowiggles='output_xi_nowiggles/output_xi_lrg'+STRTRIM(i,2)+'.dat' 
     name_nobaryons='output_xi_nobaryons/output_xi_lrg'+STRTRIM(i,2)+'.dat' 
     if i eq 0 then my_t=ascii_template(name_BAO) 
     data_BAO=read_ascii(name_BAO,template=my_t) 
     data_nowiggles=read_ascii(name_nowiggles,template=my_t) 
     data_nobaryons=read_ascii(name_nobaryons,template=my_t) 
     nr=n_elements(data_BAO.field1) 
     for k=0,n_line2-1 do begin 
        alpha=alpha_table(k) 
        nameout_BAO='output_xi_BAO/output_xi_lrg'+STRTRIM(i,2)+'-'+STRTRIM(k,2)+'.dat' 
        nameout_nowiggles='output_xi_nowiggles/output_xi_lrg'+STRTRIM(i,2)+'-'+STRTRIM(k,2)+'.dat' 
        nameout_nobaryons='output_xi_nobaryons/output_xi_lrg'+STRTRIM(i,2)+'-'+STRTRIM(k,2)+'.dat' 
        openw,unit,nameout_BAO,/get_lun 
        for l=0L,nr-1 do begin 
           bias=bias_function(data_BAO.field1(l))*linear_bias^2.0
           printf,unit,data_BAO.field1(l)/alpha,data_BAO.field2(l)*bias
        endfor
        free_lun,unit 
        openw,unit,nameout_nowiggles,/get_lun 
        for l=0L,nr-1 do begin
           bias=bias_function(data_nowiggles.field1(l))*linear_bias^2.0
           printf,unit,data_nowiggles.field1(l)/alpha,data_nowiggles.field2(l)*bias
        endfor
        free_lun,unit 
        openw,unit,nameout_nobaryons,/get_lun 
        for l=0L,nr-1 do begin
           bias=bias_function(data_nobaryons.field1(l))*linear_bias^2.0
           printf,unit,data_nobaryons.field1(l)/alpha,data_nobaryons.field2(l)*bias
        endfor
        free_lun,unit 

     endfor 
  endfor



     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
     ;;;;;;;;;;;;;;;;;;;;  Binning of the correlation functions ;;;;;;;;;;;;;;;;;;;;;;
     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  nameout_BAO='output_xi_BAO/output_xi_lrg.fits' 
  nameout_nowiggles='output_xi_nowiggles/output_xi_lrg.fits'
  nameout_nobaryons='output_xi_nobaryons/output_xi_lrg.fits'

  ;arrays of correlations
  w_BAO=make_array(n_line,n_line2,nx)   
  w_nowiggles=make_array(n_line,n_line2,nx)
  w_nobaryons=make_array(n_line,n_line2,nx)

  for i=0L,n_line-1 do begin 
     print,i+1,'/',n_line
     for k=0,n_line2-1 do begin 
        name_BAO='output_xi_BAO/output_xi_lrg'+STRTRIM(i,2)+'-'+STRTRIM(k,2)+'.dat' 
        name_nowiggles='output_xi_nowiggles/output_xi_lrg'+STRTRIM(i,2)+'-'+STRTRIM(k,2)+'.dat' 
        name_nobaryons='output_xi_nobaryons/output_xi_lrg'+STRTRIM(i,2)+'-'+STRTRIM(k,2)+'.dat' 

        if (i eq 0) and (k eq 0) then my_t=ascii_template(name_BAO) 
        data_BAO=read_ascii(name_BAO,template=my_t)
        data_nowiggles=read_ascii(name_nowiggles,template=my_t)
        data_nobaryons=read_ascii(name_nobaryons,template=my_t)
        m_BAO=make_array(nx) 
        m_nowiggles=make_array(nx) 
        m_nobaryons=make_array(nx) 
        ind=0L 
        for l=0L,nx-1 do begin 
           xinf=x(l)-dx/2.0
           xsup=x(l)+dx/2.0 
           while 0.5*(data_BAO.field1(ind)+data_BAO.field1(ind+1)) lt xinf do ind++ 
           if 0.5*(data_BAO.field1(ind)+data_BAO.field1(ind+1)) gt xsup then begin 
              m_BAO(l)=data_BAO.field2(ind) 
              m_nowiggles(l)=data_nowiggles.field2(ind) 
              m_nobaryons(l)=data_nobaryons.field2(ind) 
           endif else begin 
              coeff=0.0d
              current_x=xinf 
              next_x=0.5*(data_BAO.field1(ind)+data_BAO.field1(ind+1)) 
              while next_x lt xsup do begin 
                 coeff_temp=next_x^3.0-current_x^3.0 
                 coeff+=coeff_temp 
                 m_BAO(l)+=coeff_temp*data_BAO.field2(ind) 
                 m_nowiggles(l)+=coeff_temp*data_nowiggles.field2(ind) 
                 m_nobaryons(l)+=coeff_temp*data_nobaryons.field2(ind) 
                 ind++ 
                 current_x=next_x 
                 next_x=0.5*(data_BAO.field1(ind)+data_BAO.field1(ind+1)) 
              endwhile
              next_x=xsup 
              coeff_temp=next_x^3.0-current_x^3.0 
              coeff+=coeff_temp 
              m_BAO(l)+=coeff_temp*data_BAO.field2(ind) 
              m_BAO(l)/=coeff 
              m_nowiggles(l)+=coeff_temp*data_nowiggles.field2(ind) 
              m_nowiggles(l)/=coeff 
              m_nobaryons(l)+=coeff_temp*data_nobaryons.field2(ind) 
              m_nobaryons(l)/=coeff 
           endelse 
        endfor 
        w_BAO(i,k,*)=m_BAO 
        w_nowiggles(i,k,*)=m_nowiggles
        w_nobaryons(i,k,*)=m_nobaryons

        ;erase temp xi
        spawn,'rm '+name_BAO
        spawn,'rm '+name_nowiggles
        spawn,'rm '+name_nobaryons
     endfor 
  endfor

  writefits,nameout_BAO,w_BAO
  writefits,nameout_nowiggles,w_nowiggles
  writefits,nameout_nobaryons,w_nobaryons

end
