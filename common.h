#ifndef COMMON_H
#define COMMON_H

#include "image.h"
#include "utils.h"

void select_command(struct image *loaded_image);

void crop_command(struct image *loaded_image);

void save_command(struct image *loaded_image);

void histogram_command(struct image *loaded_image);

void equalize_command(struct image *loaded_image);

void apply_command(struct image *loaded_image);

void rotate_command(struct image *loaded_image);

void merge(struct image *image);

#endif
