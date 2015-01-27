
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

	zend_bool _12, _13, _14, _15, _16;
	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL, *_10 = NULL, *_22 = NULL, *_23 = NULL;
	zephir_fcall_cache_entry *_5 = NULL;
	zval *matches;
	int offset, x = 0, y = 0, ZEPHIR_LAST_CALL_STATUS;
	zval *text_param = NULL, *offset_param = NULL, *length = NULL, *stringLen = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3, _4 = zval_used_for_init, *_6, *_8 = NULL, *_9, *_11 = NULL, _17 = zval_used_for_init, _18 = zval_used_for_init, *_20 = NULL, _21, *_24;
	zval *text = NULL, *regex, *_19 = NULL;

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
	ZEPHIR_CALL_METHOD(&stringLen, this_ptr, "strlen", NULL, text);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_8);
	if (offset < 0) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, 0);
		ZEPHIR_INIT_VAR(_9);
		ZVAL_LONG(_9, (zephir_get_numberval(stringLen) + offset));
		ZEPHIR_CALL_FUNCTION(&_8, "max", &_10, _1, _9);
		zephir_check_call_status();
	} else {
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_LONG(&_4, offset);
		ZEPHIR_INIT_NVAR(_8);
		ZVAL_LONG(_8, zephir_get_intval(&_4));
	}
	offset = zephir_get_numberval(_8);
	ZEPHIR_INIT_VAR(_11);
	if (Z_TYPE_P(length) == IS_NULL) {
		ZEPHIR_INIT_NVAR(_11);
		ZVAL_NULL(_11);
	} else {
		ZEPHIR_INIT_NVAR(_11);
		ZVAL_LONG(_11, zephir_get_intval(length));
	}
	ZEPHIR_CPY_WRT(length, _11);
	_12 = ZEPHIR_IS_LONG_IDENTICAL(length, 0);
	if (!(_12)) {
		_12 = ZEPHIR_LT_LONG(stringLen, offset);
	}
	_13 = _12;
	if (!(_13)) {
		_14 = ZEPHIR_LT_LONG(length, 0);
		if (_14) {
			_14 = ZEPHIR_LE_LONG(length, (offset - zephir_get_numberval(stringLen)));
		}
		_13 = _14;
	}
	if (_13) {
		RETURN_MM_STRING("", 1);
	}
	_15 = offset == 0;
	if (_15) {
		_16 = Z_TYPE_P(length) == IS_NULL;
		if (!(_16)) {
			_16 = ZEPHIR_GE(length, stringLen);
		}
		_15 = _16;
	}
	if (_15) {
		RETURN_CTOR(text);
	}
	ZEPHIR_INIT_VAR(regex);
	ZVAL_STRING(regex, "^", 1);
	if (offset > 0) {
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_DOUBLE(&_4, zephir_safe_div_long_long(offset, 65535 TSRMLS_CC));
		x = zephir_get_intval(&_4);
		ZEPHIR_SINIT_VAR(_17);
		ZVAL_LONG(&_17, (offset % 65535));
		y = zephir_get_intval(&_17);
		ZEPHIR_INIT_LNVAR(_11);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_STRING(_11, "", 1);
		} else {
			ZEPHIR_SINIT_VAR(_18);
			ZVAL_LONG(&_18, x);
			ZEPHIR_INIT_VAR(_19);
			ZEPHIR_CONCAT_SVS(_19, "(?:.{65535}){", &_18, "}");
			ZEPHIR_CPY_WRT(_11, _19);
		}
		zephir_concat_self(&regex, _11 TSRMLS_CC);
		ZEPHIR_INIT_VAR(_20);
		if (y == 0) {
			ZEPHIR_INIT_NVAR(_20);
			ZVAL_STRING(_20, "", 1);
		} else {
			ZEPHIR_SINIT_VAR(_21);
			ZVAL_LONG(&_21, y);
			ZEPHIR_INIT_LNVAR(_19);
			ZEPHIR_CONCAT_SVS(_19, ".{", &_21, "}");
			ZEPHIR_CPY_WRT(_20, _19);
		}
		zephir_concat_self(&regex, _20 TSRMLS_CC);
	}
	if (Z_TYPE_P(length) == IS_NULL) {
		zephir_concat_self_str(&regex, "(.*)", sizeof("(.*)")-1 TSRMLS_CC);
	} else if (ZEPHIR_GT_LONG(length, 0)) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_LONG(_1, (zephir_get_numberval(stringLen) - offset));
		ZEPHIR_CALL_FUNCTION(&_2, "min", &_22, _1, length);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(length, _2);
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_DOUBLE(&_4, zephir_safe_div_zval_long(length, 65535 TSRMLS_CC));
		x = zephir_get_intval(&_4);
		y = (zephir_get_intval(length) % 65535);
		zephir_concat_self_str(&regex, "(", sizeof("(")-1 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_20);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_20);
			ZVAL_STRING(_20, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_17);
			ZVAL_LONG(&_17, x);
			ZEPHIR_INIT_LNVAR(_19);
			ZEPHIR_CONCAT_SVS(_19, "(?:.{65535}){", &_17, "}");
			ZEPHIR_CPY_WRT(_20, _19);
		}
		zephir_concat_self(&regex, _20 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_18);
		ZVAL_LONG(&_18, y);
		ZEPHIR_INIT_LNVAR(_19);
		ZEPHIR_CONCAT_SVS(_19, ".{", &_18, "})");
		zephir_concat_self(&regex, _19 TSRMLS_CC);
	} else {
		zephir_negate(length TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_DOUBLE(&_4, zephir_safe_div_zval_long(length, 65535 TSRMLS_CC));
		x = zephir_get_intval(&_4);
		zephir_negate(length TSRMLS_CC);
		y = (zephir_get_intval(length) % 65535);
		zephir_concat_self_str(&regex, "(.*)", sizeof("(.*)")-1 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_20);
		if (x == 0) {
			ZEPHIR_INIT_NVAR(_20);
			ZVAL_STRING(_20, "", 1);
		} else {
			ZEPHIR_SINIT_NVAR(_17);
			ZVAL_LONG(&_17, x);
			ZEPHIR_INIT_LNVAR(_19);
			ZEPHIR_CONCAT_SVS(_19, "(?:.{65535}){", &_17, "}");
			ZEPHIR_CPY_WRT(_20, _19);
		}
		zephir_concat_self(&regex, _20 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_18);
		ZVAL_LONG(&_18, y);
		ZEPHIR_INIT_LNVAR(_19);
		ZEPHIR_CONCAT_SVS(_19, ".{", &_18, "}");
		zephir_concat_self(&regex, _19 TSRMLS_CC);
	}
	ZEPHIR_INIT_LNVAR(_19);
	ZEPHIR_CONCAT_SVS(_19, "#", regex, "#us");
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match", &_23, _19, text, matches);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	zephir_array_fetch_long(&_24, matches, 1, PH_NOISY | PH_READONLY, "phelper/utf8.zep", 188 TSRMLS_CC);
	RETURN_CTOR(_24);

}

/**
 * Strips out device control codes in the ASCII range.
 *
 * <code>
 * $string = $utf->stripAsciiCtrl($string);
 * </code>
 */
PHP_METHOD(Phelper_Utf8, stripAsciiCtrl) {

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
PHP_METHOD(Phelper_Utf8, stripNonAscii) {

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

