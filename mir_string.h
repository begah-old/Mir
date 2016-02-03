/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * 		distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * 		means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * 		of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * 		successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * 		software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *		OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * This library processes and stores UTF-8 strings.
 * It is written in C89.
 *
 * TODO:
 *		Decode and encode other character encoding to and from UTF-8
 *
 * Bug report or Feature request:
 * 	Please send any bugs or requests at mathieu.roux22@gmail.com
 */

/** Example program */
#if 0
#include <stdio.h>
#include <stdlib.h>

#define MIR_STRING_IMPLEMENTATION
#define MIR_STRING_NAMESPACE(name) name
#include "mir_string.h"

int main() {
	string str = string_utf8_copy("ét");

	free(string_insert(&str, 2, string_getEncoding(string_getCodepoint(str, 0))));

	printf("%i:%i %s\n", str.charlength, str.bytelength, str.string);

	string str2 = string_utf8_copy("étlol");

	string_insert(&str2, 2, "f");
	string_remove(&str2, 3);

	string_insert_cstr(&str2, 3, "L25");
	string_remove_range(&str2, 3, 6);

	printf("%i:%i %s\n", str2.charlength, str2.bytelength, str2.string);

	string *str3 = malloc(sizeof(string));
	*str3 = string_utf8_copy(str2.string);

	if(string_cmp(str, *str3) == 0) {
		printf("String one and three are the same\n");
	} else
		printf("String one and three are not the same\n");

	if(string_cmp(str2, *str3) == 0) {
		printf("String two and three are the same\n");
	} else
		printf("String two and three are not the same\n");

	string_delete(str);
	string_delete(str2);
	string_deleteptr(str3);

	return EXIT_SUCCESS;
}
#endif /* Example program */

/** Function to print out bits for debugging purposed */
#if 0
void printBits(unsigned int const size, void const * const ptr)
{
 	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;

	puts("Printing bits");
	for (i=0;i < size; i++)
	{
		for (j=7; j>=0; j--)
		{
			byte = (unsigned char)(b[i] & (1<<j));
			byte >>= j;
			printf("%u", byte);

			if(j==4) printf(" ");
		}
		puts(" ");
	}
	puts("");
}
#endif /* Debug function */

#ifndef MIR_STRING_INCLUDE_H
#define MIR_STRING_INCLUDE_H

#ifndef MIR_STRING_NAMESPACE
#define MIR_STRING_NAMESPACE(name) mir##name
#endif /* MIR_STRING_NAMESPACE */

#ifndef MIR_STRING_MALLOC
#include <stdlib.h>
#define MIR_STRING_MALLOC(size) malloc(size)
#endif /* MIR_STRING_MALLOC */

#ifndef MIR_STRING_REALLOC
#include <stdlib.h>
#define MIR_STRING_REALLOC(ptr, newsize) realloc(ptr, newsize)
#endif /* MIR_STRING_REALLOC */

#ifndef MIR_STRING_FREE
#include <stdlib.h>
#define MIR_STRING_FREE(ptr) free(ptr)
#endif /* MIR_STRING_FREE */

#ifndef MIR_STRING_MEMMOVE
#include <stdlib.h>
#define MIR_STRING_MEMMOVE(to, from, size) memmove(to, from ,size)
#endif /* MIR_STRING_MEMMOVE */

#ifndef MIR_STRING_MEMCPY
#include <stdlib.h>
#define MIR_STRING_MEMCPY(to, from, size) memcpy(to, from, size)
#endif /* MIR_STRING_MEMCPY */

#ifdef MIR_STRING_STATIC
#define MIR_STRING_DEF static
#else /* MIR_STRING_STATIC */
#define  MIR_STRING_DEF extern
#endif /* MIR_STRING_STATIC */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * String structure definition
 */
struct MIR_STRING_NAMESPACE(String) {
	char *string;
	int charlength;
	unsigned long bytelength;
};

/**
 * Typedef String structure
 */
