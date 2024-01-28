// Extended Kalman filter localisation
// (c) MvM for MECHTRON 4AX3,

// THIS IS THE ASSIGMENT 4 VERSION,
// ADD THE DISTANCE ONLY UPDATES with MOVING BEAKENBS

// look for the HERE marker, that shows things you need to do
// I impelmented you the satellites, the loggine of them and
// the fucntipon prototypes you have to implement,  look for HERE  marker

#include <iostream>
#include "../../code_libraries/eigen-3.4.0/Eigen/Dense"
#include <math.h>
#include <random>

#define DT 0.01

// Turn Noise on or off
#define NOISE

Eigen::IOFormat fmt(5, 1, ", ", " ", "", "", "", "");

//-------------------------------------------------------
// Helpers

// Normalize to -Pi ... Pi
double normal_angle(double x)
{
	if (x > M_PI)
		return (x - 2. * M_PI);
	if (x < -M_PI)
		return (x + 2. * M_PI);
	return (x);
}

//-------------------------------------------------------
// Beakens (C style implementation for list)

#define MAX_NUM_B 32

class Marker
{
public:
	double px, py;
};

Marker markers[MAX_NUM_B];
int num_makers = 0;

void add_marker(double x, double y)
{
	if (num_makers > MAX_NUM_B)
		return;
	markers[num_makers].px = x;
	markers[num_makers].py = y;
	num_makers++;
}

void print_makers(void)
{
	int i;
	FILE *f_out = fopen("MARK", "w");
	for (i = 0; i < num_makers; i++)
	{
		printf("Marker %d x=%f y=%f \n", i, markers[i].px, markers[i].py);
		fprintf(f_out, "%f\t%f\n", markers[i].px, markers[i].py);
	}
	fclose(f_out);
}

//-------------------------------------------------------
// Satellite Class

#define SAT_SPEED 3

class Satellite
{
public:
	Satellite(double px, double py, double bearing);
	void Move(double dt);
	double getDistance(double x, double y);
	void Print(FILE *file);
	double getX(void) { return (px); };
	double getY(void) { return (py); };

private:
	double px, py, theta;
};

Satellite::Satellite(double _px, double _py, double bearing)
{
	px = _px;
	py = _py;
	theta = bearing;
}

void Satellite::Move(double dt)
{
	px += dt * SAT_SPEED * cos(theta);
	py += dt * SAT_SPEED * sin(theta);
	// theta is constant, the move on line but you can change it ...
}

double Satellite::getDistance(double x, double y)
{
	double d;
	d = sqrt((px - x) * (px - x) + (py - y) * (py - y));
	return (d);
}

void Satellite::Print(FILE *file)
{
	fprintf(file, "%f\t%f\t", px, py);
}

//-------------------------------------------------------
// Satellite manager (not as class ...) C style array ...

Satellite *satellites[MAX_NUM_B];
int num_satellites = 0;

void add_satelite(double x, double y, double bearing)
{
	if (num_satellites > MAX_NUM_B)
		return;
	Satellite *sat = new Satellite(x, y, bearing);
	satellites[num_satellites] = sat;
	num_satellites++;
}

void move_satellites(void)
{
	int i;
	for (i = 0; i < num_satellites; i++)
	{
		satellites[i]->Move(DT);
	}
}

void print_satellites(FILE *file)
{
	int i;
	for (i = 0; i < num_satellites; i++)
	{
		satellites[i]->Print(file);
		// fprintf("%f\t%f\t",):
	}
	fprintf(file, "\n");
}

//-------------------------------------------------------

// Robot Class

class Robot
{
public:
	Robot(double _dt);
	~Robot(){};

	void Move(void);
	Eigen::VectorXd GetState(void) { return (X); };
	void LocalizeM(void); // Marker lcoalize, angle and distance
	void LocalizeS(void); // Satellite, only distance

	void LocalizeC(void); // Compass, only direction

	void Print(void);
	void PrintVariance(void);

	void JacobiF(Eigen::MatrixXd &F, const Eigen::VectorXd &X);
	void JacobiV(Eigen::MatrixXd &V, const Eigen::VectorXd &X);
	void JacobiH(Eigen::MatrixXd &H, const Eigen::VectorXd &X, double px, double py);

	// HERE
	// THE NEW JACOBIAN YOU HAVE TO IMPLEMENT,D Distance
	void JacobiHD(Eigen::MatrixXd &H, const Eigen::VectorXd &X, double px, double py);

private:
	Eigen::VectorXd X; // Estimated State
	Eigen::VectorXd Y; // Estimated Output

