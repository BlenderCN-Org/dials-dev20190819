export DIALS="/home/jdiaz/dials-dev20190819"
export DIALS_VERSION=dev20190819
export DIALS_ENVIRONMENT=1
export DIALS_MTYPE=intel-linux-2.6-x86_64
# include at start
if [ "$LIBTBX_DISPATCHER_NAME" != "libtbx.scons" ] &&    [ -z "$PHENIX_TRUST_OTHER_ENV" ]; then
  # work around broken library environments
  LD_LIBRARY_PATH=""
  PYTHONPATH=""
fi
# include before command
#
# GUI dependencies
CCTBX_BUILD_BASE="/home/jdiaz/dials-dev20190819/base"
LIBTBX_OS_NAME=`uname -s`
if [ "$PHENIX_GUI_ENVIRONMENT" = "1" ]; then
  if [ -z "$DISABLE_PHENIX_GUI" ]; then
    export BOOST_ADAPTBX_FPE_DEFAULT=1
    export BOOST_ADAPTBX_SIGNALS_DEFAULT=1
  fi
  # echo $LIBTBX_OS_NAME
  if [ "$LIBTBX_OS_NAME" = "Linux" ]; then
    export OLD_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
    export OLD_XDG_DATA_DIRS=$XDG_DATA_DIRS
    if [ -z "$LD_LIBRARY_PATH" ]; then
      LD_LIBRARY_PATH=/home/jdiaz/dials-dev20190819/base/lib:/home/jdiaz/dials-dev20190819/base/lib64
    else
      LD_LIBRARY_PATH=/home/jdiaz/dials-dev20190819/base/lib:/home/jdiaz/dials-dev20190819/base/lib64:$LD_LIBRARY_PATH
    fi
    export LD_LIBRARY_PATH
#
    unset GTK_MODULES
    GTK_PATH=$CCTBX_BUILD_BASE/lib/gtk-2.0/2.10.0
    PANGO_RC_FILE=$CCTBX_BUILD_BASE/etc/pango/pangorc
    GTK_IM_MODULE_FILE=$CCTBX_BUILD_BASE/etc/gtk-2.0/gtk.immodules
    GDK_PIXBUF_MODULE_FILE=$CCTBX_BUILD_BASE/etc/gtk-2.0/gdk-pixbuf.loaders
    GTK2_RC_FILES=$CCTBX_BUILD_BASE/share/themes/Clearlooks/gtk-2.0/gtkrc
    FONTCONFIG_PATH=$CCTBX_BUILD_BASE/etc/fonts
    FONTCONFIG_FILE=$CCTBX_BUILD_BASE/etc/fonts/fonts.conf
    if [ -z "$XDG_DATA_DIRS" ]; then
      XDG_DATA_DIRS=$CCTBX_BUILD_BASE/share:/usr/share
    else
      XDG_DATA_DIRS=$CCTBX_BUILD_BASE/share:$XDG_DATA_DIRS
    fi
    GNOME_DISABLE_CRASH_DIALOG=1
    export PANGO_RC_FILE
    export GTK_IM_MODULE_FILE
    export GDK_PIXBUF_MODULE_FILE
    export FONTCONFIG_PATH
    export FONTCONFIG_FILE
    export XDG_DATA_DIRS
    export GTK_PATH
    export GTK2_RC_FILES
    export GNOME_DISABLE_CRASH_DIALOG
    PATH=$CCTBX_BUILD_BASE/bin:$PATH
    export PATH
  fi
fi


if [ ! -z "$QB_PYTHONPATH" ]; then
  export PYTHONPATH=$PYTHONPATH:$QB_PYTHONPATH
fi
if [ ! -z "$QB_LD_LIBRARY_PATH" ]; then
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QB_LD_LIBRARY_PATH
fi
if [ ! -z "$QB_DYLD_LIBRARY_PATH" ]; then
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$QB_DYLD_LIBRARY_PATH
fi


