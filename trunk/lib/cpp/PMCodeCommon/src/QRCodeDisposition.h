#ifndef     __QR_CODE_DISPOSITION_H__
#define     __QR_CODE_DISPOSITION_H__

// ------------------------------------------------------------------------- //
// QR�R�[�h�f�[�^�Í��N���X													 //
// ------------------------------------------------------------------------- //
class CQRCodeDisposition
{
// �R���X�g���N�V����
public:
	CQRCodeDisposition ();
	~CQRCodeDisposition ();

	int SetCodeWordPattern (char * szData, int iDataSize,int iRSLevel
						, int iWordSize, int iSymbolSize);						// �摜�f�[�^�Z�b�g	
private:
};
#endif