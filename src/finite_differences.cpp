//
// Created by andriiprysiazhnyk on 9/18/19.
//

#include "finite_differences.h"
#include "iostream"
#include <cmath>

void next_state(const Matrix &curr, Matrix &next, double alpha, double dt, double dx, double dy) {
    for (unsigned int i = 1; i < curr.m - 1; ++i) {
        for (unsigned int j = 1; j < curr.n - 1; ++j) {
            next(i, j) = curr(i, j) + dt * alpha * ((curr(i - 1, j) - 2 * curr(i, j) + curr(i + 1, j)) / pow(dx, 2) +
                                                    (curr(i, j - 1) - 2 * curr(i, j) + curr(i, j + 1)) / pow(dy, 2));
        }
    }
}

bool check_stability(double alpha, double dt, double dx, double dy) {
    return dt <= pow(std::max(dx, dy), 2) / (4 * alpha);
}