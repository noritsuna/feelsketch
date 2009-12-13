#ifndef MATHMETICS_H
#define	MATHMETICS_H

#include <math.h>
#include <limits.h>
#include <stdint.h>
#include "define.h"

	int		toMax (double *dNumerical, int iSize);
	int		toMin (double *dNumerical, int iSize);
	long	Round (double dNumerical);
	void	ClocCoordinatesToLine (int iX1, int iY1, int iX2, int iY2, double *dy, double *dax, double *db);
	double	ClocCoordinateAndLineToInterval (int iX, int iY, double dy, double dax, double db);

#endif
