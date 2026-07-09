#include <iostream>
#include <vector>
#include <Eigen/SparseLU>
#include <Eigen/SparseCholesky>  
#include <Eigen/UmfPackSupport>
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

    const int nx = 10;
    const int ny = 10;
    const int nt = 200;
    const double t_end = 10.00;
    const double Lx = 1.00; //m
    const double Ly = 1.00; //m
    const double hx = Lx/nx;
    const double hy = Ly/ny;
    const double ht = t_end/nt;
    const double a = 0.01;
    const double sx = a*ht/(hx*hx);
    const double sy = a*ht/(hy*hy);
    const double u = 100.00;
    const double l = 100.00;
    const double d = 100.00;
    const double r = 100.00;


    VectorXd plate(nx*ny);


    set_plate_borders(plate, nx, ny, l, r, u, d);
    //print_plate(plate, nx, ny);

    SparseMatrix<double> system{nx*ny, nx*ny};
    build_system_L_U_R_D(system, sx, sy, nx, ny);
    //std::cout << system << '\n';
    UmfPackLU<SparseMatrix<double>> solver;
    solver.compute(system);
    if(solver.info() != Success){
        if (solver.info() == Eigen::NumericalIssue) {
            std::cerr << "Error: Numerical Issue" << '\n';
        } else {
            std::cerr << "Error" << '\n';
        }
    }
    else{
        std::ofstream to_write;
        to_write.open("/home/nic/GithubRepos/2d-heat/build/data1.txt", std::ofstream::out | std::ofstream::trunc);
        to_write << nx << " " << ny << " " << nt << '\n';
        for (size_t i = 0; i < nt; i++)
        {
            write_plate(plate, to_write, nx, ny);
            VectorXd new_plate = solver.solve(plate);
            plate = new_plate;
            //print_plate(plate, nx, ny);
        }
        to_write.close();
    }


    return 0;
}