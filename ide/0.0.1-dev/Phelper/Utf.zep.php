<?php

namespace Phelper;

class Utf
{
    /**
     * Does the server support UTF natively?
     *
     * @var boolean
     */
    protected $_mbSupport = false;
    /**
     * Encoding using in mb_* functions
     *
     * @var string
     */
    protected $_encoding = "utf-8";

    /**
     * Class constructor
     *
     * @param string $encoding Encoding using in mb_* functions
     */
	public function __construct($encoding = "utf-8") {}

    /**
     * Tests whether a string contains only 7-bit ASCII bytes.
     * This is used to determine when to use native functions or UTF-functions.
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
     * <code>
     * $found = $utf->stristr($string, $search);
     * </code>
     *
     * @param string $stack 
     * @param string $needle Needle.
     * @param boolean $beforeNeedle [Optional]
     * @param string $haystack The input string.
     * @return mixed substring if found or false if the substring was not found
     */
	public function stristr($stack, $needle, $beforeNeedle = false) {}

    /**
     * UTF version of strstr.
     * Returns part of haystack string from the first occurrence of needle to the end of haystack.
     * This is a UTF-aware version of [stristr](http://php.net/stristr).
     * <code>
     * $found = $utf->strstr($string, $search);
     * </code>
     *
     * @param string $stack 
     * @param string $needle Needle.
     * @param boolean $beforeNeedle If TRUE returns the part of the haystack before the first occurrence of the needle (excluding the needle). [Optional]
     * @param boolean $caseInsensitive Case insensitive? [Optional]
     * @param string $haystack The input string.
     * @return mixed substring if found or FALSE if the substring was not found
     */
	public function strstr($stack, $needle, $beforeNeedle = false, $caseInsensitive = false) {}

}
