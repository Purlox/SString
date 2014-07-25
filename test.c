#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	char * str = NULL;
	char * str2 = NULL;
	SString s_str;
	SString s_str2;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < num_of_strings; i++) {
		str = malloc(size_of_strings[i] + 1);
		str2 = malloc(size_of_strings[i] + 1);
		strcpy(str2, test_strings[i]);

		strncpy(str, str2, size_of_strings[i]);

		free(str);
		free(str2);


		s_str = new_sstring(NULL, size_of_strings[i] + 1);

		copy_string_to_sstring(&s_str, test_strings[i], size_of_strings[i]);

		free(s_str.string);


		s_str = new_sstring(NULL, size_of_strings[i] + 1);

		copy_string_to_sstring2(&s_str, test_strings[i], size_of_strings[i]);

		free(s_str.string);
	}

	return 0;
}
