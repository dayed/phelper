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
 | version 3 of the License, or (at your option) any later version.       |
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
use Phelper\Utf;

/**
 * Phelper Utf Test
 *
 * @package Phelper\Tests
 */
class UtfTest extends PHPUnit_Framework_TestCase
{
    /**
     * @var Utf
     */
    protected $utf;
    protected $oldConfig;

    public function setUp()
    {
        if (!extension_loaded('phelper')) {
            $this->markTestSkipped('The Phelper extension is not available.');
        }

        $this->oldConfig = ini_get('mbstring.func_overload');

        ini_set('mbstring.func_overload', 0);
        $this->utf = new Utf;
    }

    public function tearDown()
    {
        ini_set('mbstring.func_overload', $this->oldConfig);
    }

    /**
     * @expectedException        \InvalidArgumentException
     * @expectedExceptionMessage Invalid encoding. Support only: utf-8, utf-16 and utf-32
     */
    public function testThrowsWhenInvalidEncoding()
    {
        $utf = new Utf('Abc');
    }

    /**
     * Provides test data for testConstants()
     */
    public function constantsProvider()
    {
        return array(
            array('utf-8',  Utf::UTF_8),
            array('utf-16', Utf::UTF_16),
            array('utf-32', Utf::UTF_32),
        );
    }

    /**
     * @dataProvider constantsProvider
     */
    public function testConstants($encoding, $expected)
    {
        $this->assertSame($expected, $encoding);
    }

    public function testCanSetGetEncoding()
    {
        $utf = new Utf('Utf-8');
        $this->assertSame(Utf::UTF_8, $utf->getEncoding());

        $utf->setEncoding('uTf-16');
        $this->assertSame(Utf::UTF_16, $utf->getEncoding());

        $utf->setEncoding('UTF-32');
        $this->assertSame(Utf::UTF_32, $utf->getEncoding());
    }

    /**
     * Provides test data for testIsAscii()
     */
    public function asciiProvider()
    {
        return array(
            array("\0",                 true),
            array("\$Pha\r",            true),
            array('Phålcón',            false),
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
            array('Phålcón',                     7),
            array('Phalcon',                     7),
            array('나는 유리를 먹을 수 있어요. 그래도', 20),
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
            array('Phålcón', 3, 2,    'lc'),
            array('Phålcón', 3, 9,    'lcón'),
            array('Phålcón', 3, null, 'lcón'),
            array('Phålcón', 3, -2,   'lc'),
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
     * Provides test data for testStripAsciiCtrl()
     */
    public function stripAsciiCtrlProvider()
    {
        return array(
            array("\0\021\x7F",      ''),
            array("\0I ♥ Phålcón",   'I ♥ Phålcón'),
            array("I ♥ Phålcón\021", 'I ♥ Phålcón'),
            array("\x7FI ♥ Phalcon", 'I ♥ Phalcon'),
            array("\x41",            'A'),
            array("\xFF",            "\xFF"),
        );
    }

    /**
     * @dataProvider stripAsciiCtrlProvider
     */
    public function testStripAsciiCtrl($input, $expected)
    {
        $this->assertSame($expected, $this->utf->stripAsciiCtrl($input));
    }

    /**
     * Provides test data for testStripNonAscii()
     */
    public function stripNonAsciiProvider()
    {
        return array(
            array('Phålcón',     'Phlcn'),
            array("I ♥ Phalcon", 'I  Phalcon'),
        );
    }

    /**
     * @dataProvider stripNonAsciiProvider
     */
    public function testStripNonAscii($input, $expected)
    {
        $this->assertSame($expected, $this->utf->stripNonAscii($input));
    }

    /**
     * Provides test data for testLtrim()
     */
    public function ltrimProvider()
    {
        return array(
            array(' bar ', null,            'bar '),
            array(' bar ', false,           'bar '),
            array('ålcó',  array('å', 'l'), 'có'),
            array('alco',  'a',             'lco'),
            array('ålcó',  'å',             'lcó'),
        );
    }

    /**
     * @dataProvider ltrimProvider
     */
    public function testLtrim($input, $charList, $expected)
    {
        $this->assertSame($expected, $this->utf->ltrim($input, $charList));
    }

    /**
     * Provides test data for testRtrim()
     */
    public function rtrimProvider()
    {
        return array(
            array(' bar ', null,            ' bar'),
            array(' bar ', false,           ' bar'),
            array('ålcó',  array('ó', 'c'), 'ål'),
            array('alco',  'a',             'alco'),
            array('ålcó',  'ó',             'ålc'),
        );
    }

    /**
     * @dataProvider rtrimProvider
     */
    public function testRtrim($input, $charList, $expected)
    {
        $this->assertSame($expected, $this->utf->rtrim($input, $charList));
    }

    /**
     * Provides test data for testStristr()
     */
    public function stristrProvider()
    {
        return array(
            array('I ♥ Phålcón', 'åL', false, 'ålcón'),
            array('I ♥ Phålcón', 'ål', true,  'I ♥ Ph'),
            array('I ♥ Phålcón', '♥',  false, '♥ Phålcón'),
            array('I ♥ Phålcón', 'Ж',  false, false),
        );
    }

    /**
     * @dataProvider stristrProvider
     */
    public function testStristr($stack, $needle, $before, $expected)
    {
        $this->assertSame($expected, $this->utf->stristr($stack, $needle, $before));
    }

    /**
     * Provides test data for testStrstr()
     */
    public function strstrProvider()
    {
        return array(
            array('I ♥ Phålcón', 'åL', false, false, false),
            array('I ♥ Phålcón', 'åL', false, true, 'ålcón'),
            array('I ♥ Phålcón', 'ål', true,  false, 'I ♥ Ph'),
            array('I ♥ Phålcón', 'Ål', true,  true, 'I ♥ Ph'),
            array('I ♥ Phålcón', '♥',  false, false, '♥ Phålcón'),
            array('I ♥ Phålcón', 'Ж',  false, false, false),
        );
    }

    /**
     * @dataProvider strstrProvider
     */
    public function testStrstr($stack, $needle, $before, $case, $expected)
    {
        $this->assertSame($expected, $this->utf->strstr($stack, $needle, $before, $case));
    }
}
