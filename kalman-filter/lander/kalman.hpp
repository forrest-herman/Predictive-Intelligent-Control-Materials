#include <Eigen/Dense>

#pragma once

class KalmanFilter {

public:

  /**
  *   A - System dynamics matrix
  *   C - Output matrix
  *   Q - Process noise covariance
  *   R - Measurement noise covariance
  *   P - Estimate error covariance
  */
  KalmanFilter(
      double dt,
      const Eigen::MatrixXd& A,
      const Eigen::MatrixXd& C,
      const Eigen::MatrixXd& Q,
      const Eigen::MatrixXd& R,
      const Eigen::MatrixXd& P
  );

  KalmanFilter();

  void init();
  void init(double t0, const Eigen::VectorXd& x0);

  void update(const Eigen::VectorXd& y);

  void update(const Eigen::VectorXd& y, double dt, const Eigen::MatrixXd A);

  Eigen::VectorXd state() { return x_hat; };
  double time() { return t; };

//private: // Nothing private , so we can plot the data ...

  Eigen::MatrixXd A, C, Q, R, P, K, P0;

  int m, n;

  double t0, t;
  double dt;

  bool initialized;

  Eigen::MatrixXd I;

  Eigen::VectorXd x_hat, x_hat_new;
};
