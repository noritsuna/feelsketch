#ifndef     __QR_CODE_ENCODE_H__
#define     __QR_CODE_ENCODE_H__

#include "define.h"

// ------------------------------------------------------------------------- //
// QR�R�[�h�����N���X														 //
// ------------------------------------------------------------------------- //
class CQRCodeEncode
{
// �R���X�g���N�V����
public:
	CQRCodeEncode ();
	~CQRCodeEncode ();

	int		SetQRCodeData (char * szData, UINT iDataSize, int iVersion
						, int iRSLevel, int iMaskPattern);					// QR�R�[�h���̃Z�b�g
	UINT	EncodeQRCode ();												// QR�R�[�h�̃G���R�[�h
	int		GetQRCodeImage (char *szImage, UINT iBufferSize);				// QR�R�[�h�摜�̎擾

private:
	void	SetFunctionModule ();											// �@�\���W���[���̐ݒ�
	void	SetFinderPattern(int x, int y);									// �ʒu���o�p�^�[��
	void	SetVersionPattern();											// �^�ԏ��̐ݒ�
	void	SetAlignmentPattern(int x, int y);								// �ʒu���킹�p�^�[���z�u
	int		CountPenalty();													// ���_���̎擾

private:
	char	*m_QRCodeImage;													// �p�q�R�[�h�摜
	UINT	m_uiQRCodeImageSize;											// �p�q�R�[�h�摜�T�C�Y
	char	*m_szEncodeData;												// �G���R�[�h�f�[�^
	UINT	m_uiEncodeDataSize;												// �G���R�[�h�f�[�^�T�C�Y
	int		m_iVersion;														// �^��
	int		m_iRSLevel;														// ���������x��
	int		m_iMaskPattern;													// �}�X�N�p�^�[��
	int		m_iSymbolSize;													// �V���{���T�C�Y
};
#endif
