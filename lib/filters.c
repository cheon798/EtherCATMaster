#include "filters.h"

double
LPF(const double x, double wc, const double ts)
{
	double x_1 = 0.0;
	double y_1 = 0.0;
	double y   = 0.0;

	double recur = PI * ts * wc; 
	double term[2];

	term[0] = recur / (1 + recur);
	term[1] = (1 - recur) / (1 + recur);

	y = (term[0] * (x + x_1)) + (term[1] * y_1);

	x_1 = x;
	y_1 = y;

	return y;
}

double
HPF(const double x, double wc, const double ts)
{
	double x_1 = 0.0;
	double y_1 = 0.0;
	double y   = 0.0;

	double recur = PI * ts * wc;
	double term[2];

	term[0] = 1 / (1 + recur);
	term[1] = (1 - recur) / (1 + recur);

	y = (term[0] * (x - x_1)) + (term[1] * y_1);

	x_1 = x;
	y_1 = y;

	return y;
}
