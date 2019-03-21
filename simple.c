/* simple extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_simple.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void simple_test1()
 */
PHP_FUNCTION(simple_test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "simple");
}
/* }}} */

/* {{{ string simple_test2( [ string $var ] )
 */
PHP_FUNCTION(simple_test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

/*
PHP_FUNCTION( vect ) {
        HashTable  * x;
        HashTable  * y;
        zval * z;
        int i = 0, j = 0;
        zval * val;
        zval * colval, rvc;
        Bucket p,q;

        int l,m;

        ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_ARRAY_HT(x)
        Z_PARAM_ARRAY_HT(y)
        ZEND_PARSE_PARAMETERS_END();



        array_init_size( return_value, l = zend_hash_num_elements( x ));
        zend_hash_real_init( Z_ARRVAL_P(return_value), 1);

        p = Z_ARRVAL_P(return_value)->arData[1];

        //Z_ARRVAL_P(return_value)->arData = x->arData;
        z = emalloc(zend_hash_num_elements( x ) * sizeof(zval));
        val = emalloc(sizeof(zval));
        for(i=0;i<l;i++) {
            q = x->arData[i];
            //Z_ARRVAL_P(return_value)->arData[i].val = p.val;

            array_init_size( z+i , m=zend_hash_num_elements( y ));
            zend_hash_real_init( Z_ARRVAL_P(z+i), 1);
            for(j=0;j<m;j++) {
                p = y->arData[j];
                ZVAL_LONG(val,Z_LVAL(p.val)*Z_LVAL(q.val));
                zend_hash_index_add_new(Z_ARRVAL_P(z+i),j, val);
            }
            zend_hash_index_add_new(Z_ARRVAL_P(return_value),i, z+i);
        }


        efree(z);
        efree(val);

}
*/

PHP_FUNCTION(init_array) {

	zval * val;
	val = malloc(sizeof(zval));

	array_init_size( return_value, 1);
	zend_hash_real_init( Z_ARRVAL_P(return_value), 1);
	ZVAL_LONG(val, 10)
	zend_hash_index_add_new(Z_ARRVAL_P(return_value),0, val);
	zend_hash_index_add_new(Z_ARRVAL_P(return_value),1, val);

}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(simple)
{
#if defined(ZTS) && defined(COMPILE_DL_SIMPLE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(simple)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "simple support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_simple_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_simple_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ simple_functions[]
 */
static const zend_function_entry simple_functions[] = {
	PHP_FE(simple_test1,		arginfo_simple_test1)
	PHP_FE(simple_test2,		arginfo_simple_test2)
	//PHP_FE(vect, NULL)
	PHP_FE(init_array, NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ simple_module_entry
 */
zend_module_entry simple_module_entry = {
	STANDARD_MODULE_HEADER,
	"simple",					/* Extension name */
	simple_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(simple),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(simple),			/* PHP_MINFO - Module info */
	PHP_SIMPLE_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMPLE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(simple)
#endif
