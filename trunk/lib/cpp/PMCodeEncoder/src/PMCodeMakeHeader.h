#ifndef     __PM_CODE_MAKE_HEADER_H__
#define     __PM_CODE_MAKE_HEADER_H__

#define LAYER_CODE_BUF_SIZE				12

#define LAYER_BIT						4
#define LAYER_CODE_POS_X				5
#define COLOR_CODE_POS_X				4

#include "define.h"

// ------------------------------------------------------------------------- //
// �o�l�R�[�h��̓N���X														 //
// ------------------------------------------------------------------------- //
class CPMCodeMakeHeader
{
// �R���X�g���N�V����
public:
	CPMCodeMakeHeader ();
	~CPMCodeMakeHeader ();
																				// �o�l�R�[�h�w�b�_�̃Z�b�g
	int SetPMCodeHeader (char *szImage, UINT iImageSize, int iSymbolSize, int iLayer);			

private:
	int			m_iLayer;													// �w��
	int			m_iVersion;													// �^��
	int			m_iSymbolSize;												// �V���{���T�C�Y
	int			m_iRSLevel;													// ���������x��

};
#endif