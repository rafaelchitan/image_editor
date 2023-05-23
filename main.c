#include <stdio.h>
#include <string.h>
#include "image.h"
#include "common.h"
#include "utils.h"
#include "ctype.h"

int main(void)
{
	char command_line[100] = {0};
	struct image *loaded_image = NULL;

	while (fgets(command_line, 99, stdin)) {
		char *command = strtok(command_line, "\n ");

		if (!command)
			break;

		if (strcmp(command, "LOAD") == 0) {
			load_file(&loaded_image);
			continue;
		}

		if (strcmp(command, "SAVE") == 0) {
			save_command(loaded_image);
			continue;
		}

		if (strcmp(command, "SELECT") == 0) {
			select_command(loaded_image);
			continue;
		}

		if (strcmp(command, "CROP") == 0) {
			crop_command(loaded_image);
			continue;
		}

		if (strcmp(command, "HISTOGRAM") == 0) {
			histogram_command(loaded_image);
			continue;
		}

		if (strcmp(command, "EQUALIZE") == 0) {
			equalize_command(loaded_image);
			continue;
		}

		if (strcmp(command, "APPLY") == 0) {
			apply_command(loaded_image);
			continue;
		}

		if (strcmp(command, "ROTATE") == 0) {
			rotate_command(loaded_image);
			continue;
		}

		if (strcmp(command, "EXIT") == 0) {
			if (!loaded_image) {
				printf("No image loaded\n");
				continue;
			} else {
				break;
			}
		}

		printf("Invalid command\n");
	}

	if (loaded_image)
		image_free(loaded_image);

	return 0;
}
