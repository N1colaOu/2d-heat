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
        triplets.push_back(Triplet<double>(i, i, 1 + 2*ht*a/(hx*hx) + 2*ht*a/(hy*hy)));
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
    A.makeCompressed();
}

void write_plate(const VectorXd& plate, std::fstream& file, int nx, int ny){
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

int main(int argc, char* argv[]){
    //assert(argc == 5);
    setNbThreads(1);
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
    
    build_system(system, ht, hx, hy, a, nx*ny);
    //std::cout << system << '\n';

    for (size_t i = 0; i < nx; i++)
    {
        plate(5) = 500; //k
    }
    

    SimplicialLLT<SparseMatrix<double>> solver;
    solver.compute(system);
    //print_plate(plate, nx, ny);

    std::fstream to_write;
    to_write.open("/home/nic/GithubRepos/2d-heat/build/data.txt");
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