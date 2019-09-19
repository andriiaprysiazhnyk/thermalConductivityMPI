//
// Created by andriiprysiazhnyk on 9/18/19.
//

#ifndef THERMAL_CONDUCTIVITY_MPI_CONFIG_READER_H
#define THERMAL_CONDUCTIVITY_MPI_CONFIG_READER_H

#include "matrix.h"
#include "utils.h"


configuration_t read_config(std::istream &config_file);

void read_matrix(std::ifstream &matrix_file, Matrix &matrix, unsigned int begin, unsigned int end);

#endif //THERMAL_CONDUCTIVITY_MPI_CONFIG_READER_H
