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
