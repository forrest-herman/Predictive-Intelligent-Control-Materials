// MvM 4Ax3, iterative (gradient based) LLS
#include  <stdio.h>
#include <random>
#include <math.h>



std::default_random_engine generator;
std::normal_distribution<double> distribution(0.,.5);



//#define NUM_POINTS 50
#define NUM_POINTS 100

double x[NUM_POINTS];
double y[NUM_POINTS];
double a,b;


double func(double x)
{
	return(2.*x+2.);
}


void make_data(int num) 
{
	int i;
	for(i=0;i<num;i++){
		x[i]=(double)i/(double)num;
		y[i]=func(x[i])+distribution(generator);
	}
}


double iterate(void)
{
	int i;

	double sq_error=0;;
	double da=0;
	double db=0;

	for(i=0;i<NUM_POINTS;i++){
		double er=a*x[i]+ b -y[i];
		sq_error+=er*er;
		da+= er*x[i];
		db+= er*1;
	}
	printf("ERROR is %.2f da %f  fb %f  \n",sq_error,da,db);
	double rate=0.001;
	a=a-rate * da;
	b=b-rate * db;

	return(sq_error);
}


void print_data(void)
{
	FILE* of=fopen("DATA","w");
	int i;
	for(i=0;i<NUM_POINTS;i++){	
		fprintf(of,"%f\t%f\t%f\n",x[i],y[i],a*x[i]+b);
	}
	fclose(of);

}



int main()
{
	int i=0;
	double error=1;
	double old_error=0;
	
	a=1;
	b=1;
	make_data(NUM_POINTS);
   // NOTE, we stop if we can not progress anymore, NOT if error is 0!
	while(fabs(error-old_error)>.1){
		i++;
		old_error=error;
		printf("iter %d ",i);
		error=iterate();
	}
	printf("a=%f b=%f \n",a,b);
	print_data();
	return(0);
}
