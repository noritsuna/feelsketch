#ifndef     __QR_CODE_SYMBOL_H__
#define     __QR_CODE_SYMBOL_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QR�R�[�h�V���{���Ǎ��N���X												 //
// �^���̓Ǎ��i�^�Ԃ͊��ɕ������Ă���̂ŏ������Ȃ��j						 //
// ------------------------------------------------------------------------- //
class CQRCodeSymbol
{
// �R���X�g���N�V����
public:
	CQRCodeSymbol ();
	~CQRCodeSymbol ();

	int GetQRCodeSymbol (char * szImage, UINT iImageSize, int iSymbolSize
						  , int *iRSLevel, int *iMaskPattern);				// �V���{���i�^���j�̎擾

	int SetQRCodeSymbol (int iSymbolSize, int iRSLevel, int iMaskPattern);	// �V���{���i�^���j�̐ݒ�

private:
	int	m_iRSLevel;												// ���������x��
	int	m_iMaskPattern;											// �}�X�N�p�^�[��

};
#endif
