
extern zend_class_entry *phelper_utf8_ce;

ZEPHIR_INIT_CLASS(Phelper_Utf8);

PHP_METHOD(Phelper_Utf8, __construct);
PHP_METHOD(Phelper_Utf8, isAscii);
PHP_METHOD(Phelper_Utf8, strlen);
PHP_METHOD(Phelper_Utf8, substr);
PHP_METHOD(Phelper_Utf8, stripAsciiCtrl);
PHP_METHOD(Phelper_Utf8, stripNonAscii);
PHP_METHOD(Phelper_Utf8, ltrim);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, encoding)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8_isascii, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8_strlen, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8_substr, 0, 0, 2)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8_stripasciictrl, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8_stripnonascii, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phelper_utf8_ltrim, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, charList)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(phelper_utf8_method_entry) {
	PHP_ME(Phelper_Utf8, __construct, arginfo_phelper_utf8___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Phelper_Utf8, isAscii, arginfo_phelper_utf8_isascii, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf8, strlen, arginfo_phelper_utf8_strlen, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf8, substr, arginfo_phelper_utf8_substr, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf8, stripAsciiCtrl, arginfo_phelper_utf8_stripasciictrl, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf8, stripNonAscii, arginfo_phelper_utf8_stripnonascii, ZEND_ACC_PUBLIC)
	PHP_ME(Phelper_Utf8, ltrim, arginfo_phelper_utf8_ltrim, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
