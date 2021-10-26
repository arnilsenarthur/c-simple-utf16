
/* 
 * This file is part of the C Simple UTF-16 distribution (https://github.com/arnilsenarthur).
 * Copyright (c) 2021 Árnilsen Arthur.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef lib_h
#define lib_h

#include <stdbool.h>
#include <stdint.h>

#pragma region TypeDefs
//1 Byte size variable (UTF-8 Chars)
typedef uint8_t utf8_char;
//2 Byte size variable (UTF-16 Chars)
typedef uint16_t utf16_char;
//4 Byte size variable (TEMP Conversion)
typedef uint32_t utf32_char;

//Pointers of UTF-8 chars (UTF8-String)
typedef utf8_char *utf8_string;
//Pointers of UTF-16 chars (UTF16-String)
typedef utf16_char *utf16_string;
#pragma endregion TypeDefs
 
#pragma region Conversion
/*
    Convert a utf16 string to utf8/(C++ default) string.

    1 utf8_char -> (1/2/3/4) chars
*/
utf8_string utf16_to_utf8(utf16_string chars);
utf8_string as_c_str(utf16_string s);
/*
    Convert a utf8 string to utf16 string.

    - The majority of common chars are represented with only one utf8_char but
        some needs two chars, like emojis
*/
utf16_string utf8_to_utf16(utf8_string chars);
#pragma endregion Conversion

#pragma region UTF8 Utils
/*
    Get the length of an utf8 string in real chars
*/
size_t utf8_string_length(utf8_string s);
#pragma endregion UTF8 Utils

#pragma region UTF8 Char Utils
/*
    Check if a utf8 char is a single char
*/
bool utf8_char_is_single_byte(utf8_char *c);
/*
    Check if a utf8 char is a double char
*/
bool utf8_char_is_double_byte(utf8_char *c);
/*
    Check if a utf8 char is a triple char
*/
bool utf8_char_is_triple_byte(utf8_char *c);
/*
    Check if a utf8 char is a quadruple char
*/
bool utf8_char_is_quadruple_byte(utf8_char *c);
#pragma endregion UTF8 Char Utils

#pragma region UTF16 Utils
/*
    Get the length of an utf16 string in real chars
*/
size_t utf16_string_length(utf16_string s);
/*
    Concatenate two utf16 strings
*/
utf16_string utf16_string_concat(utf16_string a, utf16_string b);
/*
    Insert an utf16 string inside another utf16 string in a certain position
*/
utf16_string utf16_string_insert(utf16_string a, utf16_string b, size_t position);
/*
    Remove a certain interval from an utf16 string
*/
utf16_string utf16_string_remove(utf16_string s, size_t from, size_t length);
/*
    Returns a certain interval from an utf16 string
*/
utf16_string utf16_string_substring(utf16_string s, size_t from, size_t length);
#pragma endregion UTF16 Utils
#endif
