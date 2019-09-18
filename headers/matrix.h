//
// Created by andriiprysiazhnyk on 9/18/19.
//

#ifndef THERMAL_CONDUCTIVITY_MPI_MATRIX_H
#define THERMAL_CONDUCTIVITY_MPI_MATRIX_H

#include <string>


class Matrix {
private:
    double *data;

public:
    unsigned int n, m;

    Matrix() = default;

    Matrix(unsigned int m, unsigned int n);

    double &operator()(unsigned int i, unsigned int j);

    const double &operator()(unsigned int i, unsigned int j) const;

    std::string to_string() const;

    void swap_buffers(Matrix& rhs);

    ~Matrix();
};


#endif //THERMAL_CONDUCTIVITY_MPI_MATRIX_H
