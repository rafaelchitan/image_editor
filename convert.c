#include <stdio.h>
#include <string.h>
#include "image.h"
#include "common.h"
#include "utils.h"
#include "convert.h"

void convert_GS(struct image *image)
{
	if (image->type == GRAYSCALE) {
		printf("Image already in GRAYSCALE\n");
		return;
	}

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
			image->matrix[i][j].black = CONVERT(image->matrix[i][j]);
	
	image->type = GRAYSCALE;
}

void convert_REDS(struct image *image)
{
	if (image->type == COLOR)
		convert_GS(image);

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++) {
			image->matrix[i][j].G = 0;
			image->matrix[i][j].B = 0;
			image->matrix[i][j].R = image->matrix[i][j].black / 0.3;
		}
	
	image->type = COLOR;
}

void convert_GREENS(struct image *image)
{
    if (image->type == COLOR)
        convert_GS(image);

    for (int i = 0; i < image->height; i++)
        for (int j = 0; j < image->width; j++) {
            image->matrix[i][j].R = 0;
            image->matrix[i][j].B = 0;
            image->matrix[i][j].G = image->matrix[i][j].black / 0.59;
        }
    
    image->type = COLOR;
}

void convert_BLUES(struct image *image)
{
    if (image->type == COLOR)
        convert_GS(image);

    for (int i = 0; i < image->height; i++)
        for (int j = 0; j < image->width; j++) {
            image->matrix[i][j].R = 0;
            image->matrix[i][j].G = 0;
            image->matrix[i][j].B = image->matrix[i][j].black / 0.11;
        }
    
    image->type = COLOR;
}