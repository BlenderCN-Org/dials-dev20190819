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
unalias libtbx.unsetpaths
if ($?PYTHONPATH) then
  setenv LIBTBX_TMPVAL "$PYTHONPATH"
else
  unsetenv LIBTBX_TMPVAL
endif
setenv PYTHONPATH "`libtbx.path_utility delete LIBTBX_TMPVAL '$LIBTBX_BUILD/../modules/cctbx_project:$LIBTBX_BUILD/../modules:$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib/site-python:$LIBTBX_BUILD/../base/lib/python2.7/site-packages' < /dev/null`"
if ("$PYTHONPATH" == "L_I_B_T_B_X_E_M_P_T_Y") unsetenv PYTHONPATH
if ($?LD_LIBRARY_PATH) then
  setenv LIBTBX_TMPVAL "$LD_LIBRARY_PATH"
else
  unsetenv LIBTBX_TMPVAL
endif
setenv LD_LIBRARY_PATH "`libtbx.path_utility delete LIBTBX_TMPVAL '$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib64:$LIBTBX_BUILD/../base/lib' < /dev/null`"
if ("$LD_LIBRARY_PATH" == "L_I_B_T_B_X_E_M_P_T_Y") unsetenv LD_LIBRARY_PATH
unsetenv LIBTBX_DIST
unsetenv TBXX_DIST
unsetenv BOOST_DIST
unsetenv BOOST_ADAPTBX_DIST
unsetenv OMPTBX_DIST
unsetenv FABLE_DIST
unsetenv SCITBX_DIST
unsetenv CHILTBX_DIST
unsetenv CCTBX_DIST
unsetenv UCIF_DIST
unsetenv SMTBX_DIST
unsetenv CCP4IO_DIST
unsetenv CCP4IO_ADAPTBX_DIST
unsetenv CBFLIB_DIST
unsetenv CBFLIB_ADAPTBX_DIST
unsetenv IOTBX_DIST
unsetenv DXTBX_DIST
unsetenv ANNLIB_DIST
unsetenv ANNLIB_ADAPTBX_DIST
unsetenv SPOTFINDER_DIST
unsetenv RSTBX_DIST
unsetenv MMTBX_DIST
unsetenv CRYS3D_DIST
unsetenv GLTBX_DIST
unsetenv CMA_ES_DIST
unsetenv SIMTBX_DIST
unsetenv PRIME_DIST
unsetenv XFEL_DIST
unsetenv DIALS_DIST
unsetenv XIA2_DIST
unsetenv IOTA_DIST
if ($?LIBTBX_OPATH) then
  setenv LIBTBX_TMPVAL "$LIBTBX_OPATH"
else
  unsetenv LIBTBX_TMPVAL
endif
setenv PATH "`libtbx.path_utility delete LIBTBX_TMPVAL '$LIBTBX_BUILD/bin' < /dev/null`"
if ("$PATH" == "L_I_B_T_B_X_E_M_P_T_Y") unsetenv PATH
unsetenv LIBTBX_TMPVAL
unsetenv LIBTBX_OPATH
