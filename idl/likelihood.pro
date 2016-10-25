;+ 
; NAME: 
;       LIKELIHOOD
;
; PURPOSE: 
;      Compute the likelihood of the SDSS DR7-Full correlation
;      function given the model correlation input_files/model/xi_lrg_BAO, the
;      covariance matrix in input_files/simu/, both for varying and
;      constant covariance matrices. Plot the different likelihood
;      contours and also compute the confidence intervals. 
;
; CALLING SEQUENCE: 
;   LIKELIHOOD,name_xi
;
; INPUTS: 
;   name_xi  -- string: name to open the data correlation function
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


pro likelihood,name_xi
  restore,'BAOlab.sav'

  ;model correlation functions
  model_all=readfits(simu_folder+'../model/xi_lrg_BAO.fits')
  
  ;inverse of model-dependent covariance matrix
  iC_all=readfits(simu_folder+'inverse_cov_all.fits') 
  ind=no/2*nalpha+nalpha/2

  ;inverse covariance matrix at center of the grid (used for constant cov matrix)
  iC0=reform(iC_all(*,*,ind),nrout,nrout)

  ;log of determinant of model-dependent cov matrix
  log_determC_all=readfits(simu_folder+'log_determ_cov_all.fits') 
  determC_all=exp(log_determC_all)

  B_table=dindgen(500)/100.0+4.0
  nB=n_elements(B_table)
  B_model=2.5^2.0

  xi=readfits(name_xi) 

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;;;;; constant cov matrix ;;;;;
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  print,'varying cov matrix'
  l0=make_array(no,nalpha,nB)
  for i=0,no-1 do begin 
     print,i,'/',no-1
     for j=0,nalpha-1 do begin 
        model=double(reform(model_all(i,j,*),nrout)) 
        ind=i*nalpha+j 
        iC=reform(iC_all(*,*,ind),nrout,nrout)
        c_det=determC_all(ind)
        for k=0,nB-1 do begin 
           B=double(B_table(k)/B_model) 
           ;likelihood formula for model-dependent cov matrix
           l0(i,j,k)=1.0/(sqrt(c_det)*B^nrout)*exp(-0.5/B^2.0*(xi-B*model)##(iC##(xi-B*model))) 
        endfor 
     endfor 
  endfor

  ;marginalize over B
  l0=l0/total(l0)
  l=make_array(no,nalpha)
  for i=0,no-1 do begin 
     for j=0,nalpha-1 do begin 
        l(i,j)=total(double(l0(i,j,*))) 
     endfor 
  endfor

  ;plot 2D posterior
  loadct,39
  window,0
  str="alpha"
  str2="Omega_m h^2"
  logl=2*alog(l)
  fac= max(logl,/nan) 
  contour, logl,o_table,alpha_table,levels=fac+[-28.74,-19.32,-11.81,-6.16,-2.29,0.0],/FILL,xtitle=str2,ytitle=str,background=255,color=0
  contour, logl, o_table,alpha_table,levels=fac+[-28.74,-19.32,-11.81,-6.16,-2.29],c_annotation=['5','4','3','2','1'],/OVERPLOT,c_charsize=1.5,c_charthick=2

  ;confidence intervals
  ;first get 1D posteriors lalpha and lo
  lo=make_array(no)
  lalpha=make_array(nalpha)
  for i=0,no-1 do begin 
     for j=0,nalpha-1 do begin 
        lo(i)+=l(i,j) 
        lalpha(j)+=l(i,j) 
     endfor 
  endfor

   ;then compute max of the posterior and extend interval around maximum until
   ;it contains 0.6827% of the posterior
   ind=array_indices(lalpha,where(lalpha eq max(lalpha)))
   i=0L
   temp=lalpha(ind)
   while temp le 0.6827 do begin 
      i+=1 
      temp+=lalpha(ind-i)+lalpha(ind+i) 
   end
   print,'alpha: ',alpha_table(ind),'+-',alpha_table(ind+i)-alpha_table(ind)
 
   ind=array_indices(lo,where(lo eq max(lo)))
   i=0L
   temp=lo(ind)
   while temp le 0.6827 do begin 
      i+=1 
      temp+=lo(ind-i)+lo(ind+i) 
   end
   print,'omega_m h^2: ',o_table(ind),'+-',o_table(ind+i)-o_table(ind)


  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;;;;; constant cov matrix ;;;;;
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  print,'constant cov matrix'
  l0=make_array(no,nalpha,nB)
  for i=0,no-1 do begin 
     print,i,'/',no-1
     for j=0,nalpha-1 do begin 
        model=double(reform(model_all(i,j,*),nrout)) 
        for k=0,nB-1 do begin 
           B=double(B_table(k)/B_model) 
           l0(i,j,k)=exp(-0.5*(xi-B*model)##(iC0##(xi-B*model))) 
        endfor 
     endfor 
  endfor

  l0=l0/total(l0)
  l=make_array(no,nalpha)
  for i=0,no-1 do begin 
     for j=0,nalpha-1 do begin 
        l(i,j)=total(double(l0(i,j,*))) 
     endfor 
  endfor

  window,1
  logl=2*alog(l)
  fac= max(logl,/nan) 
  contour, logl,o_table,alpha_table,levels=fac+[-28.74,-19.32,-11.81,-6.16,-1.0,0.0],/FILL,xtitle=str2,ytitle=str,background=255,color=0
  contour, logl, o_table,alpha_table,levels=fac+[-28.74,-19.32,-11.81,-6.16,-1.0],c_annotation=['5','4','3','2','1'],/OVERPLOT,c_charsize=1.5,c_charthick=2


  ;confidence intervals
  lo=make_array(no)
  lalpha=make_array(nalpha)
  for i=0,no-1 do begin 
     for j=0,nalpha-1 do begin 
        lo(i)+=l(i,j) 
        lalpha(j)+=l(i,j) 
     endfor 
  endfor

   ind=array_indices(lalpha,where(lalpha eq max(lalpha)))
   i=0L
   temp=lalpha(ind)
   while temp le 0.6827 do begin 
      i+=1 
      temp+=lalpha(ind-i)+lalpha(ind+i) 
   end
   print,'alpha: ',alpha_table(ind),'+-',alpha_table(ind+i)-alpha_table(ind)
 
   ind=array_indices(lo,where(lo eq max(lo)))
   i=0L
   temp=lo(ind)
   while temp le 0.6827 do begin 
      i+=1 
      temp+=lo(ind-i)+lo(ind+i) 
   end
   print,'omega_m h^2: ',o_table(ind),'+-',o_table(ind+i)-o_table(ind)

end


