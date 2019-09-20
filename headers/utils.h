//
// Created by viniavskyi on 19.09.19.
//

#ifndef THERMAL_CONDUCTIVITY_MPI_UTILS_H
#define THERMAL_CONDUCTIVITY_MPI_UTILS_H

#include <utility>
#include "matrix.h"

typedef struct configuration_t {
    unsigned int m, n, iterations, plot_freq;
    double alpha, dt, dx, dy;
    std::string path_to_matrix;
} configuration_t;

std::pair<unsigned int, unsigned int> calculate_boundaries(unsigned int m, unsigned int rank, unsigned int workers);

void fill_edges(Matrix &current, Matrix &next, unsigned int rank, unsigned int workers);

#endif //THERMAL_CONDUCTIVITY_MPI_UTILS_H
