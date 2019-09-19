//
// Created by viniavskyi on 19.09.19.
//

#include "mpi_transactions.h"
#include "utils.h"


void mpi_swap_edges(Matrix &current, boost::mpi::communicator &world, unsigned int rank, unsigned int workers) {
    unsigned int number_of_swaps = (rank == 1 || rank == workers) ? 2 : 4;
    auto reqs = new boost::mpi::request[number_of_swaps];
    if (rank != 1) {
        reqs[0] = world.isend(rank - 1, 0, &current(1, 1), current.n - 2);
        reqs[1] = world.irecv(rank - 1, 0, &current(0, 1), current.n - 2);
    }
    if (rank != workers) {
        size_t index = (rank == 1) ? 0 : 2;
        reqs[index] = world.isend(rank + 1, 0, &current(current.m - 2, 1), current.n - 2);
        reqs[index + 1] = world.irecv(rank + 1, 0, &current(current.m - 1, 1), current.n - 2);
    }
    boost::mpi::wait_all(reqs, reqs + number_of_swaps);
    delete[] reqs;
}

void mpi_gather_matrix(Matrix &full_matrix, boost::mpi::communicator &world, unsigned int workers) {
    auto reqs = new boost::mpi::request[workers];
    for (unsigned int sender_rank = 1; sender_rank <= workers; ++sender_rank) {
        auto boundaries = calculate_boundaries(full_matrix.m, sender_rank, workers);
        auto start_row = (sender_rank == 1) ? boundaries.first : boundaries.first + 1;
        auto rows_to_send = (sender_rank == 1 || sender_rank == workers) ? boundaries.second - boundaries.first :
                            boundaries.second - boundaries.first - 1;
        reqs[sender_rank - 1] = world.irecv(sender_rank, 0, &full_matrix(start_row, 0),
                                            rows_to_send * full_matrix.n);
    }
    boost::mpi::wait_all(reqs, reqs + workers);
    delete[] reqs;
}

void mpi_send_partial_matrix(Matrix &current, boost::mpi::communicator &world, unsigned int rank, unsigned int root,
                             unsigned int workers) {
    auto start_row = (rank == 1) ? 0 : 1;
    auto rows_to_send = (rank == 1 || rank == workers) ? current.m - 1 : current.m - 2;
    auto req = world.isend(root, 0, &current(start_row, 0), rows_to_send * current.n);
    req.wait();
}
