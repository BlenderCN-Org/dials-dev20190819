# THIS IS AN AUTOMATICALLY GENERATED FILE.
# DO NOT EDIT! CHANGES WILL BE LOST.
ocwd="`pwd`"
if [ -n "$LIBTBX_BUILD_RELOCATION_HINT" ]; then
  cd "$LIBTBX_BUILD_RELOCATION_HINT"
  LIBTBX_BUILD_RELOCATION_HINT=
  export LIBTBX_BUILD_RELOCATION_HINT
elif [ -n "$BASH_SOURCE" ]; then
  LIBTBX_BUILD=`dirname "$BASH_SOURCE[0]"`
  cd "$LIBTBX_BUILD"
else
  cd "/home/jdiaz/dials-dev20190819/build"
fi
LIBTBX_BUILD=`pwd -P`
export LIBTBX_BUILD
LIBTBX_OPATH="$PATH"
export LIBTBX_OPATH
PATH="$LIBTBX_BUILD/bin:$PATH"
export PATH
cd "$ocwd"
ocwd=
unalias libtbx.unsetpaths > /dev/null 2>&1
if [ -n "$PYTHONPATH" ]; then
  LIBTBX_TMPVAL="$PYTHONPATH"
else
  LIBTBX_TMPVAL=
fi
export LIBTBX_TMPVAL
PYTHONPATH=`libtbx.path_utility delete LIBTBX_TMPVAL "$LIBTBX_BUILD/../modules/cctbx_project:$LIBTBX_BUILD/../modules:$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib/site-python:$LIBTBX_BUILD/../base/lib/python2.7/site-packages" < /dev/null`
if [ "$PYTHONPATH" = "L_I_B_T_B_X_E_M_P_T_Y" ]; then unset PYTHONPATH; fi
if [ -n "$LD_LIBRARY_PATH" ]; then
  LIBTBX_TMPVAL="$LD_LIBRARY_PATH"
else
  LIBTBX_TMPVAL=
fi
export LIBTBX_TMPVAL
LD_LIBRARY_PATH=`libtbx.path_utility delete LIBTBX_TMPVAL "$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib64:$LIBTBX_BUILD/../base/lib" < /dev/null`
if [ "$LD_LIBRARY_PATH" = "L_I_B_T_B_X_E_M_P_T_Y" ]; then unset LD_LIBRARY_PATH; fi
unset LIBTBX_DIST
unset TBXX_DIST
unset BOOST_DIST
unset BOOST_ADAPTBX_DIST
unset OMPTBX_DIST
unset FABLE_DIST
unset SCITBX_DIST
unset CHILTBX_DIST
unset CCTBX_DIST
unset UCIF_DIST
unset SMTBX_DIST
unset CCP4IO_DIST
unset CCP4IO_ADAPTBX_DIST
unset CBFLIB_DIST
unset CBFLIB_ADAPTBX_DIST
unset IOTBX_DIST
unset DXTBX_DIST
unset ANNLIB_DIST
unset ANNLIB_ADAPTBX_DIST
unset SPOTFINDER_DIST
unset RSTBX_DIST
unset MMTBX_DIST
unset CRYS3D_DIST
unset GLTBX_DIST
unset CMA_ES_DIST
unset SIMTBX_DIST
unset PRIME_DIST
unset XFEL_DIST
unset DIALS_DIST
unset XIA2_DIST
unset IOTA_DIST
if [ -n "$LIBTBX_OPATH" ]; then
  LIBTBX_TMPVAL="$LIBTBX_OPATH"
else
  LIBTBX_TMPVAL=
fi
export LIBTBX_TMPVAL
PATH=`libtbx.path_utility delete LIBTBX_TMPVAL "$LIBTBX_BUILD/bin" < /dev/null`
if [ "$PATH" = "L_I_B_T_B_X_E_M_P_T_Y" ]; then unset PATH; fi
LIBTBX_TMPVAL=
LIBTBX_OPATH=
