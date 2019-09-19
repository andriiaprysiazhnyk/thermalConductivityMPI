//
// Created by viniavskyi on 19.09.19.
//

#include "mpi_transactions.h"


void mpi_swap_edges(Matrix &current, boost::mpi::communicator &world, unsigned int rank, unsigned int workers){
    unsigned int number_of_swaps = (rank == 1 || rank == workers) ? 2 : 4;
    auto reqs = new boost::mpi::request[number_of_swaps];
    if (rank != 1){
        reqs[0] = world.isend(rank - 1, 0, &current(1, 1), current.n - 2);
        reqs[1] = world.irecv(rank - 1, 0, &current(0, 1), current.n - 2);
    }
    if (rank != workers){
        size_t index = (rank == 1 )? 0 : 2;
        reqs[index] = world.isend(rank + 1, 0, &current(current.m - 2, 1), current.n - 2);
        reqs[index + 1] = world.irecv(rank + 1, 0, &current(current.m - 1, 1), current.n - 2);
    }
    boost::mpi::wait_all(reqs, reqs + number_of_swaps);
}