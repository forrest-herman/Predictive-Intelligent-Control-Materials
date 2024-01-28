#include <Eigen/Dense>

// MVM 4AX3
//
/// 

#pragma once

class LTISystem{

public:

  /**
  *  LTI System
  *   A - System dynamics matrix
  *   C - Input matrix
  *   C - Output matrix
  */
 LTISystem(
      const Eigen::MatrixXd& A,
      const Eigen::MatrixXd& B,
      const Eigen::MatrixXd& C
  );


  // Initialize with initial states.
  void init(const Eigen::VectorXd& x0);

  //* Update  with control u
  void update(const Eigen::VectorXd& u);

  /// GET a INFO
  Eigen::VectorXd state() { return stateX; };
  Eigen::VectorXd output(); 

private:

  Eigen::MatrixXd A, B, C;

  int m, n;
  bool initialized;

  // NOISE Not yet
  /*
  bool x_noise;
  bool y_noise;

  Eigen::VectorXd  r; // x_noise standard 
  Eigen::VectorXd  q;  // y_noise standard
  */


  // state; 
  Eigen::VectorXd stateX;
  Eigen::VectorXd outY;
};
