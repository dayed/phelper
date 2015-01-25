<?php
/*
 +------------------------------------------------------------------------+
 | Phalcon Helpers                                                        |
 +------------------------------------------------------------------------+
 | Copyright (c) 2015 Serghei Iakovlev (http://klay.me)                   |
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

namespace Phelper\Tests;

use PHPUnit_Framework_TestCase;
use Phelper\Utf8;

/**
 * Phelper Utf8 Test
 *
 * @package Phelper\Tests
 */
class Utf8Test extends PHPUnit_Framework_TestCase
{
    protected $utf;
    protected $oldConfig;

    public function setUp()
    {
        if (!extension_loaded('phelper')) {
            $this->markTestSkipped('The Phelper extension is not available.');
        }

        $this->oldConfig = ini_get('mbstring.func_overload');

        ini_set('mbstring.func_overload', 0);
        $this->utf = new Utf8;
    }

    public function tearDown()
    {
        ini_set('mbstring.func_overload', $this->oldConfig);
    }

    /**
     * Provides test data for testIsAscii()
     */
    public function asciiProvider()
    {
        return array(
            array("\0", true),
            array("\$Pha\r", true),
            array('Phålcón', false),
            array(array('Phal', 'con'), true),
            array(array('Phål', 'cón'), false),
        );
    }

    /**
     * @dataProvider asciiProvider
     */
    public function testIsAscii($input, $expected)
    {
        $this->assertSame($expected, $this->utf->isAscii($input));
    }

    /**
     * Provides test data for testStrlen()
     */
    public function strlenProvider()
    {
        return array(
            array('Phålcón', 7),
            array('Phalcon', 7),
        );
    }

    /**
     * @dataProvider strlenProvider
     */
    public function testStrlen($input, $expected)
    {
        $this->assertSame($expected, $this->utf->strlen($input));
    }

    /**
     * Provides test data for testSubstr()
     */
    public function substrProvider()
    {
        return array(
            array('Phålcón', 3, 2, 'lc'),
            array('Phålcón', 3, 9, 'lcón'),
            array('Phålcón', 3, null, 'lcón'),
            array('Phålcón', 3, -2, 'lc'),
        );
    }

    /**
     * @dataProvider substrProvider
     */
    public function testSubstr($input, $offset, $length, $expected)
    {
        $this->assertSame($expected, $this->utf->substr($input, $offset, $length));
    }

    /**
     * Provides test data for testToAscii()
     */
    public function toAsciiProvider()
    {
        return array(
            array('Phålcón', Utf8::LOWER_CASE, 'Phalcon'),
            array('PHÅLCÓN', Utf8::LOWER_CASE, 'PHÅLCÓN'),
            array('Phålcón', Utf8::IGNORE_CASE, 'Phalcon'),
            array('PHÅLCÓN', Utf8::IGNORE_CASE, 'PHALCON'),
            array('Phålcón', Utf8::UPPER_CASE, 'Phålcón'),
            array('PHÅLCÓN', Utf8::UPPER_CASE, 'PHALCON'),
        );
    }

    /**
     * @dataProvider toAsciiProvider
     */
    public function testToAscii($input, $case, $expected)
    {
        $this->assertSame($expected, $this->utf->toAscii($input, $case));
    }
}
