;+ 
; NAME: 
;       TRANSFORM_COVMATRIX
;
; PURPOSE: 
;      Compute different transforms of a model-dependent input
;      covariance matrix (inverse, square root and
;      determinants). These transforms are used in the bao detection
;      programs delta_chi2 and lratio
;
; CALLING SEQUENCE: 
;   TRANSFORM_COVMATRIX,name_cov
;
; INPUTS: 
;   name_cov  -- string: name to open input model-dependent covariance matrix
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


pro transform_covmatrix,name_cov
  ;restore parameters of script.pro
  restore,'BAOlab.sav'

  ;read model-dependent covariance matrix
  c_all=readfits(name_cov)

  c_all=double(c_all)

  ;square root and inverse model-dependent covariance matrix
  ;note the model-dependence is the 3rd dimension 
  ;this is required by the programs delta_chi2 and lratio for BAO detection
  sC_all=double(make_array(nrout,nrout,no*nalpha))
  iC_all=double(make_array(nrout,nrout,no*nalpha))

  ;compute square root cov matrix
  for i=0L,no-1 do begin 
     for j=0,nalpha-1 do begin 
        ind=i*nalpha+j 
        A=reform(c_all(i,j,*,*),nrout,nrout) 
        TRIRED, A, D, E 
        TRIQL, D, E, A 
        sC_all(*,*,ind)=A#diag_matrix(sqrt(D))#transpose(A) 
     endfor 
  endfor

  ;compute inverse cov matrix
  for i=0L,no-1 do begin 
     for j=0,nalpha-1 do begin 
        ind=i*nalpha+j 
        A=reform(c_all(i,j,*,*),nrout,nrout) 
        iC_all(*,*,ind)=invert(A,/double) 
     endfor 
  endfor


  name_sC_all=simu_folder+'sqrt_cov_all.fits'
  writefits,name_sC_all,float(sC_all)
  name_iC_all=simu_folder+'inverse_cov_all.fits'
  writefits,name_iC_all,float(iC_all)

  ;compute determinant. divide by matrix in the middle of the grid 
  ;to avoid precision error. This has no influence because it only
  ;gives a constant multiplicative factor in the determinant
  log_determC_all=double(make_array(no*nalpha))
  c0=reform(c_all(no/2,nalpha/2,*,*),nrout,nrout)
  for i=0L,no-1 do begin 
     for j=0,nalpha-1 do begin 
        ind=i*nalpha+j 
        c=reform(c_all(i,j,*,*),nrout,nrout)/mean(c0) 
        log_determC_all(ind)=alog(determ(c,/double)) 
     endfor 
  endfor
  name_log_determC_all=simu_folder+'log_determ_cov_all.fits'
  writefits,name_log_determC_all,float(log_determC_all)

  ;write constant cov matrix transforms chosen at the middle of the grid
  ind=no/2*nalpha+nalpha/2
  sC=reform(sC_all(*,*,ind),nrout,nrout)
  name_sC=simu_folder+'sqrt_cov.fits'
  writefits,name_sC,sC

  iC=reform(sC_all(*,*,ind),nrout,nrout)
  name_iC=simu_folder+'inverse_cov.fits'
  writefits,name_iC,iC
end


