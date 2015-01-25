Phelper
=======
[![Build Status](https://travis-ci.org/sergeyklay/phelper.svg?branch=master)](https://travis-ci.org/sergeyklay/phelper)
[![License](https://poser.pugx.org/sergeyklay/phelper/license.svg)](https://github.com/sergeyklay/phelper/blob/master/LICENSE.txt)

### What is Phelper

The Phelper package provides Phalcon Helpers written in Zephir.

**Work In Progress. Do not use in production!**

### Current Components

-----------------
| Component | Version | Status | Description |
|-----------|---------|--------|-------|
|**Utf8**|0.0.1-dev|OK|Provides multi-byte aware replacement string functions.|


### System Requirements

To build it you need the following requirements:

* g++ >= 4.4/clang++ >= 3.x/vc++ 9
* gnu make 3.81 or later
* autoconf 2.31 or later
* automake 1.14 or later
* PHP development headers and tools

If you're using Ubuntu, you can install the required packages this way:

```
sudo apt-get update
sudo apt-get install git gcc make re2c php5 php5-json php5-dev libpcre3-dev
```

### Build Zephir code

To build the zep Source to C extension:
* [zephir](https://github.com/phalcon/zephir)

### Tests

#### Tested Zephir version

* 0.5.9a

#### Tested PHP versions

* 5.3
* 5.4
* 5.5
* 5.6

#### Additional requirements

* PHPUnit 4.0.0 or higher (for testing)
* Xdebug (for testing)

To run the tests at the command line, go to the `tests` directory and run `phpunit`:

```sh
$ cd tests
$ phpunit
```

For additional information see [PHPUnit The Command-Line Test Runner](https://phpunit.de/manual/current/en/textui.html)

### License

Licensed under the [GNU Lesser General Public License](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt). Copyright (c) 2015 Serghei Iakovlev
