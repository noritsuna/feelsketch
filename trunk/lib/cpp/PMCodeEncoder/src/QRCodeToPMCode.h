#ifndef     __QR_CODE_TO_PM_CODE_H__
#define     __QR_CODE_TO_PM_CODE_H__

#include "define.h"

// ��Ƃm�� B-007

// ------------------------------------------------------------------------- //
// QR�R�[�h���o�l�R�[�h�ϊ��N���X											 //
// ------------------------------------------------------------------------- //
class CQRCodeToPMCode
{
// �R���X�g���N�V����
public:
	CQRCodeToPMCode ();
	~CQRCodeToPMCode ();

	int QRCodeToPMCode (char *szQRCodeImage, UINT uiQRCodeImageSize, long lColorCode
					, char *szPMCodeImage, UINT uiPMCodeImageSize, int uiSymbolSize);		// �p�q�R�[�h���o�l�R�[�h�ϊ�
};
#endif
