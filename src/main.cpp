#include <iostream>
#include <fstream>
#include <boost/mpi.hpp>
#include "config_reader.h"
#include "matrix.h"
#include "finite_differences.h"
#include "mpi_transactions.h"
#include "utils.h"


const unsigned int ROOT = 0;


int main(int argc, char *argv[]) {
    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world;
    const unsigned int rank = world.rank(), process_number = world.size();

//    reading config file

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

//    check stability condition
    if (!check_stability(parameters.alpha, parameters.dt, parameters.dx, parameters.dy)) {
        std::cerr << "Error: stability condition does not hold" << std::endl;
        return 4;
    }

    std::ifstream matrix_file(parameters.path_to_matrix);

    if (!matrix_file.is_open()) {
        std::cerr << "Can not open file with matrix " << parameters.path_to_matrix << std::endl;
        return 5;
    }


//    read initial conditions for non-root processes
    if (rank != ROOT) {

        auto boundaries = calculate_boundaries(parameters.m, rank, process_number - 1);
        unsigned int height = boundaries.second - boundaries.first + 1;

//        create current and next matrices
        Matrix current(height, parameters.n), next(height, parameters.n);
        try {
            read_matrix(matrix_file, current, boundaries.first, boundaries.second);
        } catch (std::exception &error) {
            std::cerr << "Error: " << error.what() << std::endl;
            return 6;
        }

//        fill in side edge conditions for next matrix
        fill_edges(current, next, rank, process_number - 1);

        for (unsigned int i = 1; i <= parameters.iterations; ++i) {
//        calculate new matrix
            next_state(current, next, parameters.alpha, parameters.dt, parameters.dx, parameters.dy);
            current.swap_buffers(next);
//        send and receive data
            mpi_swap_edges(current, world, rank, process_number - 1);

            if (i % parameters.plot_freq == 0) {
                mpi_send_partial_matrix(current, world, rank, ROOT, process_number - 1);
            }
        }
    } else { // root
        Matrix full_matrix(parameters.m, parameters.n);
//        read full matrix
        read_matrix(matrix_file, full_matrix, 0, parameters.m - 1);
        std::cout << full_matrix.to_string() << std::endl;

        unsigned int epochs = parameters.iterations / parameters.plot_freq;
        for (unsigned int epoch = 0; epoch < epochs; ++epoch) {
            mpi_gather_matrix(full_matrix, world, process_number - 1);
            std::cout << full_matrix.to_string() << std::endl;
        }
    }
    return 0;
}
