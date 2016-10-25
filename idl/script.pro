;;;;
;;;; Script which performs the main operations for the tasks of BAO
;;;; detection and BAO parameter constraints. It is based on the work
;;;; in the 2 ApJ papers Labatie et al. 2012. The main novelty of our
;;;; approach is that it enables to obtain a model-dependent
;;;; covariance matrix which can change the results both for BAO
;;;; detection and for parameter constraints. You might need to adjust
;;;; the script to your needs (especially all the parameter values
;;;; that are stored in the file 'BAOlab.sav' at the beginning). Be
;;;; careful that most parameters should be changed accordingly in the
;;;; parameter files of the different programs in the folder ../param/ !
;;;; You might also have to restore the parameters in the file
;;;; 'BAOlab.sav' when you do not start the script at the beginning. 
;;;;
;;;; The study Labatie et al. 2012 used the same parameters as the
;;;; ones in this script, except that it used both the Northern
;;;; Hemisphere and Southern Hemisphere for the simulations. This
;;;; study gave the resulting covariance matrices cov_all.fits,
;;;; originally present in the BAOlab package, in the folder input_files/simu/
;;;;
;;;; To obtain these results again, compared to this script, there
;;;; would only be to add lognormal simulations with the Southern
;;;; mask, compute the covariance matrices both for the North only and
;;;; for the South only, and finally optimally combine these
;;;; covariance matrices with the idl procedure
;;;; gather_covmatrix.pro. Note however that this study required a lot
;;;; of computation time, and had to be performed on a large computer
;;;; cluster. So you might want to decrease the number of parameter
;;;; values or simulations according to your needs.
;;;;


;;;;; First set some parameters and save them in the file 'BAOlab.sav'
;;;;; These parameters can be recovered using the command
;;;;; restore,'BAOlab.sav'

