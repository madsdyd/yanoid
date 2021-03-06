dnl ------------------------------------------------------------
dnl configure.in for yanoid
dnl ------------------------------------------------------------

dnl CREDITS: Most of this stuff was stolen! Mostly from 
dnl tuxracer, but also some stuff from tuxkart and kde.


dnl ------------------------------------------------------------
dnl Setup initial stuff
dnl ------------------------------------------------------------


AC_INIT([yanoid], [0.4.0])
AC_CONFIG_SRCDIR([src/main.cc])
AM_INIT_AUTOMAKE

dnl ------------------------------------------------------------
dnl Make CDPATH not influence stuff. 
dnl ------------------------------------------------------------
CDPATH=${ZSH_VERSION+.}:

dnl Enable --maintainer-mode
dnl AM_MAINTAINER_MODE
dnl Put config.h in src
AM_CONFIG_HEADER(src/config.h)
dnl Interestingly enough, the SDL check fails if we say that we are 
dnl doing stuff in C++.
dnl AC_LANG_CPLUSPLUS

dnl ------------------------------------------------------------
dnl Make sure all flags are blank
dnl ------------------------------------------------------------
CPPFLAGS=""
CFLAGS=""
CXXFLAGS=""
LIBS=""


dnl ------------------------------------------------------------
dnl Check system type 
dnl ------------------------------------------------------------
dnl AC_CANONICAL_HOST


dnl ------------------------------------------------------------
dnl Checks for programs. 
dnl ------------------------------------------------------------
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_CXX
AC_PROG_CPP


dnl ------------------------------------------------------------
dnl Checks for libraries.
dnl ------------------------------------------------------------
dnl NOTE - Messages to the user is deffered, such that the user
dnl can have all messages at one time.

dnl Check for the math lib
AC_CHECK_LIB(m, sin, have_m=yes, have_m=no)
if test "x$have_m" = "xyes" ; then
    YANOID_LIBS="$YANOID_LIBS -lm"
fi

dnl Check for SDL version.
SDL_VERSION=1.2.2
AM_PATH_SDL($SDL_VERSION, have_SDL=yes, have_SDL=no)
dnl Should the -lSDL go in here? Maybe not.
YANOID_CFLAGS="$YANOID_CFLAGS $SDL_CFLAGS"
YANOID_CXXFLAGS="$YANOID_CXXFLAGS $SDL_CFLAGS"
YANOID_LIBS="$YANOID_LIBS $SDL_LIBS"

dnl Check for SDL extension libs
saved_LIBS="$LIBS"
LIBS="$saved_LIBS $YANOID_LIBS"

dnl SDL_mixer
AC_CHECK_LIB( SDL_mixer, Mix_OpenAudio, 
	      have_SDL_mixer=yes, have_SDL_mixer=no )

if test "x$have_SDL_mixer" = "xyes" ; then 
    YANOID_LIBS="$YANOID_LIBS -lSDL_mixer"
fi

dnl SDL_image
AC_CHECK_LIB( SDL_image, IMG_Load, 
	      have_SDL_image=yes, have_SDL_image=no )
if test "x$have_SDL_image" = "xyes" ; then
    YANOID_LIBS="$YANOID_LIBS -lSDL_image"
fi

LIBS="$saved_LIBS"


dnl ------------------------------------------------------------
dnl Checks for header files. (Unused?)
dnl ------------------------------------------------------------
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/time.h unistd.h)


dnl ------------------------------------------------------------
dnl Check for Python. 
dnl ------------------------------------------------------------

MBD_CHECK_PYTHON([2.0])
AC_MSG_RESULT([$PACKAGE uses
PYTHON          : $PYTHON
PYTHON_VERSION  : $PYTHON_VERSION
PYTHON_INCLUDES : $PYTHON_INCLUDES
PYTHON_LIBS     : $PYTHON_LIBS
PYTHON_LDFLAGS  : $PYTHON_LDFLAGS
PYTHON_LIBRARY  : $PYTHON_LIBRARY
])

