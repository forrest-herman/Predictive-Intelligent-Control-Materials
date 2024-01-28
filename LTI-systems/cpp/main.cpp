#include "LTI.h"

#include <iostream>
#include <stdexcept>
#include <fstream>


using namespace std;





void testLTI(void)
{

  int n=2;

  ofstream myfile;
  myfile.open ("OUT");

  // SISO
  Eigen::MatrixXd A(n, n); 
  Eigen::MatrixXd B(n, 1); 
  Eigen::MatrixXd C(1, n); 

  //A << 0.,1.,-0.980198,   1.97958; // SLOW OSCIALTOR
  A << 1.,1.,-1,0.   ; // OSCIALTOR with periode 6
  B << 0.,1.;
  C<< 0.,1.;

  Eigen::VectorXd x(n);
  Eigen::VectorXd u(1);

//  x<<1,1;
   x<<1,0;
  u<<0;


  std::cout << "A: \n" << A << std::endl;
  std::cout << "B: \n" << B << std::endl;
  std::cout << "C: \n" << C << std::endl;

  // Construct the  SYSTEM
  LTISystem lti(A, B, C);
  lti.init(x);

  for(int i=0;i<1000;i++){
  	lti.update(u);
	//std::cout << "X= "<<std::endl;
	//std::cout << (lti.state())[0] << std::endl;
	//std::cout << (lti.output())[0] << std::endl;
	//myfile << (lti.output())[0] << std::endl;
	myfile << (lti.output()) << std::endl;
   };
   myfile.close();
}




int main(void)
{


	testLTI();
	return(0);
}
