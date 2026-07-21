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
    input.open("../input.txt");
    int ex{};
    input >> ex;
    while (ex != 1)
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

    input.close();

    
    const double hx = Lx/nrows;
    const double hy = Ly/ncols;
    const double ht = t_end/nt;
    const double sx = a*ht/(hx*hx);
    const double sy = a*ht/(hy*hy);


    VectorXd plate(nrows*ncols);


    set_plate_borders(plate, nrows, ncols, l, u, r, d);
    //print_plate(plate, nrows, ncols);

    SparseMatrix<double> system{nrows*ncols, nrows*ncols};
    build_system_const_bord_coords(system, sx, sy, nrows, ncols, {});
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