typedef struct MIR_STRING_NAMESPACE(String) MIR_STRING_NAMESPACE(string);

/**
 * Create a new string from utf8 or ascii input.
 * No memory is allocated, pointer gave is used.
 *
 * @param cstr raw string pointer to analyze and use
 * @return A new string, no need to free it
 */
MIR_STRING_DEF MIR_STRING_NAMESPACE(string) MIR_STRING_NAMESPACE(string_utf8)(char *cstr);

/**
 * Create a new string from utf8 or ascii input.
 * Memory is allocated to copy the raw string.
 *
 * @param cstr raw string pointer to copy and analyze
 * @return A new string, raw copy of string must be freed
 */
MIR_STRING_DEF MIR_STRING_NAMESPACE(string) MIR_STRING_NAMESPACE(string_utf8_copy)(const char *cstr);

/**
 * Copy a string and allocate a new raw string.
 *
 * @param strc string to be copied
 * @return A new string, new allocated raw string
 */
MIR_STRING_DEF MIR_STRING_NAMESPACE(string) MIR_STRING_NAMESPACE(string_copy)(MIR_STRING_NAMESPACE(string) strc);

/**
 * Analyze and count bytes in raw string.
 *
 * @param string Raw strubg to be analyzed
 * @return Numbre of bytes in raw string
 */
MIR_STRING_DEF unsigned long MIR_STRING_NAMESPACE(string_cstr_bytelength)(const char *string);

/**
 * Insert a character at given position.
 *
 * @param str string to insert character too
 * @param position position where character should be added, range : 0 to charlength
 * @param character utf8 encoded character to insert
 * @return Returns character
 */
MIR_STRING_DEF char *MIR_STRING_NAMESPACE(string_insert)(MIR_STRING_NAMESPACE(string) *str, int position, const char *character);

/**
 * Insert a string at given position.
 *
 * @param str string to insert string too
 * @param position position where string should be added, range : 0 to charlength
 * @param character utf8 encoded string to insert
 * @return Returns character
 */
MIR_STRING_DEF char *MIR_STRING_NAMESPACE(string_insert_cstr)(MIR_STRING_NAMESPACE(string) *str, int position, const char *cstr2);

/**
 * Remove a character at given position.
 *
 * @param str string to remove character from
 * @param position character position to be removed, range : 0 to charlength - 1 (at position charlength is the terminating character)
 */
MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_remove)(MIR_STRING_NAMESPACE(string) *str, int position);

/**
 * Remove a range of character at given positions.
 *
 * @param str string to remove character from
 * @param startposition starting position to remove
 * @param endposition final character position to remove ( exclusive )
 */
MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_remove_range)(MIR_STRING_NAMESPACE(string) *str, int startposition, int endposition);

/**
 * Calculate Unicode codepoint of a character in string.
 *
 * @param str string to be used
 * @param position position of character to be processed, range : 0 to charlength - 1 (at position charlength is the terminating character)
 * @return Unicode codepoint of given character position
 */
MIR_STRING_DEF int MIR_STRING_NAMESPACE(string_getCodepoint)(MIR_STRING_NAMESPACE(string) str, int position);

/**
 * Calculate utf8 character from Unicode codepoint.
 *
 * @param codepoint Unicode codepoint of character
 * @return UTF8 encoded character from codepoint, must be free'd
 */
MIR_STRING_DEF char *MIR_STRING_NAMESPACE(string_getEncoding)(int codepoint);

/**
 * Compares two strings together.
 *
 * @param str1 first string to be compared
 * @param str2 second string to be compared to str1
 * @return 0 if success ( works like strcmp )
 */
MIR_STRING_DEF int MIR_STRING_NAMESPACE(string_cmp)(MIR_STRING_NAMESPACE(string) str1, MIR_STRING_NAMESPACE(string) str2);

/**
 * Free the memory allocated by the string.
 * Caution : Only use this if string was created by string_utf8_copy or created with string_utf8 with a non string literal as parameter.
 *
 * @param str string to be deleted
 */
MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_delete)(MIR_STRING_NAMESPACE(string) str);

/**
 * Free the memory allocated by the string and the pointer.
 * Caution : Only use this if string was created by string_utf8_copy or created with string_utf8 with a non string literal as parameter.
 *
 * @param str pointer to string to be deleted
 */
MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_deleteptr)(MIR_STRING_NAMESPACE(string) *str);

#ifdef __cplusplus
}
#endif

#endif /* MIR_STRING_INCLUDE_H */

#ifdef MIR_STRING_IMPLEMENTATION
#include <string.h>

static unsigned int codepoint__size(char character) {
	unsigned char byte = (unsigned char)character;
	if (byte == 0) { return 1; } /* NULL byte. */
	else if (byte <= 0x7f) { return 1; } /* 0x74 = 0111 1111 */
	else if (byte <= 0xbf) { return 0; } /* 1011 1111. Invalid for a starting byte. */
	else if (byte <= 0xdf) { return 2; } /* 1101 1111 */
	else if (byte <= 0xef) { return 3; } /* 1110 1111 */
	else if (byte <= 0xf7) { return 4; } /* 1111 0111 */
	else if (byte <= 0xfb) { return 5; } /* 1111 1011 */
	else if (byte <= 0xfd) { return 6; } /* 1111 1101 */
	else { return 0; }
}

MIR_STRING_DEF MIR_STRING_NAMESPACE(string) MIR_STRING_NAMESPACE(string_utf8)(char *cstr)
{
	MIR_STRING_NAMESPACE(string) str;
	const char *original = cstr;
	unsigned int codepointSize;

	str.bytelength = MIR_STRING_NAMESPACE(string_cstr_bytelength)(cstr); /* Calculate number of bytes in string */

	str.string = cstr;
	str.charlength = 0;

	while(cstr - original < str.bytelength) { /* Count number of characters */
		str.charlength++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	}
	str.charlength--; /* Remove the terminating character in the count */

	return str;
}

MIR_STRING_DEF MIR_STRING_NAMESPACE(string) MIR_STRING_NAMESPACE(string_utf8_copy)(const char *cstr)
{
	MIR_STRING_NAMESPACE(string) str = MIR_STRING_NAMESPACE(string_utf8)((char *)cstr); /* Create a new string like normal */
	str.string = MIR_STRING_MALLOC(sizeof(char) * str.bytelength);
	MIR_STRING_MEMCPY(str.string, cstr, sizeof(char) * str.bytelength); /* Copy string */

	return str;
}

MIR_STRING_DEF MIR_STRING_NAMESPACE(string) MIR_STRING_NAMESPACE(string_copy)(MIR_STRING_NAMESPACE(string) strc)
{
	MIR_STRING_NAMESPACE(string) str = strc;
	str.string = MIR_STRING_MALLOC(sizeof(char) * str.bytelength);
	MIR_STRING_MEMCPY(str.string, strc.string, sizeof(char) * str.bytelength); /* Copy string */

	return str;
}

MIR_STRING_DEF unsigned long MIR_STRING_NAMESPACE(string_cstr_bytelength)(const char *string) {
	const char *original = string;
	while(*string) string++;
	return string - original + 1; /* + 1 is for the terminating character */
}

MIR_STRING_DEF char *MIR_STRING_NAMESPACE(string_insert)(MIR_STRING_NAMESPACE(string) *str, int position, const char *character) {
	const char *cstr = str->string;
	int pos = 0, size = codepoint__size(*character);
	unsigned long index;
	size_t codepointSize;

	while(pos < position) { /* Loop until at given character */
		pos++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	}
	index = cstr - str->string; /* Calculate byte offset */

	str->string = MIR_STRING_REALLOC(str->string, sizeof(char) * (str->bytelength + size));
	MIR_STRING_MEMMOVE(str->string + index + size, str->string + index, sizeof(char) * (str->bytelength - index)); /* Shift right string to the right */
	MIR_STRING_MEMCPY(str->string + index, character, sizeof(char) * size); /* Copy new character */
	str->bytelength += size; str->charlength++;

	return (char *)character;
}

