#include "QRCodeDisposition.h"
#include "global.h"
#include "define.h"
//#include "CtrlBMP.h"

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�R���X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeDisposition::CQRCodeDisposition () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�f�X�g���N�^											 //
// ����				:�Ȃ�													 //
// �߂�l			:�Ȃ�													 //
// ���l				:														 //
// ------------------------------------------------------------------------- //
CQRCodeDisposition::~CQRCodeDisposition () {

}

// ------------------------------------------------------------------------- //
// �@�\�T�v			:�摜�f�[�^�Z�b�g										 //
// ����				:szData				:�f�[�^								 //
//					:uiDataSize			:�T�C�Y								 //
//					:iWordSize			:�R�[�h���f�[�^�T�C�Y				 //
//					:iSymbolSize		:�V���{���T�C�Y						 //
//					:iRSLevel			:���������x��						 //
// �߂�l			:RESULT_OK												 //
//					:RESULT_ERROR_SECURE_MEMORY								 //
// ���l				:�摜��ێ�����K�v�������ׁA�{�֐��ɂĉ�͂��s���܂�	 //
//					:�f�[�^�擾���s�̏ꍇ�A�O��Ԃ��܂��B					 //
// ------------------------------------------------------------------------- //
int CQRCodeDisposition::SetCodeWordPattern (char * szData, int iDataSize
											, int iWordSize, int iSymbolSize, int iRSLevel) {

    int iX			= iSymbolSize;											// �p�q�R�[�h�̉E������J�n����
    int iY			= iSymbolSize - 1;										// �p�q�R�[�h�̉E������J�n����
    int iDirectionX = 1;													// �����z�u����
    int iDirectionY = 1;													// �����z�u����

	if (iDataSize > iWordSize) {											// �f�[�^�T�C�Y���i�[�T�C�Y�ȏ�̏ꍇ�̓G���[�Ƃ���
		return 0;
	}

// �f�[�^�擾����
    for (int i = 0; i < iWordSize; i ++ ){
        for (int j = 0; j < 8; j ++) {
            do {
				iX += iDirectionX;
				iDirectionX *= -1;

				if (iDirectionX < 0) {
					iY += iDirectionY;

					if (iY < 0 || iY == iSymbolSize) {
						if (iY < 0) {
							iY = 0;
						} else {
							iY = iSymbolSize - 1;
						}
						iDirectionY *= -1;

						iX -= 2;

						if (iX == 6) // �^�C�~���O�p�^�[��
							iX -= 1;
					}
				}
			// �@�\���W���[���̓f�[�^���珜�O����
            } while (g_byModuleData[iX][iY] & 0x20);
			g_byModuleData[iX][iY] = (szData[i] & (1 << (7 - j))) ? '\x02' : '\x00';
        }
    }

	return RESULT_OK;
}

