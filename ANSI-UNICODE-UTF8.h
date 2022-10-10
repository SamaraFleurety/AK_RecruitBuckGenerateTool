#ifndef _ANSI_UNICODE_UTF8_h
#define _ANSI_UNICODE_UTF8_h

wchar_t* ANSIToUnicode(const char* str);

char* UnicodeToANSI(const wchar_t* str);

wchar_t* UTF8ToUnicode(const char* str);

char* UnicodeToUTF8(const wchar_t* str);

char* ANSIToUTF8(const char* str);

char* w2m(const wchar_t* wcs);

wchar_t* m2w(const char* mbs);

char* UTF8ToANSI(const char* str);

int test();
#endif
