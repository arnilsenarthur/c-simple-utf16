# C Simple UTF-16

#### Author: Árnilsen Arthur Castilho Lopes - 2021
A simple library to deal/support UTF16 strings in pure C.

## Features
- Lightweight code
- Lightweight memory usage
- Fast (Memcopy methods)
- Non-external libraries
- No platform dependent functions

## UTF-8 vs UTF-16
### UTF-8
- Smaller for ascii/english characters (Each character takes only 1 char)
- Bigger for special characters (Chars may variate from 1 byte to 4 chars)
- Harder to work in char level
- Adopted in C language
- Count the real length of an utf-8 would need to iterate through chars to check char-by-char

### UTF-16
- Fixed char size for general characters (2 char per char)
- Emotes take only 2 chars (Less than UTF-8)
- Easier to work in char level
- Adopted in Java, C#, etc... 
- Count the real length of an utf-8 would only need to find the end of the string

| Character  | UTF-8 (dec) | UTF-16 (dec)| 
| ------------- | ------------- |--------|
| A (1 char) | 65  | 0 65|
| Á  (1 char)| 195 129  | 0 193|
|😚 (2 char)|240 159 152 154|216 61 222 26|
|你 (1 char)|228 189 160|79 96|


## Basic Example
```c
#include "lib.c"
#include <stdio.h>

void main()
{
	//Create utf16 from C string literals in a fast and simple way
	utf16_string s = utf8_to_utf16("<😚>");
	utf16_string a = utf8_to_utf16("Hillo ");
	utf16_string b = utf8_to_utf16(" world!");
	
	//Manage the strings
	utf16_string c = utf16_string_concat(a, b);
	utf16_string d = utf16_string_insert(c, s, 6);
	
	//Change a char directly
	d[1] = 'e';
	
	//Output string (Converting to utf8 string again)
	printf("%s", utf16_to_utf8(d));
}
```
# Docs
## Types
| Name  | Byte size | 
| ------------- | ------------- |
|utf8_char|1|
|utf16_char|2|
|utf32_char|4|
|utf8_string|length + 1|
|utf16_string|length * 2 + 2|

## Functions
```c
//Conversion  UTF16 > UTF8
utf8_string utf16_to_utf8(utf16_string chars);
utf8_string as_c_str(utf16_string s);
//Conversion  UTF8 > UTF16
utf16_string utf8_to_utf16(utf8_string chars);

//UTF8 char util
bool utf8_char_is_single_byte(utf8_char *c);
bool utf8_char_is_double_byte(utf8_char *c);
bool utf8_char_is_triple_byte(utf8_char *c);
bool utf8_char_is_quadruple_byte(utf8_char *c);

//UTF8 string util
size_t utf8_string_length(utf8_string s);

//UTF16 string util
size_t utf16_string_length(utf16_string s);
utf16_string utf16_string_concat(utf16_string a, utf16_string b);
utf16_string utf16_string_insert(utf16_string a, utf16_string b, size_t position);
utf16_string utf16_string_remove(utf16_string s, size_t from, size_t length);
utf16_string utf16_string_substring(utf16_string s, size_t from, size_t length);
```
