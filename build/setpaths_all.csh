# THIS IS AN AUTOMATICALLY GENERATED FILE.
# DO NOT EDIT! CHANGES WILL BE LOST.
set ocwd="$cwd"
if ($?LIBTBX_BUILD_RELOCATION_HINT) then
  cd "$LIBTBX_BUILD_RELOCATION_HINT"
  unsetenv LIBTBX_BUILD_RELOCATION_HINT
else
  cd "/home/jdiaz/dials-dev20190819/build"
endif
setenv LIBTBX_BUILD "`/bin/sh -c 'pwd -P'`"
setenv LIBTBX_OPATH "$PATH"
setenv PATH "$LIBTBX_BUILD/bin:$PATH"
cd "$ocwd"
unset ocwd
alias libtbx.setpaths_all "source '$LIBTBX_BUILD/setpaths_all.csh'"
alias libtbx.unsetpaths "source '$LIBTBX_BUILD/unsetpaths.csh'"
setenv LIBTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/libtbx"
setenv TBXX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/tbxx"
setenv BOOST_DIST "$LIBTBX_BUILD/../modules/boost"
setenv BOOST_ADAPTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/boost_adaptbx"
setenv OMPTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/omptbx"
setenv FABLE_DIST "$LIBTBX_BUILD/../modules/cctbx_project/fable"
setenv SCITBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/scitbx"
setenv CHILTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/chiltbx"
setenv CCTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/cctbx"
setenv UCIF_DIST "$LIBTBX_BUILD/../modules/cctbx_project/ucif"
setenv SMTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/smtbx"
setenv CCP4IO_DIST "$LIBTBX_BUILD/../modules/ccp4io"
setenv CCP4IO_ADAPTBX_DIST "$LIBTBX_BUILD/../modules/ccp4io_adaptbx"
setenv CBFLIB_DIST "$LIBTBX_BUILD/../modules/cbflib"
setenv CBFLIB_ADAPTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/cbflib_adaptbx"
setenv IOTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/iotbx"
setenv DXTBX_DIST "$LIBTBX_BUILD/../modules/dxtbx"
setenv ANNLIB_DIST "$LIBTBX_BUILD/../modules/annlib"
setenv ANNLIB_ADAPTBX_DIST "$LIBTBX_BUILD/../modules/annlib_adaptbx"
setenv SPOTFINDER_DIST "$LIBTBX_BUILD/../modules/cctbx_project/spotfinder"
setenv RSTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/rstbx"
setenv MMTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/mmtbx"
setenv CRYS3D_DIST "$LIBTBX_BUILD/../modules/cctbx_project/crys3d"
setenv GLTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/gltbx"
setenv CMA_ES_DIST "$LIBTBX_BUILD/../modules/cctbx_project/cma_es"
setenv SIMTBX_DIST "$LIBTBX_BUILD/../modules/cctbx_project/simtbx"
setenv PRIME_DIST "$LIBTBX_BUILD/../modules/cctbx_project/prime"
setenv XFEL_DIST "$LIBTBX_BUILD/../modules/cctbx_project/xfel"
setenv DIALS_DIST "$LIBTBX_BUILD/../modules/dials"
setenv XIA2_DIST "$LIBTBX_BUILD/../modules/xia2"
setenv IOTA_DIST "$LIBTBX_BUILD/../modules/cctbx_project/iota"
if ($?LIBTBX_OPATH) then
  setenv LIBTBX_TMPVAL "$LIBTBX_OPATH"
else
  unsetenv LIBTBX_TMPVAL
endif
setenv PATH "`libtbx.path_utility prepend LIBTBX_TMPVAL '$LIBTBX_BUILD/bin' < /dev/null`"
if ("$PATH" == "L_I_B_T_B_X_E_M_P_T_Y") unsetenv PATH
unsetenv LIBTBX_TMPVAL
unsetenv LIBTBX_OPATH
