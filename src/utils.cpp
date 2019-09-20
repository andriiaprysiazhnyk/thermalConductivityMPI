//
// Created by viniavskyi on 19.09.19.
//
#include "utils.h"

std::pair<unsigned int, unsigned int> calculate_boundaries(unsigned int m, unsigned int rank, unsigned int workers) {
    std::pair<unsigned int, unsigned int> rv;
    unsigned int height = (m - 2) / workers;
    rv.first = height * (rank - 1);
    if (rank == workers) {
        height += (m - 2) % workers;
    }
    rv.second = rv.first + height + 1;
    return rv;
}

void fill_edges(Matrix &current, Matrix &next, unsigned int rank, unsigned int workers) {
    for (unsigned int i = 0; i < current.m; ++i) {
        next(i, 0) = current(i, 0);
        next(i, current.n - 1) = current(i, current.n - 1);
    }
    if (rank == 1) {
        for (unsigned int j = 1; j < current.n - 1; ++j) {
            next(0, j) = current(0, j);
        }
    } else if (rank == workers) {
        for (unsigned int j = 1; j < current.n - 1; ++j) {
            next(current.m - 1, j) = current(current.m - 1, j);
        }
    }
}