;+ 
; NAME: 
;       MK_DENS_MAX
;
; PURPOSE: 
;      Compute the maximum density corresponding to a survey density
;      and an alpha grid in order to generate lognormal simulations
;      for different values of alpha simultaneously (see Labatie et
;      al. 2012). Also compute the selection function which depends on
;      alpha.
;
; CALLING SEQUENCE: 
;   MK_DENS_MAX
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


pro mk_dens_max
  ;catalogue mean density
  name='../input_files/simu/DR7-Full_dens.dat'
  my_t=ascii_template(name)
  dens0=read_ascii(name,template=my_t)

  ;r binning for catalogue mean density
  r=dens0.field1
  nr=n_elements(r)
  dr=(max(r)-min(r))/double(nr-1.0)
  rmin=min(r)-dr/2.0
  rmax=max(r)+dr/2.0

  dens=dens0.field2

  ;restore parameters of script.pro
  restore,'BAOlab.sav'

  ;new r binning for maximum density
  nr2=round((alpha_max*rmax-alpha_min*rmin)/(rmax-rmin)*nr)
  r2min=alpha_min*rmin
  r2=(findgen(nr2)+0.5)*dr+r2min
  r2max=nr2*dr+r2min

  ;density for each value of alpha
  dens_alpha=make_array(nr2,nalpha)
  for i=0L,nalpha-1 do begin 
     for j=0L,nr2-1 do begin 
        r_alpha=r2(j)/alpha_table(i) 
        ind=floor((r_alpha-rmin)/dr) 
        rest=(r_alpha-rmin)/dr-ind 
        if (ind lt nr-1) and (ind ge 0) then dens_alpha(j,i)=(rest*dens(ind+1)+(1-rest)*dens(ind))/alpha_table(i)^3.0 
     endfor 
  endfor



  ;create max_density
  dens_max=make_array(nr2)
  for j=0L,nr2-1 do dens_max(j)=max(dens_alpha(j,*)) 
 
  ;save max density
  filename='../input_files/simu/DR7-Full_dens_max.dat'
  openw, unit, filename, /get_lun 	
  for j=0L, nr2-1 do printf, unit,r2(j),dens_max(j)  

  ;selection function= (alpha density) / (max alpha density)
  selection=make_array(nr2,nalpha)
  for j=0L,nr2-1 do selection(j,*)=dens_alpha(j,*)/dens_max(j) 
  filename='../input_files/simu/DR7-Full_selection.fits'
  writefits,filename,[[r2],[selection]]

end
