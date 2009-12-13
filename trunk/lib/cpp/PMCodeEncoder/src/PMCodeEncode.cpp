#include "define.h"
#include "Global.h"
#include "QRCodeEncode.h"
#include "PMCodeEncode.h"
//#include "CtrlBMP.h"
#include "ReedSolomon.h"

#include <stdio.h>

#define MARGIN	4


// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:����������												 //
// ------------------------------------------------------------------------- //
CPMCodeEncode::CPMCodeEncode () {

	m_szEncodeData		= NULL;
	m_uiDataSize		= 0;
	m_szPMCodeImage		= NULL;
	m_uiPMCodeImageSize	= 0;
	for (int i =0; i < MAX_LAYERSIZE; i ++){
		m_szQRCodeImage	[i]	= NULL;
		m_uiQRCodeImageSize	[i] = 0;
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:�m�ۂ����܂܂ɂȂ�Ȃ��悤�������̊J�����s��			 //
// ------------------------------------------------------------------------- //
CPMCodeEncode::~CPMCodeEncode () {

	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData	= NULL;
		m_uiDataSize	= 0;
	}

	if (m_szPMCodeImage != NULL) {
		free (m_szPMCodeImage);
		m_szPMCodeImage	= NULL;
		m_uiDataSize	= 0;
	}

	for (int i = 0; i < MAX_LAYERSIZE; i ++){
		if (m_szQRCodeImage != NULL) {
			free (m_szQRCodeImage [i]);
			m_szQRCodeImage	[i]		= NULL;
			m_uiQRCodeImageSize	[i] = 0;
		}
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�Í������f�[�^�̃Z�b�g									 //
// ����				:szData				:���̓f�[�^�i�Í������j				 //
//                  :uiDataSize			:���̓f�[�^�T�C�Y					 //
//                  :iLayer				:�w��								 //
//                  :iVersion			:�^��								 //
//                  :iRSLevel			:���������x��						 //
//					:iModuleSize		:���W���[���T�C�Y					 //
// �߂�l			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeEncode::SetPMCodeData (char *szData, UINT uiDataSize, int iLayer
								  , int iVersion, int iRSLevel, BOOL bMaskAuto, int iModuleSize) {

	// ���������m�ۂ���Ă���ꍇ�́A��x�J������
	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData	= NULL;
		m_uiDataSize	= 0;
	}
// 2006/12/27 �������ɑΉ�����ׂɁA�e�[�u������ύX
	m_szEncodeData = (char *)realloc (m_szEncodeData
		, QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer);		// �f�[�^�p�������̊m��
//	m_szEncodeData = (char *)realloc (m_szEncodeData, uiDataSize);			// �f�[�^�p�������̊m��
	if (m_szEncodeData == NULL) {											// �������m�ێ��s
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szEncodeData, '\0'
		, QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer);		//�������̏�����
//	memset (m_szEncodeData, '\0', uiDataSize);								//�������̏�����
	memcpy (m_szEncodeData, szData, uiDataSize);							// �f�[�^�̃Z�b�g
	m_uiDataSize = uiDataSize;												// �f�[�^�T�C�Y�̃Z�b�g

	m_iLayer		= iLayer;												// �w��
	m_iVersion		= iVersion;												// �^��
	m_iRSLevel		= iRSLevel;												// ���������x��
	m_bMaskAuto		= bMaskAuto;											// �}�X�N�g�p���
	m_iWidth		= VER2SYM (iVersion);									// ���i�V���{���T�C�Y�Ɠ���j
	m_iHeight		= VER2SYM (iVersion);									// �����i�V���{���T�C�Y�Ɠ���j
	m_iModuleSize	= iModuleSize;											// ���W���[���T�C�Y
	return RESULT_OK;

}

int CPMCodeEncode::SetPMCodeData (char *data, UINT data_size, int layer
								  , int version, int error_correction, BOOL use_mask, int module_size
								  , char *extension, char *note) {
	char 	*szData;
	char	szHeader [32];
	memset (szHeader, '\0', sizeof (szHeader));
	char	szSize [16];
	memset (szSize, '\0', sizeof (szSize));
	char	szHexSize [4];

	// �w�b�_�̍쐬�@��������
	// �g���q�̃Z�b�g�i�S�o�C�g�j
	if (extension != NULL) {
		if (strlen (extension) < EXT_SIZE) {
			memcpy (szHeader, extension, strlen (extension));
		} else {
			memcpy (szHeader, extension, EXT_SIZE);
		}
	}
	// �T�C�Y�̃Z�b�g�i�S�o�C�g�j
	sprintf (szSize, "%0.8X", data_size);
	for (int i = 0; i < 4; i ++) {
		szHexSize [i] = h2tb1 (&szSize [i * 2]);
	}
	memcpy (&szHeader[EXT_SIZE], szHexSize, SIZE_SIZE);
	// ���l�̃Z�b�g�i�W�o�C�g�j
	if (note != NULL) {
		if (strlen (note) < RESERVE_SIZE) {
			memcpy (&szHeader[EXT_SIZE + SIZE_SIZE], note, strlen (note));
		} else {
			memcpy (&szHeader[EXT_SIZE + SIZE_SIZE], note, RESERVE_SIZE);
		}
	}
	// �w�b�_�̍쐬�@�����܂�

	// �f�[�^�̍쐬
	szData = (char *)malloc (data_size + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE + 1);
	memset (szData, '\0', data_size + 1);
	// �w�b�_�[�����f�[�^�ɃZ�b�g
	memcpy (szData, szHeader, EXT_SIZE + SIZE_SIZE + RESERVE_SIZE);
	// �f�[�^���Z�b�g
	memcpy (&szData[EXT_SIZE + SIZE_SIZE + RESERVE_SIZE], data, data_size + 1);

	return SetPMCodeData(szData,
						data_size + EXT_SIZE + SIZE_SIZE + RESERVE_SIZE,
						layer,
						version,
						error_correction,
						use_mask,
						module_size);
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�[�^���o�l�R�[�h�Ɏ��܂邩�̃`�F�b�N					 //
// ����				:uiDataSize			:���̓f�[�^�T�C�Y					 //
//                  :iLayer				:�w��								 //
//                  :iVersion			:�^��								 //
//                  :iRSLevel			:���������x��						 //
// �߂�l			:TRUE:���܂� FALSE:���܂�Ȃ�							 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
BOOL CPMCodeEncode::CheckDataSizeToPMCode (UINT uiDataSize, int iLayer, int iVersion, int iRSLevel) {

	UINT	uiLayerDataSize;

	// ������������x���Ȃ��ōs��	

// 2006/12/13 �F���ʃR�[�h�ύX
// 2006/12/27 �������ɑΉ�����ׂɁA�e�[�u������ύX
	uiLayerDataSize = QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer;
/*
	if (iVersion < 3) {
//		uiLayerDataSize = QR_VersonInfo[iVersion].ncAllCodeWord * iLayer;
		uiLayerDataSize = QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] * iLayer;
	} else {
//		uiLayerDataSize = (QR_VersonInfo[iVersion].ncAllCodeWord - 6) * iLayer;
		uiLayerDataSize = (QR_VersonInfo[iVersion].ncDataCodeWord [iRSLevel] - 6) * iLayer;
	}
*/
	if (uiLayerDataSize >= uiDataSize){
		return TRUE;
	} else {
		return FALSE;
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�Í�������												 //
// ����				:�Ȃ�													 //
// �߂�l			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
int CPMCodeEncode::EncodePMCode () {

	CQRCodeEncode	cQRCodeEncode;
	CReedSolomon	cReedSolomon;
	BOOL	bRet;
	int		iRet;
	int		iWordSize;
	char	*szOneLayerData;												// �P�w���f�[�^�o�b�t�@
	char	*szTemp;
	int		iOneLayerDataMaxSize;
	int		iOneLayerDataSize;
	int		iQRCodeSize;
	long	lColorCode;
	int		iLength;
	int		iMaskPattern;
	int		iPos;
	int		iDataSize;
	int		iBlockSize;
	int		iCorrectPos = 0;
	int		iDataPos = 0;
	int		iCorrectDataSize;
	int		iRSBlock1;
	int		iRSBlock2;
	char	szWhite [4] = {'\xFF', '\xFF', '\xFF', '\x00'};
	char	szBlack [4] = {'\x00', '\x00', '\x00', '\x00'};
	BYTE	szCorrectBuf [256];
	BYTE	szWork [256];
	

	// �f�[�^�����܂邩�̃`�F�b�N�i��ʂɂčs���Ă��邪�A�O�̂��߁j
	bRet = CheckDataSizeToPMCode (m_uiDataSize, m_iLayer, m_iVersion, m_iRSLevel);
	if (bRet != TRUE) {
		return RESULT_ERROR_SIZE_TO_SETTING;
	}
	m_iSymbolSize  = VER2SYM(m_iVersion);

	// �p�q�R�[�h�o�b�t�@���m�ۂ���Ă���΁A�J�����Ă���
	for (int i = 0; i < MAX_LAYERSIZE; i ++){
		if (m_szQRCodeImage != NULL) {
			free (m_szQRCodeImage [i]);
			m_szQRCodeImage	[i]	= NULL;
			m_uiQRCodeImageSize	[i] = 0;
		}
	}

	// �P�w�f�[�^�p�o�b�t�@�̊m��
	// 2006/11/27 �F���ʃR�[�h�g��
// 2006/12/27 �������ɑΉ�����ׂɁA�e�[�u������ύX
	iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;
/*
	if (m_iVersion < 3) {
		iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord;
	} else {
		iWordSize = QR_VersonInfo[m_iVersion].ncAllCodeWord - 6;
	}
*/
	szOneLayerData = (char *)malloc(iWordSize + 1);
//	szOneLayerData = (char *)malloc(QR_VersonInfo[m_iVersion].ncAllCodeWord + 1);
	if (szOneLayerData == NULL){
		return RESULT_ERROR_SECURE_MEMORY;
	}

	// ������������x���Ȃ��ōs��
	// 2006/11/27 �F���ʃR�[�h�g��
//	iOneLayerDataMaxSize = iWordSize;
// 2006/12/27 �������ɑΉ�����ׂɁA�e�[�u������ύX
	iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel];
/*
	if (m_iVersion < 3) {
		iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel];
	} else {
		iOneLayerDataMaxSize = QR_VersonInfo[m_iVersion].ncDataCodeWord [m_iRSLevel] - 6;
	}
*/
	for (int i = 0; i < m_iLayer; i ++) {
		// 2006/11/27 �F���ʃR�[�h�g��
		memset (szOneLayerData, '\0', iWordSize + 1);
//		memset (szOneLayerData, '\0', QR_VersonInfo[m_iVersion].ncAllCodeWord + 1);

// 2006/12/26 �������ɑΉ� -----------------------------------------------------------------------------------
		// �f�[�^�̕�������
		if ((iOneLayerDataMaxSize * (i + 1)) < (int)m_uiDataSize) {
			iOneLayerDataSize = iOneLayerDataMaxSize;
		} else {
			iOneLayerDataSize = m_uiDataSize - (iOneLayerDataMaxSize * i);
		}
		if (iOneLayerDataSize < 0) {
			iOneLayerDataSize = 0;
		}
		iDataPos = 0;
		iDataSize = iOneLayerDataSize;										// �P�w���̃f�[�^�T�C�Y���Z�b�g
		iRSBlock1 = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncRSBlock;
		iRSBlock2 = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncRSBlock;

		for (int j = 0; j < (iRSBlock1 + iRSBlock2); j ++) {
//		for (int j = 0; j < iDataSize; j ++) {
			// j���u���b�N�ʒu�Ƃ��Ăq�r�u���b�N���Por�Q�̂ǂ�����g�p���邩���f����
			if (iRSBlock1 > j){
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncDataCodeWord;
				cReedSolomon.SetCorrectCodeSize (QR_VersonInfo[m_iVersion].RS_BlockInfo1 [m_iRSLevel].ncAllCodeWord	- iBlockSize);
			} else {
				iBlockSize = QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncDataCodeWord;
				cReedSolomon.SetCorrectCodeSize (QR_VersonInfo[m_iVersion].RS_BlockInfo2 [m_iRSLevel].ncAllCodeWord - iBlockSize);
			}
			memset (szWork, NULL, sizeof (szWork));
			memset (szCorrectBuf, NULL, sizeof (szCorrectBuf));

			// �e�u���b�N�ł̃G���R�[�h
			memcpy (szWork, &m_szEncodeData [iCorrectPos], iBlockSize);
			cReedSolomon.Encode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
/*
			if (iDataSize < iBlockSize) {									// �u���b�N�T�C�Y���A�f�[�^�������Ȃ��Ƃ�
				memcpy (szWork, &m_szEncodeData [iCorrectPos], iDataSize);
				cReedSolomon.Encode (szWork, iDataSize, szCorrectBuf, &iCorrectDataSize);
			} else {														// �u���b�N�T�C�Y���A�f�[�^���������Ƃ�
				memcpy (szWork, &m_szEncodeData [iCorrectPos], iBlockSize);
				cReedSolomon.Encode (szWork, iBlockSize, szCorrectBuf, &iCorrectDataSize);
			}
*/
			// �G���R�[�h�����f�[�^����ׂ�
			memcpy (&szOneLayerData [iDataPos], szCorrectBuf, iCorrectDataSize);
			iCorrectPos += iBlockSize;
			iDataPos += iCorrectDataSize;
			iDataSize -= iBlockSize;
		}

//		memcpy (szOneLayerData, &m_szEncodeData [iOneLayerDataMaxSize * i], iOneLayerDataSize);
// -------------------------------------------------------------------------------------------------------------

		// �p�q�R�[�h�̍쐬
		if (m_bMaskAuto == TRUE){
			iMaskPattern = -1;
		} else {
			iMaskPattern = g_MaskSetting [i];
		}
// 2006/10/12 tsato update ----------------------------------------------
// �T�C�Y��L���T�C�Y�ɂ���ƁA�Ō�̃f�[�^�������邽�߁A�ő�T�C�Y�Ƃ���
		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iDataPos
										, m_iVersion, m_iRSLevel, iMaskPattern);
//		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iOneLayerDataMaxSize
//										, m_iVersion, m_iRSLevel, iMaskPattern);
//		iRet = cQRCodeEncode.SetQRCodeData (szOneLayerData, iOneLayerDataSize
//										, m_iVersion, m_iRSLevel, iMaskPattern);
// ----------------------------------------------------------------------
		if (iRet != RESULT_OK) {
			free (szOneLayerData);											// �P�w���f�[�^�o�b�t�@�̊J��
			return bRet;
		}
		// �p�q�R�[�h�̍쐬
		iQRCodeSize = cQRCodeEncode.EncodeQRCode ();
		if (iQRCodeSize == 0) {
			free (szOneLayerData);											// �P�w���f�[�^�o�b�t�@�̊J��
			return RESULT_ERROR_DECODE_FAILURE;
		}
		m_uiQRCodeImageSize	[i] = iQRCodeSize;								// �p�q�R�[�h�T�C�Y�̃Z�b�g
		m_szQRCodeImage [i] = (char *)malloc (m_uiQRCodeImageSize [i] + 1);
		if (m_szQRCodeImage [i] == NULL){
			free (szOneLayerData);											// �P�w���f�[�^�o�b�t�@�̊J��
			return RESULT_ERROR_SECURE_MEMORY;
		}
		memset (m_szQRCodeImage	[i], '\0', m_uiQRCodeImageSize [i] + 1);

		// �p�q�R�[�h�摜�̎擾
		iRet = cQRCodeEncode.GetQRCodeImage (m_szQRCodeImage [i], m_uiQRCodeImageSize [i]);
		if (iRet != RESULT_OK) {
			free (szOneLayerData);											// �P�w���f�[�^�o�b�t�@�̊J��
			return bRet;
		}
	}
	free (szOneLayerData);													// �P�w���f�[�^�o�b�t�@�̊J��

	// �o�l�R�[�h�p�̃������m��
	iLength = CalcPitch (BIT_COUNT_24, m_iSymbolSize);
	m_uiPMCodeImageSize = iLength * m_iSymbolSize;
	if (m_szPMCodeImage != NULL){
		free (m_szPMCodeImage);
		m_szPMCodeImage = NULL;
	}
	m_szPMCodeImage = (char *)malloc (m_uiPMCodeImageSize + 1);				// �o�l�R�[�h�o�b�t�@�̊m��
	if (m_szPMCodeImage == NULL) {											// �������m�ێ��s
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (m_szPMCodeImage, '\0', m_uiPMCodeImageSize + 1);				

	// �p�q�R�[�h�̏d��
	for (int i = 0; i < m_iLayer; i ++) {
		if (g_ColorCodeSetting == 0) {										// ��`�t�@�C��
			lColorCode = g_ColorCodeSettingDefinition [i];	
		} else {															// �f�t�H���g�l
			if (m_iLayer == 3) {											// �w���͂R
				lColorCode = g_ColorCodeSetting3 [i];	
			} else if (m_iLayer >9) {										// �w���͂P�O�`�Q�S
				lColorCode = g_ColorCodeSetting10to24 [i];	
			} else {														// �w���͂R�`�X
				lColorCode = g_ColorCodeSetting4to9 [i];	
			}
		}
		iRet = QRCodeToPMCode (m_szQRCodeImage [i], m_uiQRCodeImageSize	[i], lColorCode
						, m_szPMCodeImage, m_uiPMCodeImageSize, m_iSymbolSize);		// �p�q�R�[�h���o�l�R�[�h�ϊ�
	}

// -----------------------------------------------------------------------------------
// �R�w�A�X�w�A�Q�S�w�ȊO�A�܂��͕K�v�ȑw�̂n�m�ƂȂ�ӏ����d�􂵂Ă�
// ���ƂȂ�Ȃ��ꍇ�ׂ̈ɁA�p�q�R�[�h����@�\�R�[�h��Ⴂ�A������x
// �F�̐ݒ���s��
// -----------------------------------------------------------------------------------
	// �t�H�[�}�b�g���L�q�ʒu���@�\���W���[������͂���
	for (int i = 0; i < 9; i ++) {
		// �@���㉡�@�@�@�@�@�@�@	�����c
		g_byModuleData[i][8] = g_byModuleData[8][i] = '\x00';
	}

	for (int i = 0; i < 8; i ++) {
		//		�E�㉡�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�����c
		g_byModuleData[m_iSymbolSize - 8 + i][8] = g_byModuleData[8][m_iSymbolSize - 8 + i] = '\x00';
	}

	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			if (g_byModuleData[j][m_iSymbolSize - (i + 1)] & 0x20) {
				iPos = (i * iLength) + (j * PM_CODE_BIT_SIZE);
				// ���ȊO�̂Ƃ��͔��ɂ��Ă��
				if (memcmp (&m_szPMCodeImage [iPos], szBlack, PM_CODE_BIT_SIZE) != 0) {
					memcpy (&m_szPMCodeImage [iPos], szWhite, PM_CODE_BIT_SIZE);
				}
			}
		}
	}
/*
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			// �a�l�o�摜�͂S�̔{���łȂ��Ƃ����Ȃ����߁A�V���{���T�C�Y���S�̔{���łȂ��ꍇ
			// �����߂���K�v������B�i���炩���ߏ��������s���Ă��邽�߁A�������΂������łn�j�j
// 2006/10/13 tsato update ----------------------------------------------
// �p�q�R�[�h�摜����������E��Ɍ������Ċi�[����l�ɕύX
			m_QRCodeImage [(i * iWidthLength) + j] = (BYTE)((g_byModuleData[j][m_iSymbolSize - (i + 1)] & 0x11) != 0);
//			m_QRCodeImage [(i * iWidthLength) + j] = (BYTE)((g_byModuleData[j][i] & 0x11) != 0);
// ----------------------------------------------------------------------
		}
	}
	
	
*/
	// �w�b�_���i�F���ʃR�[�h�A�w�����ʃR�[�h�j�̃Z�b�g
	iRet = SetPMCodeHeader (m_szPMCodeImage, m_uiPMCodeImageSize, m_iSymbolSize, m_iLayer);
	if (iRet != RESULT_OK) {
		return bRet;
	}

	// ����ɘg��t������
	szTemp = (char *)malloc (m_uiPMCodeImageSize);
	if (szTemp == NULL) {
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (szTemp, '\0', sizeof (szTemp));
	memcpy (szTemp, m_szPMCodeImage, m_uiPMCodeImageSize);
	m_iWidth	+= MARGIN;
	m_iHeight	+= MARGIN;
	m_uiPMCodeImageSize = CalcPitch (BIT_COUNT_24, m_iWidth) * m_iHeight;
	m_szPMCodeImage = (char *)realloc (m_szPMCodeImage, m_uiPMCodeImageSize);
	memset (m_szPMCodeImage, '\0', m_uiPMCodeImageSize);
	for (int i = 0; i < (m_iHeight - MARGIN); i ++) {
		iPos = (i + (MARGIN / 2)) * CalcPitch (BIT_COUNT_24, m_iWidth);
		iPos += ((MARGIN / 2) * PM_CODE_BIT_SIZE);
		memcpy (&m_szPMCodeImage [iPos], &szTemp [i * iLength], iLength);
	}
	free (szTemp);

	// ���W���[���R�[�h�̂��킹�Ăo�l�R�[�h���g�傷��
	ImageUtil::UpScale ((unsigned char **)&m_szPMCodeImage, (m_iWidth * m_iModuleSize), (m_iHeight * m_iModuleSize), m_iWidth, m_iHeight);
	// �㉺�����]���Ă���̂ŁA���]����B
	ImageUtil::UpDownReplace((unsigned char *)m_szPMCodeImage, (m_iWidth * m_iModuleSize), (m_iHeight * m_iModuleSize));

	m_iWidth	*= m_iModuleSize;
	m_iHeight	*= m_iModuleSize;
	iLength		= CalcPitch (BIT_COUNT_24, m_iWidth);
	m_uiPMCodeImageSize = iLength * m_iHeight;

	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�o�l�R�[�h�摜�̎擾									 //
// ����				:szImage			:�o�l�R�[�h�摜						 //
//                  :uiImageSize		:�o�l�R�[�h�摜�T�C�Y				 //
//                  :iWidth				:��									 //
//                  :iHeight			:����								 //
// �߂�l			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
int	CPMCodeEncode::GetPMCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight) {

	if (m_szPMCodeImage != NULL && m_uiPMCodeImageSize != 0){
		*szImage		= m_szPMCodeImage;									// �o�l�R�[�h�摜�̃A�h���X�Z�b�g
		*uiImageSize	= m_uiPMCodeImageSize;								// �o�l�R�[�h�摜�̃T�C�Y�Z�b�g
		*iWidth			= m_iWidth;											// �o�l�R�[�h�摜�̕��Z�b�g
		*iHeight		= m_iHeight;										// �o�l�R�[�h�摜�̍����Z�b�g
	} else {
		return RESULT_ERROR_ENCODE_YET;										// �����Í���������
	}
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:QR�R�[�h�摜�T�C�Y�̎擾								 //
// ����				:iLayer				:�w�ԍ�								 //
// �߂�l			:RESULT_OK												 //
//                  :RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
UINT CPMCodeEncode::GetQRCodeSize (int iLayer) {
	return m_uiQRCodeImageSize [iLayer];
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�p�q�R�[�h�摜�擾										 //
// ����				:szImage			:�o�l�R�[�h�摜						 //
//                  :uiImageSize		:�o�l�R�[�h�摜�T�C�Y				 //
//                  :iWidth				:��									 //
//                  :iHeight			:����								 //
//                  :iLayer				:�w�ԍ��i�|�P���Ă������I�I			 //
// �߂�l			:RESULT_OK												 //
//                  :RESULT_ERROR_ENCODE_YET								 //
// ���l				:���A�����̃s�N�Z���T�C�Y�͂o�l�R�[�h�̗l�Ƀ}�[�W����	 //
//					:�Ȃ��̂ŁA�}�[�W�����������Ă���						 //
// ------------------------------------------------------------------------- //
int	CPMCodeEncode::GetQRCodeImageData (char **szImage, UINT *uiImageSize, int *iWidth, int *iHeight, int iLayer) {

	// �ő�w���ȏ�A�܂��͑I��w�ԍ����g�p���Ă��Ȃ��ꍇ�G���[�Ƃ���
	if ((m_iLayer - 1 < iLayer) 
		|| (m_szQRCodeImage [iLayer] == NULL) 
		|| (m_uiQRCodeImageSize [iLayer] == 0)) { 
		return RESULT_ERROR_ENCODE_YET;										// �Í���������
	}
	*szImage		= m_szQRCodeImage [iLayer];								// �p�q�摜�̃A�h���X�Z�b�g
	*uiImageSize	= m_uiQRCodeImageSize [iLayer];							// �p�q�摜�̃T�C�Y�Z�b�g
	*iWidth			= (m_iWidth / m_iModuleSize) - MARGIN;					// �p�q�摜�̕��Z�b�g
	*iHeight		= (m_iHeight / m_iModuleSize) - MARGIN;					// �p�q�摜�̍����Z�b�g
	return RESULT_OK;
}

