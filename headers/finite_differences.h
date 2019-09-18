//
// Created by andriiprysiazhnyk on 9/18/19.
//

#ifndef THERMAL_CONDUCTIVITY_MPI_FINITE_DIFFERENCES_H
#define THERMAL_CONDUCTIVITY_MPI_FINITE_DIFFERENCES_H

#include "matrix.h"

void next_state(const Matrix &curr, Matrix &next, double alpha, double dt, double dx, double dy);

bool check_stability(double alpha, double dt, double dx, double dy);

#endif //THERMAL_CONDUCTIVITY_MPI_FINITE_DIFFERENCES_H
