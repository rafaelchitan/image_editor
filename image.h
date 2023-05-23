#ifndef IMAGE_H
#define IMAGE_H

#include <stdbool.h>

struct point {
	int x, y;
};

struct selection {
	struct point top_left, bottom_right;
};

enum image_type {
	GRAYSCALE,
	COLOR
};

struct image {
	enum image_type type;
	struct pixel **matrix;
	int height, width, max_value;
	struct selection select;
};

struct pixel {
	unsigned char R, G, B, black;
};

void load_file(struct image **loaded_image);
void save_file(struct image *loaded_image, const char *filename, bool ascii);

#endif
