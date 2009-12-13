#include "QRCodeEncode.h"
#include "QRCodeMask.h"
#include "QRCodeSymbol.h"
#include "QRCodeDisposition.h"
#include "global.h"
//#include "CtrlBMP.h"
#include "define.h"

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeEncode::CQRCodeEncode () {

	// ������
	m_QRCodeImage		= NULL;
	m_uiQRCodeImageSize	= 0;
	m_szEncodeData		= NULL;
	m_uiEncodeDataSize	= 0;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeEncode::~CQRCodeEncode () {

	// �������̊J��
	if (m_QRCodeImage != NULL) {
		free (m_QRCodeImage);
		m_QRCodeImage = NULL;
		m_uiQRCodeImageSize = 0;
	}
	if (m_szEncodeData != NULL) {
		free (m_szEncodeData);
		m_szEncodeData = NULL;
		m_uiEncodeDataSize = 0;
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:QR�R�[�h���̃Z�b�g									 //
// ����				:szData				:�f�[�^								 //
//					:iDataSize			:�f�[�^�T�C�Y						 //
//					:iVersion			:�^��								 //
//					:iRSLevel			:���������x��						 //
//					:iMaskPattern		:�}�X�N�p�^�[��						 //
// �߂�l			:RESULT_OK												 //
//					:RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:�{���A�p�q�R�[�h�̕����f�[�^�������������͂p�q�R�[�h	 //
//					:�����A�w�̕��������邽�߁A�o�l�R�[�h�N���X���ōs��		 //
// ------------------------------------------------------------------------- //
int	CQRCodeEncode::SetQRCodeData (char * szData, UINT iDataSize, int iVersion, int iRSLevel, int iMaskPattern) {

	int iSymbolSize;
	int iWidthLength;
	int iImageSize;

	iSymbolSize		= VER2SYM (iVersion);										// �^�Ԃɑ΂���V���{���T�C�Y�v�Z
	iWidthLength	= CalcPitch (BIT_COUNT_8, iSymbolSize);						// �摜�����T�C�Y�̌v�Z
	iImageSize		= iSymbolSize * iWidthLength;								// �摜�T�C�Y�̌v�Z

	// �����������Ɋm�ۂ���Ă���ꍇ�́A�Ċm�ۂ��Ȃ��i�T�C�Y�͓���̂��߁j
	if ((int)m_uiQRCodeImageSize != iImageSize + 1) {
		m_uiQRCodeImageSize = iImageSize + 1;
		m_QRCodeImage = (char *)malloc (m_uiQRCodeImageSize);					// �摜�f�[�^�p�������̊m��
		if (m_QRCodeImage == NULL) {											// �������m�ێ��s
			return RESULT_ERROR_SECURE_MEMORY;
		} 
	}
	memset (m_QRCodeImage, '\0', m_uiQRCodeImageSize);							// �摜�������̏�����

	m_uiEncodeDataSize = iDataSize;
	m_szEncodeData = (char *)realloc (m_szEncodeData, m_uiEncodeDataSize + 1);	// �f�[�^�p�������̊m��
	if (m_szEncodeData == NULL) {												// �������m�ێ��s
		return RESULT_ERROR_SECURE_MEMORY;
	} 
	memset (m_szEncodeData, '\0', m_uiEncodeDataSize + 1);						// �f�[�^�o�b�t�@�̏�����
	memcpy (m_szEncodeData, szData, m_uiEncodeDataSize);						// �f�[�^�̊i�[

	m_iVersion		= iVersion;													// �^�Ԃ̃Z�b�g
	m_iRSLevel		= iRSLevel;													// ���������x���̃Z�b�g
	m_iMaskPattern	= iMaskPattern;												// �}�X�N�p�^�[���̃Z�b�g
	m_iSymbolSize	= iSymbolSize;
	return RESULT_OK;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:QR�R�[�h�̃G���R�[�h									 //
// ����				:�Ȃ�													 //
// �߂�l			:QR�R�[�h�摜�f�[�^�T�C�Y								 //
// ���l				:�G���R�[�h�ł��Ȃ������ꍇ�́A���ʂ��O�ƂȂ�܂�		 //
//					:�P�w�p�ɕ����E�������ݒ菈���͌Ăяo�����ɂčs���܂�  //
// ------------------------------------------------------------------------- //
UINT CQRCodeEncode::EncodeQRCode () {

	int						iRet;
	int						iMinPenalty = 0;									// �ŏ����_��
	int						iPenalty	= 0;									// ���_��
	int						iWidthLength;										// ���T�C�Y
	int						iWordSize;
	CQRCodeSymbol			*pSymbol;											// �V���{���N���X
	CQRCodeMask				*pMask;												// �}�X�N�N���X
	CQRCodeDisposition		*pDisposition;										// �f�[�^�擾�E�����N���X

	SetFunctionModule ();														// �@�\���W���[���̐ݒ�

// �f�[�^�̃Z�b�g	
	pDisposition = new CQRCodeDisposition;
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
	iRet = pDisposition->SetCodeWordPattern (m_szEncodeData, m_uiEncodeDataSize
								, iWordSize
								, m_iSymbolSize, m_iRSLevel);					// �f�[�^�̃Z�b�g
//	iRet = pDisposition->SetCodeWordPattern (m_szEncodeData, m_uiEncodeDataSize
//								, QR_VersonInfo[m_iVersion].ncAllCodeWord
//								, m_iSymbolSize, m_iRSLevel);					// �f�[�^�̃Z�b�g
	delete pDisposition;
	if(RESULT_OK != iRet){
		return 0;
	}
// �}�X�N�����i�}�X�N�����̐��۔��肪�ł��Ȃ����߁A�߂�l��`�F�b�N�͂Ȃ��j
	pMask = new CQRCodeMask;
	if (m_iMaskPattern == -1) {													// �}�X�N�p�^�[���������̏ꍇ
		// �œK�}�X�L���O�p�^�[���I��
		// �}�X�N�O�`�V�܂Ń}�X�N���s���A���_���̏��Ȃ��}�X�N�R�[�h���̗p����
		m_iMaskPattern = MIN_MASK_NUMBER;
		pMask->SetMaskQRCode (m_iMaskPattern, m_iVersion);
		iMinPenalty = CountPenalty();

		for (int i = 1; i <= MAX_MASK_NUMBER; ++i) {
			pMask->SetMaskQRCode (i, m_iVersion);
			iPenalty = CountPenalty();
			if (iPenalty < iMinPenalty) {
				iMinPenalty = iPenalty;
				m_iMaskPattern = i;
			}
		}
		pMask->SetMaskQRCode (m_iMaskPattern, m_iVersion);
	} else if (m_iMaskPattern <= MAX_MASK_NUMBER) {
		pMask->SetMaskQRCode (m_iMaskPattern, m_iVersion);
	}
	delete pMask;

// �^��/�^�Ԃ̐ݒ�
	pSymbol = new CQRCodeSymbol;
	iRet = pSymbol->SetQRCodeSymbol (m_iSymbolSize, m_iRSLevel, m_iMaskPattern);
																				// �^���̎擾
	delete pSymbol;
	if(RESULT_OK != iRet){
		return 0;
	}
// �p�q�R�[�h�摜���쐬����
	iWidthLength = CalcPitch (BIT_COUNT_8, m_iSymbolSize);						// �摜�����T�C�Y�̌v�Z
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
	return m_uiQRCodeImageSize;
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:QR�R�[�h�摜�̎擾										 //
// ����				:szImage			:�p�q�R�[�h�摜�o�b�t�@				 //
//                  :iBufferSize		:�p�q�R�[�h�摜�T�C�Y				 //
// �߂�l			:RESULT_OK												 //
//					:RESULT_ERROR_ENCODE_YET								 //
// ���l				:�摜�f�[�^���摜�o�b�t�@�̏ꍇ�̓G���[�̕����ǂ�����	 //
// ------------------------------------------------------------------------- //
int	CQRCodeEncode::GetQRCodeImage (char *szImage, UINT iBufferSize) {

	// �p�q�R�[�h�摜����
	if (m_QRCodeImage != NULL && m_uiQRCodeImageSize != 0) {
		// �摜�f�[�^���o�b�t�@�Ɏ��܂�Ȃ�A�摜�f�[�^�������Z�b�g����
		if (iBufferSize >= m_uiQRCodeImageSize) {
			memcpy (szImage, m_QRCodeImage, m_uiQRCodeImageSize);
		// �f�[�^�o�b�t�@�T�C�Y���f�[�^��菬�����ꍇ�́A�o�b�t�@�T�C�Y���Z�b�g����
		} else {
			memcpy (szImage, m_QRCodeImage, iBufferSize);
		}
	} else {
		return RESULT_ERROR_ENCODE_YET;										// �Í���������
	}
	return RESULT_OK;														// ��������
}

// ------------------------------------------------------------------------- //
// --------------------------- �ȉ�private �֐� ---------------------------- //
// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�@�\�R�[�h�̐ݒ�										 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:�@�\���W���[���̏��������s���A�@�\�R�[�h�̐ݒ�����܂�	 //
//					:�{�֐��̏����͂p�q�R�[�h�쐬�c�[�����Q�l�ɂ��Ă��܂�	 //
// ------------------------------------------------------------------------- //
void CQRCodeEncode::SetFunctionModule () {

	memset (g_byModuleData, '\0', sizeof (g_byModuleData));						// �P�w���W���[���̏�����

	// �ʒu���o�p�^�[���̐ݒ�
	SetFinderPattern(0, 0);
	SetFinderPattern(m_iSymbolSize - 7, 0);
	SetFinderPattern(0, m_iSymbolSize - 7);

	// �ʒu���o�p�^�[���Z�p���[�^
	for (int i = 0; i < 8; i ++) {
		g_byModuleData[i][7]					= g_byModuleData[7][i] = '\x20';
		g_byModuleData[m_iSymbolSize - 8][i]	= g_byModuleData[m_iSymbolSize - 8 + i][7] = '\x20';
		g_byModuleData[i][m_iSymbolSize - 8]	= g_byModuleData[7][m_iSymbolSize - 8 + i] = '\x20';
	}

	// �t�H�[�}�b�g���L�q�ʒu���@�\���W���[�����Ƃ��ēo�^
	for (int i = 0; i < 9; i ++) {
		// �@���㉡�@�@�@�@�@�@�@	�����c
		g_byModuleData[i][8] = g_byModuleData[8][i] = '\x20';
	}

	for (int i = 0; i < 8; i ++) {
		//		�E�㉡�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�����c
		g_byModuleData[m_iSymbolSize - 8 + i][8] = g_byModuleData[8][m_iSymbolSize - 8 + i] = '\x20';
	}

// -----------------------------------------------------------------------------------------
// 2007/01/05 �^�ԃR�[�h���g�p���Ȃ��l�Ή�
	// �o�[�W�������p�^�[��
//	SetVersionPattern();
// -----------------------------------------------------------------------------------------

	// �ʒu���킹�p�^�[��
	for (int i = 0; i < QR_VersonInfo[m_iVersion].ncAlignPoint; i ++) {
		SetAlignmentPattern(QR_VersonInfo[m_iVersion].nAlignPoint[i], 6);		// �E��
		SetAlignmentPattern(6, QR_VersonInfo[m_iVersion].nAlignPoint[i]);		// ����

		for (int j = 0; j < QR_VersonInfo[m_iVersion].ncAlignPoint; j ++) {
			SetAlignmentPattern(QR_VersonInfo[m_iVersion].nAlignPoint[i]
				, QR_VersonInfo[m_iVersion].nAlignPoint[j]);
		}
	}

	// �^�C�~���O�p�^�[��
	for (int i = 8; i <= m_iSymbolSize - 9; i ++) {
		g_byModuleData[i][6] = (i % 2) == 0 ? '\x30' : '\x20';
		g_byModuleData[6][i] = (i % 2) == 0 ? '\x30' : '\x20';
	}

	// �o�l�R�[�h�p���ʏ��@�p�q�R�[�h�Ƃ��ẮA�@�\���W���[���Ƃ��Đݒ肷�邾���i��ʏ����ɂĐݒ肷��j
#if 0
	for (int i = 0; i <= 5; i ++) {
		for (int j = 9; j <= 12; j ++) {
			g_byModuleData[i][j] = '\x20';
		}
	}
#else
	// 2006/11/27 �F���ʃR�[�h�̃T�C�Y��w���ɂ��ω�������
	int iEnd;

	if (m_iVersion < 3){
		iEnd = 12;
	} else {
		iEnd = 20;
	}
	for (int i = 0; i <= 5; i ++) {
		for (int j = 9; j <= iEnd; j ++) {
			g_byModuleData[i][j] = '\x20';
		}
	}
#endif
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�@�\�R�[�h�̐ݒ�										 //
// ����				:�������w���W											 //
//					:�������x���W											 //
// �߂�l			:�Ȃ�													 //
// ���l				:�@�\���W���[���̏��������s���A�@�\�R�[�h�̐ݒ�����܂�	 //
// ------------------------------------------------------------------------- //
void CQRCodeEncode::SetFinderPattern(int x, int y) {

	static BYTE byPattern[] = {0x7f,  // 1111111b
							   0x41,  // 1000001b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x41,  // 1000001b
							   0x7f}; // 1111111b

	for (int i = 0; i < 7; i++)	{
		for (int j = 0; j < 7; j++)	{
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (6 - j))) ? '\x30' : '\x20'; 
		}
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�o�[�W����(�^��)���p�^�[���z�u						 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:�@�\���W���[���̏��������s���A�@�\�R�[�h�̐ݒ�����܂�	 //
// ------------------------------------------------------------------------- //
void CQRCodeEncode::SetVersionPattern() {

	int iVerData;

	// �U�^�ȉ��̓o�[�W������񂪖����̂ł��̂܂ܕԂ�
	if (m_iVersion <= 6) {
		return;
	}
	iVerData = m_iVersion << 12;

	// ��]�r�b�g�Z�o
	for (int i = 0; i < 6; i ++) {
		if (iVerData & (1 << (17 - i)))	{
			iVerData ^= (0x1f25 << (5 - i));
		}
	}
	iVerData += m_iVersion << 12;
	for (int i = 0; i < 6; i ++) {
		for (int j = 0; j < 3; j ++) {
			g_byModuleData[m_iSymbolSize - 11 + j][i] = g_byModuleData[i][m_iSymbolSize - 11 + j] =
			(iVerData & (1 << (i * 3 + j))) ? '\x30' : '\x20';
		}
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�ʒu���킹�p�^�[���̐ݒ�								 //
// ����				:�z�u�����w���W											 //
//					:�z�u�����x���W											 //
// �߂�l			:�Ȃ�													 //
// ���l				:�@�\���W���[���̏��������s���A�@�\�R�[�h�̐ݒ�����܂�	 //
// ------------------------------------------------------------------------- //
void CQRCodeEncode::SetAlignmentPattern(int x, int y) {

	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b

	if (g_byModuleData[x][y] & 0x20) {
		return;																	// �@�\���W���[���Əd�����邽�ߏ��O
	}
	x -= 2; y -= 2;																// ��������W�ɕϊ�

	for (int i = 0; i < 5; i ++) {
		for (int j = 0; j < 5; j ++) {
			g_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:���_���̎擾											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
int CQRCodeEncode::CountPenalty() {

	int iPenalty = 0;
	int iCount = 0;
	int	k = 0;

	// ���F�̗�̗אڃ��W���[��
	for (int i = 0; i < m_iSymbolSize; i ++)	{
		for (int j = 0; j < m_iSymbolSize - 4; j ++) {
			for (k = j + 1, iCount = 1; k < m_iSymbolSize; k++)	{
				if (((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i][k] & 0x11) == 0)) {
					iCount ++;
				} else {
					break;
				}
			}
			if (iCount >= 5) {
				iPenalty += 3 + (iCount - 5);
			}
			j = k - 1;
		}
	}

	// ���F�̍s�̗אڃ��W���[��
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 4; j ++) {
			for (k = j + 1, iCount = 1; k < m_iSymbolSize; k ++) {
				if (((g_byModuleData[j][i] & 0x11) == 0) == ((g_byModuleData[k][i] & 0x11) == 0)) {
					iCount ++;
				} else {
					break;
				}
			}
			if (iCount >= 5) {
				iPenalty += 3 + (iCount - 5);
			}
			j = k - 1;
		}
	}

	// ���F�̃��W���[���u���b�N�i�Q�~�Q�j
	for (int i = 0; i < m_iSymbolSize - 1; i ++) {
		for (int j = 0; j < m_iSymbolSize - 1; j ++) {
			if ((((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i + 1][j]		& 0x11) == 0)) &&
				(((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i]	[j + 1] & 0x11) == 0)) &&
				(((g_byModuleData[i][j] & 0x11) == 0) == ((g_byModuleData[i + 1][j + 1] & 0x11) == 0)))
			{
				iPenalty += 3;
			}
		}
	}

	// �����ɂ����� 1:1:3:1:1 �䗦�i��:��:��:��:�Áj�̃p�^�[��
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 6; j ++) {
			if (((j == 0) ||				 (! (g_byModuleData[i][j - 1] & 0x11))) && // �� �܂��� �V���{���O
											 (   g_byModuleData[i][j]     & 0x11)   && // �� - 1
											 (! (g_byModuleData[i][j + 1] & 0x11))  && // �� - 1
											 (   g_byModuleData[i][j + 2] & 0x11)   && // �� ��
											 (   g_byModuleData[i][j + 3] & 0x11)   && // �� ��3
											 (   g_byModuleData[i][j + 4] & 0x11)   && // �� ��
											 (! (g_byModuleData[i][j + 5] & 0x11))  && // �� - 1
											 (   g_byModuleData[i][j + 6] & 0x11)   && // �� - 1
				((j == m_iSymbolSize - 7) || (! (g_byModuleData[i][j + 7] & 0x11))))   // �� �܂��� �V���{���O
			{
				// �O�܂��͌��4�ȏ�̖��p�^�[��
				if (((j < 2 || ! (g_byModuleData[i][j - 2] & 0x11)) && 
					 (j < 3 || ! (g_byModuleData[i][j - 3] & 0x11)) &&
					 (j < 4 || ! (g_byModuleData[i][j - 4] & 0x11))) ||
					((j >= m_iSymbolSize - 8  || ! (g_byModuleData[i][j + 8]  & 0x11)) &&
					 (j >= m_iSymbolSize - 9  || ! (g_byModuleData[i][j + 9]  & 0x11)) &&
					 (j >= m_iSymbolSize - 10 || ! (g_byModuleData[i][j + 10] & 0x11))))
				{
					iPenalty += 40;
				}
			}
		}
	}

	// ����s�ɂ����� 1:1:3:1:1 �䗦�i��:��:��:��:�Áj�̃p�^�[��
	for (int i = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize - 6; j ++) {
			if (((j == 0) ||				 (! (g_byModuleData[j - 1][i] & 0x11))) && // �� �܂��� �V���{���O
											 (   g_byModuleData[j]    [i] & 0x11)   && // �� - 1
											 (! (g_byModuleData[j + 1][i] & 0x11))  && // �� - 1
											 (   g_byModuleData[j + 2][i] & 0x11)   && // �� ��
											 (   g_byModuleData[j + 3][i] & 0x11)   && // �� ��3
											 (   g_byModuleData[j + 4][i] & 0x11)   && // �� ��
											 (! (g_byModuleData[j + 5][i] & 0x11))  && // �� - 1
											 (   g_byModuleData[j + 6][i] & 0x11)   && // �� - 1
				((j == m_iSymbolSize - 7) || (! (g_byModuleData[j + 7][i] & 0x11))))   // �� �܂��� �V���{���O
			{
				// �O�܂��͌��4�ȏ�̖��p�^�[��
				if (((j < 2 || ! (g_byModuleData[j - 2][i] & 0x11)) && 
					 (j < 3 || ! (g_byModuleData[j - 3][i] & 0x11)) &&
					 (j < 4 || ! (g_byModuleData[j - 4][i] & 0x11))) ||
					((j >= m_iSymbolSize - 8  || ! (g_byModuleData[j + 8][i]  & 0x11)) &&
					 (j >= m_iSymbolSize - 9  || ! (g_byModuleData[j + 9][i]  & 0x11)) &&
					 (j >= m_iSymbolSize - 10 || ! (g_byModuleData[j + 10][i] & 0x11))))
				{
					iPenalty += 40;
				}
			}
		}
	}

	// �S�̂ɑ΂���Ã��W���[���̐�߂銄��
	for (int i = 0, iCount = 0; i < m_iSymbolSize; i ++) {
		for (int j = 0; j < m_iSymbolSize; j ++) {
			if (! (g_byModuleData[i][j] & 0x11)) {
				iCount ++;
			}
		}
	}
	iPenalty += (abs(50 - ((iCount * 100) / (m_iSymbolSize * m_iSymbolSize))) / 5) * 10;
	return iPenalty;
}