;;;; Get current folder (YOU MUST BE IN THE idl/ FOLDER WHEN RUNNING
;;;; THIS COMMAND AT THE TIME OF SAVING THE 'BAOlab.sav FILE
CD, Current=idl_folder   
idl_folder=idl_folder+'/'
ps_folder=idl_folder+'../output_files/ps_transform/'
lognormal_folder=idl_folder+'../output_files/lognormal/'
cf_alpha_folder=idl_folder+'../output_files/cf_alpha/'
simu_folder=idl_folder+'../input_files/simu/'
program_folder=idl_folder+'../bin/'

;;;;; alpha table in the model-dependent cov matrix
alpha_min=0.8
alpha_max=1.2
nalpha=101
d_alpha=(alpha_max-alpha_min)/double(nalpha-1.0)
alpha_table=findgen(nalpha)*d_alpha+alpha_min
 
;;;;; Omega_m h^2 table in the lognormal simulations
o_min=0.08
o_max=0.18
no1=5
d_o1=(o_max-o_min)/double(no1-1.0)
o_table1=findgen(no1)*d_o1+o_min

;;;;; Omega_m h^2 table in the model-dependent cov matrix.
;;;;; It is bigger than the one of the lognormal simulations (the cov
;;;;; matrix is obtained by linear interpolation of the smaller one
;;;;; obtained with the lognormal simulations)
no=101
d_o=(o_max-o_min)/double(no-1.0)
o_table=findgen(no)*d_o+o_min

;;;;; binning of the correlation function estimators and of the cov matrix
rout=(findgen(18)+0.5)*10.0+20.0
drout=(rout(17)-rout(0))/17.0
nrout=n_elements(rout)

;;;;; number of lognormal simulations for each value of alpha and Omega_m h^2:
;;;;; 2000 simulations enables to have a very good estimate of the cov matrix
nsimu_lognormal=2000

;;;;; number of simulations for each model in the BAO detection (i.e. in
;;;;; the programs delta_chi2 and lratio. This number creates a limit in
;;;;; the significance that can be estimated for each simulation (e.g. 50k
;;;;; simus corresponds to a limit of 4.25 sigma). It must be
;;;;; consistent with the files ../param/lratio.param and ../param/delta_chi2.param
nsimu_bao_detection=50000  


;;;;; template used to open raw catalogues after the lognormal program has run
my_t=ascii_template('../output_files/lognormal/get_template.dat') ;save template of simus
save,/all,filename='BAOlab.sav'




;;;;; Then create input power spectra using the icosmo package,
;;;;; for the different values of Omega_m h^2. Note that some
;;;;; parameters can be changed in the icosmoPS idl procedure

for i=0,no1-1 do begin $
& omegamh2=o_table(i) $
& filename=ps_folder+'/input_pk'+STRTRIM(i,2)+'.dat' $
& icosmoPS,filename=filename,omegamh2=omegamh2 $
& endfor


;;;;; Transform these power spectrum with the program ps_transform.
;;;;; This gives the power spectrum of the underlying Gaussian field
;;;;; needed by the lognormal program in order to generate the
;;;;; lognormal field. In order to avoid NaN in the program, one might
;;;;; have to adjust the number of points zero-padded (i.e. the -z
;;;;; option). One might also have to change the bias value b=2.5 if
;;;;; needed. In this case, the bias should also be changed in the 
;;;;; procedure icosmoPS_grid, which gives the model correlation
;;;;; functions

for i=0,no1-1 do begin $
&     spawn,program_folder+'ps_transform -v -z 200 -b 2.5 '+ps_folder+'input_pk'+STRTRIM(i,2)+'.dat' $
&     spawn,'cp '+ps_folder+'output_pkgaus.dat '+ps_folder+'output_pkgaus'+STRTRIM(i,2)+'.dat' $
& endfor


;;;;; Generate the lognormal fields. Can adjust the number of grid points
;;;;; with the option -d. A larger number of points gives a better
;;;;; precision (i.e. decrease the smoothing due to large cells in the
;;;;; grid). However the computation time increases as N^3, so be
;;;;; careful. You might also want to change the survey mask, mean
;;;;; number density and limits in (x,y,z) so that the cube includes
;;;;; all the survey that you want.

for i=0,no1-1 do begin $
&  for j=0L,nsimu_lognormal-1 do begin $
&      spawn,program_folder+'lognormal -v -d 800 '+ps_folder+'input_pk'+STRTRIM(i,2)+'.dat '+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_raw.dat' $
&      spawn,program_folder+'lognormal -v -d 400 -r '+ps_folder+'input_pk'+STRTRIM(i,2)+'.dat '+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_random_raw.dat' $
&  endfor $
& endfor


;;;;; Postprocess the catalogues and create the alpha belonging of
;;;;; each galaxy in the catalogue, before computing the
;;;;; alpha-dependent correlation with the program cf_alpha

for i=0,no1-1 do begin $
&  for j=0L,nsimu_lognormal-1 do begin $
&   name=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_raw.dat' $
&   data=read_ascii(name,template=my_t) $
&   name_out=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'.dat' $
&   name_out_alpha=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_alpha.dat' $
&   rmk_catalogue,data,name_out,name_out_alpha $ 
&   spawn,'rm '+name $
&   name=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_random_raw.dat' $
&   data=read_ascii(name,template=my_t) $
&   name_out=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_random.dat' $
&   name_out_alpha=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_random_alpha.dat' $
&   rmk_catalogue,data,name_out,name_out_alpha $ 
&   spawn,'rm '+name $
&  endfor $
& endfor


;;;;; Compute the alpha-dependent correlation with the program
;;;;; cf_alpha. You might want to adjust the minimum distance of the
;;;;; binning with -m and maximum distance with -M. The separation of
;;;;; each bin is equal to 1 by default (you can change it with
;;;;; -s). Keep in mind that the binning is in comoving coordinates,
;;;;; and will we readjusted for each value of alpha. Therefore you
;;;;; should have the minimum distance in comoving coordinates less
;;;;; than alpha_min*min(r_out) and the maximum distance greater than
;;;;; alpha_max*max(r_out). You should also keep a sufficiently low -s
;;;;; so that the rebinning is sufficiently precise.

for i=0,no1-1 do begin $
&  for j=0L,nsimu_lognormal-1 do begin $
&    name_cat=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'.dat' $
&    name_cat_alpha=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_alpha.dat' $
&    name_rnd=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_random.dat' $
&    name_rnd_alpha=lognormal_folder+'DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'_random_alpha.dat' $
&    name_out='DR7-no_'+STRTRIM(i,2)+'-'+STRTRIM(j,2)+'.fits' $
&    spawn,program_folder+'cf_alpha -v -m 0 -M 300 -a '+name_cat_alpha+' -A '+name_rnd_alpha+' -r '+name_rnd+' '+name_cat+' '+name_out $
&  endfor $
& endfor


;;;;; Compute the model-dependent covariance matrix

name_prefix=cf_alpha_folder+'DR7-no_'
name_cov_no=simu_folder+'cov_all_no.fits'
mk_covmatrix,name_prefix,name_cov_no


;;;;; Transform the model-dependent covariance matrix: compute the square root, the inverse
;;;;; and the determinant of the model-dependent cov matrix (required for the BAO detection)

transform_covmatrix,name_cov_no


;;;;; Create the model correlation functions with BAOs (for parameter
;;;;; constraints) and wit no-wiggles and no baryons (for BAO
;;;;; detection). These are saved in ../input_files/model/

icosmoPS_grid


;;;;; Compute and plot the 2D posterior in Omega_m h^2 and alpha of the
;;;;; measured correlation function. Also gives the maximum of the
;;;;; posterior and confidence regions for each parameter Omega_m h^2
;;;;; and alpha

name_xi=simu_folder+'dr7.fits'
likelihood,name_xi


;;;;; Create histograms for the BAO detection, using either the Delta
;;;;; chi^2 or the generalized likelihood ratio, using either a
;;;;; constant covariance matrix or model-dependent covariance matrix
;;;;; (for this, just add the option -c), and using either the H0
;;;;; hypothesis or the H1 hypothesis.

spawn,program_folder+'delta_chi2 -v -h 0'
spawn,program_folder+'delta_chi2 -v -h 1'
spawn,program_folder+'delta_chi2 -v -h 0 -c'
spawn,program_folder+'delta_chi2 -v -h 1 -c'
spawn,program_folder+'lratio -v -h 0 -c'
spawn,program_folder+'lratio -v -h 1 -c'


;;;;; Compute the mean significance of the BAO detection under H1, with the
;;;;; previously calculated histograms, using either the Delta chi^2
;;;;; statistic (lratio=0) or the generalized likelihood ratio
;;;;; (lratio=1), and using either a constant cov matrix (varcov=0) or
;;;;; model-dependent cov matrix (varcov=1)

sign_bao_detection,lratio=0,varcov=0
sign_bao_detection,lratio=0,varcov=1
sign_bao_detection,lratio=1,varcov=1
