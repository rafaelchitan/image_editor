#include "common.h"
#include "image.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "filters.h"
#include <limits.h>

void select_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	char *item = strtok(NULL, "\n ");
	if (strcmp(item, "ALL") == 0) {
		select_all(loaded_image);
		printf("Selected ALL\n");
		return;
	}

	int coord[4] = {INT_MIN, INT_MIN, INT_MIN, INT_MIN};

	for (int i = 0; i < 4; i++) {
		if (!item) {
			printf("Invalid command\n");
			return;
		}

		sscanf(item, "%d", &coord[i]);

		if (coord[i] == INT_MIN) {
			printf("Invalid command\n");
			return;
		}

		if (coord[i] < 0) {
			printf("Invalid set of coordinates\n");
			return;
		}

		item = strtok(NULL, "\n ");
	}

	struct point top_left = {.x = coord[0], .y = coord[1]};
	struct point bottom_right = {.x = coord[2], .y = coord[3]};

	if (bottom_right.x < top_left.x) {
		struct point aux = top_left;
		top_left = bottom_right;
		bottom_right = aux;
	}

	if (bottom_right.y < top_left.y) {
		int aux = top_left.y;
		top_left.y = bottom_right.y;
		bottom_right.y = aux;
	}

	if (bottom_right.y > loaded_image->height ||
		bottom_right.x > loaded_image->width) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (top_left.x == bottom_right.x || top_left.y == bottom_right.y) {
		printf("Invalid set of coordinates\n");
		return;
	}

	select_points(loaded_image, top_left, bottom_right);

	printf("Selected %d %d %d %d\n",
		   top_left.x, top_left.y,
		   bottom_right.x, bottom_right.y);
}

void crop_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	int new_height = loaded_image->select.bottom_right.y -
		loaded_image->select.top_left.y;
	int new_width = loaded_image->select.bottom_right.x -
		loaded_image->select.top_left.x;

	struct pixel **mat = malloc(new_height * sizeof(struct pixel *));

	for (int i = loaded_image->select.top_left.y, ii = 0;
		 i < loaded_image->select.bottom_right.y; i++, ii++) {
		mat[ii] = malloc(new_width * sizeof(struct pixel));

		for (int j = loaded_image->select.top_left.x, jj = 0;
			 j < loaded_image->select.bottom_right.x; j++, jj++)
			mat[ii][jj] = loaded_image->matrix[i][j];
	}

	matrix_free((void **)loaded_image->matrix, loaded_image->height);

	loaded_image->matrix = mat;
	loaded_image->height = new_height;
	loaded_image->width = new_width;
	select_all(loaded_image);

	printf("Image cropped\n");
}

void save_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	const char *filename = strtok(NULL, "\n ");
	const char *ascii = strtok(NULL, "\n ");

	if (!filename) {
		printf("Invalid command\n");
		return;
	}

	if (!ascii)
		save_file(loaded_image, filename, false);
	else
		save_file(loaded_image, filename, strcmp(ascii, "ascii") == 0);
}

void histogram_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	int x, y;
	char *item = strtok(NULL, "\n ");
	if (!item) {
		printf("Invalid command\n");
		return;
	}
	sscanf(item, "%d", &x);

	item = strtok(NULL, "\n ");
	if (!item) {
		printf("Invalid command\n");
		return;
	}
	sscanf(item, "%d", &y);

	item = strtok(NULL, "\n ");
	if (item) {
		printf("Invalid command\n");
		return;
	}

	bool two_power = (y >= 2 && y <= 256) && ((y & (y - 1)) == 0);

	if (!two_power) {
		printf("Invalid command\n");
		return;
	}

	if (loaded_image->type != GRAYSCALE) {
		printf("Black and white image needed\n");
		return;
	}

	int *bin_stack = calloc(y, sizeof(int)), max_freq = -1;
	const int num_on_interval = 256 / y;

	// first bin from 0 to num_on_interval - 1
	// etc

	struct pixel **mat = loaded_image->matrix;
	for (int i = 0; i < loaded_image->height; i++)
		for (int j = 0; j < loaded_image->width; j++)
			if (++bin_stack[mat[i][j].black / num_on_interval] > max_freq)
				max_freq = bin_stack[mat[i][j].black / num_on_interval];

	for (int i = 0; i < y; i++) {
		int starnum = (int)(bin_stack[i] * 1.0 / max_freq * x);

		printf("%d\t|\t", starnum);
		for (int j = 1; j <= starnum; j++)
			printf("*");

		printf("\n");
	}

	free(bin_stack);
}

void equalize_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	if (loaded_image->type != GRAYSCALE) {
		printf("Black and white image needed\n");
		return;
	}

	int pixfreq[256] = {0};
	int area = loaded_image->height * loaded_image->width;

	struct pixel **mat = loaded_image->matrix;
	for (int i = 0; i < loaded_image->height; i++)
		for (int j = 0; j < loaded_image->width; j++)
			pixfreq[mat[i][j].black]++;

	for (int i = 1; i < 256; i++)
		pixfreq[i] += pixfreq[i - 1];

	for (int i = 0; i < loaded_image->height; i++) {
		for (int j = 0; j < loaded_image->width; j++) {
			double new_val = 255 * pixfreq[mat[i][j].black] * 1.0 / area;

			new_val = clamp(new_val, 0.0, 255.0);

			mat[i][j].black = (unsigned char)round(new_val);
		}
	}

	printf("Equalize done\n");
}

