#include "QRCodeMask.h"
#include "define.h"
#include "global.h"
//#include "CtrlBMP.h"

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeMask::CQRCodeMask () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeMask::~CQRCodeMask () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�}�X�N��������											 //
// ����				:szImage			:�p�q�摜�f�[�^						 //
//					:iImageSize			:�摜�T�C�Y							 //
//					:iMaskPattern		:�}�X�N�p�^�[��						 //
//					:byModuleData		:�@�\���W���[���f�[�^				 //
//					:iVersion			:�^��								 //
// �߂�l			:�Ȃ�													 //
// ���l				:�摜��ێ�����K�v�������ׁA�{�֐��ɂĉ�͂��s���܂�	 //
//					:�Ăяo�����ŉ摜�T�C�Y�ƃV���{���T�C�Y�̃`�F�b�N���s���I//
// ------------------------------------------------------------------------- //
void CQRCodeMask::GetMaskQRCode (char * szImage, UINT iImageSize, int iMaskPattern, int iVersion) {
	
	int		iLineLength;
	int		iSymbolSize = VER2SYM (iVersion);

	iLineLength = CalcPitch (BIT_COUNT_8, iSymbolSize);
	
	for (int i = 0; i < iSymbolSize; i ++) {
		for (int j = 0; j < iSymbolSize; j ++) {
			// �@�\���W���[�������O
			if (! (g_byModuleData[j][i] & 0x20)) {
				BOOL bMask;

				switch (iMaskPattern) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}
				// �}�X�N����
// 2006/10/11 tsato update ----------------------------------------------
				szImage[(i * iLineLength) + j] = (char)(szImage[(i * iLineLength) + j] ^ bMask);
//				szImage[(((iSymbolSize - 1) - i) * iLineLength) + j] = (char)(szImage[(((iSymbolSize - 1) - i) * iLineLength) + j] ^ bMask);
// ----------------------------------------------------------------------
			}
		}
	}
}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�}�X�N����												 //
// ����				:szImage			:�p�q�摜�f�[�^						 //
//					:iImageSize			:�摜�T�C�Y							 //
//					:iMaskPattern		:�}�X�N�p�^�[��						 //
//					:byModuleData		:�@�\���W���[���f�[�^				 //
//					:iVersion			:�^��								 //
// �߂�l			:�Ȃ�													 //
// ���l				:�摜��ێ�����K�v�������ׁA�{�֐��ɂĉ�͂��s���܂�	 //
//					:�Ăяo�����ŉ摜�T�C�Y�ƃV���{���T�C�Y�̃`�F�b�N���s���I//
// ------------------------------------------------------------------------- //
void CQRCodeMask::SetMaskQRCode (int iMaskPattern, int iVersion) {
	
	int	iSymbolSize = VER2SYM (iVersion);
	
	for (int i = 0; i < iSymbolSize; i ++) {
		for (int j = 0; j < iSymbolSize; j ++) {
			// �@�\���W���[�������O
			if (! (g_byModuleData[j][i] & 0x20)) {
				BOOL bMask;

				switch (iMaskPattern) {
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}
				// �}�X�N����
				g_byModuleData[j][i] = (BYTE)((g_byModuleData[j][i] & 0xfe) | (((g_byModuleData[j][i] & 0x02) > 1) ^ bMask));
			}
		}
	}
}
