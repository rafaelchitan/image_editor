#include <stdio.h>
#include <string.h>
#include "image.h"
#include "common.h"
#include "utils.h"
#include "ctype.h"
#include "shapes.h"
#include "convert.h"

void print_commands(void)
{
	printf("Possible commands:\n");
	printf("LOAD <image_path> - load image into program\n");
	printf("SAVE <image_path> - save image in memory\n");
	printf("SELECT <x1> <y1> <x2> <y2> - select area to apply filters on\n");
	printf("CROP - crop image to current selection\n");
	printf("HISTOGRAM - show histogram of current selection\n");
	printf("EQUALIZE - equalize current selection\n");
	printf("APPLY <filter_name> - apply filter on current selection\n");
	printf("ROTATE <angle> - rotate current selection\n");
	printf("MERGE <image_path> - merge current image with another image\n");
	printf("FREE - free current image\n");
	printf("CONVERT <color_space> - convert current image to another color space\n");
	printf("EXIT - exit program\n");
	printf("DRAW_CIRCLE <x> <y> <radius> - draw a black circle in the image\n");
	printf("DRAW_RECTANGLE <x> <y> <width> <height> - draw a black rectangle in the image\n");
	printf("DRAW_TRIANGLE <x1> <y1> <x2> <y2> <x3> <y3> - draw a black triangle in the image\n");
}

int main(void)
{
	char command_line[100] = {0};
	struct image *loaded_image = NULL;
	printf("Image editor starting...\n");
	printf("For more info about possible commands, type HELP\n");

	while (fgets(command_line, 99, stdin)) {
		char *command = strtok(command_line, "\n ");

		if (!command)
			break;

		if (strcmp(command, "LOAD") == 0) {
			load_file(&loaded_image);
			continue;
		}

		if (strcmp(command, "HELP") == 0) {
			print_commands();
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
				printf("No image loaded. Quitting...\n");
				break;
			} else {
				printf("Getting memory freed...\nQuitting...\n");
				break;
			}
		}

		if (strcmp(command, "MERGE") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			merge(loaded_image);
			continue;
		}

		if (strcmp(command, "FREE") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			image_free(loaded_image);
			printf("Image freed! Ready for new image!\n");
			continue;
		}

		if (strcmp(command, "CONVERT") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			char *type = strtok(NULL, "\n ");
			if (!type) {
				printf("No type specified!\n");
				continue;
			} else if (strcmp(type, "GRAYSCALE") == 0) {
				convert_GS(loaded_image);
				printf("Image converted successfully!\n");
				continue;
			} else if (strcmp(type, "REDS") == 0) {
				convert_REDS(loaded_image);
				printf("Image converted successfully!\n");
				continue;
			} else if (strcmp(type, "GREENS") == 0) {
				convert_GREENS(loaded_image);
				printf("Image converted successfully!\n");
				continue;
			} else if (strcmp(type, "BLUES") == 0) {
				convert_BLUES(loaded_image);
				printf("Image converted successfully!\n");
				continue;
			} else {
				printf("Invalid type!\n");
				continue;
			}
		}

		if (strcmp(command, "CONVERT_REDS") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			convert_REDS(loaded_image);
			printf("Image converted successfully!\n");
			continue;
		}

		if (strcmp(command, "DRAW_CIRCLE") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			draw_circle(loaded_image);
			printf("Circle drawn successfully!\n");
			continue;
		}

		if (strcmp(command, "DRAW_RECTANGLE") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			draw_rectangle(loaded_image);
			printf("Rectangle drawn successfully!\n");
			continue;
		}

		if (strcmp(command, "DRAW_TRIANGLE") == 0) {
			if (!loaded_image) {
				printf("No image loaded!\n");
				continue;
			}
			draw_triangle(loaded_image);
			printf("Triangle drawn successfully!\n");
			continue;
		}

		printf("Invalid command\n");
	}

	if (loaded_image)
		image_free(loaded_image);

	return 0;
}
