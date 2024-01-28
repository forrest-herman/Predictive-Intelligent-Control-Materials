// MvM 4Ax3  LLS with eigen
#include <iostream>
#include "../../code_libraries/eigen-3.4.0/Eigen/Dense"

using namespace std;
using namespace Eigen;

#include <random>
#include <math.h>

std::default_random_engine generator;
std::normal_distribution<double> distribution(0., .5);

#define NUM_POINTS 50

double func(double x)
{
    return (2. * x + 2.);
}

double x[NUM_POINTS];
double y[NUM_POINTS];
double a, b;

void make_data(int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        x[i] = (double)i / (double)num;
        y[i] = func(x[i]) + distribution(generator);
    }
}

// Equalt spaced x
void make_matrix(int num, MatrixXd &A, VectorXd &b)
{
    int i;
    // double t;
    for (i = 0; i < NUM_POINTS; i++)
    {
        // t=(double)i;
        A(i, 0) = 1.;
        A(i, 1) = x[i];

        // b(i)=func(x) +distribution(generator);
        b(i) = y[i];
    }
}

void print_data(void)
{
    FILE *of = fopen("DATA", "w");
    int i;
    for (i = 0; i < NUM_POINTS; i++)
    {
        fprintf(of, "%f\t%f\t%f\n", x[i], y[i], a * x[i] + b);
    }
    fclose(of);
}

int main()
{

    VectorXd y = VectorXd(NUM_POINTS);
    MatrixXd A = MatrixXd(NUM_POINTS, 2);

    make_data(NUM_POINTS);
    make_matrix(NUM_POINTS, A, y);

    //   cout << "matrix A:\n" << A << endl;
    //  cout << "vector b:\n" << b << endl;

    VectorXd res = A.bdcSvd(ComputeThinU | ComputeThinV).solve(y);

    cout << "solution is:\n"
         << res << endl;
    a = res(0);
    b = res(1);
    print_data();
}
