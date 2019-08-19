#!/bin/sh

export LD_LIBRARY_PATH=../base/lib
../base/bin/python ../modules/cctbx_project/libtbx/configure.py cctbx cbflib dxtbx scitbx libtbx iotbx mmtbx smtbx gltbx wxtbx dials xia2 prime iota --skip_phenix_dispatchers