	// Actual State
	double px, py;
	double theta;

	// Control Input
	double steer;
	double speed;
	double dt;

	Eigen::MatrixXd P; // CoVariance Matrix

	Eigen::MatrixXd R;	// Mesuement  Noise Marker (dist,angle)
	Eigen::MatrixXd Rc; // Mesuement  Noise Compass
	Eigen::MatrixXd Rs; // Mesuement  Noise  Satellite HERE
	Eigen::MatrixXd M;	// Control Noise

	Eigen::MatrixXd JF; // Jacibian of F
	Eigen::MatrixXd JV; // Jacibian of F with respect to speed, steering
	Eigen::MatrixXd JH; // Jacibian of H wiht respect to distance and angle

	Eigen::MatrixXd JHD; // Jacibian of H with respect to distance

	FILE *s_file; // States
	FILE *l_file; // Localizing

	FILE m_file; // Satellites

	// std::random_device rd;  //FOR SEED
	std::mt19937 generator;
	// std::default_random_engine generator;

	// mesurement
	std::normal_distribution<double> d_noise; // ranger
	std::normal_distribution<double> b_noise; // angle

	std::normal_distribution<double> c_noise; // compass

	// robot
	std::normal_distribution<double> v_noise; // speed
	std::normal_distribution<double> s_noise; // sterading
};
// Robot Implementation

/*
 Motion Model: f
		x= x+ dt* v*cos(thetha)
		y= y+ dt* v*sin(thetha);
		thetha=thetha +dt* steer

		dfx/dx=1 dfy/dy=1  dfte/te=

*/

void Robot::JacobiF(Eigen::MatrixXd &F, const Eigen::VectorXd &X)
{
	F << 1, 0, -dt * speed * sin(X(2)), 0, 1, dt * speed * cos(X(2)), 0, 0, 1;
}

// THis is df/dv and df/dsteer

void Robot::JacobiV(Eigen::MatrixXd &V, const Eigen::VectorXd &X)
{
	// F=Eigen::MatrixXd(3,3);
	V << dt * cos(X(2)), dt * sin(X(2)), 0, 0, 0, dt;
}

/*
	output h
		dx=sqrt( ex^2+ey*2)
		angle= atan2(ey/ex) -theta;

		// SIGN of H2,1 is positive ..

*/

void Robot::JacobiH(Eigen::MatrixXd &H, const Eigen::VectorXd &X, double px, double py)
{
	double var = (px - X(0)) * (px - X(0)) + (py - X(1)) * (py - X(1));
	const double dist = sqrt(var);

	H << -(px - X(0)) / dist, -(py - X(1)) / dist, 0,
		(py - X(1)) / var, -(px - X(0)) / var, -1;
}

// HERE  THIS IS YOURS
void Robot::JacobiHD(Eigen::MatrixXd &H, const Eigen::VectorXd &X, double px, double py)
{
	double var = (px - X(0)) * (px - X(0)) + (py - X(1)) * (py - X(1));
	const double dist = sqrt(var);

	H << -(px - X(0)) / dist, -(py - X(1)) / dist, 0;
}

