<?php

namespace Phelper;

/**
 * Provides multi-byte aware replacement string functions.
 * This class contains some part from port of [phputf8](http://phputf8.sourceforge.net/) to a unified set of files.
 *
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
class Utf
{
    // Encoding names
    const UTF_1      = "utf-1";
    const UTF_7      = "utf-7";
    const UTF_8      = "utf-8";
    const UTF_16     = "utf-16";
    const UTF_32     = "utf-32";
    const UTF_EBCDIC = "utf-ebcdic";

    /**
     * Does the server support UTF natively?
     * @var boolean
     */
    protected $_mbSupport = false;

    /**
     * Encoding using in mb_* functions
     * @var string
     */
    protected $_encoding = Utf::UTF_8;

    /**
     * Class constructor
     *
     * @param string $encoding Encoding using in mb_* functions
     */
    public function __construct($encoding = Utf::UTF_8) {}

    /**
     * Tests whether a string contains only 7-bit ASCII bytes.
     * This is used to determine when to use native functions or UTF-functions.
     *
     * <code>
     * $ascii = $utf->isAscii($string);
     * </code>
     *
     * @param mixed $text String or array of strings to check
     * @return bool
     */
    public function isAscii($text) {}

    /**
     * Get string length.
     * This is a UTF8-aware version of [strlen](http://php.net/strlen).
     *
     * <code>
     * $length = $utf->strlen($string);
     * </code>
     *
     * @param string $text String being measured for length
     * @return int
     */
    public function strlen($text) {}

    /**
     * Get part of a UTF-string.
     * This is a UTF8-aware version of [substr](http://php.net/substr).
     *
     * <code>
     * $sub = $utf->substr($string, $offset);
     * </code>
     *
     * @param string $text
     * @param int $offset
     * @param mixed $length
     * @return string
     */
    public function substr($text, $offset, $length = null) {}

    /**
     * Strips out device control codes in the ASCII range.
     *
     * <code>
     * $string = $utf->stripAsciiCtrl($string);
     * </code>
     *
     * @param string $text
     * @return string
     */
    public function stripAsciiCtrl($text) {}

    /**
     * Strips out all non-7bit ASCII bytes.
     *
     * <code>
     * $string = $utf->stripNonAscii($string);
     * <code>
     *
     * @param string $text
     * @return string
     */
    public function stripNonAscii($text) {}

    /**
     * Strips whitespace (or other UTF-characters) from the beginning of a string.
     * This is a UTF8-aware version of [ltrim](http://php.net/ltrim).
     *
     * <code>
     * $string = $utf->ltrim($string);
     * </code>
     *
     * @author Andreas Gohr <andi@splitbrain.org>
     * @param string $text Input string
     * @param string $charList String of characters (or array of chars) to remove
     * @return string
     */
    public function ltrim($text, $charList = null) {}

    /**
     * Strips whitespace (or other UTF-characters) from the end of a string.
     * This is a UTF8-aware version of [rtrim](http://php.net/rtrim).
     *
     * <code>
     * $string = $utf->rtrim($string);
     * </code>
     *
     * @@author Harry Fuecks <hfuecks@gmail.com>
     * @param string $text Input string
     * @param string $charList String of characters (or array of chars) to remove
     * @return string
     */
    public function rtrim($text, $charList = null) {}

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
     * @param string $stack The input string.
     * @param string $needle Needle.
     * @param boolean $beforeNeedle [Optional]
     * @return mixed substring if found or false if the substring was not found
     */
    public function stristr($stack, $needle, $beforeNeedle = false) {}

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
     * @param string $stack The input string.
     * @param string $needle Needle.
     * @param boolean $beforeNeedle If TRUE returns the part of the haystack before the first occurrence of the needle (excluding the needle). [Optional]
     * @param boolean $caseInsensitive Case insensitive? [Optional]
     * @return mixed substring if found or FALSE if the substring was not found
     */
    public function strstr($stack, $needle, $beforeNeedle = false, $caseInsensitive = false) {}
}
