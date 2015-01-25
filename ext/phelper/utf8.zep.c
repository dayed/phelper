
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/string.h"
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
 * A port of [phputf8](http://phputf8.sourceforge.net/) to a unified set of files.
 * Provides multi-byte aware replacement string functions.
 *
 * For UTF-8 support to work correctly, the following requirements must be met:
 *
 * - PCRE needs to be compiled with UTF-8 support (--enable-utf8)
 * - Support for [Unicode properties](http://php.net/manual/reference.pcre.pattern.modifiers.php)
 *   is highly recommended (--enable-unicode-properties)
 * - The [mbstring extension](http://php.net/mbstring) is highly recommended,
 *   but must not be overloading string functions
 *
 * @package Phelper
 * @version 0.0.1-dev
 */
ZEPHIR_INIT_CLASS(Phelper_Utf8) {

	ZEPHIR_REGISTER_CLASS(Phelper, Utf8, phelper, utf8, phelper_utf8_method_entry, 0);

	/**
	 * Does the server support UTF-8 natively?
	 * @var boolean
	 */
	zend_declare_property_bool(phelper_utf8_ce, SL("_mbSupport"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * Encoding using in mb_* functions
	 * @var string
	 */
	zend_declare_property_string(phelper_utf8_ce, SL("_encoding"), "utf-8", ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

/**
 * Class constructor
 * @param string encoding Encoding using in mb_* functions
 */
PHP_METHOD(Phelper_Utf8, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *encoding_param = NULL, _0, *_1 = NULL;
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
	zephir_update_property_this(this_ptr, SL("_encoding"), encoding TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Tests whether a string contains only 7-bit ASCII bytes.
 * This is used to determine when to use native functions or UTF-8 functions.
 *
 * <code>
 *   $ascii = $utf->isAscii($str);
 * </code>
 *
 * @param mixed text String or array of strings to check
 */
PHP_METHOD(Phelper_Utf8, isAscii) {

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
 *   $length = $utf->strlen($str);
 * </code>
 *
 * @param string text String being measured for length
 */
PHP_METHOD(Phelper_Utf8, strlen) {

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
 * Get part of a UTF-8 string.
 * This is a UTF8-aware version of [substr](http://php.net/substr).
 *
 * <code>
 *  $sub = $utf->substr($string, $offset);
 * </code>
 */
PHP_METHOD(Phelper_Utf8, substr) {

	zend_bool _13, _14, _15, _16, _17;
	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL, *_11 = NULL, *_21 = NULL, *_22 = NULL;
	zephir_fcall_cache_entry *_5 = NULL;
	zval *matches;
	int offset, x = 0, y = 0, ZEPHIR_LAST_CALL_STATUS;
	zval *text_param = NULL, *offset_param = NULL, *length = NULL, *stringLen = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3, _4 = zval_used_for_init, *_6, *_9 = NULL, *_10, *_12 = NULL, *_19 = NULL, _20 = zval_used_for_init, *_23;
	zval *text = NULL, *regex, *_8 = NULL, *_18 = NULL;

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
			ZEPHIR_CALL_FUNCTION(&_2, "mb_strlen", NULL, text);
			zephir_check_call_status();
			_3 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
			ZEPHIR_SINIT_VAR(_4);
			ZVAL_LONG(&_4, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "mb_substr", &_5, text, &_4, _2, _3);
			zephir_check_call_status();
		} else {
			_6 = zephir_fetch_nproperty_this(this_ptr, SL("_encoding"), PH_NOISY_CC);
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "mb_substr", &_5, text, &_4, length, _6);
			zephir_check_call_status();
		}
		RETURN_CCTOR(_1);
	}
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "isascii", NULL, text);
	zephir_check_call_status();
	if (zephir_is_true(_2)) {
		ZEPHIR_INIT_NVAR(_1);
		if (Z_TYPE_P(length) == IS_NULL) {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "substr", &_7, text, &_4);
			zephir_check_call_status();
		} else {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, offset);
			ZEPHIR_CALL_FUNCTION(&_1, "substr", &_7, text, &_4, length);
			zephir_check_call_status();
		}
		RETURN_CCTOR(_1);
	}
	zephir_get_strval(_8, text);
	ZEPHIR_CPY_WRT(text, _8);
	ZEPHIR_CALL_METHOD(&stringLen, this_ptr, "strlen", NULL, text);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_9);
	if ((offset < 0)) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, 0);
		ZEPHIR_INIT_VAR(_10);
		ZVAL_LONG(_10, (zephir_get_numberval(stringLen) + offset));
		ZEPHIR_CALL_FUNCTION(&_9, "max", &_11, _1, _10);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_9);
		ZVAL_LONG(_9, offset);
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
		x = (int) (zephir_safe_div_long_long(offset, 65535 TSRMLS_CC));
		y = ((offset % 65535));
		ZEPHIR_INIT_LNVAR(_12);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_12);
			ZVAL_STRING(_12, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, x);
			ZEPHIR_INIT_VAR(_18);
			ZEPHIR_CONCAT_SVS(_18, "(?:.{65535}){", &_4, "}");
			ZEPHIR_CPY_WRT(_12, _18);
		}
		zephir_concat_self(&regex, _12 TSRMLS_CC);
		ZEPHIR_INIT_VAR(_19);
		if (y == 0) {
			ZEPHIR_INIT_NVAR(_19);
			ZVAL_STRING(_19, "", 1);
		} else {
			ZEPHIR_SINIT_VAR(_20);
			ZVAL_LONG(&_20, y);
			ZEPHIR_INIT_LNVAR(_18);
			ZEPHIR_CONCAT_SVS(_18, ".{", &_20, "}");
			ZEPHIR_CPY_WRT(_19, _18);
		}
		zephir_concat_self(&regex, _19 TSRMLS_CC);
	}
	if (Z_TYPE_P(length) == IS_NULL) {
		zephir_concat_self_str(&regex, "(.*)", sizeof("(.*)")-1 TSRMLS_CC);
	} else if (ZEPHIR_GT_LONG(length, 0)) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, (zephir_get_numberval(stringLen) - offset));
		ZEPHIR_CALL_FUNCTION(&_2, "min", &_21, _1, length);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(length, _2);
		x = (int) (zephir_safe_div_long_long(zephir_get_intval(length), 65535 TSRMLS_CC));
		y = ((zephir_get_intval(length) % 65535));
		zephir_concat_self_str(&regex, "(", sizeof("(")-1 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_19);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_19);
			ZVAL_STRING(_19, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, x);
			ZEPHIR_INIT_LNVAR(_18);
			ZEPHIR_CONCAT_SVS(_18, "(?:.{65535}){", &_4, "}");
			ZEPHIR_CPY_WRT(_19, _18);
		}
		zephir_concat_self(&regex, _19 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_20);
		ZVAL_LONG(&_20, y);
		ZEPHIR_INIT_LNVAR(_18);
		ZEPHIR_CONCAT_SVS(_18, ".{", &_20, "})");
		zephir_concat_self(&regex, _18 TSRMLS_CC);
	} else {
		zephir_negate(length TSRMLS_CC);
		x = (int) (zephir_safe_div_long_long(zephir_get_intval(length), 65535 TSRMLS_CC));
		zephir_negate(length TSRMLS_CC);
		y = ((zephir_get_intval(length) % 65535));
		zephir_concat_self_str(&regex, "(.*)", sizeof("(.*)")-1 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_19);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_19);
			ZVAL_STRING(_19, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_LONG(&_4, x);
			ZEPHIR_INIT_LNVAR(_18);
			ZEPHIR_CONCAT_SVS(_18, "(?:.{65535}){", &_4, "}");
			ZEPHIR_CPY_WRT(_19, _18);
		}
		zephir_concat_self(&regex, _19 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_20);
		ZVAL_LONG(&_20, y);
		ZEPHIR_INIT_LNVAR(_18);
		ZEPHIR_CONCAT_SVS(_18, ".{", &_20, "}");
		zephir_concat_self(&regex, _18 TSRMLS_CC);
	}
	ZEPHIR_INIT_LNVAR(_18);
	ZEPHIR_CONCAT_SVS(_18, "#", regex, "#us");
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_22, _18, text, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	zephir_array_fetch_long(&_23, matches, 1, PH_NOISY | PH_READONLY, "phelper/utf8.zep", 189 TSRMLS_CC);
	RETURN_CTOR(_23);

}

