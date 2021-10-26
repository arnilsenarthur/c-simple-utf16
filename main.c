#include "lib.c"
#include <stdio.h>

void main()
{
    utf16_string s = utf8_to_utf16("<😚>");
    utf16_string a = utf8_to_utf16("Hillo ");
    utf16_string b = utf8_to_utf16(" world!");
    utf16_string c = utf16_string_concat(a, b);
    utf16_string d = utf16_string_insert(c, s, 6);

    printf("%s", utf16_to_utf8(d));
}
