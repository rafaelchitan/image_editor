#ifndef FILTERS_H
#define FILTERS_H

const int EDGE_FILTER = 1 - 1;
const int SHARPEN_FILTER = 1;
const int BLUR_FILTER = 2;
const int GAUSS_BLUR_FILTER = 3;

const double filters[4][3][3] = {
	{{-1.0, -1.0, -1.0},
	 {-1.0, 8.0, -1.0},
	 {-1.0, -1.0, -1.0}},
	{{0.0, -1.0, 0.0},
	 {-1.0, 5.0, -1.0},
	 {0.0, -1.0, 0.0}},
	{{1.0 / 9, 1.0 / 9, 1.0 / 9},
	 {1.0 / 9, 1.0 / 9, 1.0 / 9},
	 {1.0 / 9, 1.0 / 9, 1.0 / 9}},
	{{1.0 / 16, 2.0 / 16, 1.0 / 16},
	 {2.0 / 16, 4.0 / 16, 2.0 / 16},
	 {1.0 / 16, 2.0 / 16, 1.0 / 16}}};

#endif
