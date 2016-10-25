;+ 
; NAME: 
;       ICOSMOPS
;
; PURPOSE: 
;      Generate a power spectrum with the iCosmo package given the
;      value of Omega_m h^2 and write it to a file
;
; CALLING SEQUENCE: 
;   ICOSMOPS, filename=filename, omegamh2=omegamh2
;
; INPUTS: 
;   none.
;
; OPTIONAL INPUT PARAMETERS: 
;   filename  -- string: name of the file to write the power spectrum
;   omegamh2  -- float:  value of Omega_m h^2
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

pro icosmoPS,filename=filename,omegamh2=omegamh2

    omegabh2=0.02227d
    if not keyword_set(omegamh2) then omegamh2=0.13
    fid_BAO=set_fiducial(cosmo_in={h:0.7d,omega_b:omegabh2/0.7^2.0,omega_m:omegamh2/0.7^2.0,omega_l:0.73d,w0:-1.0d,n:0.966d,tau:0.085d,sigma8:0.81d},calc_in={fit_nl:2,fit_tk:1,n_k:700,k_ran:[0.0001,1000]},expt_in={sv1_n_zbin:2,sv1_zerror:0.02d})
    fid_nowiggles=set_fiducial(cosmo_in={h:0.7d,omega_b:omegabh2/0.7^2.0,omega_m:omegamh2/0.7^2.0,omega_l:0.73d,w0:-1.0d,n:0.966d,tau:0.085d,sigma8:0.81d},calc_in={fit_nl:2,fit_tk:0,n_k:700,k_ran:[0.0001,1000]},expt_in={sv1_n_zbin:2,sv1_zerror:0.02d})


   cosmo_BAO=mk_cosmo(fid_BAO)
   cosmo_nowiggles=mk_cosmo(fid_nowiggles)
   z_cat=0.3


   pk_BAO=get_pk(cosmo_BAO, z=z_cat) ;get pk at given redshift
   pk_BAO_l=pk_BAO.pk_l 
   pk_nowiggles=get_pk(cosmo_nowiggles, z=z_cat)
   pk_nowiggles_l=pk_nowiggles.pk_l 
   pk_nowiggles_nl=pk_nowiggles.pk 

   ;Save pk
   n_pk=N_ELEMENTS(pk_BAO_l) 
   if not keyword_set(filename) then filename='../output_files/ps_transform/input_Pk_wmap7.dat'

   nl_radius=9.5d

   openw, unit, filename, /get_lun 	
   for j=0L, n_pk-1 do begin 
      k=pk_BAO.k(j) 
      kernel=exp(-0.5*(k*nl_radius)^2.0) ;kernel for the non linear degradation of BAO feature
      correction_nl=pk_nowiggles_nl(j)/pk_nowiggles_l(j) ;non linear scale invariant effect
      printf, unit, k, (pk_nowiggles_l(j)+(pk_BAO_l(j)- pk_nowiggles_l(j))*kernel)*correction_nl  
   endfor 
   free_lun, unit  
end

