#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	char * str = NULL;
	SString s_str;
	SString s_str2;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < num_of_strings; i++) {
		str = malloc(size_of_strings[i] * 2 + 1);
		strcpy(str, test_strings[i]);

		strcat(str, test_strings[i]);

		free(str);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_sstring(&s_str, &s_str2);

		free(s_str.string);
		free(s_str2.string);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_sstring2(&s_str, &s_str2);

		free(s_str.string);
		free(s_str2.string);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_sstring3(&s_str, &s_str2);

		free(s_str.string);
		free(s_str2.string);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_sstring4(&s_str, &s_str2);

		free(s_str.string);
		free(s_str2.string);


		s_str = new_sstring(test_strings[i], size_of_strings[i] * 2 + 1);
		s_str2 = new_sstring(test_strings[i], size_of_strings[i]);

		append_sstring5(&s_str, &s_str2);

		free(s_str.string);
		free(s_str2.string);
	}

	return 0;
}
