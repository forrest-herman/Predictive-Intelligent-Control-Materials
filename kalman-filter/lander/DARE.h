#include <Eigen/Dense>

#pragma once



// Finite Horizon Ricatti soltuion, MVM	

class DARE{

public:

  /**
  *   Ricatte soltuion 
  *   A - System dynamics matrix
  *   B - Input matrix

  *   Q - State  cost
  *   R - control cost
  */

 DARE(
      const Eigen::MatrixXd& A,
      const Eigen::MatrixXd& B,
      const Eigen::MatrixXd& Q,
      const Eigen::MatrixXd& R,
      int horizon
  );

  ~DARE();


  void init(void);
  void print(void);


  Eigen::MatrixXd getK(int i)
  {
	return(K_array[i]);
  }


  Eigen::MatrixXd getS(int i)
  {
	return(S_array[i]);
  }

private:

  Eigen::MatrixXd A, B, Q, R;

  int  N; /// TOtal Time
  int state_n;
  bool initialized;

  // NOISE
  bool x_noise;
  bool y_noise;


  // Storage for Gains
#define NUM 256
  
  Eigen::MatrixXd* K_array;
  Eigen::MatrixXd* S_array;

};