Robot::Robot(double _dt)
{

	dt = _dt;
	// Allocate Matrix
	X = Eigen::VectorXd(3); // State
	Y = Eigen::VectorXd(2); //  Output

	M = Eigen::MatrixXd(2, 2);	//  Control Noise
	R = Eigen::MatrixXd(2, 2);	// Mesurement Noise
	Rc = Eigen::MatrixXd(1, 1); // Compass Mesurement Noise
	Rs = Eigen::MatrixXd(1, 1); // Compass Mesurement Satellite
	JF = Eigen::MatrixXd(3, 3); // Jacibian of F
	JH = Eigen::MatrixXd(2, 3); // Jacibian of H
	JV = Eigen::MatrixXd(3, 2); // Jacibian of V

	JHD = Eigen::MatrixXd(1, 3); //  Jacobina of H/d

	P = Eigen::MatrixXd(3, 3); // Co-Varianze

	// CO-VARIANCES

	// Noise in control Space
	const double sigma_vel = .1; // 0.01;
	const double sigma_steer = .1;
	M << sigma_vel * sigma_vel, 0., 0., sigma_steer * sigma_steer;

	// Noise in Mesurement Space
	/// PLAY WITH THESE TO SEE HOW WELL IT WORKS !!!
	const double sigma_range = .1; // .1, 1, 3.;
	const double sigma_bear = .1;  // .2, 1., 2.,
	R << sigma_range * sigma_range, 0, 0, sigma_bear * sigma_bear;

	const double sigma_compass = 0.02; // 0.02 .2, 1., 2.,
	Rc << sigma_compass * sigma_compass;

	// Sateite
	Rs << sigma_range * sigma_range;

	// NOISE
	std::random_device rd; // FOR SEED
	printf("%d \n", rd());
	std::mt19937 generator(rd());
	// std::default_random_engine generator;

	std::normal_distribution<double> d_noise(0., sigma_range * sigma_range);
	std::normal_distribution<double> b_noise(0., sigma_bear * sigma_bear);

	std::normal_distribution<double> c_noise(0., sigma_compass * sigma_compass);

	std::normal_distribution<double> v_noise(0., sigma_vel * sigma_vel);
	std::normal_distribution<double> s_noise(0., sigma_steer * sigma_steer);

	// INITIAL

	px = 2.;
	py = 6.;
	theta = .3; // Initial state
	steer = .01;
	speed = 2.; // Control input

	// Initial Estimate
	// X<<px,py-.1,theta-.2;
	X << px, py, theta;
	// INITIAL
	P << .1, 0, 0, 0, .1, 0, 0, 0, .1; // Co-Varianze

	JacobiF(JF, X);
	JacobiV(JV, X);

	s_file = fopen("ROB", "w");
	l_file = fopen("LOC", "w");
}

void Robot::Move(void)
{

// Update Robot, this is the simulation of the robot physics, it has noise ...
// Note, if the steering noise is too large then the constant speed model
// can not keep up ... we would have to model the speed too,
#ifdef NOISE
	const double noisy_speed = speed + .2 * v_noise(generator);
	const double noisy_steer = steer + .1 * s_noise(generator);
#else
	const double noisy_speed = speed;
	const double noisy_steer = steer;
#endif
	px = px + dt * noisy_speed * cos(theta);
	py = py + dt * noisy_speed * sin(theta);
	theta = theta + dt * noisy_steer;

	// Update Estimate
	X(0) = X(0) + dt * speed * cos(X(2));
	X(1) = X(1) + dt * speed * sin(X(2));
	X(2) = X(2) + dt * steer;

	// Update Covariance
	JacobiF(JF, X);
	JacobiV(JV, X);

	P = JF * P * JF.transpose() + JV * M * JV.transpose();
}

void Robot::Print(void)
{
	// x,y,theta,  sigma x, sigma y, E_x, E_y, E_theta
	// TRUE POSITION
	fprintf(s_file, "%f\t%f\t%f\t%f\t%f\t%f\n", px, py, theta, X(0), X(1), X(2));
	//  ESTIMATES
}

void Robot::PrintVariance(void)
{
	// x,y,theta,  sigma x, sigma y, E_x, E_y, E_theta
	// TRUE POSITION
	fprintf(l_file, "%f\t%f\t%f\t%f\t%f\n", X(0), X(1), 180. * X(2) / M_PI - 00, P(0, 0), P(1, 1));

	//  ESTIMATES
}

// Update heading based on Compass
// simple h(x,y,theta)= (0 0 1)
void Robot::LocalizeC(void)
{

	Eigen::MatrixXd K(1, 1);
	Eigen::MatrixXd I(3, 3);
	I.setIdentity();
	Eigen::MatrixXd C(1, 3);
	Eigen::MatrixXd S(1, 1);
	Eigen::MatrixXd error(1, 1);

	error << normal_angle(theta - X(2));
// error<< ( theta- X(2));
#ifdef NOISE
	error(0) += .01 * c_noise(generator);
#endif
	std::cout << "COMPAS ERROR " << error << std::endl;

	C << 0, 0, 1;

	S = C * P * C.transpose() + Rc;
	K = P * C.transpose() * S.inverse();
	X = X + K * error;
	P = (I - K * C) * P;
}

