#include <stdio.h>
#include <stdlib.h>
#include "harness.h"

int* retrieve_set_tag_params(int pid, int new_tag) {
	int *array = malloc(4 * sizeof(int));
	array[0] = 336;
	array[1] = 2;
	array[2] = pid;
	array[3] = new_tag;

	return array;
}

int* retrieve_get_tag_params(int pid) {
	int *array = malloc(3 * sizeof(int));
	array[0] = 335;
	array[1] = 1;
	array[2] = pid;

	return array;
}

int interpret_set_tag_result(int ret_value) {
	if (ret_value < 0) {
		printf("The set_tag syscall failed.\n");
	} else {
		printf("The set_tag syscall set the tag successfully.\nDetails:\n");
		printf("\t- Full tag value: %d\n", ret_value);
		printf("\t- Level: %d\n", (ret_value & 3));
		printf("\t- Bitmap value: %d\n", (ret_value >> 2));
	}

	return ret_value;
}

int interpret_get_tag_result(int ret_value) {
	if (ret_value < 0) {
		printf("The get_tag syscall failed.\n");
	} else {
		printf("The get_tag syscall retrieved the tag successfully.\nDetails:\n");
                printf("\t- Full tag value: %d\n", ret_value);
                printf("\t- Level: %d\n", (ret_value & 3));
                printf("\t- Bitmap value: %d\n", (ret_value >> 2));
	}

	return ret_value;
}
