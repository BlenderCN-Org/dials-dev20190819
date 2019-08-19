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
# DIALS_ENABLE_COMMAND_LINE_COMPLETION
[ -n "$BASH_VERSION" ] && {
 source $(libtbx.find_in_repositories dials/util/autocomplete.sh) && source $LIBTBX_BUILD/dials/autocomplete/bash.sh || echo dials command line completion not available
}
cd "$ocwd"
ocwd=
alias libtbx.setpaths_all=". \"$LIBTBX_BUILD/setpaths_all.sh\""
alias libtbx.unsetpaths=". \"$LIBTBX_BUILD/unsetpaths.sh\""
LIBTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/libtbx"
export LIBTBX_DIST
TBXX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/tbxx"
export TBXX_DIST
BOOST_DIST="$LIBTBX_BUILD/../modules/boost"
export BOOST_DIST
BOOST_ADAPTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/boost_adaptbx"
export BOOST_ADAPTBX_DIST
OMPTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/omptbx"
export OMPTBX_DIST
FABLE_DIST="$LIBTBX_BUILD/../modules/cctbx_project/fable"
export FABLE_DIST
SCITBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/scitbx"
export SCITBX_DIST
CHILTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/chiltbx"
export CHILTBX_DIST
CCTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/cctbx"
export CCTBX_DIST
UCIF_DIST="$LIBTBX_BUILD/../modules/cctbx_project/ucif"
export UCIF_DIST
SMTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/smtbx"
export SMTBX_DIST
CCP4IO_DIST="$LIBTBX_BUILD/../modules/ccp4io"
export CCP4IO_DIST
CCP4IO_ADAPTBX_DIST="$LIBTBX_BUILD/../modules/ccp4io_adaptbx"
export CCP4IO_ADAPTBX_DIST
CBFLIB_DIST="$LIBTBX_BUILD/../modules/cbflib"
export CBFLIB_DIST
CBFLIB_ADAPTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/cbflib_adaptbx"
export CBFLIB_ADAPTBX_DIST
IOTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/iotbx"
export IOTBX_DIST
DXTBX_DIST="$LIBTBX_BUILD/../modules/dxtbx"
export DXTBX_DIST
ANNLIB_DIST="$LIBTBX_BUILD/../modules/annlib"
export ANNLIB_DIST
ANNLIB_ADAPTBX_DIST="$LIBTBX_BUILD/../modules/annlib_adaptbx"
export ANNLIB_ADAPTBX_DIST
SPOTFINDER_DIST="$LIBTBX_BUILD/../modules/cctbx_project/spotfinder"
export SPOTFINDER_DIST
RSTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/rstbx"
export RSTBX_DIST
MMTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/mmtbx"
export MMTBX_DIST
CRYS3D_DIST="$LIBTBX_BUILD/../modules/cctbx_project/crys3d"
export CRYS3D_DIST
GLTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/gltbx"
export GLTBX_DIST
CMA_ES_DIST="$LIBTBX_BUILD/../modules/cctbx_project/cma_es"
export CMA_ES_DIST
SIMTBX_DIST="$LIBTBX_BUILD/../modules/cctbx_project/simtbx"
export SIMTBX_DIST
PRIME_DIST="$LIBTBX_BUILD/../modules/cctbx_project/prime"
export PRIME_DIST
XFEL_DIST="$LIBTBX_BUILD/../modules/cctbx_project/xfel"
export XFEL_DIST
DIALS_DIST="$LIBTBX_BUILD/../modules/dials"
export DIALS_DIST
XIA2_DIST="$LIBTBX_BUILD/../modules/xia2"
export XIA2_DIST
IOTA_DIST="$LIBTBX_BUILD/../modules/cctbx_project/iota"
export IOTA_DIST
if [ -n "$LIBTBX_OPATH" ]; then
  LIBTBX_TMPVAL="$LIBTBX_OPATH"
else
  LIBTBX_TMPVAL=
fi
export LIBTBX_TMPVAL
PATH=`libtbx.path_utility prepend LIBTBX_TMPVAL "$LIBTBX_BUILD/bin" < /dev/null`
export PATH
if [ "$PATH" = "L_I_B_T_B_X_E_M_P_T_Y" ]; then unset PATH; fi
LIBTBX_TMPVAL=
LIBTBX_OPATH=
