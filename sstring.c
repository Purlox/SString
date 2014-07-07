#include "sstring.h"

#include <string.h>

typedef SString * (s_str_init_func)(SString * restrict s_str,
                                     const char * restrict string,
                                     size_t size);


static SString *
NULL_array_0_size(SString * restrict s_str,
                  const char * restrict string,
                  size_t size)
{
	s_str->length = 0;
	s_str->size = 0;
	s_str->string = NULL;

	return s_str;
}


static SString *
NULL_array_non0_size(SString * restrict s_str,
                     const char * restrict string,
                     size_t size)
{
	s_str->length = 0;
	s_str->size = size;

	s_str->string = malloc(s_str->size);
	if(NULL == s_str->string)
		return NULL;

	s_str->string[0] = '\0';

	return s_str;
}


static SString *
nonNULL_array_0_size(SString * restrict s_str,
                     const char * restrict string,
                     size_t size)
{
	s_str->length = strlen(string);
	s_str->size = s_str->length + 1;

	s_str->string = malloc(s_str->size);
	if(NULL == s_str->string)
		return NULL;

	strncpy(s_str->string, string, s_str->size);

	return s_str;
}


static SString *
nonNULL_array_non0_size(SString * restrict s_str,
                        const char * restrict string,
                        size_t size)
{
	s_str->size = size;
	s_str->length = (size <= strlen(string)) ? size - 1 : strlen(string);

	s_str->string = malloc(s_str->size);
	if(NULL == s_str->string)
		return NULL;

	strncpy(s_str->string, string, s_str->length);
	s_str->string[s_str->length] = '\0';

	return s_str;
}


static s_str_init_func *
get_function(const char * restrict string,
             size_t size)
{
	if(NULL == string)
		switch(size) {
		case 0:
			return NULL_array_0_size;

		default:
			return NULL_array_non0_size;
		}
	else
		switch(size) {
		case 0:
			return nonNULL_array_0_size;

		default:
			return nonNULL_array_non0_size;
		}
}


SString *
new_sstring(const char * restrict string,
            size_t size)
{
	SString * restrict s_str;

	s_str_init_func * init_function = get_function(string, size);

	s_str = malloc(sizeof(SString));
	if(NULL == s_str)
		return NULL;

	return((init_function)(s_str, string, size));
}


void
s_empty(SString * restrict str)
{
	if(str == NULL)
		return ;

	if(str->string != NULL) {
		free(str->string);
		str->string = NULL;
	}

	str->length = 0;
	str->size = 0;
}


void
s_free(SString * restrict str)
{
	if(str == NULL)
		return ;

	if(str->string != NULL)
		free(str->string);

	free(str);
	str = NULL;
}


char *
s_sstonews(SString * restrict str)
{
	char * s;
	unsigned int i;

	if(NULL == str  ||  NULL == str->string)
		return NULL;

	 s = malloc(str->length + 1);

	for(i = 0; i < str->length; i++) {
		s[i] = str->string[i];
	}

	s[str->length] = '\0';

	return s;
}

// Version 2
char *
s_sstonews2(SString * restrict str)
{
	char * s;
	register unsigned int i;

	if(NULL == str  ||  NULL == str->string)
		return NULL;

	s = malloc(str->length + 1);

	for(i = 0; i < str->length; i++) {
		s[i] = str->string[i];
	}

	s[str->length] = '\0';

	return s;
}


void *
s_memnull(void * restrict memory,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)memory + i) = 0;

	return memory;
}

// version 2
void *
s_memnull2(void * memory,
           size_t num)
{
	char * restrict _help = memory;
	unsigned int i;

	for(i = 0; i < num; i++)
		_help[i] = 0;

	return _help;
}

// version 3
void *
s_memnull3(void * restrict memory,
           size_t num)
{
	register unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)memory + i) = 0;

	return memory;
}

// version 4
void *
s_memnull4(void * memory,
           size_t num)
{
	char * restrict _help = memory;
	register unsigned int i;

	for(i = 0; i < num; i++)
		_help[i] = 0;

	return _help;
}


void *
s_memcpy(void * destination,
         const void * source,
         size_t num)
{
	char * restrict _help = destination;
	const char * restrict _help2 = source;
	unsigned int i;

	for(i = 0; i < num; i++)
		_help[i] = _help2[i];

	return _help;
}