void apply_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	const char *item = strtok(NULL, "\n ");
	if (!item) {
		printf("Invalid command\n");
		return;
	}

	if (loaded_image->type == GRAYSCALE) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	int filter = -1;
	if (strcmp(item, "EDGE") == 0)
		filter = EDGE_FILTER;
	if (strcmp(item, "SHARPEN") == 0)
		filter = SHARPEN_FILTER;
	if (strcmp(item, "BLUR") == 0)
		filter = BLUR_FILTER;
	if (strcmp(item, "GAUSSIAN_BLUR") == 0)
		filter = GAUSS_BLUR_FILTER;
	if (filter == -1) {
		printf("APPLY parameter invalid\n");
		return;
	}

	struct pixel **mat = loaded_image->matrix;
	struct pixel **aux = calloc(loaded_image->height,
								sizeof(struct pixel *));

	for (int i = loaded_image->select.top_left.y;
		i < loaded_image->select.bottom_right.y; i++) {
		aux[i] = malloc(sizeof(struct pixel) *
						loaded_image->width);
		if (i == 0 || (i == loaded_image->height - 1))
			continue;

		for (int j = loaded_image->select.top_left.x;
			j < loaded_image->select.bottom_right.x; j++) {
			if (j == 0 || (j == loaded_image->width - 1))
				continue;

			double red_sum = 0.0, green_sum = 0.0, blue_sum = 0.0;
			for (int ii = i - 1, fi = 0; ii <= i + 1; ii++, fi++) {
				for (int jj = j - 1, fj = 0; jj <= j + 1; jj++, fj++) {
					red_sum += mat[ii][jj].R * filters[filter][fi][fj];
					green_sum += mat[ii][jj].G * filters[filter][fi][fj];
					blue_sum += mat[ii][jj].B * filters[filter][fi][fj];
				}
			}

			aux[i][j].R = clamp(round(red_sum), 0.0, 255.0);
			aux[i][j].G = clamp(round(green_sum), 0.0, 255.0);
			aux[i][j].B = clamp(round(blue_sum), 0.0, 255.0);
		}
	}

	for (int i = loaded_image->select.top_left.y;
		i < loaded_image->select.bottom_right.y; i++) {
		if (i == 0 || (i == loaded_image->height - 1))
			continue;

		for (int j = loaded_image->select.top_left.x;
			j < loaded_image->select.bottom_right.x; j++) {
			if (j == 0 || (j == loaded_image->width - 1))
				continue;

			mat[i][j].R = aux[i][j].R;
			mat[i][j].G = aux[i][j].G;
			mat[i][j].B = aux[i][j].B;
		}
	}
	matrix_free((void **)aux, loaded_image->height);
	printf("APPLY %s done\n", item);
}

void rotate_command(struct image *loaded_image)
{
	if (!loaded_image) {
		printf("No image loaded\n");
		return;
	}

	const char *item = strtok(NULL, "\n ");
	if (!item) {
		printf("Invalid command\n");
		return;
	}

	int degs = INT_MIN;
	int ret = sscanf(item, "%d", &degs);
	if (ret == EOF) {
		printf("Invalid command\n");
		return;
	}
	if (degs == INT_MIN) {
		printf("Invalid command\n");
		return;
	}

	if (abs(degs) > 360 || degs % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	int rotations;
	if (degs < 0)
		rotations = (360 + degs) / 90;
	else
		rotations = degs / 90;

	int select_size = loaded_image->select.bottom_right.y -
		loaded_image->select.top_left.y;
	if (select_size == loaded_image->height &&
		loaded_image->select.bottom_right.x -
		loaded_image->select.top_left.x == loaded_image->width) {
		while (rotations--) {
			struct pixel **mat;
			mat = rotate_pixelmat(loaded_image->matrix,
								  loaded_image->select.top_left,
								  loaded_image->select.bottom_right);
			matrix_free((void **)loaded_image->matrix, loaded_image->height);
			loaded_image->matrix = mat;

			int aux = loaded_image->height;
			loaded_image->height = loaded_image->width;
			loaded_image->width = aux;
			aux = loaded_image->select.bottom_right.x;
			loaded_image->select.bottom_right.x =
				loaded_image->select.bottom_right.y;
			loaded_image->select.bottom_right.y = aux;
		}
		printf("Rotated %d\n", degs);
		return;
	}

	if ((select_size != loaded_image->select.bottom_right.x -
		loaded_image->select.top_left.x)) {
		printf("The selection must be square\n");
		return;
	}

	while (rotations--) {
		struct pixel **mat = rotate_pixelmat(loaded_image->matrix,
											 loaded_image->select.top_left,
											 loaded_image->select.bottom_right);
		for (int i = loaded_image->select.top_left.y, ii = 0;
			 i < loaded_image->select.bottom_right.y; i++, ii++) {
			for (int j = loaded_image->select.top_left.x, jj = 0;
				 j < loaded_image->select.bottom_right.x; j++, jj++)
				loaded_image->matrix[i][j] = mat[ii][jj];
		}

		matrix_free((void **)mat, select_size);
	}
	printf("Rotated %d\n", degs);
}
