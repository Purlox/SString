#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	char * str = NULL;
	SString * s_str = NULL;
	SString * s_str2 = NULL;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < num_of_strings; i++) {
		str = malloc(size_of_strings[i] * 2 + 1);
		strcpy(str, test_strings[i]);

		strncat(str, test_strings[i], size_of_strings[i]);

		free(str);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_n_sstring(s_str, s_str2, size_of_strings[i]);

		free_sstring(s_str);
		free_sstring(s_str2);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_n_sstring2(s_str, s_str2, size_of_strings[i]);

		free_sstring(s_str);
		free_sstring(s_str2);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_n_sstring3(s_str, s_str2, size_of_strings[i]);

		free_sstring(s_str);
		free_sstring(s_str2);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_n_sstring4(s_str, s_str2, size_of_strings[i]);

		free_sstring(s_str);
		free_sstring(s_str2);
	}

	return 0;
}
