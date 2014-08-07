#include "sstring.h"

#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static void
ensure_necessary_size(SString * str,
                      size_t size,
                      bool to_realloc)
{
	if(str->size > size)
		return ;

	if(to_realloc == true) {
		realloc(str->string, str->size);
	} else {
		if(str->string != NULL)
			free(str->string);

		malloc(str->string);
	}
}


typedef SString (new_SString_func)(const char * string,
                                   size_t size);


static SString
NULL_string_0_size(const char * string,
                   size_t size)
{
	return (SString){ 0, 0, NULL };
}

static SString
NULL_string_non0_size(const char * string,
                      size_t size)
{
	SString str = (SString){ 0, size, NULL };

	str.string = malloc(str.size);
	if(NULL == str.string)
		return (SString){0};

	str.string[0] = '\0';

	return str;
}

static SString
nonNULL_string_0_size(const char * string,
                      size_t size)
{
	size_t length = strlen(string);
	SString str = (SString){ length, length + 1, NULL };

	str.string = malloc(str.size);
	if(NULL == str.string)
		return (SString){0};

	strcpy(str.string, string);

	return str;
}

static SString
nonNULL_string_non0_size(const char * string,
                         size_t size)
{
	size_t length = strlen(string);
	SString str = (SString){
		.size = size,
		.length = (length > size) ? size - 1 : length
	};

	str.string = malloc(str.size);
	if(NULL == str.string)
		return (SString){0};

	strncpy(str.string, string, str.length);
	str.string[str.length] = '\0';

	return str;
}

static new_SString_func *
get_new_SString_func(const char * string,
                     size_t size)
{
	if(NULL == string)
		switch(size) {
		case 0:
			return NULL_string_0_size;

		default:
			return NULL_string_non0_size;
		}
	else
		switch(size) {
		case 0:
			return nonNULL_string_0_size;

		default:
			return nonNULL_string_non0_size;
		}
}

SString
new_sstring(const char * string,
            size_t size)
{
	new_SString_func * new_func = get_new_SString_func(string, size);
	return (new_func)(string, size);
}


inline void
empty_sstring(SString * str)
{
	if(str == NULL)
		return ;

	str->string = NULL;
	str->length = 0;
	str->size = 0;
}


inline void
free_sstring(SString * str)
{
	if(str == NULL)
		return ;

	if(str->string != NULL)
		free(str->string);
	empty_sstring(str);
}


inline void
free_sstrings(struct SStrings ** strs)
{
	size_t i;

	if(strs == NULL  ||  *strs == NULL)
		return ;

	for(i = 0; i < (*strs)->length; i++)
		if((*strs)->sstrings[i].string != NULL)
			free((*strs)->sstrings[i].string);

	free(*strs);

	*strs = NULL;
}


inline SString
connect_sstrings(const struct SStrings * strs,
                 const SString * connector)
{
	size_t i;
	SString str = (SString){0};

	if(strs == NULL  ||  strs->length == 0  ||  connector == NULL
		    ||  connector->string == NULL)
		return (SString){0};

	for(i = 0; i < strs->length - 1; i++) {
		append_sstring(&str, &(strs->sstrings[i]));
		append_sstring(&str, connector);
	}
	append_sstring(&str, &(strs->sstrings[i]));

	return str;
}


