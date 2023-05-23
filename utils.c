
#include "utils.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>

void select_all(struct image *loaded_image)
{
	loaded_image->select.top_left =
		(struct point){.x = 0, .y = 0};

	loaded_image->select.bottom_right =
		(struct point){.x = loaded_image->width, .y = loaded_image->height};
}

void select_points(struct image *loaded_image,
				   struct point top_left, struct point bottom_right)
{
	loaded_image->select.top_left = top_left;
	loaded_image->select.bottom_right = bottom_right;
}

void matrix_free(void **matrix, int y)
{
	if (!matrix)
		return;

	for (int i = 0; i < y; i++)
		if (matrix[i])
			free(matrix[i]);

	free(matrix);
}

void image_free(struct image *loaded_image)
{
	if (!loaded_image)
		return;

	matrix_free((void **)loaded_image->matrix, loaded_image->height);
	free(loaded_image);
}

double clamp(double val, double min, double max)
{
	if (val < min)
		return min;
	if (val > max)
		return max;

	return val;
}

struct pixel **rotate_pixelmat(struct pixel **mat,
							   struct point start, struct point end)
{
	int new_height = end.x - start.x;
	int new_width = end.y - start.y;

	struct pixel **new_mat = calloc(new_height, sizeof(struct pixel *));

	for (int i = 0; i < new_height; i++)
		new_mat[i] = malloc(new_width * sizeof(struct pixel));

	for (int i = start.y; i < end.y; i++)
		for (int j = start.x; j < end.x; j++)
			new_mat[j - start.x][start.y + new_width - i - 1] = mat[i][j];

	return new_mat;
}
