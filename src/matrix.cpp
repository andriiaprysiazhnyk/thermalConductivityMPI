//
// Created by andriiprysiazhnyk on 9/18/19.
//

#include "matrix.h"
#include <sstream>
#include <iomanip>

Matrix::Matrix(unsigned int m, unsigned int n) : n(n), m(m) {
    data = new double[n * m];
}

double &Matrix::operator()(unsigned int i, unsigned int j) {
    return data[i * n + j];
}


const double &Matrix::operator()(unsigned int i, unsigned int j) const {
    return data[i * n + j];
}


std::string Matrix::to_string() const {
    std::ostringstream ss;

    for (int row = 0; row < m; ++row) {
        ss << '|';
        for (int column = 0; column < n; ++column) {
            ss << std::setw(7) << data[n * row + column] << " ";
        }
        ss << "|\n";
    }

    return ss.str();
}


void Matrix::swap_buffers(Matrix& rhs) {
    double *temp = data;
    data = rhs.data;
    rhs.data = temp;
}


Matrix::~Matrix() {
    delete[] data;
}
