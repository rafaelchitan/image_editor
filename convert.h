#ifndef CONVERT_H
#define CONVERT_H

#define CONVERT(x) ((double) x.R * 0.3 + (double) x.G * 0.59 + (double) x.B * 0.11)

void convert_GS(struct image *image);

void convert_REDS(struct image *image);

void convert_GREENS(struct image *image);

void convert_BLUES(struct image *image);

#endif