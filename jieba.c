/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_jieba.h"

ZEND_DECLARE_MODULE_GLOBALS(jieba)

static int le_jieba;
static zend_class_entry *jieba_ce = NULL;

const zend_function_entry jieba_functions[] = {
    PHP_ME(jieba, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(jieba, __destruct,  NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
    PHP_ME(jieba, cut, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(jieba, cut_all, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(jieba, cut_for_search, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(jieba, extract_tags, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(jieba, cut_tagging, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(jieba, insert_word, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(jieba, del_word, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in jieba_functions[] */
};
/* }}} */




/* {{{ jieba_module_entry
 */
zend_module_entry jieba_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"jieba",
	jieba_functions,
	PHP_MINIT(jieba),
	PHP_MSHUTDOWN(jieba),
	PHP_RINIT(jieba),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(jieba),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(jieba),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_JIEBA_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_JIEBA
ZEND_GET_MODULE(jieba)
#endif



PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("jieba.dict", "/etc/jieba/jieba.dict.utf8", PHP_INI_ALL, OnUpdateString, dict_path, zend_jieba_globals, jieba_globals)
	STD_PHP_INI_ENTRY("jieba.hmm", "/etc/jieba/hmm_model.utf8", PHP_INI_ALL, OnUpdateString, hmm_path, zend_jieba_globals, jieba_globals)
	STD_PHP_INI_ENTRY("jieba.user", "/etc/jieba/user.dict.utf8", PHP_INI_ALL, OnUpdateString, user_path, zend_jieba_globals, jieba_globals)
	STD_PHP_INI_ENTRY("jieba.idf", "/etc/jieba/idf.utf8", PHP_INI_ALL, OnUpdateString, idf_path, zend_jieba_globals, jieba_globals)
	STD_PHP_INI_ENTRY("jieba.stop_words", "/etc/jieba/stop_words.utf8", PHP_INI_ALL, OnUpdateString, stop_words_path, zend_jieba_globals, jieba_globals)
PHP_INI_END()


static void php_jieba_init_globals(zend_jieba_globals *jieba_globals)
{
	jieba_globals->jieba_obj    = NULL;
	//jieba_globals->dict_path    = NULL;
	//jieba_globals->hmm_path     = NULL;
	//jieba_globals->user_path = NULL;
	//jieba_globals->idf_path = NULL;
	//jieba_globals->stop_words_path = NULL;
}



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(jieba)
{
	REGISTER_INI_ENTRIES();
	ZEND_INIT_MODULE_GLOBALS(jieba, php_jieba_init_globals, NULL);
	zend_class_entry jieba_entry; 
	INIT_CLASS_ENTRY(jieba_entry, "jieba", jieba_functions);
    //jieba_ce = zend_register_internal_class_ex(&jieba_entry,NULL,NULL TSRMLS_CC);
	jieba_ce = zend_register_internal_class(&jieba_entry TSRMLS_CC); 
	
	const char* DICT_PATH = JIEBA_G(dict_path);
	const char* HMM_PATH = JIEBA_G(hmm_path);
	const char* USER_DICT = JIEBA_G(user_path);
	const char* IDF_PATH = JIEBA_G(idf_path);
	const char* STOP_WORD_PATH = JIEBA_G(stop_words_path);
	if (access(DICT_PATH, R_OK|F_OK) != 0 || access(HMM_PATH, R_OK|F_OK) != 0 || access(USER_DICT, R_OK|F_OK) != 0 || access(IDF_PATH, R_OK|F_OK) != 0 || access(STOP_WORD_PATH, R_OK|F_OK) != 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to load the dict file");
		return FAILURE;
	}
	JIEBA_G(jieba_obj) = NewJieba(DICT_PATH, HMM_PATH, USER_DICT, IDF_PATH, STOP_WORD_PATH);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(jieba)
{
	UNREGISTER_INI_ENTRIES();
	 FreeJieba(JIEBA_G(jieba_obj));
	//efree(JIEBA_G(jieba_obj));
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(jieba)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(jieba)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(jieba)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "jieba support", "enabled");
	php_info_print_table_row(2, "version", PHP_JIEBA_VERSION);
	php_info_print_table_row(2, "jieba Description", "Jieba (Chinese for to stutter) Chinese text segmentation");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

PHP_METHOD(jieba, __construct) {	
}

PHP_METHOD(jieba, __destruct) {
}

PHP_METHOD(jieba, cut) {
	char *sentence;
	int sentence_len;	
	zend_bool hmm = 0;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &sentence, &sentence_len,&hmm) == FAILURE) {
			return;
	}
	if (!sentence_len)
    {
        RETURN_NULL();
    }
	CJiebaWord *words = Cut(JIEBA_G(jieba_obj), sentence, sentence_len, hmm TSRMLS_CC); 
	CJiebaWord* x;
	array_init(return_value);
    for (x = words; x && x->word; x++) {
        add_next_index_string(return_value, x->word, 1);
   }
   FreeWords(words);
}

PHP_METHOD(jieba, cut_all) {
	char *sentence;
	int sentence_len;	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sentence, &sentence_len) == FAILURE) {
			return;
	}
	if (!sentence_len)
    {
        RETURN_NULL();
    }
	CJiebaWord *words = CutAll(JIEBA_G(jieba_obj), sentence, sentence_len); 
	CJiebaWord* x;
	array_init(return_value);
    for (x = words; x && x->word; x++) {
        add_next_index_string(return_value, x->word, 1);
   }
   FreeWords(words);
}

PHP_METHOD(jieba, cut_for_search) {
	char *sentence;
	int sentence_len;	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sentence, &sentence_len) == FAILURE) {
			return;
	}
	if (!sentence_len)
    {
        RETURN_NULL();
    }
	CJiebaWord *words = CutForSearch(JIEBA_G(jieba_obj), sentence, sentence_len); 
	CJiebaWord* x;
	array_init(return_value);
    for (x = words; x && x->word; x++) {
        add_next_index_string(return_value, x->word, 1);
   }
   FreeWords(words);
}

PHP_METHOD(jieba, cut_tagging) {
	char *sentence;
	int sentence_len;	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sentence, &sentence_len) == FAILURE) {
			return;
	}
	if (!sentence_len)
    {
        RETURN_NULL();
    }
	CJiebaWord *words = CutWithTagging(JIEBA_G(jieba_obj), sentence, sentence_len); 
	CJiebaWord* x;
	array_init(return_value);
    zval *row;
	for (x = words; x && x->word; x++) {
		MAKE_STD_ZVAL(row);
		array_init(row);
		add_assoc_stringl(row, "word", (char *) x->word, x->len, 1);
		add_assoc_string(row, "tagging", (char *) x->tagging, 1);
		add_next_index_zval(return_value, row);
#if PHP_MAJOR_VERSION >= 7
		efree(row);
#endif
	}
   FreeWords(words);
}

PHP_METHOD(jieba, extract_tags) {
	char *sentence;
	int sentence_len;
	long top_k = 5;
	zend_bool is_weight = 0;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|lb", &sentence, &sentence_len, &top_k, &is_weight) == FAILURE) {
			return;
	}
	if (!sentence_len)
    {
        RETURN_NULL();
    }
	CJiebaWord *words = Extract(JIEBA_G(jieba_obj), sentence, sentence_len, top_k); 
	CJiebaWord* x;
	array_init(return_value);
	if (is_weight)
	{
		zval *row;
		for (x = words; x && x->word; x++) {
			MAKE_STD_ZVAL(row);
			array_init(row);
			add_assoc_stringl(row, "word", (char *) x->word, x->len, 1);
			add_assoc_double(row, "weight", (double) x->weight);
			add_next_index_zval(return_value, row);
#if PHP_MAJOR_VERSION >= 7
			efree(row);
#endif
		}
	}else{
		for (x = words; x && x->word; x++) {
			add_next_index_string(return_value, x->word, 1);
	   }
	}
    
   FreeWords(words);
}

PHP_METHOD(jieba, insert_word) {
	char *word;
	int word_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE) {
			return;
	}
	if (!word_len)
    {
        RETURN_BOOL(0);
    }
	zend_bool b = JiebaInsertUserWord(JIEBA_G(jieba_obj), word);
	RETURN_BOOL(b);
}



PHP_METHOD(jieba, del_word) {
    char *word;
    int word_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE) {
            return;
    }
    if (!word_len)
    {
        RETURN_BOOL(0);
    }
    zend_bool b = JiebaInsertUserWord(JIEBA_G(jieba_obj), word);
    RETURN_BOOL(b);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
