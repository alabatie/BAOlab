;+ 
; NAME: 
;       RMK_CATALOGUE
;
; PURPOSE: 
;      Remake a data catalogue before calculating the correlation
;      function as a function of alpha. And create an alpha belonging
;      for each galaxy in the output catalogue. (see Labatie et al. 2012)
;
; CALLING SEQUENCE: 
;   RMK_CATALOGUE,data,name_out,name_out_alpha
;
; INPUTS: 
;   data  -- struct: catalogue data (with x=data.field1, y=data.field2
;            and z=data.field3)
;   name_out  -- string: name to write the output catalogue before
;                calculating the correlation function as a function of alpha
;   name_out_alpha  -- string: alpha belonging of each galaxy in the
;                      output catalogue 
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


pro rmk_catalogue,data,name_out,name_out_alpha
  ;restore parameters of script.pro
  restore,'BAOlab.sav'

  ;selection function as a function of alpha
  name=simu_folder+'DR7-Full_selection.fits'


  selection=readfits(name)
  ;r binning
  r=selection(*,0)
  nr=n_elements(r)
  dr=(r(nr-1)-r(0))/double(nr-1.0)
  rmin=r(0)-dr/2.0
  selection=selection(*,1:nalpha)

  ;number of galaxies in catalogue
  n=n_elements(data.field1)
  ;distance of each point in the catalogue
  d=sqrt(data.field1^2.+data.field2^2.+data.field3^2.)

  ;count number of points
  ;generate a random u in [0,1] for each galaxy
  ;the galaxy belongs to the catalogue for a value alpha if
  ;u<selection_alpha(r) with r the distance of the galaxy
  ;for each different interval [alpha,alpha'] where the galaxy belongs
  ;to the catalogue, create a new galaxy (see Labatie et al. 2012)
  n2=0L
  u=randomn(seed,n,/uniform)
  for i=0L,n-1 do begin 
     indr=floor((d(i)-rmin)/dr) 
     if indr lt 0 then indr=0 
     if indr ge nr then indr=nr-1 
     temp1=(selection(indr,0:(nalpha-2)) gt u(i))*(selection(indr,1:(nalpha-1)) le u(i)) ;catch transitions in->out of catalogue
     temp2=(selection(indr,0:(nalpha-2)) le u(i))*(selection(indr,1:(nalpha-1)) gt u(i)) ;catch transitions out->in of catalogue
     temp=reform(temp1+temp2,nalpha-1) 
     if selection(indr,0) gt u(i) then temp=[1.0,temp] else temp=[0.0,temp] 
     if total(temp) gt 0.1 then begin 
        ind=array_indices(temp,where (temp gt 0.1)) 
        l=ind 
        if abs(n_elements(l)/2.0-floor(n_elements(l)/2.0)) gt 0.1 then l=[l,nalpha] 
        n2+=round(n_elements(l)/2.0) 
     endif 
  endfor
     

  openw,unit,name_out,/get_lun
  openw,unit_alpha,name_out_alpha,/get_lun
  
  printf,unit,n2,3,1
  printf,unit,0,0,0
  printf,unit,0,0,0
  printf,unit,0,0,0
  printf,unit_alpha,n2,2,1
  printf,unit_alpha,0,0,0
  printf,unit_alpha,0,0,0

  ;write the output catalogue and alpha belonging file
  for i=0L,n-1 do begin 
     indr=floor((d(i)-rmin)/dr) 
     if indr lt 0 then indr=0 
     if indr ge nr then indr=nr-1 
     temp1=(selection(indr,0:(nalpha-2)) gt u(i))*(selection(indr,1:(nalpha-1)) le u(i)) ;catch transitions in->out of catalogue
     temp2=(selection(indr,0:(nalpha-2)) le u(i))*(selection(indr,1:(nalpha-1)) gt u(i)) ;catch transitions out->in of catalogue
     temp=reform(temp1+temp2,nalpha-1) 
     if selection(indr,0) gt u(i) then temp=[1.0,temp] else temp=[0.0,temp] 
     if total(temp) gt 0.1 then begin 
        ind=array_indices(temp,where (temp gt 0.1)) 
        l=ind 
        if abs(n_elements(l)/2.0-floor(n_elements(l)/2.0)) gt 0.1 then l=[l,nalpha] 
        n2=round(n_elements(l)/2.0) 
        for j=0L,n2-1 do begin 
           printf,unit,data.field1(i),data.field2(i),data.field3(i) 
           printf,unit_alpha,alpha_min+d_alpha*l(2*j),alpha_min+d_alpha*l(2*j+1) 
        endfor 
     endif 
  endfor

  free_lun,unit
  free_lun,unit_alpha


end
