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
    // Character cases
    const IGNORE_CASE = 0;
    const LOWER_CASE = -1;
    const UPPER_CASE = 1;

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
     * Map of UTF-8 lower case characters for replacing
     * @var array
     */
    protected _lowerCharsMap = [
        "à": "a",  "ô": "o",  "ď": "d",  "ḟ": "f",  "ë": "e",  "š": "s",  "ơ": "o",
        "ß": "ss", "ă": "a",  "ř": "r",  "ț": "t",  "ň": "n",  "ā": "a",  "ķ": "k",
        "ŝ": "s",  "ỳ": "y",  "ņ": "n",  "ĺ": "l",  "ħ": "h",  "ṗ": "p",  "ó": "o",
        "ú": "u",  "ě": "e",  "é": "e",  "ç": "c",  "ẁ": "w",  "ċ": "c",  "õ": "o",
        "ṡ": "s",  "ø": "o",  "ģ": "g",  "ŧ": "t",  "ș": "s",  "ė": "e",  "ĉ": "c",
        "ś": "s",  "î": "i",  "ű": "u",  "ć": "c",  "ę": "e",  "ŵ": "w",  "ṫ": "t",
        "ū": "u",  "č": "c",  "ö": "o",  "è": "e",  "ŷ": "y",  "ą": "a",  "ł": "l",
        "ų": "u",  "ů": "u",  "ş": "s",  "ğ": "g",  "ļ": "l",  "ƒ": "f",  "ž": "z",
        "ẃ": "w",  "ḃ": "b",  "å": "a",  "ì": "i",  "ï": "i",  "ḋ": "d",  "ť": "t",
        "ŗ": "r",  "ä": "a",  "í": "i",  "ŕ": "r",  "ê": "e",  "ü": "u",  "ò": "o",
        "ē": "e",  "ñ": "n",  "ń": "n",  "ĥ": "h",  "ĝ": "g",  "đ": "d",  "ĵ": "j",
        "ÿ": "y",  "ũ": "u",  "ŭ": "u",  "ư": "u",  "ţ": "t",  "ý": "y",  "ő": "o",
        "â": "a",  "ľ": "l",  "ẅ": "w",  "ż": "z",  "ī": "i",  "ã": "a",  "ġ": "g",
        "ṁ": "m",  "ō": "o",  "ĩ": "i",  "ù": "u",  "į": "i",  "ź": "z",  "á": "a",
        "û": "u",  "þ": "th", "ð": "dh", "µ": "u",  "ĕ": "e",  "ı": "i",  "а": "a",
        "б": "b",  "в": "v",  "г": "g",  "д": "d",  "е": "e",  "ё": "e",  "ж": "zh",
        "з": "z",  "и": "i",  "й": "i",  "к": "k",  "л": "l",  "м": "m",  "н": "n",
        "о": "o",  "п": "p",  "р": "r",  "с": "s",  "т": "t",  "у": "u",  "ф": "f",
        "х": "h",  "ц": "ts", "ч": "ch", "ш": "sh", "щ": "sch","ъ": "",   "ы": "yi",
        "ь": "",   "э": "e",  "ю": "yu", "я": "ya", "æ": "ae", "ǽ": "ae", "ª": "a",
        "ₐ": "a",  "ₑ": "e",  "ⁿ": "n",  "ₒ": "o",  "œ": "oe", "ĳ": "ij"
    ];


    /**
     * Map of UTF-8 upper case characters for replacing
     * @var array
     */
    protected _upperCharsMap = [
        "À": "A",  "Ô": "O",  "Ď": "D",  "Ḟ": "F",  "Ë": "E",  "Š": "S",  "Ơ": "O",
        "Ă": "A",  "Ř": "R",  "Ț": "T",  "Ň": "N",  "Ā": "A",  "Ķ": "K",  "Ĕ": "E",
        "Ŝ": "S",  "Ỳ": "Y",  "Ņ": "N",  "Ĺ": "L",  "Ħ": "H",  "Ṗ": "P",  "Ó": "O",
        "Ú": "U",  "Ě": "E",  "É": "E",  "Ç": "C",  "Ẁ": "W",  "Ċ": "C",  "Õ": "O",
        "Ṡ": "S",  "Ø": "O",  "Ģ": "G",  "Ŧ": "T",  "Ș": "S",  "Ė": "E",  "Ĉ": "C",
        "Ś": "S",  "Î": "I",  "Ű": "U",  "Ć": "C",  "Ę": "E",  "Ŵ": "W",  "Ṫ": "T",
        "Ū": "U",  "Č": "C",  "Ö": "O",  "È": "E",  "Ŷ": "Y",  "Ą": "A",  "Ł": "L",
        "Ų": "U",  "Ů": "U",  "Ş": "S",  "Ğ": "G",  "Ļ": "L",  "Ƒ": "F",  "Ž": "Z",
        "Ẃ": "W",  "Ḃ": "B",  "Å": "A",  "Ì": "I",  "Ï": "I",  "Ḋ": "D",  "Ť": "T",
        "Ŗ": "R",  "Ä": "A",  "Í": "I",  "Ŕ": "R",  "Ê": "E",  "Ü": "U",  "Ò": "O",
        "Ē": "E",  "Ñ": "N",  "Ń": "N",  "Ĥ": "H",  "Ĝ": "G",  "Đ": "D",  "Ĵ": "J",
        "Ÿ": "Y",  "Ũ": "U",  "Ŭ": "U",  "Ư": "U",  "Ţ": "T",  "Ý": "Y",  "Ő": "O",
        "Â": "A",  "Ľ": "L",  "Ẅ": "W",  "Ż": "Z",  "Ī": "I",  "Ã": "A",  "Ġ": "G",
        "Ṁ": "M",  "Ō": "O",  "Ĩ": "I",  "Ù": "U",  "Į": "I",  "Ź": "Z",  "Á": "A",
        "Û": "U",  "Þ": "Th", "Ð": "Dh", "İ": "I",  "А": "A",  "Б": "B",  "В": "V",
        "Г": "G",  "Д": "D",  "Е": "E",  "Ё": "E",  "Ж": "ZH", "З": "Z",  "И": "I",
        "Й": "I",  "К": "K",  "Л": "L",  "М": "M",  "Н": "N",  "О": "O",  "П": "P",
        "Р": "R",  "С": "S",  "Т": "T",  "У": "U",  "Ф": "F",  "Х": "H",  "Ц": "TS",
        "Ч": "CH", "Ш": "SH", "Щ": "SCH","Ъ": "",   "Ы": "YI", "Ь": "",   "Э": "E",
        "Ю": "YU", "Я": "YA", "Æ": "Ae", "Ǽ": "Ae", "Œ": "Oe", "ß": "ss", "Ĳ": "IJ"
    ];


    /**
     * Map of other UTF-8 characters for replacing
     * @var array
     */
    protected _otherCharsMap = [
        "⁰": "0", "¹": "1", "²": "2", "³": "3",  "⁴": "4",  "⁵": "5", "⁶": "6",
        "⁷": "7", "⁸": "8", "⁹": "9", "₀": "0",  "₁": "1",  "₂": "2", "₃": "3",
        "₄": "4", "₅": "5", "₆": "6", "₇": "7",  "₈": "8",  "₉": "9", "±": "+-",
        "×": "x", "₊": "+", "₌": "=", "⁼": "=",  "⁻": "-",  "₋": "-", "–": "-",
        "—": "-", "‑": "-", "․": ".", "‥": "..", "…": "...","‧": ".", "™": "TM"
    ];


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
        let stringLen = this->strlen(text);
        let offset = (offset < 0) ? max(0, stringLen + offset) : intval(offset);
        let length = (length === null) ? null : intval(length);

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
            let x = intval(offset / 65535);
            let y = intval(offset % 65535);

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
     * Replaces special and accented UTF-8 characters by ASCII-7 "equivalents"
     *
     * <code>
     * $ascii = $utf->toAscii($utfString);
     * <code>
     *
     * @todo Add setters for Utf8::_lowerCharsMap, Utf8::_upperCharsMap, Utf8::_otherCharsMap
     * @link http://cldr.unicode.org/index/cldr-spec/transliteration-guidelines
     * @param string text String to transliterate
     * @param integer textCase -1 lowercase only, +1 uppercase only, 0 both cases
     */
    public function toAscii(string! text, int! textCase = self::IGNORE_CASE)
    {
        if textCase <= self::IGNORE_CASE {
            return str_replace(array_keys(this->_lowerCharsMap), array_values(this->_lowerCharsMap), text);
        }

        if textCase >= self::IGNORE_CASE {
            let text = str_replace(array_keys(this->_upperCharsMap), array_values(this->_upperCharsMap), text);
        }

        let text = str_replace(array_keys(this->_otherCharsMap), array_values(this->_otherCharsMap), text);

        return text;
    }

    public function test()
    {
        array  patterns     = ["/quick/", "/brown/", "/fox/"];
        array  replacements = ["bear", "black", "slow"];
        string subject = "The quick brown fox jumped over the lazy dog.";

        return preg_replace(patterns, replacements, subject);
    }
}