// version 2
void *
s_memcpy2(void * restrict destination,
          const void * restrict source,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)destination + i) = *((const char*)source + i);

	return destination;
}

// version 3
void *
s_memcpy3(void * restrict destination,
          const void *restrict source,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)destination++) = *((const char*)source++);

	return destination;
}


void *
s_memmove(void * destination,
          const void * source,
          size_t num)
{
	char * restrict _copy = malloc(num);
	char * _help = destination;
	const char *_help2 = source;
	unsigned int i;

	for(i = 0; i < num; i++) {
		_copy[i] = _help2[i];
	}

	for(i = 0; i < num; i++) {
		_help[i] = _copy[i];
	}

	free(_copy);

	return _help;
}

// version 2
void *
s_memmove2(void * destination,
           const void * source,
           size_t num)
{
	void * restrict _copy = malloc(num);
	unsigned int i;

	for(i = 0; i < num; i++) {
		*((char *)_copy + i) = *((const char*)source + i);
	}

	for(i = 0; i < num; i++) {
		*((char*)destination + i) = *((char *)_copy + i);
	}

	free(_copy);

	return destination;
}

// version 3
void *
s_memmove3(void * destination,
           const void * source,
           size_t num)
{
	void * restrict _copy = malloc(num);
	unsigned int i;

	for(i = 0; i < num; i++) {
		*((char *)_copy++) = *((const char*)source++);
	}

	_copy -= num;

	for(i = 0; i < num; i++) {
		*((char*)destination++) = *((char *)_copy++);
	}

	_copy -= num;
	free(_copy);

	return destination;
}


SString *
s_strcpy(SString * restrict destination,
         const SString * restrict source)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = length;

	return destination;
}

// Version 2
SString *
s_strcpy2(SString * restrict destination,
          const SString * restrict source)
{
	register unsigned int i;
	register size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = length;

	return destination;
}

// Version 3
SString *
s_strcpy3(SString * restrict destination,
          const SString * restrict source)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length / 8; i++)
		((uint64_t *)(destination->string))[i] = ((uint64_t *)(source->string))[i];

	for(i *= 8; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = i;

	return destination;
}

// Version 4
SString *
s_strcpy4(SString * restrict destination,
          const SString * restrict source)
{
	unsigned int i;
	uint64_t * _dest = (uint64_t *)(destination->string);
	uint64_t * _sour = (uint64_t *)(source->string);
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length / 8; i++)
		_dest[i] = _sour[i];

	for(i *= 8; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = i;

	return destination;
}


SString *
s_strncpy(SString * restrict destination,
          const SString * restrict source,
          size_t num)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	if(0 == num)
		return destination;

	if(num > source->length)
		length = (source->length > destination->size) ? destination->size
		          : source->length;
	else
		length = (num > destination->size) ? destination->size : num;

	for(i = 0; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = length;

	return destination;
}

// version 2
SString *
s_strncpy2(SString * restrict destination,
           const SString * restrict source,
           size_t num)
{
	unsigned int i;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	if(0 == num)
		return destination;

	for(i = 0; i < num / 8; i++)
		((uint64_t *)(destination->string))[i] = ((uint64_t *)(source->string))[i];

	for(i *= 8; i < num; i++)
		destination->string[i] = source->string[i];

	destination->length = num;

	return destination;
}

// version 3
SString *
s_strncpy3(SString * destination,
           const SString * source,
           size_t num)
{
	unsigned int i;
	uint64_t * restrict _dest = (uint64_t *)(destination->string);
	uint64_t * restrict _sour = (uint64_t *)(source->string);

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	if(0 == num)
		return destination;

	_dest = (uint64_t *)(destination->string);
	_sour = (uint64_t *)(source->string);

	for(i = 0; i < num / 8; i++)
		_dest[i] = _sour[i];

	for(i *= 8; i < num; i++)
		_dest[i] = _sour[i];

	destination->length = num;

	return destination;
}


SString *
s_strcpyX(SString * restrict destination,
          const char * source,
          size_t num)
{
	unsigned int i;
	size_t max;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string)
		return NULL;

	max = (destination->size >= num) ? num - 1 : destination->size - 1;
	for(i = 0; i < max; i++)
		destination->string[i] = source[i];
	destination->string[max] = '\0';

	destination->length = max;

	return destination;
}


