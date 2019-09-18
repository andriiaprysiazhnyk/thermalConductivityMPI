#include <iostream>
#include <fstream>
#include "config_reader.h"
#include "matrix.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect number of arguments." << std::endl;
        return 1;
    }

    std::string file_name(argv[1]);
    std::ifstream config_file(file_name);
    if (!config_file.is_open()) {
        std::cerr << "Can not open configuration file " << file_name << std::endl;
        return 2;
    }

    configuration_t parameters;
    try {
        parameters = read_config(config_file);
    } catch (std::exception &error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 3;
    }

    std::ifstream matrix_file(parameters.path_to_matrix);
    if (!matrix_file.is_open()) {
        std::cerr << "Can not open file with matrix " << file_name << std::endl;
        return 4;
    }

    unsigned int process_number = 3, rank = 1;
    unsigned int height = (parameters.m - 2) / process_number;
    unsigned int begin = height * rank;
    if (rank == process_number - 1) {
        height += (parameters.m - 2) % process_number;
    }
    unsigned int end = begin + height + 1;
    Matrix matrix(height + 2, parameters.n);
    std::cout << "Start\n" ;

    try {
        read_matrix(matrix_file, matrix, begin, end);
    } catch (std::exception &error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 3;
    }

    std::cout << matrix.to_string();

    return 0;
}
