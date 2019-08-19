import sys
import gdb

# Update module path.
dir_ = '/scratch/jenkins_agent/workspace/dials_package_linux/build_dials/base/share/glib-2.0/gdb'
if not dir_ in sys.path:
    sys.path.insert(0, dir_)

from glib import register
register (gdb.current_objfile ())
