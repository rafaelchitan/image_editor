#include <stdio.h>
#include <string.h>
#include "image.h"
#include "common.h"
#include "utils.h"
#include "ctype.h"
#include "shapes.h"

void draw_circle(struct image *image)
{
    char *item = strtok(NULL, "\n ");
    int x, y, radius;
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &x);
    item = strtok(NULL, "\n ");
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &y);
    item = strtok(NULL, "\n ");
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &radius);
    int i, j;
    for (i = x - radius; i <= x + radius; i++)
    {
        for (j = y - radius; j <= y + radius; j++)
        {
            if (i >= 0 && i < image->width && j >= 0 && j < image->height)
            {
                if ((i - x) * (i - x) + (j - y) * (j - y) <= radius * radius)
                {
                    image->matrix[i][j].R = 0;
                    image->matrix[i][j].G = 0;
                    image->matrix[i][j].B = 0;
                    image->matrix[i][j].black = 0;
                }
            }
        }
    }
}

void draw_rectangle(struct image *image)
{
    int x, y, width, height;
    char *item = strtok(NULL, "\n ");
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &x);
    item = strtok(NULL, "\n ");
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &y);
    item = strtok(NULL, "\n ");
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &width);
    item = strtok(NULL, "\n ");
    if (!item)
    {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &height);
    int i, j;
    for (i = x; i <= x + width; i++)
    {
        for (j = y; j <= y + height; j++)
        {
            if (i >= 0 && i < image->width && j >= 0 && j < image->height)
            {
                image->matrix[i][j].R = 0;
                image->matrix[i][j].G = 0;
                image->matrix[i][j].B = 0;
                image->matrix[i][j].black = 0;
            }
        }
    }
}

void draw_triangle(struct image *image)
{
    int x1, y1, x2, y2, x3, y3;
    char *item = strtok(NULL, "\n ");
    if (!item) {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &x1);
    item = strtok(NULL, "\n ");
    if (!item) {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &y1);
    item = strtok(NULL, "\n ");
    if (!item) {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &x2);
    item = strtok(NULL, "\n ");
    if (!item) {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &y2);
    item = strtok(NULL, "\n ");
    if (!item) {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &x3);
    item = strtok(NULL, "\n ");
    if (!item) {
        printf("Not enough arguments!\n");
        return;
    }
    sscanf(item, "%d", &y3);
    int i, j;
    for (i = 0; i < image->height; i++)
    {
        for (j = 0; j < image->width; j++)
        {
            if ((i - x1) * (y2 - y1) - (j - y1) * (x2 - x1) >= 0 &&
                (i - x2) * (y3 - y2) - (j - y2) * (x3 - x2) >= 0 &&
                (i - x3) * (y1 - y3) - (j - y3) * (x1 - x3) >= 0)
            {
                image->matrix[i][j].R = 0;
                image->matrix[i][j].G = 0;
                image->matrix[i][j].B = 0;
                image->matrix[i][j].black = 0;
            }

            if ((i - x1) * (y2 - y1) - (j - y1) * (x2 - x1) <= 0 &&
                (i - x2) * (y3 - y2) - (j - y2) * (x3 - x2) <= 0 &&
                (i - x3) * (y1 - y3) - (j - y3) * (x1 - x3) <= 0)
            {
                image->matrix[i][j].R = 0;
                image->matrix[i][j].G = 0;
                image->matrix[i][j].B = 0;
                image->matrix[i][j].black = 0;
            }

        }
    }
}
