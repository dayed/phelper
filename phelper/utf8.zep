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
 * A port of [phputf8](http://phputf8.sourceforge.net/) to a unified set of files.
 * Provides multi-byte aware replacement string functions.
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
class Utf8
{
    /**
     * Does the server support UTF-8 natively?
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
     * This is used to determine when to use native functions or UTF-8 functions.
     *
     * <code>
     *   $ascii = $utf->isAscii($str);
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
     *   $length = $utf->strlen($str);
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
     * Get part of a UTF-8 string.
     * This is a UTF8-aware version of [substr](http://php.net/substr).
     *
     * <code>
     *  $sub = $utf->substr($string, $offset);
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
            return (length === null)
                ? mb_substr(text, offset, mb_strlen(text), this->_encoding)
                : mb_substr(text, offset, length, this->_encoding);
        }

        if this->isAscii(text) {
            return (length === null) ? substr(text, offset) : substr(text, offset, length);
        }

        // Normalize params
        let text = (string) text;
        let stringLen = this->strlen(text);
        let offset = (int) (offset < 0) ? max(0, stringLen + offset) : offset;
        let length = (length === null) ? null : (int) length;

        // Impossible
        if length === 0 || offset >= stringLen || (length < 0 && length <= offset - stringLen) {
            return "";
        }

        // Whole string
        if offset == 0 && (length === null || length >= stringLen) {
            return text;
        }

        // Build regex
        let regex = "^";

        // Create an offset expression
        if offset > 0 {
            // PCRE repeating quantifiers must be less than 65536, so repeat when necessary
            let x = (int) (offset / 65535);
            let y = (int) (offset % 65535);

            let regex .= (x == 0) ? "" : "(?:.{65535}){".x."}";
            let regex .= (y == 0) ? "" : ".{".y."}";
        }

        // Create a length expression
        if length === null {
            // No length set, grab it all
            let regex .= "(.*)";
        } elseif length > 0 {
            // Reduce length so that it can't go beyond the end of the string
            let length = min(stringLen - offset, length);

            let x = (int) ((int) length / 65535);
            let y = ((int) length % 65535);
            let regex .= "(";
            let regex .= (x == 0) ? "" : "(?:.{65535}){".x."}";
            let regex .= ".{".y."})";
        } else {
            // Find length from the right (negative length)
            let x = (int) ((int) -length / 65535);
            let y = ((int) -length % 65535);
            let regex .= "(.*)";
            let regex .= (x == 0) ? "" : "(?:.{65535}){".x."}";
            let regex .= ".{".y."}";
        }

        preg_match("#".regex."#us", text, matches);
        return matches[1];
    }
}