MIR_STRING_DEF char *MIR_STRING_NAMESPACE(string_insert_cstr)(MIR_STRING_NAMESPACE(string) *str, int position, const char *cstr2) {
	const char *cstr = str->string;
	int pos = 0, totalSize = 0, totalCharacters = 0;
	unsigned long index;
	size_t codepointSize;

	while(pos < position) { /* Loop until at given character */
		pos++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	}
	index = cstr - str->string; /* Calculate byte offset */

	cstr = cstr2;
	while(*cstr) {
		int cdSize = codepoint__size(*cstr);
		totalCharacters++;
		totalSize += cdSize;
		cstr += cdSize;
	}

	str->string = MIR_STRING_REALLOC(str->string, sizeof(char) * (str->bytelength + totalSize));
	MIR_STRING_MEMMOVE(str->string + index + totalSize, str->string + index, sizeof(char) * (str->bytelength - index)); /* Shift right string to the right */
	MIR_STRING_MEMCPY(str->string + index, cstr2, sizeof(char) * totalSize); /* Copy new character */
	str->bytelength += totalSize; str->charlength+= totalCharacters;

	return (char *)cstr2;
}

MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_remove)(MIR_STRING_NAMESPACE(string) *str, int position) {
	if(position < 0 || position >= str->charlength) return;

	const char *cstr = str->string;
	int pos = 0, size;
	unsigned long index;
	size_t codepointSize;

	while(pos < position) { /* Loop until at given character */
		pos++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	}

	index = cstr - str->string; /* Calculate byte offset */

	size = codepoint__size(str->string[index]);

	MIR_STRING_MEMMOVE(str->string + index, str->string + index + size, sizeof(char) * (str->bytelength - index - size)); /* Shift right string to the left */
	str->bytelength -= size; str->charlength--;
	str->string = MIR_STRING_REALLOC(str->string, sizeof(char) * str->bytelength);
}

MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_remove_range)(MIR_STRING_NAMESPACE(string) *str, int startposition, int endposition)
{
	if(startposition < 0 || startposition >= str->charlength) return;
	if(endposition < 0 || endposition >= str->charlength || endposition < startposition) return;
	if(startposition == endposition) MIR_STRING_NAMESPACE(string_remove)(str, startposition);

	const char *cstr = str->string;
	int pos = 0;
	unsigned long index1, index2, characterCount = 0;
	size_t codepointSize;

	while(pos < startposition) { /* Loop until at given character */
		pos++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	}

	index1 = cstr - str->string; /* Calculate byte offset */

	do { /* Loop until at given character */
		pos++;
		characterCount++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	} while(pos < endposition);

	index2 = cstr - str->string; /* Calculate byte offset */

	MIR_STRING_MEMMOVE(str->string + index1, str->string + index2, sizeof(char) * (str->bytelength - index2)); /* Shift right string to the left */
	str->bytelength -= index2 - index1; str->charlength -= characterCount;
	str->string = MIR_STRING_REALLOC(str->string, sizeof(char) * str->bytelength);
}