static SString
to_X_sstring(const SString * str,
             bool to_lower)
{
	long location = 0;
	long previous_location = 0;
	SString new_str = (SString){0};
	SString Xcase = (to_lower == true) ? SSTRING("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
	                : SSTRING("abcdefghijklmnopqrstuvwxyz");
	int (* to_X_case)(int) = (to_lower == true) ? tolower : toupper;

	for(;;) {
		location = find_chars_in_sstring(str, &Xcase, location, 0);

		if(location == -1  ||  -2 == location)
			break;

		append_n_sstring(&new_str, str, previous_location,
		                 location - previous_location);
		new_str.string[location] = (to_X_case)(str->string[location]);
		new_str.length++;

		previous_location = location + 1;
	}

	append_n_sstring(&new_str, str, previous_location, 0);

	return new_str;
}


SString
to_lower_sstring(const SString * str)
{
	if(str == NULL  ||  str->string == NULL)
		return (SString){0};

	return to_X_sstring(str, true);
}


SString
to_upper_sstring(const SString * str)
{
	if(str == NULL  ||  str->string == NULL)
		return (SString){0};

	return to_X_sstring(str, false);
}


int
copy_sstring(SString * restrict destination,
             const SString * restrict source)
{
	if(destination == NULL  ||  source == NULL  ||  NULL == source->string)
		return -1;

	ensure_necessary_size(destination, source->length + 1, false);
	if(destination == NULL)
		return -2;

	strcpy(destination->string, source->string);

	destination->length = source->length;

	return 1;
}


int
copy_n_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start,
               size_t num)
{
	size_t length;
	char temp;

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string
		    ||  start >= source->length)
		return -1;

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	ensure_necessary_size(destination, length + 1, false);
	if(destination == NULL)
		return -2;

	temp = source->string[start + length];
	source->string[start + length] = '\0';

	strcpy(destination->string, &(source->string[start]));

	source->string[start + length] = temp;

	destination->length = length;

	return 1;
}


int
copy_string_to_sstring(SString * restrict destination,
                       const char * restrict source,
                       size_t num)
{
	size_t len;
	size_t lenght;

	if(destination == NULL  ||  source == NULL)
		return -1;

	len = strlen(source);
	lenght = (num > len  ||  0 == num) ? len : num;

	ensure_necessary_size(destination, lenght + 1, false);
	if(destination == NULL)
		return -2;

	strncpy(destination->string, source, lenght);
	destination->string[lenght] = '\0';

	destination->length = lenght;

	return 1;
}


int
append_sstring(SString * restrict destination,
               const SString * restrict source)
{
	if(destination == NULL  ||  source == NULL  ||  NULL == source->string)
		return -1;

	ensure_necessary_size(destination, source->length + destination->length + 1, true);
	if(destination == NULL)
		return -2;

	strcpy(&(destination->string[destination->length]), source->string);

	destination->length += source->length;

	return 1;
}


int
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num)
{
	size_t length;
	char temp;

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string
		    ||  start >= source->length)
		return -1;

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	ensure_necessary_size(destination, length + destination->length + 1, true);
	if(destination == NULL)
		return -2;

	temp = source->string[start + length];
	source->string[start + length] = '\0';

	strcpy(&(destination->string[destination->length]), &(source->string[start]));

	source->string[start + length] = temp;

	destination->length += length;

	return 1;
}


int
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t insert_start)
{
	SString temp;

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string)
		return -1;

	ensure_necessary_size(destination, destination->length + source->length + 1, true);
	if(destination == NULL)
		return -2;

	temp = new_sstring(&(destination->string[insert_start]), 0);

	destination->string[insert_start] = '\0';
	destination->length = insert_start;

	append_sstring(destination, source);
	append_sstring(destination, &temp);

	free(temp.string);

	return 1;
}


int
insert_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t insert_start,
                 size_t source_start,
                 size_t num)
{
	SString temp;
	size_t length;

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string
		    ||  source_start >= source->length)
		return -1;

	length = (source_start + num > source->length  ||  num == 0)
	         ? source->length - source_start : num;

	ensure_necessary_size(destination, destination->length + length + 1, true);
	if(destination == NULL)
		return -2;

	temp = new_sstring(&(destination->string[insert_start]), 0);

	destination->string[insert_start] = '\0';
	destination->length = insert_start;

	append_n_sstring(destination, source, source_start, length);
	append_sstring(destination, &temp);

	free(temp.string);

	return 1;
}


int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2)
{
	if(str1 == NULL  ||  str2 == NULL  ||  NULL == str1->string
		    ||  NULL == str2->string)
		return 0;

	return strcmp(str1->string, str2->string);
}


