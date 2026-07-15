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
    
    int nx, ny, nt;
    input >> nx >> ny >> nt;
    double t_end;
    input >> t_end;
    double Lx, Ly; //m
    input >> Lx >> Ly;
    double a;
    input >> a;

    double u, l, r, d;
    input >> u >> l >> r >> d;

    input.close();

    
    const double hx = Lx/nx;
    const double hy = Ly/ny;
    const double ht = t_end/nt;
    const double sx = a*ht/(hx*hx);
    const double sy = a*ht/(hy*hy);


    VectorXd plate(nx*ny);


    set_plate_borders(plate, nx, ny, l, u, r, d);
    //print_plate(plate, nx, ny);

    SparseMatrix<double> system{nx*ny, nx*ny};
    build_system_L_U_R_D_coords(system, sx, sy, nx, ny, {});
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
        to_write.open("../build/data1.txt", std::ofstream::out | std::ofstream::trunc);
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