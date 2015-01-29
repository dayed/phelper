
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/array.h"


/*
 +------------------------------------------------------------------------+
 | Phalcon Helpers                                                        |
 +------------------------------------------------------------------------+
 | Copyright (c) 2015 Serghei Iakovlev (http://klay.me)                   |
 | Copyright (c) 2007-2012 Kohana Team                                    |
 | Copyright (c) 2005 Harry Fuecks                                        |
 +------------------------------------------------------------------------+
 | This library is free software; you can redistribute it and/or          |
 | modify it under the terms of the GNU Lesser General Public             |
 | License as published by the Free Software Foundation; either           |
 | version 2.1 of the License, or (at your option) any later version.     |
 |                                                                        |
 | This library is distributed in the hope that it will be useful,        |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      |
 | Lesser General Public License for more details.                        |
 |                                                                        |
 | You should have received a copy of the GNU Lesser General Public       |
 | License along with this library; if not, write to the                  |
 | Free Software Foundation,                                              |
 | Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA     |
 +------------------------------------------------------------------------+
 | Author: Serghei Iakovlev <sadhooklay@gmail.com>                        |
 +------------------------------------------------------------------------+
 */
/**
 * Unicode manipulation class.
 * Provides multi-byte aware replacement string functions and other Unicode utils.
 * This class contains some part from port of [phputf8](http://phputf8.sourceforge.net/) to a unified set of files.
 *
 * Supported encodings:
 *
 * - UTF-8
 * - UTF-16
 * - UTF-32
 *
 * For UTF-8 support to work correctly, the following requirements must be met:
 *
 * - PCRE needs to be compiled with UTF-8 support (--enable-utf8)
 * - Support for [Unicode properties](http://php.net/manual/reference.pcre.pattern.modifiers.php) is highly recommended (--enable-unicode-properties)
 * - The [mbstring extension](http://php.net/mbstring) is highly recommended, but must not be overloading string functions   
 *
 * @package Phelper
 * @version 0.0.1-dev
 */