YANOID_CPPFLAGS="$YANOID_CPPFLAGS $PYTHON_INCLUDES"
YANOID_LIBS="$YANOID_LIBS $PYTHON_LDFLAGS $PYTHON_LIBRARY $PYTHON_LIBS"


dnl ------------------------------------------------------------
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ------------------------------------------------------------
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME


dnl ------------------------------------------------------------
dnl Check for special functions (unused)
dnl ------------------------------------------------------------
AC_CHECK_FUNCS(getcwd)


dnl ------------------------------------------------------------
dnl General options
dnl ------------------------------------------------------------

WFLAGS="-Wall -Wpointer-arith -Wconversion -Woverloaded-virtual"
OFLAGS="-O2 -ffast-math -fexpensive-optimizations"
EONPFLAGS="-fomit-frame-pointer"

YANOID_CFLAGS="$YANOID_CFLAGS $WFLAGS"
YANOID_CXXFLAGS="$YANOID_CXXFLAGS $WFLAGS"
YANOID_OP_CFLAGS=$OFLAGS
YANOID_OP_CXXFLAGS=$OFLAGS

dnl ------------------------------------------------------------
dnl Set optimization flags for platform - unused
dnl ------------------------------------------------------------
dnl case "$host" in
dnl i*86-*-*) YANOID_OP_CFLAGS="$YANOID_OP_CFLAGS -malign-loops=2 -malign-jumps=2 -malign-functions=2";
 dnl         YANOID_OP_CXXFLAGS="$YANOID_OP_CXXFLAGS -malign-loops=2 -malign-jumps=2 -malign-functions=2";;
dnl alpha*-*-linux-*) YANOID_CFLAGS="$YANOID_CFLAGS -mieee";;
dnl esac


dnl ------------------------------------------------------------
dnl debug, optimize, pedantic and profile options
dnl ------------------------------------------------------------
AC_ARG_ENABLE(debug,     [  --disable-debug         Produce an executable without debugging symbols], 
			 [disable_debug=yes], [disable_debug=no])


if test "x$disable_debug" = "xno" ; then 
  AC_DEFINE(DEBUG, 1, High level debug setting)
  YANOID_CFLAGS="-g $YANOID_CFLAGS"
  YANOID_CXXFLAGS="-g $YANOID_CXXFLAGS"
  AC_CHECK_HEADER(execinfo.h, [AC_DEFINE(HAVE_EXECINFO_H, 1, Wheter or not execinfo.h is present)
		 YANOID_LIBS="$YANOID_LIBS -rdynamic"])
fi

AC_ARG_ENABLE(optimize,  [  --enable-optimize       Enable various optimizations. Looooong compile time],
                         [enable_optimize=yes], [enable_optimize=no])

AC_ARG_ENABLE(optimize-size,  [  --enable-optimize-size  Enable optimization for size. Only relevante in contest.],
			 [YANOID_CFLAGS="$YANOID_CFLAGS -O"; YANOID_CXXFLAGS="$YANOID_CXXFLAGS -O"])

AC_ARG_ENABLE(pedantic,  [  --enable-pedantic       Enable -pedantic (and -ansi for C) when compiling], 
                         [YANOID_CFLAGS="$YANOID_CFLAGS -ansi -pedantic"; YANOID_CXXFLAGS="$YANOID_CXXFLAGS -pedantic"])

AC_ARG_ENABLE(profile,   [  --enable-profile        Produce a profiled executable],
                         [enable_profile=yes], [enable_profile=no])

dnl Set optimize flags
if test "x$enable_optimize" = "xyes" ; then
  YANOID_CFLAGS="$YANOID_CFLAGS $YANOID_OP_CFLAGS"
  YANOID_CXXFLAGS="$YANOID_CXXFLAGS $YANOID_OP_CXXFLAGS"
  if test "x$enable_profile" = "xno" ; then
    YANOID_CFLAGS="$YANOID_CFLAGS $EONPFLAGS"
    YANOID_CXXFLAGS="$YANOID_CXXFLAGS $EONPFLAGS"
  fi
fi