SString *
s_strcat(SString * restrict destination,
         const SString * restrict source)
{
	unsigned int i;
	unsigned int j;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->length) ? destination->length
	         : source->length;

	for(i = destination->length, j = 0; j < length; i++, j++)
		destination->string[i] = source->string[j];

	destination->length += source->length;

	return destination;
}

// version 2
SString *
s_strcat2(SString * restrict destination,
          const SString * restrict source)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->length) ? destination->length
	         : source->length;

	for(i = 0; i < length; i++)
		destination->string[i + destination->length] = source->string[i];

	destination->length += source->length;

	return destination;
}


SString *
s_strncat(SString * restrict destination,
          const SString * restrict source,
          size_t num)
{
	unsigned int i;
	unsigned int j;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	for(i = destination->length, j = 0; j < num; i++, j++)
		destination->string[i] = source->string[j];

	destination->length += num;

	return destination;
}

// version 2
SString *
s_strncat2(SString * restrict destination,
           const SString * restrict source,
           size_t num)
{
	unsigned int i;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	for(i = 0; i < num; i++)
		destination->string[i + destination->length] = source->string[i];

	destination->length += num;

	return destination;
}

// version 3
SString *
s_strncat3(SString * restrict destination,
           const SString * restrict source,
           size_t num)
{
	int _help;
	unsigned int i;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	_help = destination->length;

	for(i = 0; i < num; i++)
		destination->string[i + _help] = source->string[i];

	destination->length += num;

	return destination;
}


int
s_memcmp(const void * ptr1,
         const void * ptr2,
         size_t num)
{
	const char * restrict _p1 = ptr1;
	const char * restrict _p2 = ptr2;
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(_p1[i] > _p2[i])
			return (_p1[i] - _p2[i]);
		else if(_p1[i] < _p2[i])
			return (_p1[i] - _p2[i]);
	}

	return 0;
}

// Version 2
int
s_memcmp2(const void * restrict ptr1,
          const void * restrict ptr2,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(*((char*)ptr1 + i) > *((char*)ptr2 + i))
			return (*((char*)ptr1 + i) - *((char*)ptr2 + i));
		else if(*((char*)ptr1 + i) < *((char*)ptr2 + i))
			return (*((char*)ptr1 + i) - *((char*)ptr2 + i));
	}

	return 0;
}


int
s_strcmp(const SString * restrict str1,
         const SString * restrict str2)
{
	unsigned int i;

	for(i = 0; i < str1->length && i < str2->length; i++) {
		if(str1->string[i] > str2->string[i])
			return (str1->string[i] - str2->string[i]);
		else if(str1->string[i] < str2->string[i])
			return (str1->string[i] - str2->string[i]);
	}

	return 0;
}

// Version 2
int
s_strcmp2(const SString * str1,
          const SString * str2)
{
	unsigned int i;
	const SString * restrict _help = str1;
	const SString * restrict _help2 = str2;

	for(i = 0; i < _help->length && i < _help2->length; i++) {
		if(_help->string[i] > _help2->string[i])
			return (_help->string[i] - _help2->string[i]);
		else if(_help->string[i] < _help2->string[i])
			return (_help->string[i] - _help2->string[i]);
	}

	return 0;
}


int
s_strncmp(const SString * str1,
          const SString * str2,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num && i < str1->length && i < str2->length; i++) {
		if(str1->string[i] > str2->string[i])
			return (str1->string[i] - str2->string[i]);
		else if(str1->string[i] < str2->string[i])
			return (str1->string[i] - str2->string[i]);
	}

	return 0;
}


void *
s_memchr(void * ptr,
         int value,
         size_t num)
{
	char * _p = ptr;
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(_p[i] == value)
			return(ptr+i);
	}

	return NULL;
}

// version 2
void *
s_memchr2(void * ptr,
          int value,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(*((char *)ptr + i) == value)
			return(ptr+i);
	}

	return NULL;
}


SString *
s_strchr(SString * str,
         int character)
{
	unsigned int i;

	for(i = 0; i < str->length; i++) {
		if(str->string[i] == character)
			return(str+i);
	}

	return NULL;
}


