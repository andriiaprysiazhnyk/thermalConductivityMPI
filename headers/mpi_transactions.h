//
// Created by viniavskyi on 19.09.19.
//

#ifndef THERMAL_CONDUCTIVITY_MPI_MPI_TRANSACTIONS_H
#define THERMAL_CONDUCTIVITY_MPI_MPI_TRANSACTIONS_H

#include <boost/mpi.hpp>
#include "matrix.h"


void mpi_swap_edges(Matrix &current, boost::mpi::communicator &world, unsigned int rank, unsigned int workers);

void mpi_gather_matrix(Matrix &full_matrix, boost::mpi::communicator &world, unsigned int workers);

void mpi_send_partial_matrix(Matrix &current, boost::mpi::communicator &world, unsigned int rank, unsigned int root,
                             unsigned int workers);

#endif //THERMAL_CONDUCTIVITY_MPI_MPI_TRANSACTIONS_H