dnl Set profile flags
if test "x$enable_profile" = "xyes" ; then
  YANOID_CFLAGS="$YANOID_CFLAGS -pg -O"; 
  YANOID_CXXFLAGS="$YANOID_CXXFLAGS -pg -O"
fi


dnl ------------------------------------------------------------
dnl Specify data directory - stolen from tuxkart
dnl ------------------------------------------------------------

dnl I am not exactly sure about datarootdir vs datadir, but here goes.
MY_EXPAND_DIR(yanoid_datadir, "$datarootdir/games/$PACKAGE")
AC_DEFINE_UNQUOTED(YANOID_DATADIR, "$yanoid_datadir", Defines the location of the data directory)

dnl AC_ARG_WITH(data-dir, [  --with-data-dir=PATH    Default yanoid data directory], 
dnl                      YANOID_CPPFLAGS="$YANOID_CPPFLAGS -DDATA_DIR=\\\"$withval\\\"")


dnl ------------------------------------------------------------
dnl Print messages about missing or incomplete stuff.
dnl ------------------------------------------------------------
AC_MSG_CHECKING(if there were any problems with the environment)

have_trouble=no

if test "x$have_m" = "xno" ; then
    have_trouble=yes
    AC_MSG_WARN([
*** PROBLEM:
*** Unable to find the math library.
*** This is required for yanoid to work.
*** This is most probably a problem with your compiler installation
])
fi

if test "x$have_SDL" = "xno" ; then
    have_trouble=yes
    AC_MSG_WARN([
*** PROBLEM:
*** Unable to find a version of SDL that was never than 1.2.2
*** This is required for yanoid to work.
*** Most distributions include at least SDL 1.2.2 - never
*** versions may be found at http://www.libsdl.org
])
fi

if test "x$have_SDL_image" = "xno" ; then 
    have_trouble=yes
    AC_MSG_WARN([
*** PROBLEM:
*** Unable to find SDL_image library
*** This is required for yanoid to work.
*** Most Linux distributions include at least SDL_image 1.1 - never
*** versions may be found at http://www.libsdl.org
])
fi

if test "x$have_SDL_mixer" = "xno" ; then 
    have_trouble=yes
    AC_MSG_WARN([
*** PROBLEM:
*** Unable to find SDL_mixer library (optional)
*** This is required for music and sound support in yanoid
*** and is required for yanoid to work.
*** Most Linux distributions include at least SDL_mixer 1.1 - never
*** versions may be found at http://www.libsdl.org
])
fi

if test "x$mbd_check_python_failure" != "xno"; then
    have_trouble=yes
    AC_MSG_WARN([
*** PROBLEM:
*** There was a problem with your Python installation:
*** ${mbd_check_python_msg}
*** Please check that you have a reasonable up-to-date Python
*** installation and that you have the program "python" somewhere 
*** in your path. If you use a package based system, you may need to
*** install a "python-devel" package or similar.
*** Fixing this problem is required for yanoid to work.
*** Most Linux distributions include at least Python 2.0 - newer
*** version may be found at http://www.python.org
])
fi

dnl Final check, test for any troubles
if test "x$have_trouble" != "xno" ; then
    AC_MSG_ERROR([Unable to continue])
fi

AC_MSG_RESULT(no)

dnl ------------------------------------------------------------
dnl Set final flags, output Makefiles.
dnl ------------------------------------------------------------

dnl Set the final compiler and linker flags

CPPFLAGS="$YANOID_CPPFLAGS"
CFLAGS="NOTUSEDIRECKON $YANOID_CFLAGS"
CXXFLAGS="$YANOID_CXXFLAGS"
LIBS="$YANOID_LIBS"

AC_OUTPUT(src/Makefile data/Makefile data/graphics/Makefile data/graphics/fonts/Makefile data/graphics/objects/Makefile data/graphics/balls/Makefile data/graphics/paddles/Makefile data/graphics/bricks/Makefile data/graphics/powerups/Makefile data/graphics/shots/Makefile data/sounds/Makefile data/music/Makefile data/scripts/Makefile data/maps/Makefile data/objects/Makefile Makefile yanoid.spec)
