#include <iostream>
#include <vector>
#include <Eigen/SparseLU>
#include <Eigen/SparseCholesky>  
#include <cassert>
#include <iomanip> // for std::setw
#include <fstream>
#include "setups.h"

using namespace Eigen;


int main(int argc, char* argv[]){
    //assert(argc == 5);
    // const int nx = (int)argv[1][0]-48;
    // const int ny = (int)argv[2][0]-48;
    // const double ht = std::stof(argv[3]);
    // const int t_max = std::stoull(argv[4]);
    //std::cout << t_max << '\n';

    const int nx = 11;
    const int ny = 11;
    const int nt = 100;
    const double t_end = 1.00;
    const double Lx = 1.00; //m
    const double Ly = 1.00; //m
    const double hx = Lx/nx;
    const double hy = Ly/ny;
    const double ht = t_end/nt;
    const double a = 0.4;

    VectorXd plate(nx*ny);
    SparseMatrix<double> system{nx*ny, nx*ny};
    
    build_system_0_0_0_0(system, ht, hx, hy, a, nx*ny);
    //std::cout << system << '\n';

    for (size_t i = 0; i < nx; i++)
    {
        plate(5) = 500; //k
    }
    

    SimplicialLLT<SparseMatrix<double>> solver;
    solver.compute(system);
    //print_plate(plate, nx, ny);

    std::ofstream to_write;
    to_write.open("/home/nic/GithubRepos/2d-heat/build/data0.txt", std::ofstream::out | std::ofstream::trunc);
    to_write << nx << " " << ny << " " << nt << '\n';


    for (size_t i = 0; i < nt; i++)
    {
        write_plate(plate, to_write, nx, ny);
        plate = solver.solve(plate);
        if(solver.info() != Success){
            std::cout << "WRONG BOOM!\n";
            break;
        }
        //print_plate(plate, nx, ny);
    }
    to_write.close();
    return 0;
}