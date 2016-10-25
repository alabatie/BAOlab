cmake_minimum_required(VERSION 2.6)
include(FindPkgConfig)


project(BAOlab)
enable_language (Fortran)

set(CMAKE_CXX_FLAGS "-O3 -fomit-frame-pointer -fno-common -fPIC -fopenmp")


###### Automatically find cfitsio and fftw3 library paths 

pkg_check_modules(libs_required REQUIRED cfitsio fftw3)
include_directories(${libs_required_INCLUDE_DIRS})
link_directories(${libs_required_LIBRARY_DIRS})
SET(LIBS ${libs_required_LIBRARIES} "-lstdc++ -lm")

##### Uncomment this line and comment the previous lines if the libraries cfitsio and fftw3 can be found automatically by your linker
##### SET(LIBS "-lstdc++ -lm -lfftw3 -lcfitsio")


add_library(BAOlab_lib STATIC lib/BAOlab_lib/DefMath.cc lib/BAOlab_lib/GetOpt.cc lib/BAOlab_lib/IM_IO.cc lib/BAOlab_lib/OptMedian.cc lib/BAOlab_lib/Memory.cc lib/BAOlab_lib/DefPoint.cc)
add_library(fftlog lib/fftlog/cdgamma.f lib/fftlog/drfftb.f lib/fftlog/drfftf.f lib/fftlog/drffti.f lib/fftlog/fftlog.f)


add_executable(delta_chi2 src/bao_detection/delta_chi2.cc)
target_link_libraries(delta_chi2 BAOlab_lib ${LIBS})


add_executable(lratio src/bao_detection/lratio.cc)
target_link_libraries(lratio BAOlab_lib ${LIBS})


set(OBJ_LOGNORMAL src/lognormal/im_poisson.cc)
add_executable(lognormal src/lognormal/lognormal.cc ${OBJ_LOGNORMAL})
target_link_libraries(lognormal BAOlab_lib ${LIBS})


add_executable(ps_transform src/ps_transform/ps_transform.c )
target_link_libraries(ps_transform fftlog fftlog "-lgfortran")


set(OBJ_CF src/cf/cf_obj.cc src/cf/cf_tools.cc)
add_executable(cf src/cf/cf.cc ${OBJ_CF})
target_link_libraries(cf BAOlab_lib ${LIBS})


set(OBJ_CF_ALPHA src/cf_alpha/cf_alpha_obj.cc src/cf_alpha/cf_tools.cc)
add_executable(cf_alpha src/cf_alpha/cf_alpha.cc ${OBJ_CF_ALPHA})
target_link_libraries(cf_alpha BAOlab_lib ${LIBS})




###### Install (by default in the project directory) ######

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
if(CUSTOM_INSTALL)
set(CMAKE_INSTALL_PREFIX $ENV{INSTALL_DIR})
endif(CUSTOM_INSTALL)

install(TARGETS delta_chi2 lratio lognormal ps_transform cf cf_alpha DESTINATION bin)

