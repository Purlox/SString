#include <stdio.h>
#include <time.h>
#include <string.h>

#include "s_string.h"

// NOTE: Works only on Linux because standard C implementation wouldn't be
//           accurate enough.

#define START_COUNTING  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start)
#define END_COUNTING    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end)
#define TIME_DIFFERENCE (end.tv_nsec - start.tv_nsec)

#define NUM_OF_LOOPS  16

long long
average(long numbers[NUM_OF_LOOPS])
{
	long long sum = 0;
	int i;

	for(i = 0; i < NUM_OF_LOOPS; i++)
		sum += numbers[i];

	return sum / NUM_OF_LOOPS;
}


int main(void) {
	struct timespec start;
	struct timespec end;
	long time_diffs[NUM_OF_LOOPS];
	int i;
	char * str;
	s_string s_str;
	const char * test_strings[] = {
		"Testing",
		"Testing a test.",
		"Lorem ipsum dolor sit volutpat.",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit posuere.",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam faucibus libero mi, vitae ornare justo tristique eget turpis duis.",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et scelerisque ipsum. Curabitur non ipsum auctor, porttitor quam quis, cursus lacus. Fusce suscipit porta arcu vel mollis. Ut commodo arcu at odio volutpat tempus. Nam scelerisque id orci id."
	};
	const int num_of_strings = 6;

	for(i = 0; i < NUM_OF_LOOPS; i++) {
		START_COUNTING;
		str = "Testing";
		END_COUNTING;
		time_diffs[i] = TIME_DIFFERENCE;
	}
	printf("%lli ns - assigning 8 chars to a string\n", average(time_diffs));

	for(i = 0; i < NUM_OF_LOOPS; i++) {
		START_COUNTING;
		s_str = S_STRING("Testing");
		END_COUNTING;
		time_diffs[i] = TIME_DIFFERENCE;
	}
	printf("%lli ns - assigning 8 chars to a s_string\n", average(time_diffs));

	for(i = 0; i < NUM_OF_LOOPS; i++) {
		START_COUNTING
		str = malloc(8);
		str = strcpy(test_strings[0]);
		END_COUNTING
		time_diffs[i] = TIME_DIFFERENCE;
	}
	printf("%lli ns - copying 8 chars into a string\n", average(time_diffs));

	for(i = 0; i < NUM_OF_LOOPS; i++) {
		START_COUNTING
		s_init(s_str, test_strings[0], 8);
		END_COUNTING
		time_diffs[i] = TIME_DIFFERENCE;
	}
	printf("%lli ns - copying 8 chars into a s_string\n", average(time_diffs));

	for(i = 0; i < NUM_OF_LOOPS; i++) {
		START_COUNTING
		s_init2(s_str, test_strings[0], 8);
		END_COUNTING
		time_diffs[i] = TIME_DIFFERENCE;
	}
	printf("%lli ns - copying 8 chars into a s_string 2\n", average(time_diffs));


	return 0;
}
