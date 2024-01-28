
// MvM,  find max in noisy data, "read between the noisy lines"
// data is written you so you can plot it
// There is a naive max, run it several times and you see how it jumps

#include <iostream>
#include <string>
#include <random>

#include "../../code_libraries/eigen-3.4.0/Eigen/Dense"

using namespace std;
using namespace Eigen;

#define MAX_NUM 2014

#define MIN .12

double x_val[MAX_NUM];
double y_val[MAX_NUM];

void make_data(int num)
{

	if (num > MAX_NUM)
		return;

	std::random_device rd; // FOR SEED
	std::mt19937 generator(rd());
	std::uniform_real_distribution<double> distribution_x(-5, 5);
	std::normal_distribution<double> distribution_y(0, 1);

	for (int i = 0; i < num; ++i)
	{
		x_val[i] = distribution_x(generator);
		y_val[i] = -(x_val[i] - MIN) * (x_val[i] - MIN) + distribution_y(generator);
	}
}

void print_data(const char *f_name, int num)
{
	int i;
	FILE *out_file = fopen(f_name, "w");
	for (i = 0; i < num; i++)
	{
		fprintf(out_file, "%f\t%f\n", x_val[i], y_val[i]);
	}
}

// native, find largest y and return corcoeffsponding x
double find_max_naive(double *x, double *y, int num)
{
	int i;
	double max_x, max_y;
	max_y = y[0];
	for (i = 1; i < num; i++)
	{
		if (y[i] > max_y)
		{
			max_y = y[i];
			max_x = x[i];
		}
	}
	return (max_x);
}

// YOU IMPLEMENT  THIS, ~15-20 lines ...
double func(double x, double a, double b, double c)
{
	return (a * x * x + b * x + c);
}

// fit y=a*x^2+b*x+c lls and return -b/2*a as  the position x that maximizes the fit.
//  The other code examples I gave have the pieces needed.
double find_max(double *x, double *y, int num)
{
	// HERE
	// (A^T A)^-1
	// printf("x %f\n", x)
	// for (int i = 0; i < num; i++)
	// {

	// 	std::cout << x[i] << "\n";
	// }

	MatrixXd A = MatrixXd(num, 3);
	VectorXd Y = VectorXd(num);

	// make matrix
	int i;
	// double t;
	for (i = 0; i < num; i++)
	{
		// t=(double)i;
		A(i, 0) = 1.;
		A(i, 1) = x[i];
		A(i, 2) = x[i] * x[i];

		// b(i)=func(x) +distribution(generator);
		Y(i) = y[i];
	}

	// cout << "matrix A:\n"
	// 	 << A << endl;
	// cout << "vector b:\n"
	// 	 << Y << endl;

	VectorXd coeffs = (A.transpose() * A).bdcSvd(ComputeThinU | ComputeThinV).solve(A.transpose() * Y);

	double a, b, c;
	c = coeffs(0);
	b = coeffs(1);
	a = coeffs(2);

	double maxCoord = (-b / (2 * a));

	printf("maxValue %f\n", func(maxCoord, a, b, c));
	printf("a=%f,	b=%f,	c=%f\n", a, b, c);

	return maxCoord;
}

int main(void)
{

	// DATA is in global x_val,y_val ...
	make_data(256);
	print_data("DATA", 256);

	const double n_max = find_max_naive(x_val, y_val, 256);
	const double a_max = find_max(x_val, y_val, 256);

	printf("TRUE MAX %f  Naive:  %f  New Max %f \n", MIN, n_max, a_max);

	return (0);
}
