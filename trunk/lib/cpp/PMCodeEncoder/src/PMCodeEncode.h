#ifndef     __PM_CODE_ENCODE_H__
#define     __PM_CODE_ENCODE_H__

#include "define.h"
#include "QRCodeToPMCode.h"
#include "PMCodeMakeHeader.h"
#include "ImageUtil.h"

// ------------------------------------------------------------------------- //
// PM�R�[�h���[�_�@�\���N���X												 //
// ------------------------------------------------------------------------- //
class CPMCodeEncode : public CQRCodeToPMCode, CPMCodeMakeHeader
{
// �R���X�g���N�V����
public:
	CPMCodeEncode ();
	~CPMCodeEncode ();

	int		SetPMCodeData (char *szData, UINT uiDataSize, int iLayer, int iVersion, int iRSLevel, BOOL bMaskAuto, int iModuleSize);
																			// �Í������f�[�^�̃Z�b�g
	int 	SetPMCodeData (char *data, UINT data_size, int layer
						  , int version, int error_correction, BOOL use_mask, int module_size
						  , char *extension, char *note);
	int		EncodePMCode ();												// �Í�������
																			// �o�l�R�[�h�摜�̎擾
	int		GetPMCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight);
																			// �f�[�^���o�l�R�[�h�Ɏ��܂邩�̃`�F�b�N
	BOOL	CheckDataSizeToPMCode (UINT uiDataSize, int iLayer, int iVersion, int iRSLevel);

	// �p�q�R�[�h�摜�擾
	UINT	GetQRCodeSize (int iLayer);										// QR�R�[�h�摜�T�C�Y�̎擾
	int		GetQRCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer);

private:
	char		*m_szEncodeData;											// �Í����i���j�f�[�^
	UINT		m_uiDataSize;												// �f�[�^�T�C�Y
	char		*m_szPMCodeImage;											// �o�l�R�[�h�摜
	UINT		m_uiPMCodeImageSize;										// �o�l�R�[�h�摜�T�C�Y
	char		*m_szQRCodeImage [MAX_LAYERSIZE];							// �p�q�R�[�h�摜
	UINT		m_uiQRCodeImageSize [MAX_LAYERSIZE];						// �p�q�R�[�h�摜�T�C�Y
	int			m_iWidth;													// �摜���s�N�Z���T�C�Y
	int			m_iHeight;													// �摜�c�s�N�Z���T�C�Y
	int			m_iLayer;													// �w��
	int			m_iVersion;													// �^��
	int			m_iSymbolSize;												// �V���{���T�C�Y
	int			m_iRSLevel;													// ���������x��
	BOOL		m_bMaskAuto;												// �}�X�N�̎���/�蓮�t���O
	int			m_iModuleSize;												// ���W���[���T�C�Y
};
#endif
