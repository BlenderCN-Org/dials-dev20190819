#! /bin/csh -f
set noglob
set verbose
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/libtbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/boost_adaptbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/fable/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/scitbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/cctbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/cctbx/run_examples.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/smtbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/cbflib_adaptbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/iotbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/dxtbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/annlib_adaptbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/spotfinder/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/rstbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/mmtbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/gltbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/cma_es/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/cctbx_project/simtbx/run_tests.py" $*
libtbx.python "/home/jdiaz/dials-dev20190819/modules/dials/run_tests.py" $*