ZEPHIR_INIT_CLASS(Phelper_Utf) {

	ZEPHIR_REGISTER_CLASS(Phelper, Utf, phelper, utf, phelper_utf_method_entry, 0);

	/**
	 * Does the server support UTF natively?
	 * @var boolean
	 */
	zend_declare_property_bool(phelper_utf_ce, SL("_mbSupport"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * Encoding using in mb_* functions
	 * @var string
	 */
	zend_declare_property_string(phelper_utf_ce, SL("_encoding"), "utf-8", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_class_constant_string(phelper_utf_ce, SL("UTF_8"), "utf-8" TSRMLS_CC);

	zend_declare_class_constant_string(phelper_utf_ce, SL("UTF_16"), "utf-16" TSRMLS_CC);

	zend_declare_class_constant_string(phelper_utf_ce, SL("UTF_32"), "utf-32" TSRMLS_CC);

	return SUCCESS;

}

/**
 * Class constructor
 * @param string encoding Encoding using in mb_* functions
 */
PHP_METHOD(Phelper_Utf, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *encoding_param = NULL, _0, *_1 = NULL, *_3;
	zval *encoding = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &encoding_param);

	if (!encoding_param) {
		ZEPHIR_INIT_VAR(encoding);
		ZVAL_STRING(encoding, "utf-8", 1);
	} else {
	if (unlikely(Z_TYPE_P(encoding_param) != IS_STRING && Z_TYPE_P(encoding_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'encoding' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(encoding_param) == IS_STRING)) {
		zephir_get_strval(encoding, encoding_param);
	} else {
		ZEPHIR_INIT_VAR(encoding);
		ZVAL_EMPTY_STRING(encoding);
	}
	}


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "mbstring", 0);
	ZEPHIR_CALL_FUNCTION(&_1, "extension_loaded", &_2, &_0);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_mbSupport"), _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	zephir_fast_strtolower(_3, encoding);
	zephir_update_property_this(this_ptr, SL("_encoding"), _3 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Get a BOM (Byte Order Mark).
 * It defines if a document is encoded with big or little endian, and should be in begining of document.
 *
 * @return mixed a BOM (string) or NULL if encoding not supported
 */
PHP_METHOD(Phelper_Utf, getBom) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	zval *bigEndian_param = NULL, *_0, _1 = zval_used_for_init, *_2 = NULL, *_4 = NULL, *_5 = NULL, *_6 = NULL;
	zend_bool bigEndian;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &bigEndian_param);

	if (!bigEndian_param) {
		bigEndian = 1;
	} else {
		bigEndian = zephir_get_boolval(bigEndian_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
	do {
		if (ZEPHIR_IS_STRING(_0, "utf-8")) {
			ZEPHIR_SINIT_VAR(_1);
			ZVAL_LONG(&_1, 0xEF);
			ZEPHIR_CALL_FUNCTION(&_2, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, 0xBB);
			ZEPHIR_CALL_FUNCTION(&_4, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, 0xBF);
			ZEPHIR_CALL_FUNCTION(&_5, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_CONCAT_VVV(return_value, _2, _4, _5);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(_0, "utf-16")) {
			if (bigEndian) {
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_LONG(&_1, 0xFE);
				ZEPHIR_CALL_FUNCTION(&_2, "chr", &_3, &_1);
				zephir_check_call_status();
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_LONG(&_1, 0xFF);
				ZEPHIR_CALL_FUNCTION(&_4, "chr", &_3, &_1);
				zephir_check_call_status();
				ZEPHIR_CONCAT_VV(return_value, _2, _4);
				RETURN_MM();
			}
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, 0xFF);
			ZEPHIR_CALL_FUNCTION(&_5, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, 0xFE);
			ZEPHIR_CALL_FUNCTION(&_6, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_CONCAT_VV(return_value, _5, _6);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(_0, "utf-32")) {
			if (bigEndian) {
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_LONG(&_1, 0xFE);
				ZEPHIR_CALL_FUNCTION(&_2, "chr", &_3, &_1);
				zephir_check_call_status();
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_LONG(&_1, 0xFF);
				ZEPHIR_CALL_FUNCTION(&_4, "chr", &_3, &_1);
				zephir_check_call_status();
				ZEPHIR_CONCAT_VV(return_value, _2, _4);
				RETURN_MM();
			}
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, 0xFF);
			ZEPHIR_CALL_FUNCTION(&_5, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, 0xFE);
			ZEPHIR_CALL_FUNCTION(&_6, "chr", &_3, &_1);
			zephir_check_call_status();
			ZEPHIR_CONCAT_VV(return_value, _5, _6);
			RETURN_MM();
		}
		RETURN_MM_NULL();
	} while(0);

	RETURN_MM_NULL();

}

/**
 * Tests whether a string contains only 7-bit ASCII bytes.
 * This is used to determine when to use native functions or UTF-functions.
 *
 * <code>
 * $ascii = $utf->isAscii($string);
 * </code>
 *
 * @param mixed text String or array of strings to check
 */
PHP_METHOD(Phelper_Utf, isAscii) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *text = NULL, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &text);

	ZEPHIR_SEPARATE_PARAM(text);


	if (Z_TYPE_P(text) == IS_ARRAY) {
		ZEPHIR_INIT_VAR(_0);
		zephir_fast_join_str(_0, SL(""), text TSRMLS_CC);
		ZEPHIR_CPY_WRT(text, _0);
	}
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "/[^\\x00-\\x7F]/S", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_FUNCTION(&_1, "preg_match", &_2, _0, text);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM_BOOL(!zephir_is_true(_1));

}

/**
 * Get string length.
 * This is a UTF8-aware version of [strlen](http://php.net/strlen).
 *
 * <code>
 * $length = $utf->strlen($string);
 * </code>
 *
 * @param string text String being measured for length
 */
PHP_METHOD(Phelper_Utf, strlen) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *text_param = NULL, *_0, *_1, *_2 = NULL, *_3 = NULL;
	zval *text = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &text_param);

	if (unlikely(Z_TYPE_P(text_param) != IS_STRING && Z_TYPE_P(text_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'text' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(text_param) == IS_STRING)) {
		zephir_get_strval(text, text_param);
	} else {
		ZEPHIR_INIT_VAR(text);
		ZVAL_EMPTY_STRING(text);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_mbSupport"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("mb_strlen", NULL, text, _1);
		zephir_check_call_status();
		RETURN_MM();
	}
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "isascii", NULL, text);
	zephir_check_call_status();
	if (zephir_is_true(_2)) {
		RETURN_MM_LONG(zephir_fast_strlen_ev(text));
	}
	ZEPHIR_CALL_FUNCTION(&_3, "utf8_decode", NULL, text);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_3, "length", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Get part of a UTF-string.
 * This is a UTF8-aware version of [substr](http://php.net/substr).
 *
 * <code>
 * $sub = $utf->substr($string, $offset);
 * </code>
 */
PHP_METHOD(Phelper_Utf, substr) {

	zend_bool _13, _14, _15, _16, _17;
	zephir_nts_static zephir_fcall_cache_entry *_8 = NULL, *_11 = NULL, *_23 = NULL, *_24 = NULL;
	zephir_fcall_cache_entry *_6 = NULL;
	zval *matches;
	int offset, x = 0, y = 0, ZEPHIR_LAST_CALL_STATUS;
	zval *text_param = NULL, *offset_param = NULL, *length = NULL, *stringLen = NULL, *_0, *_1 = NULL, *_2, *_3 = NULL, *_4, _5 = zval_used_for_init, *_7, *_9 = NULL, *_10, *_12 = NULL, _18 = zval_used_for_init, _19 = zval_used_for_init, *_21 = NULL, _22, *_25;
	zval *text = NULL, *regex, *_20 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &text_param, &offset_param, &length);

	if (unlikely(Z_TYPE_P(text_param) != IS_STRING && Z_TYPE_P(text_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'text' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(text_param) == IS_STRING)) {
		zephir_get_strval(text, text_param);
	} else {
		ZEPHIR_INIT_VAR(text);
		ZVAL_EMPTY_STRING(text);
	}
	if (unlikely(Z_TYPE_P(offset_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'offset' must be a long/integer") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	offset = Z_LVAL_P(offset_param);
	if (!length) {
		ZEPHIR_CPY_WRT(length, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(length);
	}
	ZEPHIR_INIT_VAR(matches);
	array_init(matches);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_mbSupport"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		ZEPHIR_INIT_VAR(_1);
		if (Z_TYPE_P(length) == IS_NULL) {
			_2 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&_3, "mb_strlen", NULL, text, _2);
			zephir_check_call_status();
			_4 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
			ZEPHIR_SINIT_VAR(_5);
			ZVAL_LONG(&_5, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "mb_substr", &_6, text, &_5, _3, _4);
			zephir_check_call_status();
		} else {
			_7 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
			ZEPHIR_SINIT_NVAR(_5);
			ZVAL_LONG(&_5, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "mb_substr", &_6, text, &_5, length, _7);
			zephir_check_call_status();
		}
		RETURN_CCTOR(_1);
	}
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "isascii", NULL, text);
	zephir_check_call_status();
	if (zephir_is_true(_3)) {
		ZEPHIR_INIT_NVAR(_1);
		if (Z_TYPE_P(length) == IS_NULL) {
			ZEPHIR_SINIT_NVAR(_5);
			ZVAL_LONG(&_5, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "substr", &_8, text, &_5);
			zephir_check_call_status();
		} else {
			ZEPHIR_SINIT_NVAR(_5);
			ZVAL_LONG(&_5, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "substr", &_8, text, &_5, length);
			zephir_check_call_status();
		}
		RETURN_CCTOR(_1);
	}
	ZEPHIR_CALL_METHOD(&stringLen, this_ptr, "strlen", NULL, text);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_9);
	if (offset < 0) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, 0);
		ZEPHIR_INIT_VAR(_10);
		ZVAL_LONG(_10, (zephir_get_numberval(stringLen) + offset));
		ZEPHIR_CALL_FUNCTION(&_9, "max", &_11, _1, _10);
		zephir_check_call_status();
	} else {
		ZEPHIR_SINIT_NVAR(_5);
		ZVAL_LONG(&_5, offset);
		ZEPHIR_INIT_NVAR(_9);
		ZVAL_LONG(_9, zephir_get_intval(&_5));
	}
	offset = zephir_get_numberval(_9);
	ZEPHIR_INIT_VAR(_12);
	if (Z_TYPE_P(length) == IS_NULL) {
		ZEPHIR_INIT_NVAR(_12);
		ZVAL_NULL(_12);
	} else {
		ZEPHIR_INIT_NVAR(_12);
		ZVAL_LONG(_12, zephir_get_intval(length));
	}
	ZEPHIR_CPY_WRT(length, _12);
	_13 = ZEPHIR_IS_LONG_IDENTICAL(length, 0);
	if (!(_13)) {
		_13 = ZEPHIR_LT_LONG(stringLen, offset);
	}
	_14 = _13;
	if (!(_14)) {
		_15 = ZEPHIR_LT_LONG(length, 0);
		if (_15) {
			_15 = ZEPHIR_LE_LONG(length, (offset - zephir_get_numberval(stringLen)));
		}
		_14 = _15;
	}
	if (_14) {
		RETURN_MM_STRING("", 1);
	}
	_16 = offset == 0;
	if (_16) {
		_17 = Z_TYPE_P(length) == IS_NULL;
		if (!(_17)) {
			_17 = ZEPHIR_GE(length, stringLen);
		}
		_16 = _17;
	}
	if (_16) {
		RETURN_CTOR(text);
	}
	ZEPHIR_INIT_VAR(regex);
	ZVAL_STRING(regex, "^", 1);
	if (offset > 0) {
		ZEPHIR_SINIT_NVAR(_5);
		ZVAL_DOUBLE(&_5, zephir_safe_div_long_long(offset, 65535 TSRMLS_CC));
		x = zephir_get_intval(&_5);
		ZEPHIR_SINIT_VAR(_18);
		ZVAL_LONG(&_18, (offset % 65535));
		y = zephir_get_intval(&_18);
		ZEPHIR_INIT_LNVAR(_12);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_12);
			ZVAL_STRING(_12, "", 1);
		} else {
			ZEPHIR_SINIT_VAR(_19);
			ZVAL_LONG(&_19, x);
			ZEPHIR_INIT_VAR(_20);
			ZEPHIR_CONCAT_SVS(_20, "(?:.{65535}){", &_19, "}");
			ZEPHIR_CPY_WRT(_12, _20);
		}
		zephir_concat_self(&regex, _12 TSRMLS_CC);
		ZEPHIR_INIT_VAR(_21);
		if (y == 0) {
			ZEPHIR_INIT_NVAR(_21);
			ZVAL_STRING(_21, "", 1);
		} else {
			ZEPHIR_SINIT_VAR(_22);
			ZVAL_LONG(&_22, y);
			ZEPHIR_INIT_LNVAR(_20);
			ZEPHIR_CONCAT_SVS(_20, ".{", &_22, "}");
			ZEPHIR_CPY_WRT(_21, _20);
		}
		zephir_concat_self(&regex, _21 TSRMLS_CC);
	}
	if (Z_TYPE_P(length) == IS_NULL) {
		zephir_concat_self_str(&regex, "(.*)", sizeof("(.*)")-1 TSRMLS_CC);
	} else if (ZEPHIR_GT_LONG(length, 0)) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, (zephir_get_numberval(stringLen) - offset));
		ZEPHIR_CALL_FUNCTION(&_3, "min", &_23, _1, length);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(length, _3);
		ZEPHIR_SINIT_NVAR(_5);
		ZVAL_DOUBLE(&_5, zephir_safe_div_zval_long(length, 65535 TSRMLS_CC));
		x = zephir_get_intval(&_5);
		y = (zephir_get_intval(length) % 65535);
		zephir_concat_self_str(&regex, "(", sizeof("(")-1 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_21);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_21);
			ZVAL_STRING(_21, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_18);
			ZVAL_LONG(&_18, x);
			ZEPHIR_INIT_LNVAR(_20);
			ZEPHIR_CONCAT_SVS(_20, "(?:.{65535}){", &_18, "}");
			ZEPHIR_CPY_WRT(_21, _20);
		}
		zephir_concat_self(&regex, _21 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_19);
		ZVAL_LONG(&_19, y);
		ZEPHIR_INIT_LNVAR(_20);
		ZEPHIR_CONCAT_SVS(_20, ".{", &_19, "})");
		zephir_concat_self(&regex, _20 TSRMLS_CC);
	} else {
		zephir_negate(length TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_5);
		ZVAL_DOUBLE(&_5, zephir_safe_div_zval_long(length, 65535 TSRMLS_CC));
		x = zephir_get_intval(&_5);
		zephir_negate(length TSRMLS_CC);
		y = (zephir_get_intval(length) % 65535);
		zephir_concat_self_str(&regex, "(.*)", sizeof("(.*)")-1 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_21);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_21);
			ZVAL_STRING(_21, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_18);
			ZVAL_LONG(&_18, x);
			ZEPHIR_INIT_LNVAR(_20);
			ZEPHIR_CONCAT_SVS(_20, "(?:.{65535}){", &_18, "}");
			ZEPHIR_CPY_WRT(_21, _20);
		}
		zephir_concat_self(&regex, _21 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_19);
		ZVAL_LONG(&_19, y);
		ZEPHIR_INIT_LNVAR(_20);
		ZEPHIR_CONCAT_SVS(_20, ".{", &_19, "}");
		zephir_concat_self(&regex, _20 TSRMLS_CC);
	}
	ZEPHIR_INIT_LNVAR(_20);
	ZEPHIR_CONCAT_SVS(_20, "#", regex, "#us");
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_24, _20, text, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	zephir_array_fetch_long(&_25, matches, 1, PH_NOISY | PH_READONLY, "phelper/utf.zep", 228 TSRMLS_CC);
	RETURN_CTOR(_25);

}

