#pragma once
#include <iostream>
#include <vector>
#include <Eigen/SparseLU>
#include <Eigen/SparseCholesky>  
#include <cassert>
#include <iomanip> // for std::setw
#include <fstream>

using namespace Eigen;

void print_plate(const VectorXd& plate, int nrows, int ncols){
    for (size_t i = 0; i < nrows; i++)
    {
        for (size_t j = 0; j < ncols; j++)
        {
            std::cout << std::setw(8) << plate[i*nrows + j] << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void write_plate(const VectorXd& plate, std::ofstream& file, int nrows, int ncols){
    for (size_t i = 0; i < nrows; i++)
    {
        for (size_t j = 0; j < ncols; j++)
        {
            file << plate[i*ncols + j] << " ";
        }
        file << '\n';
    }
    file << '\n';
}

void build_system_const_bord_coords(SparseMatrix<double>& A, double sx, double sy, int nrows, int ncols, const std::vector<std::pair<double,double>>& coords){

    int N = nrows*ncols;
    std::vector<Triplet<double>> triplets;
    triplets.reserve(5*N);
    
    for (size_t i = 0; i < ncols; i++) // U border
    {
        triplets.push_back(Triplet<double>(i, i, 1));
    }
    int counter = ncols;
    for (size_t i = 1; i < nrows-1; i++)
    {
        triplets.push_back(Triplet<double>(counter, counter, 1)); // R component border
        counter++;
        for (size_t j = 1; j < ncols-1; j++) // the inner stuff, starts from 1 bc RcompBord and ends in -1 bc of LcompBord
        {
            triplets.push_back(Triplet<double>(counter, counter - ncols, -sy));
            triplets.push_back(Triplet<double>(counter, counter - 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter, 1 + 2*sx + 2*sy));
            triplets.push_back(Triplet<double>(counter, counter + 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter + ncols, -sy));
            counter++;
        }
        triplets.push_back(Triplet<double>(counter, counter, 1)); // L component border
        counter++;
    }

    for (size_t i = (nrows-1)*ncols; i < N; i++) // D border
    {
        triplets.push_back(Triplet<double>(i, i, 1));
    }

    A.setFromTriplets(triplets.begin(), triplets.end());
    for (auto c : coords) {
        int row = c.first * ncols + c.second;   
        A.coeffRef(row, row) = 1;
        A.coeffRef(row, row - 1) = 0;
        A.coeffRef(row, row + 1) = 0;
        A.coeffRef(row, row - ncols) = 0;      
        A.coeffRef(row, row + ncols) = 0;      
    }
    A.makeCompressed();
}

void set_plate_borders(VectorXd& plate, int nrows, int ncols, double l, double u, double r, double d){
    for (size_t i = 0; i < ncols; i++)
    {
        plate(i) = u;
    }
    for (size_t i = 1; i < nrows-1; i++)
    {
        plate(i*ncols) = l;
        plate((i+1)*ncols - 1) = r;
    }
    for (size_t i = 0; i < ncols; i++)
    {
        plate((nrows-1)*ncols + i) = d;
    }
}
