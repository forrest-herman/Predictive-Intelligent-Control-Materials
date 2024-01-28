#include "DARE.h"

#include <iostream>
#include <stdexcept>
#include <fstream>


using namespace std;





void setup(void)
{

  int n=2;
  double dt=1/30.;

  int horizon=10;

  ofstream myfile;
  myfile.open ("OUT");

  // SISO
  Eigen::MatrixXd A(n, n); 
  Eigen::MatrixXd B(n, 1); 

  Eigen::MatrixXd Q(n, n); 
  Eigen::MatrixXd R(1, 1); 


  A << 1.,1., 0,1;
  B << 0.,1.;

  Q.setIdentity();
  R<<1;


  // Construct the  SYSTEM
  DARE contr(A, B, Q,R, horizon );

  contr.init();

  contr.print();


  // RUN SYTEM WIHT THAT CONTROLLER !
//const IOFormat fmt(5, DontAlignCols, "\t", " ", "", "", "", "");
 //Eigen::IOFormat fmt(5, 0, ", ", ";\n", "[", "]", "[", "]");
 Eigen::IOFormat fmt(5, 0, ", ", " ", "", "", "", "");

 Eigen::VectorXd x(n);
 Eigen::VectorXd u(1);

  x<<1,0;
  for(int i=0;i<horizon;i++){
	 u= -1* contr.getK(i) *x;
         x=A*x+B*u;
	//std::cout << "X= "<<std::endl;
	std::cout << "x= "<< x.format(fmt)  << std::endl;
	myfile << x.format(fmt)  << std::endl;
   };
   myfile.close();
}





int main(void)
{


	setup();
	return(0);
}