int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num)
{
	int return_val;
	size_t length;
	char temp1;
	char temp2;

	if(str1 == NULL  ||  str2 == NULL  ||  NULL == str1->string
		    ||  NULL == str2->string  ||  num == 0)
		return 0;

	length = (str1->length > str2->length) ? str2->length : str1->length;

	if(num >= length)
		return strcmp(str1->string, str2->string);
	else
		length = num;

	temp1 = str1->string[length];
	temp2 = str2->string[length];
	str1->string[length] = '\0';
	str2->string[length] = '\0';

	return_val = strcmp(str1->string, str2->string);

	str1->string[length] = temp1;
	str2->string[length] = temp2;

	return return_val;
}


long
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      size_t start,
                      bool inverse)
{
	size_t ret_value;
	char * found_char;

	if(str == NULL  ||  chars == NULL  ||  NULL == str->string
		    ||  NULL == chars->string  ||  start >= str->length)
		return -1;

	if(inverse == false)
		ret_value = strcspn(&(str->string[start]), chars->string);
	else
		ret_value = strspn(&(str->string[start]), chars->string);

	if(ret_value != str->length)
		return ret_value;

	found_char = strchr(chars->string, str->string[str->length - 1]);

	if(inverse == false)
		return (found_char == NULL) ? -2 : str->length - 1;
	else
		return (found_char != NULL) ? -2 : str->length - 1;
}


long
find_str_in_sstring(const SString * str,
                    const SString * sub_str,
                    size_t start)
{
	char * return_val;

	if(str == NULL  ||  sub_str == NULL  ||  NULL == str->string
		    ||  NULL == sub_str->string  ||  start >= str->length)
		return -1;

	return_val = strstr(&(str->string[start]), sub_str->string);

	return (return_val == NULL) ? -2 : (int)(return_val - str->string);
}


struct SStrings *
split_sstring(const SString * str,
              const SString * separator)
{
	struct SStrings * split;
	int * locations;  // locations of separators relative to start of str
	int allocated_num = 8;  // allocated number of locations
	int num_of_locations = 0;
	int actual_num_of_strings;
	size_t i;

	if(str == NULL  ||  separator == NULL  ||  NULL == str->string
		    ||  NULL == separator->string)
		return NULL;

	locations = malloc(sizeof(int) * allocated_num);
	if(locations == NULL)
		return NULL;

	locations[num_of_locations] = find_str_in_sstring(str, separator, 0);
	if(locations[num_of_locations] == -1) {
		split = NULL;
		goto bad_allocation;
	}
	if(locations[num_of_locations] == -2)  // No separator found
		locations[num_of_locations] = str->length;

	do {
		 num_of_locations++;

		if(num_of_locations >= allocated_num) {
			allocated_num += 8;
			locations = realloc(locations, sizeof(int) * allocated_num);
		}

		locations[num_of_locations] = find_str_in_sstring(str, separator,
			    locations[num_of_locations - 1] + separator->length);
	} while(locations[num_of_locations] != -1  ||  locations[num_of_locations] == -2);

	split = malloc(sizeof(struct SStrings) + num_of_locations * sizeof(SString));
	if(split == NULL)
		goto bad_allocation;

	split->length = num_of_locations;

	actual_num_of_strings = num_of_locations;

	for(i = 0; i < split->length; i++)
		split->sstrings[0] = (SString){0};

	if(locations[0] != 0)
		copy_n_sstring(&(split->sstrings[0]), str, 0, locations[0]);
	else
		actual_num_of_strings--;

	for(i = 1; i < split->length - 1; i++) {
		if(locations[i] != locations[i - 1] + separator->length)
			copy_n_sstring(&(split->sstrings[i]), str,
			               locations[i - 1] + separator->length,
			               locations[i] - locations[i - 1] - separator->length);
		else
			actual_num_of_strings--;
	}

	if(split->length > 1  && -1 == copy_n_sstring(&(split->sstrings[i]), str,
		    locations[i - 1] + separator->length, 0))
		actual_num_of_strings--;

	split = realloc(split, sizeof(struct SStrings) + actual_num_of_strings
	                * sizeof(SString));
	if(split == NULL)
		goto bad_allocation;

	split->length = actual_num_of_strings;

bad_allocation:
	free(locations);

	return split;
}
