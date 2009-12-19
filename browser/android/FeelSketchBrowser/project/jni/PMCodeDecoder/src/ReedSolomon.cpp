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

#include "ReedSolomon.h"

CReedSolomon::CReedSolomon () {
	m_iCorrectCodeSize = 0;
}

CReedSolomon::~CReedSolomon () {

}

BOOL CReedSolomon::SetCorrectCodeSize (int iCorrectCodeSize) {

	if (iCorrectCodeSize > MAX_LENGTH) {
		return FALSE;
	}
	m_iCorrectCodeSize = iCorrectCodeSize;
    compute_genpoly(m_iCorrectCodeSize, m_genPoly);
	return TRUE;
}

BOOL CReedSolomon::Encode (unsigned char * szData, int iDataSize, unsigned char * szRsCode, int *iRsCodeSize) {

	int iLFSR [MAX_LENGTH + 1];
	int	iByte;

	if (m_iCorrectCodeSize == 0 || iDataSize > MAX_LENGTH) {
		return FALSE;
	}
	for(int i = 0; i < MAX_LENGTH + 1; i ++) {
		iLFSR [i] = 0;
	}
	for (int i = 0; i < iDataSize; i ++) {
		iByte = szData [i] ^ iLFSR [m_iCorrectCodeSize - 1];
		for (int j = m_iCorrectCodeSize - 1; j > 0; j --) {
			iLFSR [j] = iLFSR [j - 1] ^ gmult (m_genPoly [j], iByte);
		}
		iLFSR [0] = gmult (m_genPoly [0], iByte);
	}

	for (int i = 0; i < m_iCorrectCodeSize; i ++) {
		m_Bytes [i] = iLFSR [i];
	}

	for (int i = 0; i < iDataSize; i ++) {
		szRsCode [i] = szData [i];
	}
	for (int i = 0; i < m_iCorrectCodeSize; i ++) {
		szRsCode[i + iDataSize] = m_Bytes [m_iCorrectCodeSize - 1 - i];
	}
	*iRsCodeSize = iDataSize + m_iCorrectCodeSize;
	return TRUE;
}

BOOL CReedSolomon::Decode (unsigned char * szData, int iDataSize, unsigned char * szCorrectData, int *iCorrectDataSize) {

	int	sum;
	int	nz = 0;
	int erasures[256];
	int nerasures = 0;
	int iRet;

	for (int i = 0; i < MAX_LENGTH; i ++) {
		m_synBytes [i] = 0;
	}
	for (int j = 0; j < m_iCorrectCodeSize; j ++) {
		sum	= 0;
		for (int i = 0; i < iDataSize; i ++) {
			sum = szData [i] ^ gmult (byExpToInt [j + 1], sum);
		}
		m_synBytes [j]  = sum;
	}
	for (int i = 0; i < m_iCorrectCodeSize; i ++) {
		if (m_synBytes [i] != 0) {
			nz = 1;
		}
	}
	if (nz == 0) {
		memcpy (szCorrectData, szData, iDataSize);
		*iCorrectDataSize = iDataSize - m_iCorrectCodeSize;
		return TRUE;
	}

    iRet = correct_errors_erasures (szData, iDataSize, nerasures, erasures);
	if (iRet == 0) {
		return FALSE;
	}
	memcpy (szCorrectData, szData, iDataSize - m_iCorrectCodeSize);
	*iCorrectDataSize = iDataSize - m_iCorrectCodeSize;
	return TRUE;
}

