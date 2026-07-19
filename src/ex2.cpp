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
    while (ex != 2)
    {   
        std::string get_rid_of;
        getline(input, get_rid_of);
        input >> ex;
    }
    
    int nrows, ncols, nt;
    input >> nrows >> ncols >> nt;
    double t_end;
    input >> t_end;
    double Lx, Ly; //m
    input >> Lx >> Ly;
    double a;
    input >> a;

    double u, l, r, d;
    input >> u >> l >> r >> d;

    
    
    const double hx = Lx/nrows;
    const double hy = Ly/ncols;
    const double ht = t_end/nt;
    const double sx = a*ht/(hx*hx);
    const double sy = a*ht/(hy*hy);
    
    VectorXd plate(nrows*ncols);
    
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
        plate(row * ncols + col) = val;
        coords.push_back(to_add);
    }
    input.close();



    set_plate_borders(plate, nrows, ncols, l, u, r, d);
    //print_plate(plate, nrows, ncols);

    SparseMatrix<double> system{nrows*ncols, nrows*ncols};
    build_system_const_bord_coords(system, sx, sy, nrows, ncols, coords);
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
        to_write.open("../build/data2.txt", std::ofstream::out | std::ofstream::trunc);
        to_write << nrows << " " << ncols << " " << nt << '\n';
        for (size_t i = 0; i < nt; i++)
        {
            write_plate(plate, to_write, nrows, ncols);
            VectorXd new_plate = solver.solve(plate);
            plate = new_plate;
            //print_plate(plate, nrows, ncols);
        }
        to_write.close();
    }


    return 0;
}