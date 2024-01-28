
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <random>




#include "kalman.hpp"

#define P_SIGMA_H  0.01
#define P_SIGMA_G 0.1

#define M_SIGMA 1.


double g,hight,v,t;

double dt;


void fall_init(double x0, double gravity)
{
	g=gravity; 
        hight= x0;
        v=0;

}


// falling with noise injected and control input u.
double falling(double noise_p,double noise_g,double u)
{

  hight=hight+dt*v + dt*noise_p;
  v=v-dt*(g+noise_g)+dt*u;
  
  return(hight);

}





int main(int argc, char* argv[]) {

  int n = 3; // Number of states
  int m = 1; // Number of measurements

  // SOME NOISE
  std::default_random_engine generator;
  std::normal_distribution<double> p_noise_h(0.0,P_SIGMA_H*P_SIGMA_H);
  std::normal_distribution<double> p_noise_g(0.0,P_SIGMA_G*P_SIGMA_G);
  std::normal_distribution<double> m_noise(0.0,M_SIGMA*M_SIGMA);


  dt = 1.0/30; // Time step

  Eigen::MatrixXd A(n, n); // System dynamics matrix
  Eigen::MatrixXd C(m, n); // Output matrix
  Eigen::MatrixXd Q(n, n); // Process noise covariance
  Eigen::MatrixXd R(m, m); // Measurement noise covariance
  Eigen::MatrixXd P(n, n); // Estimate error covariance



  A << 1.,dt,0,  0,1,dt,  0,0,1;
 
  C << 1,0,0;

  // Reasonable covariance matrices
  Q <<P_SIGMA_H*P_SIGMA_H  ,0.,0, 0,0,0, 0,0,P_SIGMA_G*P_SIGMA_G ;
  R << M_SIGMA*M_SIGMA;

  P <<  1,0,0,  0,1,0, 0,0,1;

  std::cout << "A: \n" << A << std::endl;
  std::cout << "C: \n" << C << std::endl;
  std::cout << "Q: \n" << Q << std::endl;
  std::cout << "R: \n" << R << std::endl;
  std::cout << "P: \n" << P << std::endl;


  KalmanFilter kf(dt,A, C, Q, R, P);


  Eigen::VectorXd x0(n);
  // The third is initial guess of gravity
  x0 << 110,0,0;
  //x0 << 100,0,2; ///  WOrks even with wrong sign
  kf.init(dt,x0);


  //
  fall_init(100,5.5); // Physical conditions

  FILE* log;
  log=fopen("DATA","w");


  double t = 0;
  int num=100;
  Eigen::VectorXd y(m);


  y << falling(0.,0.,0.);

  //printf("%f\t %f\t%f\t%f\t%f \n",t, y(0),  kf.state()[0],kf.state()[1],kf.state()[2]);
  fprintf(log,"%f\t %f\t%f\t%f\t%f \n",t, y(0),  kf.state()[0],kf.state()[1],kf.state()[2]);

  while(y[0]>0.){
    t += dt;
    kf.update(y);
    //printf("%f\t %f\t%f\t%f\t%f \n",t, y(0),  kf.state()[0],kf.state()[1],kf.state()[2]);
    fprintf(log,"%f\t %f\t%f\t%f\t%f \n",t, y(0),  kf.state()[0],kf.state()[1],kf.state()[2]);
    y << falling(p_noise_h(generator),p_noise_g(generator),0.);
    // ADD COME NOISE
    y(0)+=m_noise(generator);
  }

  return 0;
}
