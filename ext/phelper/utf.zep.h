
extern zend_class_entry *phelper_utf_ce;

ZEPHIR_INIT_CLASS(Phelper_Utf);

PHP_METHOD(Phelper_Utf, __construct);
PHP_METHOD(Phelper_Utf, getBom);
PHP_METHOD(Phelper_Utf, isAscii);
PHP_METHOD(Phelper_Utf, strlen);
PHP_METHOD(Phelper_Utf, substr);
PHP_METHOD(Phelper_Utf, stripAsciiCtrl);
PHP_METHOD(Phelper_Utf, stripNonAscii);
PHP_METHOD(Phelper_Utf, ltrim);
PHP_METHOD(Phelper_Utf, rtrim);
PHP_METHOD(Phelper_Utf, stristr);
PHP_METHOD(Phelper_Utf, strstr);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, encoding)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_getbom, 0, 0, 0)
	ZEND_ARG_INFO(0, bigEndian)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_isascii, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_strlen, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_substr, 0, 0, 2)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_stripasciictrl, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_stripnonascii, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_ltrim, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, charList)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_rtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, charList)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_stristr, 0, 0, 2)
	ZEND_ARG_INFO(0, stack)
	ZEND_ARG_INFO(0, needle)
	ZEND_ARG_INFO(0, beforeNeedle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf_strstr, 0, 0, 2)
	ZEND_ARG_INFO(0, stack)
	ZEND_ARG_INFO(0, needle)
	ZEND_ARG_INFO(0, beforeNeedle)
	ZEND_ARG_INFO(0, caseInsensitive)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(phelper_utf_method_entry) {
	PHP_ME(Phelper_Utf, __construct, arginfo_phelper_utf___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Phelper_Utf, getBom, arginfo_phelper_utf_getbom, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, isAscii, arginfo_phelper_utf_isascii, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, strlen, arginfo_phelper_utf_strlen, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, substr, arginfo_phelper_utf_substr, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, stripAsciiCtrl, arginfo_phelper_utf_stripasciictrl, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, stripNonAscii, arginfo_phelper_utf_stripnonascii, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, ltrim, arginfo_phelper_utf_ltrim, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, rtrim, arginfo_phelper_utf_rtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, stristr, arginfo_phelper_utf_stristr, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf, strstr, arginfo_phelper_utf_strstr, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