/**
 * Strips out device control codes in the ASCII range.
 *
 * <code>
 * $string = $utf->stripAsciiCtrl($string);
 * </code>
 */
PHP_METHOD(Phelper_Utf, stripAsciiCtrl) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *text_param = NULL, *_0, *_1;
	zval *text = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &text_param);

	if (unlikely(Z_TYPE_P(text_param) != IS_STRING && Z_TYPE_P(text_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'text' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(text_param) == IS_STRING)) {
		zephir_get_strval(text, text_param);
	} else {
		ZEPHIR_INIT_VAR(text);
		ZVAL_EMPTY_STRING(text);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "#[\\x00-\\x08\\x0B\\x0C\\x0E-\\x1F\\x7F]+#S", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", &_2, _0, _1, text);
	zephir_check_temp_parameter(_0);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Strips out all non-7bit ASCII bytes.
 *
 * <code>
 * $string = $utf->stripNonAscii($string);
 * <code>
 */
PHP_METHOD(Phelper_Utf, stripNonAscii) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *text_param = NULL, *_0, *_1;
	zval *text = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &text_param);

	if (unlikely(Z_TYPE_P(text_param) != IS_STRING && Z_TYPE_P(text_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'text' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(text_param) == IS_STRING)) {
		zephir_get_strval(text, text_param);
	} else {
		ZEPHIR_INIT_VAR(text);
		ZVAL_EMPTY_STRING(text);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "#[^\\x00-\\x7F]+#S", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", &_2, _0, _1, text);
	zephir_check_temp_parameter(_0);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Strips whitespace (or other UTF-characters) from the beginning of a string.
 * This is a UTF8-aware version of [ltrim](http://php.net/ltrim).
 *
 * <code>
 * $string = $utf->ltrim($string);
 * </code>
 *
 * @author Andreas Gohr <andi@splitbrain.org>
 * @param string text Input string
 * @param string charList String of characters (or array of chars) to remove
 */
PHP_METHOD(Phelper_Utf, ltrim) {

	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *text_param = NULL, *charList = NULL, *_1 = NULL, *_2 = NULL, *_3, *_4 = NULL, *_6;
	zval *text = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &text_param, &charList);

	if (unlikely(Z_TYPE_P(text_param) != IS_STRING && Z_TYPE_P(text_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'text' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(text_param) == IS_STRING)) {
		zephir_get_strval(text, text_param);
	} else {
		ZEPHIR_INIT_VAR(text);
		ZVAL_EMPTY_STRING(text);
	}
	if (!charList) {
		ZEPHIR_CPY_WRT(charList, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(charList);
	}


	_0 = Z_TYPE_P(charList) == IS_NULL;
	if (!(_0)) {
		_0 = ZEPHIR_IS_FALSE_IDENTICAL(charList);
	}
	if (_0) {
		ZEPHIR_INIT_VAR(_1);
		zephir_fast_trim(_1, text, NULL , ZEPHIR_TRIM_LEFT TSRMLS_CC);
		RETURN_CCTOR(_1);
	}
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "isascii", NULL, text);
	zephir_check_call_status();
	if (zephir_is_true(_2)) {
		ZEPHIR_INIT_NVAR(_1);
		zephir_fast_trim(_1, text, charList, ZEPHIR_TRIM_LEFT TSRMLS_CC);
		RETURN_CCTOR(_1);
	}
	if (Z_TYPE_P(charList) == IS_ARRAY) {
		ZEPHIR_INIT_NVAR(_1);
		zephir_fast_join_str(_1, SL(""), charList TSRMLS_CC);
		ZEPHIR_CPY_WRT(charList, _1);
	}
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "#[-\\[\\]:\\\\^/]#", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "\\\\$0", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_replace", &_5, _1, _3, charList);
	zephir_check_temp_parameter(_1);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(charList, _4);
	ZEPHIR_INIT_VAR(_6);
	ZEPHIR_CONCAT_SVS(_6, "/^[", charList, "]+/u");
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", &_5, _6, _1, text);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Strips whitespace (or other UTF-characters) from the end of a string.
 * This is a UTF8-aware version of [rtrim](http://php.net/rtrim).
 *
 * <code>
 * $string = $utf->rtrim($string);
 * </code>
 *
 * @@author Harry Fuecks <hfuecks@gmail.com>
 * @param string text Input string
 * @param string charList String of characters (or array of chars) to remove
 */
PHP_METHOD(Phelper_Utf, rtrim) {

	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *text_param = NULL, *charList = NULL, *_1 = NULL, *_2 = NULL, *_3, *_4 = NULL, *_6;
	zval *text = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &text_param, &charList);

	if (unlikely(Z_TYPE_P(text_param) != IS_STRING && Z_TYPE_P(text_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'text' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(text_param) == IS_STRING)) {
		zephir_get_strval(text, text_param);
	} else {
		ZEPHIR_INIT_VAR(text);
		ZVAL_EMPTY_STRING(text);
	}
	if (!charList) {
		ZEPHIR_CPY_WRT(charList, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(charList);
	}


	_0 = Z_TYPE_P(charList) == IS_NULL;
	if (!(_0)) {
		_0 = ZEPHIR_IS_FALSE_IDENTICAL(charList);
	}
	if (_0) {
		ZEPHIR_INIT_VAR(_1);
		zephir_fast_trim(_1, text, NULL , ZEPHIR_TRIM_RIGHT TSRMLS_CC);
		RETURN_CCTOR(_1);
	}
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "isascii", NULL, text);
	zephir_check_call_status();
	if (zephir_is_true(_2)) {
		ZEPHIR_INIT_NVAR(_1);
		zephir_fast_trim(_1, text, charList, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
		RETURN_CCTOR(_1);
	}
	if (Z_TYPE_P(charList) == IS_ARRAY) {
		ZEPHIR_INIT_NVAR(_1);
		zephir_fast_join_str(_1, SL(""), charList TSRMLS_CC);
		ZEPHIR_CPY_WRT(charList, _1);
	}
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "#[-\\[\\]:\\\\^/]#", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "\\\\$0", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_replace", &_5, _1, _3, charList);
	zephir_check_temp_parameter(_1);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(charList, _4);
	ZEPHIR_INIT_VAR(_6);
	ZEPHIR_CONCAT_SVS(_6, "/[", charList, "]++$/uD");
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", &_5, _6, _1, text);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Case-insensitive UTF version of strstr.
 * Returns part of haystack string from the first occurrence of needle to the end of haystack.
 * This is a UTF-aware version of [stristr](http://php.net/stristr).
 * Returns the portion of string, or FALSE if needle is not found.
 *
 * <code>
 * $found = $utf->stristr($string, $search);
 * </code>
 *
 * @param string haystack The input string.
 * @param string needle Needle.
 * @param boolean beforeNeedle [Optional]
 * @return mixed matched substring if found or false if the substring was not found
 */
PHP_METHOD(Phelper_Utf, stristr) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool beforeNeedle;
	zval *stack_param = NULL, *needle, *beforeNeedle_param = NULL, *_0;
	zval *stack = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &stack_param, &needle, &beforeNeedle_param);

	if (unlikely(Z_TYPE_P(stack_param) != IS_STRING && Z_TYPE_P(stack_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'stack' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(stack_param) == IS_STRING)) {
		zephir_get_strval(stack, stack_param);
	} else {
		ZEPHIR_INIT_VAR(stack);
		ZVAL_EMPTY_STRING(stack);
	}
	if (!beforeNeedle_param) {
		beforeNeedle = 0;
	} else {
		beforeNeedle = zephir_get_boolval(beforeNeedle_param);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, 1);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "strstr", NULL, stack, needle, (beforeNeedle ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), _0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * UTF version of strstr.
 * Returns part of haystack string from the first occurrence of needle to the end of haystack.
 * This is a UTF-aware version of [stristr](http://php.net/strstr).
 * Returns the portion of string, or FALSE if needle is not found.
 *
 * <code>
 * $found = $utf->strstr($string, $search);
 * </code>
 *
 * @param string haystack The input string.
 * @param string needle Needle.
 * @param boolean beforeNeedle If TRUE returns the part of the haystack before the first occurrence of the needle (excluding the needle). [Optional]
 * @param boolean caseInsensitive Case insensitive? [Optional]
 * @return mixed matched substring if found or FALSE if the substring was not found
 */
PHP_METHOD(Phelper_Utf, strstr) {

	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL, *_5 = NULL, *_8 = NULL, *_11 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *matches;
	zend_bool beforeNeedle, caseInsensitive, _1;
	zval *stack_param = NULL, *needle, *beforeNeedle_param = NULL, *caseInsensitive_param = NULL, *_0 = NULL, *_2 = NULL, *_3 = NULL, _6, *_7 = NULL, *_9 = NULL, *_10, *_12, *_13 = NULL;
	zval *stack = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 2, &stack_param, &needle, &beforeNeedle_param, &caseInsensitive_param);

	if (unlikely(Z_TYPE_P(stack_param) != IS_STRING && Z_TYPE_P(stack_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'stack' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(stack_param) == IS_STRING)) {
		zephir_get_strval(stack, stack_param);
	} else {
		ZEPHIR_INIT_VAR(stack);
		ZVAL_EMPTY_STRING(stack);
	}
	if (!beforeNeedle_param) {
		beforeNeedle = 0;
	} else {
		beforeNeedle = zephir_get_boolval(beforeNeedle_param);
	}
	if (!caseInsensitive_param) {
		caseInsensitive = 0;
	} else {
		caseInsensitive = zephir_get_boolval(caseInsensitive_param);
	}
	ZEPHIR_INIT_VAR(matches);
	array_init(matches);


	ZEPHIR_CALL_METHOD(&_0, this_ptr, "isascii", NULL, stack);
	zephir_check_call_status();
	_1 = zephir_is_true(_0);
	if (_1) {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "isascii", NULL, needle);
		zephir_check_call_status();
		_1 = zephir_is_true(_2);
	}
	if (_1) {
		ZEPHIR_INIT_VAR(_3);
		if (caseInsensitive) {
			ZEPHIR_CALL_FUNCTION(&_3, "stristr", &_4, stack, needle, (beforeNeedle ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
			zephir_check_call_status();
		} else {
			ZEPHIR_CALL_FUNCTION(&_3, "strstr", &_5, stack, needle, (beforeNeedle ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
			zephir_check_call_status();
		}
		RETURN_CCTOR(_3);
	}
	if (ZEPHIR_IS_STRING(needle, "")) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_SINIT_VAR(_6);
	ZVAL_STRING(&_6, "/", 0);
	ZEPHIR_CALL_FUNCTION(&_7, "preg_quote", &_8, needle, &_6);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_9);
	if (caseInsensitive) {
		ZEPHIR_INIT_NVAR(_9);
		ZVAL_STRING(_9, "i", 1);
	} else {
		ZEPHIR_INIT_NVAR(_9);
		ZVAL_STRING(_9, "", 1);
	}
	ZEPHIR_INIT_VAR(_10);
	ZEPHIR_CONCAT_SVSV(_10, "/^(.*?)", _7, "/us", _9);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_11, _10, stack, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_array_isset_long(matches, 1)) {
		if (beforeNeedle) {
			zephir_array_fetch_long(&_12, matches, 1, PH_NOISY | PH_READONLY, "phelper/utf.zep", 369 TSRMLS_CC);
			RETURN_CTOR(_12);
		} else {
			zephir_array_fetch_long(&_12, matches, 1, PH_NOISY | PH_READONLY, "phelper/utf.zep", 371 TSRMLS_CC);
			ZEPHIR_CALL_METHOD(&_13, this_ptr, "strlen", NULL, _12);
			zephir_check_call_status();
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "substr", NULL, stack, _13);
			zephir_check_call_status();
			RETURN_MM();
		}
	}
	RETURN_MM_BOOL(0);

}

