/* simple extension for PHP */

#ifndef PHP_SIMPLE_H
# define PHP_SIMPLE_H

extern zend_module_entry simple_module_entry;
# define phpext_simple_ptr &simple_module_entry

# define PHP_SIMPLE_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_SIMPLE)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_SIMPLE_H */

