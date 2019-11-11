#
# Process this file with autoconf to produce a configure script.




# SUBROUTINE
#
# AC_DEFINE_SUBST(NAME, VALUE, DESCRIPTION)
# -----------------------------------------
AC_DEFUN([AC_DEFINE_SUBST], [
AC_DEFINE([$1], [$2], [$3])
AC_SUBST([$1], ['$2'])
])


m4_define([pkg_major], [0]) dnl
m4_define([pkg_minor], [1]) dnl
m4_define([pkg_patch], [0]) dnl
m4_define([pkg_version], [pkg_major[.]pkg_minor[.]pkg_patch]) dnl


AC_PREREQ([2.69])
AC_INIT(teobase, [pkg_version], [internal lib])

AC_SUBST([DOLLAR_SIGN],[$])

AC_DEFINE_SUBST(LIBRARY_CURRENT,  1, [teobase dynamic library version])
AC_DEFINE_SUBST(LIBRARY_REVISION, 0, [teobase dynamic library version])
AC_DEFINE_SUBST(LIBRARY_AGE,      1, [teobase dynamic library version])

AC_DEFINE([PACKAGE_DESCRIPTION], ["teobase lib"], [Application description])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/teobase/socket.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])


AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_PROG_CXX
AC_PROG_CC
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

LT_PREREQ([2.4])
LT_INIT

# Checks for doxygen
AC_CHECK_PROG(HAVE_DOXYGEN, doxygen, true, false)
AM_CONDITIONAL(HAVE_DOXYGEN,$HAVE_DOXYGEN)
if test $HAVE_DOXYGEN = "false"; then
    AC_MSG_WARN([*** doxygen not found, docs will not be built])
else

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(Teobase, Doxyfile, docs)

fi

AC_CONFIG_FILES([
        Makefile
        src/Makefile
])

AC_OUTPUT
