;+ 
; NAME: 
;       SIGN_BAO_DETECTION
;
; PURPOSE: 
;      Compute the significance of the BAO detection, either for the
;      Delta chi^2 statistic or for the generalized likelihood ratio
;      statistic, and either for a constant covariance matrix or
;      model-dependent covariance matrix
;
; CALLING SEQUENCE: 
;   SIGN_BAO_DETECTION,lratio=lratio,varcov=varcov
;
; INPUTS: 
;   none.
;
; OPTIONAL INPUT PARAMETERS: 
;   lratio  -- int: set to 1 for the generalized likelihood statistic,
;              and otherwise corresponds to the Delta chi^2 statistic
;   varcov  -- int: set to 1 for model-dependent covariance matrix,
;              and otherwise corresponds to constant covariance matrix
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

pro sign_bao_detection,lratio=lratio,varcov=varcov
  ;restore parameters of script.pro
  restore,'BAOlab.sav'

  ;if not provided, statistic is Delta chi^2
  if not keyword_set(lratio) then lratio=0
  ;if not provided, cov matrix is constant
  if not keyword_set(varcov) then varcov=0

  print,'Significance of BAO detection'
  print,'Use lratio?',lratio
  print,'Use varcov?',varcov

  if lratio eq 1 then begin 
     prefix=idl_folder+'../output_files/lratio/lratio_' 
  endif else begin
     prefix=idl_folder+'../output_files/delta_chi2/Dchi2_' 
  endelse
  if varcov eq 1 then prefix=prefix+'varcov_'

  name0=prefix+'h0.fits'
  k0=readfits(name0)

  name1=prefix+'h1.fits'
  k1=readfits(name1)

  ;deal with non finite histogram values
  for i=0L,n_elements(k0)-1 do begin 
     if finite(k0(i)) eq 0 then k0(i)=0 
  endfor 
  k0+=min(k0)*(k0 eq 0)
  
  ;size of binning for significance (adapted to full H0 histogram)
  binsize=0.2
  h0_all=histogram(k0,binsize=binsize)
  n0=n_elements(h0_all)
  x0=(indgen(n0)+0.5)*binsize+min(k0)
  dx0=x0(1)-x0(0)
  x0min=min(x0)

  ;number of models in the H_0 hypothesis (see Labatie et al. 2012)
  ;=number of histogram values / number of simulations for each model
  ngrid=n_elements(k0)/nsimu_bao_detection
  
  ;histogram for each model
  h0grid=make_array(ngrid,n0)
  for i=0L,ngrid-1 do begin 
     h0grid(i,*)=histogram(k0((i*nsimu_bao_detection):((i+1)*nsimu_bao_detection-1)),min=min(x0)-dx0/2.0,max=max(x0)+dx0/2.0,nbin=n0) 
  endfor
  for i=0L,ngrid-1 do h0grid(i,*)/=total(h0grid(i,*))
  
  ;cumulative function for each model
  f0grid=make_array(ngrid,n0)
  for i=0L,ngrid-1 do begin 
     f0grid(i,0)=double(h0grid(i,0))/2.0 
     for j=1L,n0-1 do f0grid(i,j)=f0grid(i,j-1)+double(h0grid(i,j)+h0grid(i,j-1))/2.0 
  endfor

  ;cumulative function with minimum of each model
  ;corresponds to the conservative approach in order
  ;to reject all H0 models simultaneously (see Labatie et al. 2012)
  f0=make_array(n0)
  for j=0L,n0-1 do f0(j)=min(f0grid(*,j)) 

  ;compute corresponding histogram
  h0=make_array(n0)
  h0(0)=f0(0)
  for j=1L,n0-1 do h0(j)=f0(j)-f0(j-1) 


  ;make H1 histogram
  h1=histogram(k1,binsize=binsize)
  n1=n_elements(h1)
  x1=(indgen(n1)+0.5)*binsize+min(k1)
  h1/=total(h1)

  pvalue=make_array(n1)
  
  ;Create p value table
  for i=0L,n1-1 do begin 
     x=x1(i) 
     ind=round((x-x0min)/dx0) 
     if ind lt 0 then pvalue(i)=1.0 
     if ind ge n0 then pvalue(i)=1.0/double(nsimu_bao_detection) 
     if (ind eq 0) or (ind eq n0-1) then pvalue(i)=1.0-f0(ind) 
     if (ind gt 0) and (ind lt n0-1) then begin 
        rest=(x-x0min)/dx0-round((x-x0min)/dx0) 
        if rest le 0 then pvalue(i)=1.0-abs(rest)*f0(ind-1)-(1-abs(rest))*f0(ind) 
        if rest ge 0 then pvalue(i)=1.0-abs(rest)*f0(ind+1)-(1-abs(rest))*f0(ind) 
     endif 
     if pvalue(i) le 1.0/double(nsimu_bao_detection)  then pvalue(i)=1.0/double(nsimu_bao_detection) 
  endfor
  ;p-value table times H1 histogram gives mean p-value under H1
  print,'mean p=', total(pvalue*h1)
 

  ;Find mean sigma
  ;Create a table to convert a number of sigmas to a p-value
  g=randomn(seed,10000000)
  s2p=make_array(1000)
  s=findgen(1000)/1000.0*6.0
  for i=0, 999 do begin 
     if i/10 eq i/10.0 then print,i/10,'%' 
     s2p(i)=mean(abs(g) gt s(i)) 
  endfor

  ;Create sigma table
  sigma=make_array(n1)
  for i=0L,n1-1 do begin 
     p=pvalue(i) 
     if p lt 0 then p=0 
     j=0L 
     while s2p(j) gt p do j++ 
     sigma(i)=s(j) 
  endfor

  ;Find limit sigma that can be evaluated with restricted number of simulations
  p=1/double(nsimu_bao_detection)
  j=0L
  while s2p(j) gt p do j++
  t=s(j) 

  ;sigma table times H1 histogram gives mean sigma under H1
  print,'mean sigma=', total(sigma*h1)

  ;mean sigma when considering only simu below the limit sigma 
  print,'mean sigma for simu under threshold corresponding to ',STRTRIM(nsimu_bao_detection,2),' simulations=',total(h1*(sigma*(sigma lt t)))/total(h1*(sigma lt t))



end