size_t
s_strspn(const SString * str1,
         const SString * str2)
{
	unsigned int i;
	unsigned int j;

	for(i = 0; i < str1->length; i++) {
		for(j = 0; j < str2->length; j++) {
			if(str1->string[i] != str2->string[j])
				return i;
		}
	}

	return -1;
}


size_t
s_strcspn(const SString * str1,
          const SString * str2)
{
	unsigned int i;
	unsigned int j;

	for(i = 0; i < str1->length; i++) {
		for(j = 0; j < str2->length; j++) {
			if(str1->string[i] == str2->string[j])
	 			return i;
		}
	}

	return -1;
}


char *
s_strpbrk(const SString * str1,
          const SString * str2)
{
	unsigned int i;
	unsigned int j;

	for(i = 0; i < str1->length; i++) {
		for(j = 0; j < str2->length; j++) {
			if(str1->string[i] == str2->string[j])
				return(&(str1->string[i]));
		}
	}

	return NULL;
}


char *
s_strrchr(SString * str,
          int character)
{
	char * pointer = NULL;
	unsigned int i;

	for(i = 0; i < str->length; i++) {
		if(str->string[i] == character)
			pointer = &(str->string[i]);
	}

	return pointer;
}


char *
s_strstr(const SString * str1,
         const SString * str2)
{
	unsigned int matching = 0;
	unsigned int i;

	for(i = 0; i < str1->length; i++) {
		if(str1->string[i] == str2->string[matching])
			matching++;
		else if(matching > 0)
			matching = 0;

		if(matching == str2->length)
			return&(str1->string[i-matching]);
	}

	return NULL;
}


char *
s_strtok(SString * str,
         const SString * delimiters)
{
	static char * _lastPos = NULL;
	static SString * _str = NULL;
	char * startOfToken = NULL;
	char numOfDelimiters;
	unsigned int lengthOfToken = 0;
	unsigned int i;
	unsigned int j;
	char * newString;

	if(str != NULL)
		_str = str;

	if(_lastPos == NULL)
		_lastPos = &(str->string[0]);

	for(i = _lastPos - &(str->string[0]); i < _str->length; i++) {
		numOfDelimiters = 0;

		for(j = 0; j < delimiters->length; j++) {
			if(_str->string[i] == delimiters->string[j])
				numOfDelimiters++;
		}

		if(numOfDelimiters != 0) {
			startOfToken = &(_str->string[i]);
			break;
		}
	}

	if(startOfToken == NULL) // No new token found
		return NULL;

	lengthOfToken = _lastPos - startOfToken;
	newString = malloc(lengthOfToken+1);

	for(i = 0; i < lengthOfToken; i++)
		newString[i] = *(_lastPos + i);
	newString[lengthOfToken] = '\0';

	_lastPos = startOfToken;

	return newString;
}


SString *
s_sstrtok(SString * str,
          const SString * delimiters)
{
	static char * _lastPos = NULL;
	static SString *_str = NULL;
	char * startOfToken = NULL;
	char numOfDelimiters;
	unsigned int lengthOfToken = 0;
	unsigned int i;
	unsigned int j;
	SString * newSstr = malloc(sizeof(SString));

	if(str != NULL)
		_str = str;

	if(_lastPos == NULL)
		_lastPos = &(_str->string[0]);

	for(i = _lastPos - &(_str->string[0]); i < _str->length; i++) {
		numOfDelimiters = 0;

		for(j = 0; j < (*delimiters).length; j++) {
			if(_str->string[i] == delimiters->string[j])
				numOfDelimiters++;
		}

		if(numOfDelimiters != 0) {
			startOfToken = &(_str->string[i]);
			break;
		}
	}

	if(startOfToken == NULL) // No new token found
		return NULL;

	lengthOfToken = _lastPos - startOfToken;
	newSstr->string = malloc(lengthOfToken);

	for(i = 0; i < lengthOfToken; i++)
		newSstr->string[i] = *(_lastPos + i);
	newSstr->length = i;

	_lastPos = startOfToken;

	return newSstr;
}


void *
s_memset(void * ptr,
         int value,
         size_t num)
{
	char * _p = ptr;
	unsigned int i;

	for(i = 0; i < num; i++)
		_p[i] = value;

	return ptr;
}

// version 2
void *
s_memset2(void * ptr,
          int value,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char *)ptr + i) = value;

	return ptr;
}
