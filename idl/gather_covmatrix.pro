;+ 
; NAME: 
;       GATHER_COVMATRIX
;
; PURPOSE: 
;      Compute a covariance matrix by optimally mixing surveys '1' and
;      '2'. The resulting covariance matrix Cout verifies
;      Cout^{-1}=C1^{-1}+C2{-1}  (see Labatie et al. 2012)
;
; CALLING SEQUENCE: 
;   GATHER_COVMATRIX,name_cov1,name_cov2,name_cov_out
;
; INPUTS: 
;   name_cov1  -- string: name to open the first covariance matrix
;   name_cov2  -- string: name to open the first covariance matrix
;   name_cov_out -- string: name to write the covariance matrix
;                   corresponding to '1+2'
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


pro gather_covmatrix,name_cov_in1,name_cov_in2,name_cov_out
  ;open cov matrices
  cov_in1=readfits(name_cov_in1)
  cov_in2=readfits(name_cov_in2)
  
  ;get dimensions
  no=n_elements(cov_in1(*,0,0,0))
  na=n_elements(cov_in1(0,*,0,0))
  nr=n_elements(cov_in1(0,0,*,0))
  cov_out=make_array(no,na,nr,nr)

  for i=0L,no-1 do begin 
     for j=0L,na-1 do begin
        c1=reform(cov_in1(i,j,*,*),nr,nr)
        c2=reform(cov_in2(i,j,*,*),nr,nr)

        ;apply optimal combination formula
        c3=invert(invert(c1,/double)+invert(c2,/double),/double)
        cov_out(i,j,*,*)=reform(c3,1,1,nr,nr)
     endfor
  endfor
  writefits,name_cov_out,cov_out
end
