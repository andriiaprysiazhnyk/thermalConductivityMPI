//
// Created by viniavskyi on 19.09.19.
//
#include "heatmap.h"
#include <cmath>
#include <Magick++.h>


static void get_color(double value, uint8_t *red, uint8_t *green, uint8_t *blue) {
    const int NUM_COLORS = 5;
    static double color[NUM_COLORS][3] = {{0, 0, 1},
                                          {0, 1, 1},
                                          {0, 1, 0},
                                          {1, 1, 0},
                                          {1, 0, 0}};

    int idx1, idx2;
    double fractBetween = 0;

    if (value <= 0) { idx1 = idx2 = 0; }
    else if (value >= 1) { idx1 = idx2 = NUM_COLORS - 1; }
    else {
        value = value * (NUM_COLORS - 1);
        idx1 = std::floor(value);
        idx2 = idx1 + 1;
        fractBetween = value - idx1;
    }

    *red = ((color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0]) * 255;
    *green = ((color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1]) * 255;
    *blue = ((color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2]) * 255;
}


void generate_heatmap(Matrix &matrix, uint8_t *heatmap, std::pair<double, double> &temp_boundaries, std::string filename) {
    double value, temp_delta = temp_boundaries.second - temp_boundaries.first;
    size_t n = matrix.n;
    for (size_t i = 0; i < matrix.m; ++i) {
        for (size_t j = 0; j < matrix.n; ++j) {
            value = (matrix(i, j) - temp_boundaries.first) / temp_delta;
            get_color(value, heatmap + 3 * (i * n + j), heatmap + 3 * (i * n + j) + 1, heatmap + 3 * (i * n + j) + 2);
        }
    }
    Magick::Image image;
    image.read(matrix.n, matrix.m, "RGB", Magick::CharPixel, heatmap);
    image.write(filename);
}