// HERE IS YOURS
// LOCALIZE ON SATELITES, DISTANCE ONLY
void Robot::LocalizeS(void)
{
	int i;
	// DEFINE ALL THE MATRIX STUFF OUT NEED, its distance only
	Eigen::MatrixXd S;
	Eigen::MatrixXd K;
	Eigen::MatrixXd I;

	Eigen::MatrixXd error;

	S = Eigen::MatrixXd(3, 3);
	K = Eigen::MatrixXd(3, 1);
	I = Eigen::MatrixXd(3, 3);
	I.setIdentity();
	error = Eigen::MatrixXd(1, 1);
	Y = Eigen::VectorXd(1);

	// For all satelites
	for (i = 0; i < num_satellites; i++)
	{
		// OG
		// Observed distance
		// double o_dist = satellites[i]->getDistance(X(0), X(1));
		// // ADD NOISE !!!
		// o_dist += d_noise(generator);

		// Predict distance
		double p_dist = satellites[i]->getDistance(X(0), X(1)); // this is h(x)

		// observation
		double o_dist = satellites[i]->getDistance(px, py); // this is y
		// ADD NOISE !!!
		o_dist += .2 * d_noise(generator);
		Y << o_dist;

		error << Y(0) - p_dist;

		std::cout << "ERROR " << error.format(fmt) << std::endl;

		JacobiHD(JHD, X, satellites[i]->getX(), satellites[i]->getY());

		S = JHD * P * JHD.transpose() + Rs;
		K = P * JHD.transpose() * S.inverse();

		X = X + K * error;

		P = (I - K * JHD) * P;

		// std::cout << "K=" << K.format(fmt) << std::endl;

		printf("Marker %d Dist %f \n", i, o_dist);
	}
}

// THIS IS THE FULL MARKER LCOALIZE, Distance and  Angle
void Robot::LocalizeM(void)
{
	int i;
	double dist;
	double angl;
	Eigen::MatrixXd S;
	Eigen::MatrixXd K;
	Eigen::MatrixXd I;

	Eigen::MatrixXd error;

	S = Eigen::MatrixXd(3, 3);
	K = Eigen::MatrixXd(2, 2);
	I = Eigen::MatrixXd(3, 3);
	I.setIdentity();
	error = Eigen::MatrixXd(2, 1);

	// For all  markers
	for (i = 0; i < num_makers; i++)
	{
		// Compute ture  Observation with  Error
		double dx = markers[i].px - px; // Locators use real lcoation for mesurements
		double dy = markers[i].py - py;
		dist = sqrt(dx * dx + dy * dy);
		angl = normal_angle(atan2(dy, dx) - theta);

// Observation noise
#ifdef NOISE
		dist += .2 * d_noise(generator);
#endif
		error << dist, angl;

		// Also Compute kalman robot  estimate, that is h(x)
		dx = markers[i].px - X(0);
		dy = markers[i].py - X(1);
		angl = normal_angle(atan2(dy, dx) - X(2));
		Y << sqrt(dx * dx + dy * dy), angl;

		error = error - Y;
		// WE NEED TO NORMALIZE THE ANGLE
		error(1) = normal_angle(error(1));

		std::cout << "ERROR " << error.format(fmt) << " ANGLE:: " << angl << std::endl;

		JacobiH(JH, X, markers[i].px, markers[i].py);
		S = JH * P * JH.transpose() + R;
		K = P * JH.transpose() * S.inverse();

		X = X + K * error;

		P = (I - K * JH) * P;

		// std::cout << "K=" << K.format(fmt) << std::endl;

		printf("Marker %d Dist %f  Angle %f \n", i, dist, angl * 180. / M_PI);
	}
}

//-------------------------------------------------------
// SETUP
void init_setup(void)
{

	// FIXED MARKERS WE CAN GET DISTANCE AND AGNLE
	num_makers = 0;
	//	add_marker(5.,10.);
	//	add_marker(15.,15.);
	// add_marker(10.,5.);
	// add_marker(20.,5.);

	// add_marker(20.,12.);
	print_makers();

	// SATELITES, WE CAN ONLY GET DISTANCE
	// add_satelite(0,10,0); // Moving away

	//	add_satelite(20,5,-M_PI); // Moveing towards up
	add_satelite(5, -10, M_PI * .5); // Moveing acrossy

	// add_satelite(3,10,M_PI*-.5); // Moveing acrossy
}

//-------------------------------------------------------

#define NUM_ITER 200

int main(void)
{

	init_setup();

	FILE *sat_file = fopen("SAT", "w"); // satelite info

	int i;
	double dt = DT;
	double time = 0;
	Robot rob = Robot(dt);

	while (time < 10.)
	{
		move_satellites();

		rob.Move();

		// Determine if localizing frame
		if (i % 10 == 0)
		{
			print_satellites(sat_file);
			printf("Localize \n");
			// rob.LocalizeM(); // MARKERS
			rob.LocalizeS(); // That's the one you do ..
			// rob.LocalizeC(); // COMPASS
			rob.PrintVariance();
		}
		rob.Print();
		time += dt;
		i++;
	}
}
