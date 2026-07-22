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
        triplets.push_back(Triplet<double>(counter, counter, 1)); // L component border
        counter++;
        for (size_t j = 1; j < ncols-1; j++) // the inner stuff, starts from 1 bc LcompBord and ends in -1 bc of RcompBord
        {
            triplets.push_back(Triplet<double>(counter, counter - ncols, -sy));
            triplets.push_back(Triplet<double>(counter, counter - 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter, 1 + 2*sx + 2*sy));
            triplets.push_back(Triplet<double>(counter, counter + 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter + ncols, -sy));
            counter++;
        }
        triplets.push_back(Triplet<double>(counter, counter, 1)); // R component border
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

void build_system_const_heat_coords(SparseMatrix<double>& A, double sx, double sy, int nrows, int ncols, const std::vector<std::pair<double,double>>& coords){

    int N = nrows*ncols;
    std::vector<Triplet<double>> triplets;
    triplets.reserve(5*N);
    
    triplets.push_back(Triplet<double>(0, 0, 1 + sx + sy)); //the 4 corners
    triplets.push_back(Triplet<double>(0, 1, -sx));
    triplets.push_back(Triplet<double>(0, ncols, -sy));

    triplets.push_back(Triplet<double>(ncols - 1, ncols- 1, 1 + sx + sy));
    triplets.push_back(Triplet<double>(ncols - 1, ncols - 2, -sx));
    triplets.push_back(Triplet<double>(ncols - 1, 2*ncols - 1, -sy));

    const int last_c = (nrows-1) * ncols;
    triplets.push_back(Triplet<double>(last_c, last_c, 1 + sx + sy));
    triplets.push_back(Triplet<double>(last_c, last_c + 1, -sx));
    triplets.push_back(Triplet<double>(last_c, last_c - ncols, -sy));

    triplets.push_back(Triplet<double>(N - 1, N - 1, 1 + sx + sy));
    triplets.push_back(Triplet<double>(N - 1, N - 2, -sx));
    triplets.push_back(Triplet<double>(N - 1, N - 1 - ncols, -sy));


    for (size_t i = 1; i < ncols-1; i++) // U border
    {
        triplets.push_back(Triplet<double>(i, i, 1 + 2*sx + sy));
        triplets.push_back(Triplet<double>(i, i - 1, -sx));
        triplets.push_back(Triplet<double>(i, i + ncols, -sy));
        triplets.push_back(Triplet<double>(i, i + 1, -sx));
    }
    int counter = ncols;
    for (size_t i = 1; i < nrows-1; i++)
    {
        triplets.push_back(Triplet<double>(counter, counter, 1 + sx + 2*sy)); // L component border
        triplets.push_back(Triplet<double>(counter, counter - ncols, -sy));
        triplets.push_back(Triplet<double>(counter, counter + 1, -sx));
        triplets.push_back(Triplet<double>(counter, counter + ncols, -sy));
        counter++;
        for (size_t j = 1; j < ncols-1; j++) // the inner stuff, starts from 1 bc LcompBord and ends in -1 bc of RcompBord
        {
            triplets.push_back(Triplet<double>(counter, counter - ncols, -sy));
            triplets.push_back(Triplet<double>(counter, counter - 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter, 1 + 2*sx + 2*sy));
            triplets.push_back(Triplet<double>(counter, counter + 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter + ncols, -sy));
            counter++;
        }
        triplets.push_back(Triplet<double>(counter, counter, 1 + sx + 2*sy)); // R component border
        triplets.push_back(Triplet<double>(counter, counter - ncols, -sy));
        triplets.push_back(Triplet<double>(counter, counter - 1, -sx));
        triplets.push_back(Triplet<double>(counter, counter + ncols, -sy));
        counter++;
    }

    for (size_t i = last_c + 1; i < N-1; i++) // D border
    {
        triplets.push_back(Triplet<double>(i, i, 1 + 2*sx + sy));
        triplets.push_back(Triplet<double>(i, i - 1, -sx));
        triplets.push_back(Triplet<double>(i, i - ncols, -sy));
        triplets.push_back(Triplet<double>(i, i + 1, -sx));
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
