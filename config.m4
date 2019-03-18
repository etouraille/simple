dnl config.m4 for extension simple

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(simple, for simple support,
dnl Make sure that the comment is aligned:
dnl [  --with-simple             Include simple support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(simple, whether to enable simple support,
dnl Make sure that the comment is aligned:
[  --enable-simple          Enable simple support], no)

if test "$PHP_SIMPLE" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, SIMPLE_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-simple -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/simple.h"  # you most likely want to change this
  dnl if test -r $PHP_SIMPLE/$SEARCH_FOR; then # path given as parameter
  dnl   SIMPLE_DIR=$PHP_SIMPLE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for simple files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SIMPLE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SIMPLE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the simple distribution])
  dnl fi

  dnl # --with-simple -> add include path
  dnl PHP_ADD_INCLUDE($SIMPLE_DIR/include)

  dnl # --with-simple -> check for lib and symbol presence
  dnl LIBNAME=SIMPLE # you may want to change this
  dnl LIBSYMBOL=SIMPLE # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SIMPLE_DIR/$PHP_LIBDIR, SIMPLE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SIMPLELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong simple lib version or lib not found])
  dnl ],[
  dnl   -L$SIMPLE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SIMPLE_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_SIMPLE, 1, [ Have simple support ])

  PHP_NEW_EXTENSION(simple, simple.c, $ext_shared)
fi
