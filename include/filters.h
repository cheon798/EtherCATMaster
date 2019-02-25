#ifndef _FILTERS_H_
#define _FILTERS_H_

#include "emp_hdr.h"

double LPF(const double x, double wc, const double ts);
double HPF(const double x, double wc, const double ts);



#endif /*_FILTERS_H_*/
