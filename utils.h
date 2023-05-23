#ifndef UTILS_H
#define UTILS_H

#include "image.h"
#include <stdlib.h>

void select_all(struct image *loaded_image);

void select_points(struct image *loaded_image,
				   struct point top_left, struct point bottom_right);

void matrix_free(void **matrix, int y);

void image_free(struct image *loaded_image);

double clamp(double val, double min, double max);

struct pixel **rotate_pixelmat(struct pixel **mat,
							   struct point start, struct point end);

#endif
