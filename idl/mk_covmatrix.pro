;+ 
; NAME: 
;       MK_COVMATRIX
;
; PURPOSE: 
;      Compute the covariance matrix corresponding to a set of
;      different simulations and with different values of alpha and Omega_m h^2 
;
; CALLING SEQUENCE: 
;   MK_COVMATRIX,name_prefix,name_cov_out
;
; INPUTS: 
;   name_prefix  -- string: prefix name to open different simulations and
;                   with different values of alpha and Omega_m h^2
;   name_cov_out -- string: name to write the model-dependent
;                   covariance matrix dependening on alpha and
;                   Omega_m h^2
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

pro mk_covmatrix,name_prefix,name_cov_out
  restore,'BAOlab.sav'
  ;model-dependent cov matrix
  cov=make_array(no1,nalpha,nrout,nrout) 


  for i=0,no1-1 do begin 
     ;landy-szalay esimtor for each lognormal simu and each alpha value
     ls2=make_array(nrout,nsimu_lognormal,nalpha)

     ;bin ls estimator
     for j=0L,nsimu_lognormal-1 do begin
        print,i,j
        ;name for simu j and Omega_m h^2 value i
        name=name_prefix+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'.fits'
        k=readfits(name)

        for a=0L,nalpha-1 do begin 
           r=k(*,a,0)
           dd=k(*,a,1)
           rr=k(*,a,2)
           dr=k(*,a,3)
           ;rebinned pair counting quantities
           dd2=make_array(nrout)
           rr2=make_array(nrout)
           dr2=make_array(nrout)
         
           for l=0L,nrout-1 do begin
              rinf=rout(l)-drout/2.0
              rsup=rout(l)+drout/2.0
              ind=0L
              while 0.5*(r(ind)+r(ind+1)) lt rinf do ind++ 
              coeff=(0.5*(r(ind)+r(ind+1))-rinf)/(r(ind+1)-r(ind))
              dd2(l)+=coeff*dd(ind)
              rr2(l)+=coeff*rr(ind)
              dr2(l)+=coeff*dr(ind)
              
              ind++
              next_r=0.5*(r(ind)+r(ind+1)) 
              while next_r lt rsup do begin 
                    dd2(l)+=dd(ind)
                    rr2(l)+=rr(ind)
                    dr2(l)+=dr(ind)
                    ind++ 
                    next_r=0.5*(r(ind)+r(ind+1)) 
              endwhile
              coeff=(0.5*(r(ind)+r(ind+1))-rsup)/(r(ind+1)-r(ind))
              dd2(l)+=(1.0-coeff)*dd(ind)
              rr2(l)+=(1.0-coeff)*rr(ind)
              dr2(l)+=(1.0-coeff)*dr(ind)
           endfor 
           ls2(*,j,a)=(dd2-2*dr2)/rr2+1.0
        endfor
     endfor

        
     ;mean ls
     lsmean=make_array(nrout,nalpha)
     for l=0L,nrout-1 do begin
        for a=0L,nalpha-1 do lsmean(l,a)=mean(ls2(l,*,a))
     endfor

     ;make covariance
     for k=0L,nrout-1 do begin
        for l=0L,nrout-1 do begin
           for j=0L,nsimu_lognormal-1 do begin
              for a=0L,nalpha-1 do begin
                 cov(i,a,k,l)+=1.0/double(nsimu_lognormal-1.0)*(ls2(k,j,a)-lsmean(k,a))*(ls2(l,j,a)-lsmean(l,a))
              endfor
           endfor
        endfor
     endfor

  endfor

  ;Rebin
  cov_rebin=make_array(no,nalpha,nrout,nrout)
  for i=0,no-1 do begin 
     for j=0,nalpha-1 do begin
        ind=floor(i*d_o/d_o1)
        rest=i*d_o/d_o1-ind
        if rest eq 0 then begin
           cov_rebin(i,j,*,*)=cov(ind,j,*,*)
        endif else begin
           cov_rebin(i,j,*,*)=(1-rest)*cov(ind,j,*,*)+rest*cov(ind+1,j,*,*)
        endelse
     endfor
  endfor

  writefits,name_cov_out,cov_rebin
end