int CReedSolomon::correct_errors_erasures (unsigned char codeword [], int csize, int nerasures, int erasures []) {

	int i, err;

  /* If you want to take advantage of erasure correction, be sure to
     set m_NErasures and m_ErasureLocs[] with the locations of erasures. 
     */
	m_NErasures = nerasures;
	for (i = 0; i < m_NErasures; i ++) {
		m_ErasureLocs [i] = erasures [i];
	}
	Modified_Berlekamp_Massey ();
	Find_Roots ();

	if ((m_NErrors <= m_iCorrectCodeSize) && m_NErrors > 0) { 

		/* first check for illegal error locs */
		for (int r = 0; r < m_NErrors; r ++) {
			if (m_ErrorLocs [r] >= csize) {
				return(0);
			}
		}

		for (int r = 0; r < m_NErrors; r ++) {
			int num, denom;
			i = m_ErrorLocs [r];
			/* evaluate m_Omega at alpha^(-i) */

			num = 0;
			for (int j = 0; j < MAX_LENGTH; j ++) {
				num ^= gmult (m_Omega [j], byExpToInt [((255 - i) * j) % 255]);
			}
			/* evaluate m_Lambda' (derivative) at alpha^(-i) ; all odd powers disappear */
			denom = 0;
			for (int j = 1; j < MAX_LENGTH; j += 2) {
				denom ^= gmult (m_Lambda [j], byExpToInt [((255 - i) * (j - 1)) % 255]);
			}
			err = gmult (num, ginv (denom));
			codeword [csize - i - 1] ^= err;
	    }
		return 1;
	} else {
		return 0;
	}

}

int CReedSolomon::gmult (int a, int b) {
	int i, j, p;
	if (a == 0 || b == 0) {
		return 0;
	}
	i = byIntToExp [a];
	j = byIntToExp [b];
	if (i + j >= 256){
		p = i + j - 255;
	} else {
		p = i + j;
	}
	return (byExpToInt [p]);
}

int CReedSolomon::ginv (int elt) { 
	return (byExpToInt [255 - byIntToExp [elt]]);
}

void CReedSolomon::compute_genpoly (int nbytes, int genpoly []) {

	int tp [MAX_LENGTH];
	int tp1 [MAX_LENGTH];

	/* multiply (x + a^n) for n = 1 to nbytes */
	for (int i = 0; i < MAX_LENGTH; i ++) {
		tp1 [i] = 0;
	}
	tp1 [0] = 1;

	for (int i = 1; i <= nbytes; i++) {
		for (int j = 0; j < MAX_LENGTH; j ++) {
			tp [j] = 0;
		}
		tp [0] = byExpToInt [i];		/* set up x+a^n */
		tp [1] = 1;
		mult_polys (genpoly, tp, tp1);
		for (int j = 0; j < MAX_LENGTH; j ++) {
			tp1 [j] = genpoly [j];
		}
	}
}

/* polynomial multiplication */
void CReedSolomon::mult_polys (int dst [], int p1 [], int p2 []) {

	int tmp1 [MAX_LENGTH * 2];
	int iMaxLength = m_iCorrectCodeSize * 2;

	for (int i=0; i < (MAX_LENGTH * 2); i ++) {
		dst [i] = 0;
	}
	
	for (int i = 0; i < iMaxLength; i ++) {
		for (int j = iMaxLength; j < (MAX_LENGTH * 2); j ++) {
			tmp1 [j] = 0;
		}
		
		/* scale tmp1 by p1[i] */
		for (int j = 0; j < iMaxLength; j ++) {
			tmp1 [j] = gmult (p2 [j], p1 [i]);
		}
		/* and mult (shift) tmp1 right by i */
		for (int j = (iMaxLength * 2) - 1; j >= i; j --) {
			tmp1 [j] = tmp1 [j - i];
		}
		for (int j = 0; j < i; j ++) {
			tmp1 [j] = 0;
		}
			
		/* add into partial product */
		for (int j = 0; j < (iMaxLength * 2); j ++) {
			dst [j] ^= tmp1 [j];
		}
	}
}

