#include <stdio.h>
#include <string.h>

#define STR_LARGE_LINE 100
#define MAX_CONFIG_LINE 200

char* read_config(char* path_name, char* searched_key, char* found_value) {
	char key[STR_LARGE_LINE], value[STR_LARGE_LINE], line[MAX_CONFIG_LINE];
	FILE* file = fopen(path_name, "r");

	// Reading config file
	if (file == NULL) {
		printf("Failed to open file\n");
		return NULL;
	}

	// Reading line by line
	while (fgets(line, MAX_CONFIG_LINE, file) != NULL) {
		// Reading key and value
		sscanf(line, "%s %s", key, value);

		// Comparing key
		if (strcmp(key, searched_key) == 0) {
			// Closing file and returning value
			fclose(file);
			strcpy(found_value, value);
			return found_value;
		}
	}

	// Closing file and returning NULL
	fclose(file);
	return NULL;
}