/**
***                  "Feel Sketch" PMCode Encoder & Decoder.
***    Copyright (C) 2009, Content Idea of ASIA Co.,Ltd. (oss.pmcode@ci-a.com)
***
***    This program is free software: you can redistribute it and/or modify
***    it under the terms of the GNU General Public License as published by
***    the Free Software Foundation, either version 3 of the License, or
***    (at your option) any later version.
***
***    This program is distributed in the hope that it will be useful,
***    but WITHOUT ANY WARRANTY; without even the implied warranty of
***    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
***    GNU General Public License for more details.
***
***    You should have received a copy of the GNU General Public License
***    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mathematics.h"

int	toMax (double *dNumerical, int iSize) {

	int		iNum = 0;
	double	dTemp = 0;
	for (int i = 0; i < iSize; i ++) {
		if (dTemp < dNumerical [i]) {
			dTemp = dNumerical [i];
			iNum = i;
		}
	}
	return iNum;
}

int	toMin (double *dNumerical, int iSize) {

	int		iNum = 0;
	double	dTemp = INT32_MAX;
	for (int i = 0; i < iSize; i ++) {
		if (dTemp > dNumerical [i]) {
			dTemp = dNumerical [i];
			iNum = i;
		}
	}
	return iNum;
}

long Round (double dNumerical) {

	if(dNumerical != 0) {
		if(dNumerical >= 0) {
			dNumerical = dNumerical += 0.5;
			return (long)floor(dNumerical);
		} else if(dNumerical <= 0) {
			dNumerical = dNumerical -= 0.5;
			return (long)floor(dNumerical);
		}
	}
	return 0;
}

void ClocCoordinatesToLine (int iX1, int iY1, int iX2, int iY2, double *dy, double *dax, double *db) {

	if (iX1 == iX2) {
		*dy		= 0;
		*dax	= 1;
		*db		= iX1 * -1;
	} else if (iY1 == iY2) {
		*dy		= 1;
		*dax	= 0;
		*db		= iY1;
	} else {
		*dy		= 1;
		*dax	= (double)(iY2 - iY1) / (double)(iX2 - iX1);
		*db		= iY1 - (*dax * iX1);
	}
}

double ClocCoordinateAndLineToInterval (int iX, int iY, double dy, double dax, double db) {

	if (dy == 0) {
		return iX - db;	
	} else {
		return fabs (iY - (dax * iX) - db) / sqrt((double)(1 + (dax * dax)));
	}
}

