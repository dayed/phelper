<?php

namespace Phelper;

class Utf8
{
    /**
     * Does the server support UTF-8 natively?
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
     * This is used to determine when to use native functions or UTF-8 functions.
     * <code>
     * $ascii = $utf->isAscii($str);
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
     * $length = $utf->strlen($str);
     * </code>
     *
     * @param string $text String being measured for length
     * @return int 
     */
	public function strlen($text) {}

    /**
     * Get part of a UTF-8 string.
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

}
