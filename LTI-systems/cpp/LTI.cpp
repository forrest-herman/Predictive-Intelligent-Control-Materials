#include <iostream>
#include <stdexcept>

#include "LTI.h"

LTISystem::LTISystem(
    const Eigen::MatrixXd& A,
    const Eigen::MatrixXd& B,
    const Eigen::MatrixXd& C)
    : A(A), B(B), C(C)
{
  n=A.rows();
  m=C.rows();
  initialized=false;
}

//LTISystem::LTISystem() {}

void LTISystem::init(const Eigen::VectorXd& x0) {
  initialized = true;
  stateX=x0;
//  outY=C*stateX; // NOT Needed
}


void LTISystem::update(const Eigen::VectorXd& u) {

  if(!initialized) throw std::runtime_error("Not init");

   stateX= A*stateX + B*u;
   //outY=C*stateX;  // Not needed ...
}


Eigen::VectorXd LTISystem::output()
{
	 return(C*stateX);
};


