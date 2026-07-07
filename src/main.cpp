#include <iostream>
#include <vector>
#include <Eigen/SparseLU>  
#include <cassert>
#include <iomanip> // for std::setw

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


void build_system(SparseMatrix<double>& A, double ht, double hx, double hy, double a, int N){

    std::vector<Triplet<double>> triplets;
    triplets.reserve(5*N);
    for (size_t i = 3; i < N; i++) // ll diag
    {
        triplets.push_back(Triplet<double>(i, i - 3, -a*ht/(hy*hy)));
    }
    for (size_t i = 1; i < N; i++) // l diag
    {
        triplets.push_back(Triplet<double>(i, i - 1, -a*ht/(hx*hx)));
    }
    for (size_t i = 0; i < N; i++) // diag
    {
        triplets.push_back(Triplet<double>(i, i, 1 + 2/(hx*hx) + 2/(hy*hy)));
    }
    for (size_t i = 0; i < N-1; i++) // u diag
    {
        triplets.push_back(Triplet<double>(i, i + 1, -a*ht/(hx*hx)));
    }
    for (size_t i = 0; i < N-3; i++) // uu diag
    {
        triplets.push_back(Triplet<double>(i, i + 3, -a*ht/(hy*hy)));
    }
    A.setFromTriplets(triplets.begin(), triplets.end());
}


int main(int argc, char* argv[]){
    assert(argc == 5);

    const int nx = (int)argv[1][0]-48;
    const int ny = (int)argv[2][0]-48;
    const double ht = std::stof(argv[3]);
    const int t_max = std::stoull(argv[4]);
    //std::cout << t_max << '\n';

    double Lx = 0.50; //m
    double Ly = 0.50; //m
    double hx = Lx/nx;
    double hy = Ly/ny;
    double a = 1;

    VectorXd plate_old(nx*ny);
    VectorXd plate_cur(nx*ny);
    SparseMatrix<double> system{nx*ny, nx*ny};
    
    build_system(system, ht, hx, hy, a, nx*ny);
    //std::cout << system << '\n';

    plate_old(4) = 400; // K 

    SparseLU<SparseMatrix<double>> solver;
    solver.compute(system);
    plate_cur = solver.solve(plate_old);

    print_plate(plate_old, nx, ny);
    print_plate(plate_cur, nx, ny);
    return 0;
}