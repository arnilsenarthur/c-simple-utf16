/* 
 * This file is part of the C Simple UTF-15 distribution (https://github.com/arnilsenarthur).
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

#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

size_t utf8_string_length(utf8_string s)
{
    size_t length = 0;
    char c;
    while ((c = *s++) != '\0')
    {
        if (c < 0 || c > 127)
        {
            if ((c & 0xE0) == 0xC0)
                s += 1;
            else if ((c & 0xF0) == 0xE0)
                s += 2;
            else if ((c & 0xF8) == 0xF0)
                s += 3;
        }

        length++;
    }
    return length;
}

size_t utf16_string_length(utf16_string s)
{
    size_t i = 0;
    while (s[i++] != 0)
        ;
    return i - 1;
}

utf8_string utf16_to_utf8(utf16_string chars)
{
    utf8_char *str = (utf8_char *)malloc(16);
    size_t sz = 16;
    size_t n = 0;
    size_t i = 0;

    while (true)
    {
        utf16_char ch = chars[i++];

        if (ch == 0)
            break;

        if (n == sz)
        {
            utf8_char *tmp;
            tmp = realloc(str, sz + 16);
            if (tmp != NULL)
            {
                str = tmp;
            }

            sz += 16;
        }

        if (ch < 0x80)
        {
            str[n] = ch;
            n++;
        }
        else if (ch < 0x800)
        {
            str[n] = 0xc0 | (ch >> 6);
            str[n + 1] = 0x80 | (ch & 0x3f);
            n += 2;
        }
        else if (ch < 0xd800 || ch >= 0xe000)
        {
            str[n] = 0xe0 | (ch >> 12);
            str[n + 1] = 0x80 | ((ch >> 6) & 0x3f);
            str[n + 2] = 0x80 | (ch & 0x3f);
            n += 3;
        }
        //Char pair (Emojis, etc...)
        else
        {
            utf32_char a = ch;
            utf32_char b = chars[i++];
            utf32_char c = 0x10000 + (((a & 0x3ff) << 10) | (b & 0x3ff));

            str[n] = 0xf0 | (c >> 18);
            str[n + 1] = 0x80 | ((c >> 12) & 0x3f);
            str[n + 2] = 0x80 | ((c >> 6) & 0x3f);
            str[n + 3] = 0x80 | (c & 0x3f);
            n += 4;
        }
    }

    realloc(str, n + 1);

    str[n] = 0;
    return str;
}

utf16_string utf8_to_utf16(utf8_string chars)
{
    size_t length = utf8_string_length(chars);
    utf16_char *str = (utf16_char *)malloc(2 * (length + 1));
    str[length] = 0;

    size_t n = 0;
    size_t i = 0;
    
    while (true)
    {
        utf8_char ch = chars[i++];

        if (ch == '\0')
            break;

        if (ch >> 7 == 0)
        {
            str[n] = ch;
            n++;
            continue;
        }

        //ERROR CHECK:
        /*
        if (c >> 6 == 0x02)
        {
            continue;
        }
        */

        // Get byte length
        char extraChars = 0;
        if (ch >> 5 == 0x06)
        {
            extraChars = 1;
        }
        else if (ch >> 4 == 0x0e)
        {
            extraChars = 2;
        }
        else if (ch >> 3 == 0x1e)
        {
            extraChars = 3;
        }
        else if (ch >> 2 == 0x3e)
        {
            extraChars = 4;
        }
        else if (ch >> 1 == 0x7e)
        {
            extraChars = 5;
        }
        else
        {
            continue;
        }

        utf32_char mask = (1 << (8 - extraChars - 1)) - 1;
        utf32_char res = ch & mask;
        utf8_char nextChar;
        size_t count;

        for (count = 0; count < extraChars; count++)
        {
            nextChar = chars[i++];

            if (nextChar >> 6 != 0x02)
                break;

            res = (res << 6) | (nextChar & 0x3f);
        }

        if (count != extraChars)
        {
            i--;
            continue;
        }

        if (res <= 0xffff)
        {
            str[n] = res;
            n++;
            continue;
        }

        res -= 0x10000;

        utf16_char high = ((res >> 10) & 0x3ff) + 0xd800;
        utf16_char low = (res & 0x3ff) + 0xdc00;

        str[n] = high;
        str[n + 1] = low;
        n += 2;
    }

    str[n] = 0;

    return str;
}

bool utf8_char_is_single_byte(utf8_char *c)
{
    return c[0] >> 7 == 0;
}

bool utf8_char_is_double_byte(utf8_char *c)
{
    return c[0] >> 5 == 0x06;
}

bool utf8_char_is_triple_byte(utf8_char *c)
{
    return c[0] >> 4 == 0x0e;
}

bool utf8_char_is_quadruple_byte(utf8_char *c)
{
    return c[0] >> 3 == 0x1e;
}

utf8_string as_c_str(utf16_string s)
{
    return utf16_to_utf8(s);
}

utf16_string utf16_string_concat(utf16_string a, utf16_string b)
{
    size_t lena = utf16_string_length(a);
    size_t lenb = utf16_string_length(b);

    utf16_char *c = (utf16_char *) malloc((lena + lenb + 1) * sizeof(utf16_char));

    memcpy(c, a, lena * sizeof(utf16_char));
    memcpy(&c[lena], b, lenb * sizeof(utf16_char));

    c[lena + lenb] = 0;
    return c;
}

utf16_string utf16_string_insert(utf16_string a, utf16_string b,size_t position)
{
    size_t lena = utf16_string_length(a);
    size_t lenb = utf16_string_length(b);

    utf16_char *c = (utf16_char *) malloc((lena + lenb + 1) * sizeof(utf16_char));

    memcpy(c, a, position * sizeof(utf16_char));
    memcpy(&c[position], b, lenb * sizeof(utf16_char));
    memcpy(&c[position + lenb], &a[position], (lena - position) * sizeof(utf16_char));
    c[lena + lenb] = 0;
    return c;
}

utf16_string utf16_string_remove(utf16_string s,size_t from,size_t length)
{
    size_t len = utf16_string_length(s);
    utf16_char *c = (utf16_char *) malloc(((len + 1) - length) * sizeof(utf16_char));

    memcpy(c, s, from * sizeof(utf16_char));
    memcpy(&c[from], &s[from + length], (len - (from + length)) * sizeof(utf16_char));

    c[len - length] = 0;
    return c;
}

utf16_string utf16_string_substring(utf16_string s,size_t from,size_t length)
{
    utf16_char *c = (utf16_char *) malloc((length + 1) * sizeof(utf16_char));

    memcpy(c, &s[from], length * sizeof(utf16_char));

    c[length] = 0;
    return c;
}
