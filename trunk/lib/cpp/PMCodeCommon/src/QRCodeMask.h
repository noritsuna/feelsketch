#ifndef     __QR_CODE_MASK_H__
#define     __QR_CODE_MASK_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QR�R�[�h�}�X�N�N���X														 //
// ------------------------------------------------------------------------- //
class CQRCodeMask
{
// �R���X�g���N�V����
public:
	CQRCodeMask ();
	~CQRCodeMask ();

	void GetMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern
												, int iVersion);			// �}�X�N��������
	void SetMaskQRCode (int iMaskPattern, int iVersion);					// �}�X�N����
};
#endif
