#include <iostream>
#include <fstream>
#include <utility>
#include <boost/mpi.hpp>
#include "config_reader.h"
#include "matrix.h"
#include "finite_differences.h"
#include "mpi_transactions.h"


const unsigned int ROOT = 0;

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
//        calculate new middle of the matrix
            next_state(current, next, parameters.alpha, parameters.dt, parameters.dx, parameters.dy);
            current.swap_buffers(next);
//        send and receive data
            mpi_swap_edges(current, world, rank, process_number - 1);

            if (i % parameters.plot_freq == 0) {
                boost::mpi::request req;
                if (rank == 1) {
                    req = world.isend(ROOT, 0, &current(0, 0), (current.m - 1) * current.n);
                } else if (rank == process_number - 1) {
                    req = world.isend(ROOT, 0, &current(1, 0), (current.m - 1) * current.n);
                } else {
                    req = world.isend(ROOT, 0, &current(1, 0), (current.m - 2) * current.n);
                }
                req.wait();
            }
        }
    } else { // root
        Matrix full_matrix(parameters.m, parameters.n);
//        read full matrix
        read_matrix(matrix_file, full_matrix, 0, parameters.m - 1);
        std::cout << full_matrix.to_string() << std::endl;
        unsigned int epochs = parameters.iterations / parameters.plot_freq;
        for (unsigned int epoch = 0; epoch < epochs; ++epoch) {
            boost::mpi::request reqs[process_number - 1];
            for (unsigned int sender_rank = 1; sender_rank < process_number; ++sender_rank) {
                auto boundaries = calculate_boundaries(parameters.m, sender_rank, process_number - 1);

                if (sender_rank == 1) {
                    reqs[sender_rank - 1] = world.irecv(sender_rank, 0, &full_matrix(boundaries.first, 0), (boundaries.second - boundaries.first) * parameters.n);
                } else if (sender_rank == process_number - 1) {
                    reqs[sender_rank - 1] = world.irecv(sender_rank, 0, &full_matrix(boundaries.first + 1, 0), (boundaries.second - boundaries.first) * parameters.n);
                } else {
                    reqs[sender_rank - 1] = world.irecv(sender_rank, 0, &full_matrix(boundaries.first + 1, 0),
                                                        (boundaries.second - boundaries.first - 1) * parameters.n);
                }
            }
            boost::mpi::wait_all(reqs, reqs + process_number - 1);
            std::cout << full_matrix.to_string() << std::endl;
        }
    }
    return 0;
}
