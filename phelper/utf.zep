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
    /**
     * Does the server support UTF natively?
     * @var boolean
     */
    protected _mbSupport = false;

    /**
     * Encoding using in mb_* functions
     * @var string
     */
    protected _encoding = "utf-8";

    /**
     * Class constructor
     * @param string encoding Encoding using in mb_* functions
     */
    public function __construct(string! encoding="utf-8")
    {
        let this->_mbSupport = extension_loaded("mbstring");
        let this->_encoding = encoding;
    }

    /**
     * Tests whether a string contains only 7-bit ASCII bytes.
     * This is used to determine when to use native functions or UTF-functions.
     *
     * <code>
     * $ascii = $utf->isAscii($string);
     * </code>
     *
     * @param mixed text String or array of strings to check
     */
    public function isAscii(var text) ->boolean
    {
        if typeof text === "array" {
            let text = join("", text);
        }

        return !preg_match("/[^\x00-\x7F]/S", text);
    }

    /**
     * Get string length.
     * This is a UTF8-aware version of [strlen](http://php.net/strlen).
     *
     * <code>
     * $length = $utf->strlen($string);
     * </code>
     *
     * @param string text String being measured for length
     */
    public function strlen(string! text) ->int
    {
        if this->_mbSupport {
            return mb_strlen(text, this->_encoding);
        }

        if this->isAscii(text) {
            return text->length();
        }

        return utf8_decode(text)->length();
    }

    /**
     * Get part of a UTF-string.
     * This is a UTF8-aware version of [substr](http://php.net/substr).
     *
     * <code>
     * $sub = $utf->substr($string, $offset);
     * </code>
     */
    public function substr(string! text, int! offset, var length=null) ->string
    {
        var stringLen;
        array matches = [];
        string regex;
        int x = 0,
            y = 0;

        if this->_mbSupport {
            return (typeof length === "null")
                ? mb_substr(text, offset, mb_strlen(text, this->_encoding), this->_encoding)
                : mb_substr(text, offset, length, this->_encoding);
        }

        if this->isAscii(text) {
            return (typeof length === "null") ? substr(text, offset) : substr(text, offset, length);
        }

        // Normalize params
        let stringLen = this->strlen(text);
        let offset = (offset < 0) ? max(0, stringLen + offset) : intval(offset);
        let length = (typeof length === "null") ? null : intval(length);

        // Impossible
        if length === 0 || offset >= stringLen || (length < 0 && length <= offset - stringLen) {
            return "";
        }

        // Whole string
        if offset == 0 && (typeof length === "null" || length >= stringLen) {
            return text;
        }

        // Build regex
        let regex = "^";

        // Create an offset expression
        if offset > 0 {
            // PCRE repeating quantifiers must be less than 65536, so repeat when necessary
            let x = intval(offset / 65535);
            let y = intval(offset % 65535);

            let regex .= (x == 0) ? "" : "(?:.{65535}){".x."}";
            let regex .= (y == 0) ? "" : ".{".y."}";
        }

        // Create a length expression
        if typeof length === "null" {
            // No length set, grab it all
            let regex .= "(.*)";
        } elseif length > 0 {
            // Reduce length so that it can't go beyond the end of the string
            let length = min(stringLen - offset, length);

            let x = intval(length / 65535);
            let y = intval(length) % 65535;
            let regex .= "(";
            let regex .= (x == 0) ? "" : "(?:.{65535}){".x."}";
            let regex .= ".{".y."})";
        } else {
            // Find length from the right (negative length)
            let x = intval(-length / 65535);
            let y = intval(-length) % 65535;
            let regex .= "(.*)";
            let regex .= (x == 0) ? "" : "(?:.{65535}){".x."}";
            let regex .= ".{".y."}";
        }

        preg_match("#".regex."#us", text, matches);
        return matches[1];
    }

    /**
     * Strips out device control codes in the ASCII range.
     *
     * <code>
     * $string = $utf->stripAsciiCtrl($string);
     * </code>
     */
    public function stripAsciiCtrl(string! text) ->string
    {
        return preg_replace("#[\x00-\x08\x0B\x0C\x0E-\x1F\x7F]+#S", "", text);
    }

    /**
     * Strips out all non-7bit ASCII bytes.
     *
     * <code>
     * $string = $utf->stripNonAscii($string);
     * <code>
     */
    public function stripNonAscii(string! text) ->string
    {
        return preg_replace("#[^\x00-\x7F]+#S", "", text);
    }

    /**
     * Strips whitespace (or other UTF-characters) from the beginning of a string.
     * This is a UTF8-aware version of [ltrim](http://php.net/ltrim).
     *
     * <code>
     * $string = $utf->ltrim($string);
     * </code>
     *
     * @author Andreas Gohr <andi@splitbrain.org>
     * @param string text Input string
     * @param string charList String of characters (or array of chars) to remove
     */
    public function ltrim(string! text, var charList = null) ->string
    {
        if typeof charList === "null" || charList === false {
            return text->trimleft();
        }

        if this->isAscii(text) {
            return text->trimleft(charList);
        }

        if typeof charList == "array" {
            let charList = join("", charList);
        }

        let charList = preg_replace("#[-\[\]:\\\\^/]#", "\\\\$0", charList);

        return preg_replace("/^[" . charList . "]+/u", "", text);
    }

    /**
     * Strips whitespace (or other UTF-characters) from the end of a string.
     * This is a UTF8-aware version of [rtrim](http://php.net/rtrim).
     *
     * <code>
     * $string = $utf->rtrim($string);
     * </code>
     *
     * @@author Harry Fuecks <hfuecks@gmail.com>
     * @param string text Input string
     * @param string charList String of characters (or array of chars) to remove
     */
    public function rtrim(string! text, var charList = null) ->string
    {
        if typeof charList === "null" || charList === false {
            return text->trimright();
        }

        if this->isAscii(text) {
            return text->trimright(charList);
        }

        if typeof charList === "array" {
            let charList = join("", charList);
        }

        let charList = preg_replace("#[-\[\]:\\\\^/]#", "\\\\$0", charList);

        return preg_replace("/[" . charList . "]++$/uD", "", text);
    }

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
     * @param string haystack The input string.
     * @param string needle Needle.
     * @param boolean beforeNeedle [Optional]
     * @return mixed matched substring if found or false if the substring was not found
     */
    public function stristr(string! stack, var needle, boolean beforeNeedle = false)
    {
        return this->strstr(stack, needle, beforeNeedle, true);
    }

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
     * @param string haystack The input string.
     * @param string needle Needle.
     * @param boolean beforeNeedle If TRUE returns the part of the haystack before the first occurrence of the needle (excluding the needle). [Optional]
     * @param boolean caseInsensitive Case insensitive? [Optional]
     * @return mixed matched substring if found or FALSE if the substring was not found
     */
    public function strstr(string! stack, var needle, boolean beforeNeedle = false, boolean caseInsensitive = false)
    {
        array matches = [];

        if this->isAscii(stack) &&  this->isAscii(needle)  {
            return caseInsensitive ? stristr(stack, needle, beforeNeedle) : strstr(stack, needle, beforeNeedle);
        }

        if needle == "" {
            return false;
        }

        preg_match("/^(.*?)" . preg_quote(needle, "/") . "/us" . (caseInsensitive ? "i" : ""), stack, matches);

        if isset(matches[1]) {
            if beforeNeedle {
                return matches[1];
            } else {
                return this->substr(stack, this->strlen(matches[1]));
            }
        }

        return false;
    }
}
