#include <iostream>
#include <vector>
#include <Eigen/SparseLU>
#include <Eigen/SparseCholesky>  
#include <cassert>
#include <iomanip> // for std::setw
#include <fstream>
#include "setups.h"

using namespace Eigen;


int main(){

    std::ifstream input;
    input.open("../app/input.txt");
    int ex{};
    input >> ex;
    while (ex != 1)
    {   
        std::string get_rid_of;
        getline(input, get_rid_of);
        input >> ex;
    }
    int nx;
    input >> nx;
    int ny;
    input >> ny;
    int nt;
    input >> nt;
    double t_end;
    input >> t_end;
    double Lx; //m
    input >> Lx;
    double Ly; //m
    input >> Ly;
    double a;
    input >> a;
    input.close();


    const double hx = Lx/nx;
    const double hy = Ly/ny;
    const double ht = t_end/nt;
    const double sx = a*ht/(hx*hx);
    const double sy = a*ht/(hy*hy);

    VectorXd plate(nx*ny);
    SparseMatrix<double> system{nx*ny, nx*ny};
    
    build_system_0_0_0_0(system, sx, sy, nx, ny);
    
    // int middle = (ny/2)*nx + nx/2;
    // plate(middle) = 100;

    SimplicialLLT<SparseMatrix<double>> solver;
    solver.compute(system);

    std::ofstream to_write;
    to_write.open("../build/data0.txt", std::ofstream::out | std::ofstream::trunc);
    to_write << nx << " " << ny << " " << nt << '\n';


    for (size_t i = 0; i < nt; i++)
    {
        write_plate(plate, to_write, nx, ny);
        plate = solver.solve(plate);
        if(solver.info() != Success){
            std::cout << "WRONG BOOM!\n";
            break;
        }
    }
    to_write.close();
    return 0;
}