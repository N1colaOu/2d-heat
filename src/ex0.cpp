#include <iostream>
#include <vector>
#include <Eigen/UmfPackSupport> 
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
    while (ex != 0)
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
    double t_end; //s
    input >> t_end;
    double Lx; //m
    input >> Lx;
    double Ly; //m
    input >> Ly;
    double a;
    input >> a;
    
    VectorXd plate(nx*ny);

    int c_size;
    input >> c_size;
    std::vector<std::pair<double, double>> coords;
    coords.reserve(c_size);
    for (size_t i = 0; i < c_size; i++)
    {
        int row, col;
        double val;
        input >> row >> col >> val;
        std::pair<double, double> to_add{row, col};
        plate(row*nx + col) = val;
        coords.push_back(to_add);
    }
    input.close();


    const double hx = Lx/nx;
    const double hy = Ly/ny;
    const double ht = t_end/nt;
    const double sx = a*ht/(hx*hx);
    const double sy = a*ht/(hy*hy);

    SparseMatrix<double> system{nx*ny, nx*ny};
    
    build_system_0_0_0_0_coords(system, sx, sy, nx, ny, coords);

    UmfPackLU<SparseMatrix<double>> solver;
    solver.compute(system);
    //std::cout << system << '\n';
    if(solver.info() != Success){
        if (solver.info() == Eigen::NumericalIssue) {
            std::cerr << "Error: Numerical Issue" << '\n';
        } else {
            std::cerr << "Error" << '\n';
        }
    }
    else{
        std::ofstream to_write;
        to_write.open("../build/data0.txt", std::ofstream::out | std::ofstream::trunc);
        to_write << nx << " " << ny << " " << nt << '\n';
        for (size_t i = 0; i < nt; i++)
        {
            write_plate(plate, to_write, nx, ny);
            VectorXd new_plate = solver.solve(plate);
            plate = new_plate;
        }
        to_write.close();
    }
    return 0;
}