MIR_STRING_DEF int MIR_STRING_NAMESPACE(string_getCodepoint(MIR_STRING_NAMESPACE(string) str, int position))
{
	const char *cstr = str.string;
	int pos = 0, size;
	unsigned long index;
	size_t codepointSize;

	while(pos < position) { /* Loop until at given character */
		pos++;

		codepointSize = codepoint__size(*cstr);
		if(codepointSize == 0) {
			/* Handle error */
		}
		cstr += codepointSize;
	}

	index = cstr - str.string; /* Calculate byte offset */
	size = codepoint__size(str.string[index]);

	if(size == 1) {
		return (unsigned char)str.string[index];
	} else if(size == 2) {
		return ((unsigned char)str.string[index] - 192) * 64 + (unsigned char)str.string[index + 1] - 128;
	} else if(size == 3) {
		return ((unsigned char)str.string[index] - 224) * 4096 + ((unsigned char)str.string[index + 1] - 128) * 64 + (unsigned char)str.string[index + 2] - 128;
	} else {
		return ((unsigned char)str.string[index] - 240) * 262144 + ((unsigned char)str.string[index + 1] - 128) * 4096 + ((unsigned char)str.string[index + 2] - 128) * 64 + (unsigned char)str.string[index + 3] - 128;
	}
}

MIR_STRING_DEF char *MIR_STRING_NAMESPACE(string_getEncoding)(int c) {
	unsigned char *buf = MIR_STRING_MALLOC(sizeof(unsigned char) * 6);

	if (c <= 0x7F)  /* 0XXX XXXX one byte */
	{
		buf[0] = (unsigned char) c;
	}
	else if (c <= 0x7FF)  /* 110X XXXX  two bytes */
	{
		buf[0] = (unsigned char) ( 0xC0 | (c >> 6) );
		buf[1] = (unsigned char) ( 0x80 | (c & 0x3F) );
	}
	else if (c <= 0xFFFF)  /* 1110 XXXX  three bytes */
	{
		buf[0] = (unsigned char)(0xE0 | (c >> 12));
		buf[1] = (unsigned char)(0x80 | ((c >> 6) & 0x3F));
		buf[2] = (unsigned char)(0x80 | (c & 0x3F));
	}
	else if (c <= 0x1FFFFF)  /* 1111 0XXX  four bytes */
	{
		buf[0] = (unsigned char) (0xF0 | (c >> 18));
		buf[1] = (unsigned char) (0x80 | ((c >> 12) & 0x3F));
		buf[2] = (unsigned char) (0x80 | ((c >> 6) & 0x3F));
		buf[3] = (unsigned char) (0x80 | (c & 0x3F));
	}
	else if (c <= 0x3FFFFFF)  /* 1111 10XX  five bytes */
	{
		buf[0] = (unsigned char) (0xF8 | (c >> 24));
		buf[1] = (unsigned char) (0x80 | (c >> 18));
		buf[2] = (unsigned char) (0x80 | ((c >> 12) & 0x3F));
		buf[3] = (unsigned char) (0x80 | ((c >> 6) & 0x3F));
		buf[4] = (unsigned char) (0x80 | (c & 0x3F));
	}
	else if (c <= 0x7FFFFFFF)  /* 1111 110X  six bytes */
	{
		buf[0] = (unsigned char) (0xFC | (c >> 30));
		buf[1] = (unsigned char) (0x80 | ((c >> 24) & 0x3F));
		buf[2] = (unsigned char) (0x80 | ((c >> 18) & 0x3F));
		buf[3] = (unsigned char) (0x80 | ((c >> 12) & 0x3F));
		buf[4] = (unsigned char) (0x80 | ((c >> 6) & 0x3F));
		buf[5] = (unsigned char) (0x80 | (c & 0x3F));
	}
	else
	{
		free(buf);
		buf = NULL;
	};

	return (char *)buf;
}

MIR_STRING_DEF int MIR_STRING_NAMESPACE(string_cmp)(MIR_STRING_NAMESPACE(string) str1, MIR_STRING_NAMESPACE(string) str2) {
	return strcmp(str1.string, str2.string);
}

MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_delete)(MIR_STRING_NAMESPACE(string) str) {
	MIR_STRING_FREE(str.string);
	str.string = NULL;
}

MIR_STRING_DEF void MIR_STRING_NAMESPACE(string_deleteptr)(MIR_STRING_NAMESPACE(string) *str) {
	MIR_STRING_FREE(str->string);
	MIR_STRING_FREE(str);
}
#endif /* MIR_STRING_IMPLEMENTATION */