void CReedSolomon::Modified_Berlekamp_Massey (void) {	

	int L, L2, k, d;
	int psi [MAX_LENGTH], psi2 [MAX_LENGTH], D [MAX_LENGTH];
	int gamma [MAX_LENGTH];
	int iMaxLength = m_iCorrectCodeSize * 2;
	
	/* initialize Gamma, the erasure locator polynomial */
	init_gamma (gamma);

	/* initialize to z */
	copy_poly (D, gamma);
	mul_z_poly (D);
	
	copy_poly (psi, gamma);	
	k = -1;
	L = m_NErasures;
	
	for (int n = m_NErasures; n < m_iCorrectCodeSize; n ++) {
		d = compute_discrepancy (psi, m_synBytes, L, n);
		
		if (d != 0) {
			/* psi2 = psi - d*D */
			for (int i = 0; i < iMaxLength; i++) {
				psi2 [i] = psi [i] ^ gmult (d, D [i]);
			}
			if (L < (n - k)) {
				L2 = n - k;
				k = n - L;
				/* D = scale_poly(ginv(d), psi); */
				for (int i = 0; i < iMaxLength; i ++) {
					D [i] = gmult (psi [i], ginv (d));
				}
				L = L2;
			}
			/* psi = psi2 */
			for (int i = 0; i < iMaxLength; i ++) {
				psi [i] = psi2 [i];
			}
		}
		mul_z_poly (D);
	}
	
	for (int i = 0; i < MAX_LENGTH; i ++) {
		m_Lambda [i] = psi [i];
	}
	compute_modified_omega ();
}

void CReedSolomon::Find_Roots (void) {

	int sum;	
	m_NErrors = 0;
  
	for (int r = 1; r < 256; r ++) {
		sum = 0;
		/* evaluate lambda at r */
		for (int k = 0; k < m_iCorrectCodeSize + 1; k ++) {
			sum ^= gmult (byExpToInt [(k * r) % 255], m_Lambda [k]);
		}
		if (sum == 0) {
			m_ErrorLocs [m_NErrors] = (255 - r);
			m_NErrors ++; 
		}
	}
}

/* gamma = product (1-z*a^Ij) for erasure locs Ij */
void CReedSolomon::init_gamma (int gamma []) {

	int tmp [MAX_LENGTH];
	
	for (int i = 0; i < MAX_LENGTH; i ++) {
		gamma [i]	= 0;
		tmp [i]		= 0;
	}
	gamma[0] = 1;
	for (int e = 0; e < m_NErasures; e ++) {
		copy_poly (tmp, gamma);
		scale_poly (byExpToInt [m_ErasureLocs [e]], tmp);
		mul_z_poly (tmp);
		add_polys (gamma, tmp);
	}
}

void CReedSolomon::copy_poly (int dst [], int src []) {

	for (int i = 0; i < MAX_LENGTH; i ++) {
		dst [i] = src [i];
	}
}

void CReedSolomon::mul_z_poly (int src[]) {

	for (int i = (m_iCorrectCodeSize * 2) - 1; i > 0; i --) {
		src [i] = src [i - 1];
	}
	src [0] = 0;
}

int CReedSolomon::compute_discrepancy (int lambda [], int S [], int L, int n) {

	int sum = 0;

	for (int i = 0; i <= L; i ++) {
		sum ^= gmult (lambda [i], S [n - i]);
	}
	return (sum);
}

void CReedSolomon::compute_modified_omega () {

	int product [MAX_LENGTH * 2];
	
	mult_polys (product, m_Lambda, m_synBytes);	
	for (int i = 0; i < MAX_LENGTH; i ++) {
		m_Omega [i] = 0;
	}
	for(int i = 0; i < m_iCorrectCodeSize; i ++) {
		m_Omega [i] = product [i];
	}
}

void CReedSolomon::scale_poly (int k, int poly []) {	

	for (int i = 0; i < MAX_LENGTH; i ++) {
		poly [i] = gmult (k, poly [i]);
	}
}

void CReedSolomon::add_polys (int dst[], int src []) {

	for (int i = 0; i < MAX_LENGTH; i ++) {
		dst [i] ^= src [i];
	}
}
