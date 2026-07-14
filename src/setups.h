#pragma once
#include <iostream>
#include <vector>
#include <Eigen/SparseLU>
#include <Eigen/SparseCholesky>  
#include <cassert>
#include <iomanip> // for std::setw
#include <fstream>

using namespace Eigen;

void print_plate(const VectorXd& plate, int nx, int ny){
    for (size_t i = 0; i < nx; i++)
    {
        for (size_t j = 0; j < ny; j++)
        {
            std::cout << std::setw(8) << plate[i*nx + j] << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void write_plate(const VectorXd& plate, std::ofstream& file, int nx, int ny){
    for (size_t i = 0; i < nx; i++)
    {
        for (size_t j = 0; j < ny; j++)
        {
            file << plate[i*nx + j] << " ";
        }
        file << '\n';
    }
    file << '\n';
}

void build_system_0_0_0_0(SparseMatrix<double>& A, double sx, double sy, int nx, int ny){

    const int N = nx*ny;
    std::vector<Triplet<double>> triplets;
    triplets.reserve(5*N);
    for (size_t i = 3; i < N; i++) // ll diag
    {
        triplets.push_back(Triplet<double>(i, i - nx, -sy));
    }
    for (size_t i = 1; i < N; i++) // l diag
    {
        triplets.push_back(Triplet<double>(i, i - 1, -sx));
    }
    for (size_t i = 0; i < N; i++) // diag
    {
        triplets.push_back(Triplet<double>(i, i, 1 + 2*sx + 2*sy));
    }
    for (size_t i = 0; i < N-1; i++) // u diag
    {
        triplets.push_back(Triplet<double>(i, i + 1, -sx));
    }
    for (size_t i = 0; i < N-3; i++) // uu diag
    {
        triplets.push_back(Triplet<double>(i, i + nx, -sy));
    }
    A.setFromTriplets(triplets.begin(), triplets.end());
    A.makeCompressed();
}

void build_system_0_0_0_0_coords(SparseMatrix<double>& A, double sx, double sy, int nx, int ny, const std::vector<std::pair<double, double>>& coords){
    
    const int N = nx*ny;
    std::vector<Triplet<double>> triplets;
    triplets.reserve(5*N);
    // Assembly loops with correct stride and bounds
    for (size_t i = nx; i < N; i++)          // ll diag
        triplets.push_back(Triplet<double>(i, i - nx, -sy));
    for (size_t i = 1; i < N; i++)           // l diag (ideally add i % nx != 0)
        triplets.push_back(Triplet<double>(i, i - 1, -sx));
    for (size_t i = 0; i < N; i++)           // main diag
        triplets.push_back(Triplet<double>(i, i, 1 + 2*sx + 2*sy));
    for (size_t i = 0; i < N-1; i++)         // u diag (ideally add (i+1) % nx != 0)
        triplets.push_back(Triplet<double>(i, i + 1, -sx));
    for (size_t i = 0; i < N - nx; i++)      // uu diag
        triplets.push_back(Triplet<double>(i, i + nx, -sy));

    
    
    A.setFromTriplets(triplets.begin(), triplets.end());
    for (auto c : coords) {
        int row = c.first * nx + c.second;   
        A.coeffRef(row, row) = 1;
        A.coeffRef(row, row - 1) = 0;
        A.coeffRef(row, row + 1) = 0;
        A.coeffRef(row, row - nx) = 0;      
        A.coeffRef(row, row + nx) = 0;      
    }

    A.makeCompressed();
}

void build_system_L_U_R_D(SparseMatrix<double>& A, double sx, double sy, int nx, int ny){

    int N = nx*ny;
    std::vector<Triplet<double>> triplets;
    triplets.reserve(5*N);
    
    for (size_t i = 0; i < nx; i++) // U border
    {
        triplets.push_back(Triplet<double>(i, i, 1));
    }
    int counter = nx;
    for (size_t i = 1; i < ny-1; i++)
    {
        triplets.push_back(Triplet<double>(counter, counter, 1)); // R component border
        counter++;
        for (size_t j = 1; j < nx-1; j++) // the inner stuff, starts from 1 bc RcompBord and ends in -1 bc of LcompBord
        {
            triplets.push_back(Triplet<double>(counter, counter - nx, -sy));
            triplets.push_back(Triplet<double>(counter, counter - 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter, 1 + 2*sx + 2*sy));
            triplets.push_back(Triplet<double>(counter, counter + 1, -sx));
            triplets.push_back(Triplet<double>(counter, counter + nx, -sy));
            counter++;
        }
        triplets.push_back(Triplet<double>(counter, counter, 1)); // L component border
        counter++;
    }

    for (size_t i = (ny-1)*nx; i < N; i++) // D border
    {
        triplets.push_back(Triplet<double>(i, i, 1));
    }

    A.setFromTriplets(triplets.begin(), triplets.end());
    A.makeCompressed();
}

void set_plate_borders(VectorXd& plate, int nx, int ny, double l, double u, double r, double d){
    for (size_t i = 0; i < nx; i++)
    {
        plate(i) = u;
    }
    for (size_t i = 1; i < ny-1; i++)
    {
        plate(i*nx) = l;
        plate((i+1)*nx - 1) = r;
    }
    for (size_t i = 0; i < nx; i++)
    {
        plate((ny-1)*nx + i) = d;
    }
}
