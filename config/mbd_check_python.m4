dnl ----------------------------------------------------------------------
dnl MBD_CHECK_PYTHON([MINIIMUM-VERSION])
dnl ----------------------------------------------------------------------

dnl This macro tries to figure out the neccesary includes and flags to 
dnl add when compiling a C/C++ program to embed Python in. 

dnl On succes, the following variables are set to
dnl TODO

dnl On failure, the following variables are set to
dnl

dnl If the MINIUMUM-VERSION argument is passed, MBD_CHECK_PYTHON will
dnl cause an error if the version of python installed on the system
dnl doesn't meet the requirement.  MINIMUM-VERSION should consist of
dnl numbers and dots only.

dnl This macro probably only works with Python version 1.5.2 and never.
dnl Python must be present in the users path. Development libraries for python
dnl must be present on the system. The module distutils.sysconfig and the call
dnl distutils.sysconfig.get_config_vars() must be present

dnl Most of the code in here was stolen from either the KDE project - a macro
dnl called KDE_CHECK_PYTHON - or from the pygtk project - a macro called 
dnl "GED". KDE_CHECK_PATH has the problem - as I see it - that it 
dnl has troubles locating the python installation, whereas the "GED" 
dnl macro is aimed at people extending Python. This macro tries to combine 
dnl those two macros.

dnl Especially from pygtk, certain paragraphs are taken verbatim.

dnl CREDITS: I have no idea who wrote the KDE_CHECK_PATH, but "GED"
dnl was written by James Henstridge and Andrew Dalke, AFAIK.

dnl Mads Bondo Dydensborg

AC_DEFUN([MBD_CHECK_PYTHON], 
 [
  dnl Set failure value - change to yes, if we fail.
  mbd_check_python_failure=no

  dnl Locate python in the users path
  AC_PATH_PROG(PYTHON, python python2.1 python2.0 python1.6 python1.5)

  dnl The version check is taken verbatim from pygtk/"GED",
  dnl except that we do not error
  dnl should we do the version check?
  ifelse([$1],[],,[
    AC_MSG_CHECKING(if Python version >= $1)
    changequote(<<, >>)dnl
    prog="
import sys, string
minver = '$1'
pyver = string.split(sys.version)[0]  # first word is version string
# split strings by '.' and convert to numeric
minver = map(string.atoi, string.split(minver, '.'))
if hasattr(sys, 'version_info'):
    pyver = sys.version_info[:3]
else:
    pyver = map(string.atoi, string.split(pyver, '.'))
# we can now do comparisons on the two lists:
if pyver >= minver:
        sys.exit(0)
else:
        sys.exit(1)"
    changequote([, ])dnl
    if $PYTHON -c "$prog" 1>&AC_FD_CC 2>&AC_FD_CC
    then
      AC_MSG_RESULT(okay)
    else
      AC_MSG_RESULT(too old)
      mbd_check_python_failure=yes
    fi
  ])

  dnl Next logical script is to set the version 
  dnl This is the way "GED" does it, at least
  AC_MSG_CHECKING([local Python version])

  dnl Query Python for its version number.  Getting [:3] seems to be
  dnl the best way to do this; it's what "site.py" does in the standard
  dnl library.  Need to change quote character because of [:3]

  AC_SUBST(PYTHON_VERSION)
  changequote(<<, >>)dnl
  PYTHON_VERSION=`$PYTHON -c "import sys; print sys.version[:3]"`
  changequote([, ])dnl

  AC_MSG_RESULT([found $PYTHON_VERSION])

  dnl Locate the directory with the Python.h file in.
  dnl This code also mostly from "ged"  	
  if test "x$mbd_check_python_failure" != "xyes"; then
    AC_MSG_CHECKING(for headers required to compile programs that use Python)
    dnl deduce PYTHON_INCLUDES
    py_prefix=`$PYTHON -c "import sys; print sys.prefix"`
    py_exec_prefix=`$PYTHON -c "import sys; print sys.exec_prefix"`

    PYTHON_INCLUDES="-I${py_prefix}/include/python${PYTHON_VERSION}"
    if test "$py_prefix" != "$py_exec_prefix"; then
      PYTHON_INCLUDES="$PYTHON_INCLUDES -I${py_exec_prefix}/include/python${PYTHON_VERSION}"
    fi
    AC_SUBST(PYTHON_INCLUDES)
    dnl check if the headers exist:
    save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
    AC_TRY_CPP([
    #include <Python.h>
    ],
    [AC_MSG_RESULT(found)
    ],
    [AC_MSG_RESULT(not found)
    mbd_check_python_failure=yes])
    CPPFLAGS="$save_CPPFLAGS"
  fi

  dnl Now, check what libraries python needs. 
  dnl This test needs support from distutils.sysconfig.get_config_var
  dnl One should probably check for this first. Dunno how, though
  changequote(<<, >>)dnl
  PYTHON_LIBS=`$PYTHON -c "from distutils.sysconfig import get_config_var; print get_config_var(\"LIBS\")"`
  PYTHON_SYSLIBS=`$PYTHON -c "from distutils.sysconfig import get_config_var; print get_config_var(\"SYSLIBS\")"`
  changequote([, ])dnl

 ]
)