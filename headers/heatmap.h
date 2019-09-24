//
// Created by viniavskyi on 19.09.19.
//

#ifndef THERMAL_CONDUCTIVITY_MPI_HEATMAP_H
#define THERMAL_CONDUCTIVITY_MPI_HEATMAP_H

#include <string>
#include "matrix.h"

void generate_heatmap(Matrix &matrix, uint8_t *heatmap, std::pair<double, double> &temp_boundaries, std::string filename);

#endif //THERMAL_CONDUCTIVITY_MPI_HEATMAP_H
