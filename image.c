#include "image.h"
#include "utils.h"
#include "string.h"
#include <stdio.h>

void skip_comment(FILE *file)
{
	char c;
	c = fgetc(file);
	if (c == '#') {
		do {
			c = fgetc(file);
		} while (c != '\n');

		skip_comment(file);
	} else {
		fseek(file, -1, SEEK_CUR);
	}
}

void load_binary(FILE *file, struct image *loaded_image)
{
	switch (loaded_image->type) {
	case GRAYSCALE:
		for (int i = 0; i < loaded_image->height; i++)
			for (int j = 0; j < loaded_image->width; j++)
				fread(&loaded_image->matrix[i][j].black, 1, 1, file);
		break;

	case COLOR:
		for (int i = 0; i < loaded_image->height; i++)
			for (int j = 0; j < loaded_image->width; j++) {
				fread(&loaded_image->matrix[i][j].R, 1, 1, file);
				fread(&loaded_image->matrix[i][j].G, 1, 1, file);
				fread(&loaded_image->matrix[i][j].B, 1, 1, file);
			}
		break;
	}
}

void load_ascii(FILE *file, struct image *loaded_image)
{
	switch (loaded_image->type) {
	case GRAYSCALE:
		for (int i = 0; i < loaded_image->height; i++)
			for (int j = 0; j < loaded_image->width; j++)
				fscanf(file, "%hhu", &loaded_image->matrix[i][j].black);
		break;

	case COLOR:
		for (int i = 0; i < loaded_image->height; i++)
			for (int j = 0; j < loaded_image->width; j++)
				fscanf(file, "%hhu%hhu%hhu", &loaded_image->matrix[i][j].R,
					   &loaded_image->matrix[i][j].G,
					   &loaded_image->matrix[i][j].B);
		break;
	}
}

void save_binary(FILE *file, struct image *loaded_image)
{
	if (loaded_image->type == COLOR)
		fprintf(file, "P6\n");
	else
		fprintf(file, "P5\n");

	fprintf(file, "%d %d\n%d\n", loaded_image->width,
			loaded_image->height, loaded_image->max_value);

	switch (loaded_image->type) {
	case GRAYSCALE:
		for (int i = 0; i < loaded_image->height; i++)
			for (int j = 0; j < loaded_image->width; j++)
				fwrite(&loaded_image->matrix[i][j].black, 1, 1, file);
		break;

	case COLOR:
		for (int i = 0; i < loaded_image->height; i++)
			for (int j = 0; j < loaded_image->width; j++) {
				fwrite(&loaded_image->matrix[i][j].R, 1, 1, file);
				fwrite(&loaded_image->matrix[i][j].G, 1, 1, file);
				fwrite(&loaded_image->matrix[i][j].B, 1, 1, file);
			}
		break;
	}
}

void save_ascii(FILE *file, struct image *loaded_image)
{
	if (loaded_image->type == COLOR)
		fprintf(file, "P3\n");
	else
		fprintf(file, "P2\n");

	fprintf(file, "%d %d\n%d\n", loaded_image->width,
			loaded_image->height, loaded_image->max_value);

	switch (loaded_image->type) {
	case GRAYSCALE:
		for (int i = 0; i < loaded_image->height; i++) {
			for (int j = 0; j < loaded_image->width; j++)
				fprintf(file, "%hhu ", loaded_image->matrix[i][j].black);

			fprintf(file, "\n");
		}
		break;

	case COLOR:
		for (int i = 0; i < loaded_image->height; i++) {
			for (int j = 0; j < loaded_image->width; j++)
				fprintf(file, "%hhu %hhu %hhu ", loaded_image->matrix[i][j].R,
						loaded_image->matrix[i][j].G,
						loaded_image->matrix[i][j].B);

			fprintf(file, "\n");
		}
		break;
	}
}

void load_file(struct image **loaded_image)
{
	if (*loaded_image) {
		image_free(*loaded_image);
		*loaded_image = NULL;
	}

	FILE *file;

	char *filename = strtok(NULL, "\n ");
	if (!filename) {
		printf("Invalid command\n");
		return;
	}

	file = fopen(filename, "rb");
	if (!file) {
		printf("Failed to load %s\n", filename);
		return;
	}

	*loaded_image = malloc(sizeof(struct image));

	skip_comment(file);

	char ptype[3];
	fgets(ptype, 3, file);

	if (ptype[1] == '2' || ptype[1] == '5')
		(*loaded_image)->type = GRAYSCALE;
	else if (ptype[1] == '3' || ptype[1] == '6')
		(*loaded_image)->type = COLOR;

	skip_comment(file);

	fscanf(file, "%d%d%d", &(*loaded_image)->width,
		   &(*loaded_image)->height,
		   &(*loaded_image)->max_value);

	fgetc(file);

	(*loaded_image)->matrix =
		calloc((*loaded_image)->height, sizeof(struct pixel *));

	for (int i = 0; i < (*loaded_image)->height; i++)
		(*loaded_image)->matrix[i] =
			malloc((*loaded_image)->width * sizeof(struct pixel));

	if (ptype[1] == '2' || ptype[1] == '3')
		load_ascii(file, *loaded_image);
	else
		load_binary(file, *loaded_image);

	select_all(*loaded_image);
	fclose(file);
	printf("Loaded %s\n", filename);
}

void save_file(struct image *loaded_image, const char *filename, bool ascii)
{
	FILE *file = fopen(filename, "wb");
	if (!file)
		return;

	if (ascii)
		save_ascii(file, loaded_image);
	else
		save_binary(file, loaded_image);

	fclose(file);
	printf("Saved %s\n", filename);
}
