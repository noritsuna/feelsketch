#include "mathematics.h"

// ------------------------------------------------------------------------- //
// 機能概要			:最大値の取得											 //
// 引数				:dNumerical			:数値（配列）						 //
//					:iSize				:配列サイズ							 //
// 戻り値			:最大値の格納されている０からの配列番号					 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
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

// ------------------------------------------------------------------------- //
// 機能概要			:最小値の取得											 //
// 引数				:dNumerical			:数値								 //
//					:iSize				:配列サイズ							 //
// 戻り値			:最小値の格納されている０からの配列番号					 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
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

// ------------------------------------------------------------------------- //
// 機能概要			:四捨五入												 //
// 引数				:dNumerical			:数値								 //
// 戻り値			:四捨五入した値											 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
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

// ------------------------------------------------------------------------- //
// 機能概要			:２点間を通る直線式の算出								 //
// 引数				:iX1				:１つ目のＸ座標						 //
//					:iY1				:１つ目のＹ座標						 //
//					:iX2				:２つ目のＸ座標						 //
//					:iY2				:２つ目のＹ座標						 //
//					:dy					:Ｙの係数							 //
//					:dax				:Ｘの係数							 //
//					:db					:ｂの係数							 //
// 戻り値			:なし													 //
// 備考				:計算後の式は dy Y = dax X + dbとなる					 //
// ------------------------------------------------------------------------- //
void ClocCoordinatesToLine (int iX1, int iY1, int iX2, int iY2, double *dy, double *dax, double *db) {

	if (iX1 == iX2) {														// Ｘ座標が同値
		// X = iX1となるためYの係数を０とし、bの係数を移項するため、−１倍する
		*dy		= 0;
		*dax	= 1;
		*db		= iX1 * -1;
	} else if (iY1 == iY2) {												// Ｙ座標が同値
		// Y = iY1となるため、Xの係数を０とする
		*dy		= 1;
		*dax	= 0;
		*db		= iY1;
	} else {																// y = ax + b
		// aの係数(Y2 - Y1) / (X2 - X1)
		// aの係数を算出したら、座標を代入してbの値を算出する
		*dy		= 1;
		*dax	= (double)(iY2 - iY1) / (double)(iX2 - iX1);
		*db		= iY1 - (*dax * iX1);
	}
}

// ------------------------------------------------------------------------- //
// 機能概要			:直線と座標の距離の算出									 //
// 引数				:iX					:Ｘ座標								 //
//					:iY					:Ｙ座標								 //
//					:dy					:Ｙの係数							 //
//					:dax				:Ｘの係数							 //
//					:db					:ｂの係数							 //
// 戻り値			:直線と座標の距離										 //
// 備考				:														 //
// ------------------------------------------------------------------------- //
double ClocCoordinateAndLineToInterval (int iX, int iY, double dy, double dax, double db) {

	if (dy == 0) {															// X = *の式の場合
		// iX - bが距離となる
		return iX - db;	
	} else {																// 通常の式
		// d = |y - ax - b| / √（1 + mの２乗）
		return fabs (iY - (dax * iX) - db) / sqrt((double)(1 + (dax * dax)));
	}
}

