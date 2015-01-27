
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_PHELPER_H
#define PHP_PHELPER_H 1

#ifdef PHP_WIN32
#define ZEPHIR_RELEASE 1
#endif

#include "kernel/globals.h"

#define PHP_PHELPER_NAME        "phelper"
#define PHP_PHELPER_VERSION     "0.0.1-dev"
#define PHP_PHELPER_EXTNAME     "phelper"
#define PHP_PHELPER_AUTHOR      "Serghei Iakovlev"
#define PHP_PHELPER_ZEPVERSION  "0.5.9a"
#define PHP_PHELPER_DESCRIPTION "Phalcon Helpers written in Zephir"



ZEND_BEGIN_MODULE_GLOBALS(phelper)

	int initialized;

	/* Memory */
	zephir_memory_entry *start_memory; /**< The first preallocated frame */
	zephir_memory_entry *end_memory; /**< The last preallocate frame */
	zephir_memory_entry *active_memory; /**< The current memory frame */

	/* Virtual Symbol Tables */
	zephir_symbol_table *active_symbol_table;

	/** Function cache */
	HashTable *fcache;

	/* Cache enabled */
	unsigned int cache_enabled;

	/* Max recursion control */
	unsigned int recursive_lock;

	/* Global constants */
	zval *global_true;
	zval *global_false;
	zval *global_null;
	
ZEND_END_MODULE_GLOBALS(phelper)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(phelper)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) TSRMG(phelper_globals_id, zend_phelper_globals *, v)
#else
	#define ZEPHIR_GLOBAL(v) (phelper_globals.v)
#endif

#ifdef ZTS
	#define ZEPHIR_VGLOBAL ((zend_phelper_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(phelper_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(phelper_globals)
#endif

#define ZEPHIR_API ZEND_API

#define zephir_globals_def phelper_globals
#define zend_zephir_globals_def zend_phelper_globals

extern zend_module_entry phelper_module_entry;
#define phpext_phelper_ptr &phelper_module_entry

#endif
