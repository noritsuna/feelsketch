//============================================================================
// Name        : PMCode.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include "global.h"
#include "PMCodeReader.h"
#include "data.h" // ここに画像データを入れてあります。

#define BLACK_COLOR_CODE		"000000"


// ＰＭコードのデコード処理
int decodePMCode () {

	int		iRet;
	int		iLayer;
	int		iRSLevel;
	int		iVersion;
	int		iModuleSize;
	int		iOldCount;
	unsigned int uiSize;
	char	szReserve [16];
	char	szExt [8];
	char	szSize [8];
	char	szSizeBuf [16];

	int     m_iDataSize;
	char*   m_szDataBuf = NULL;

	// デコードしたい画像のパラメータ
	char *m_szPMCodeImage = (char*)imageData;
	UINT m_uiPMCodeImageSize = sizeof(imageData);
	int m_iHeight = 29;
	int m_iWidth = 29;


	// デコーダーを new します。
	CPMCodeReader m_cPMCodeReader;

	// デコードしたい画像をセットします。
	// 本番では、基準色となる色もセットする仕様となります。いまは、FF0000,00FF00,0000FFで決めうちです。
	m_cPMCodeReader.SetBaseImage(m_szPMCodeImage, m_uiPMCodeImageSize, m_iHeight, m_iWidth);


	// デコードします。
	m_iDataSize = m_cPMCodeReader.DecodePMCodeImage ();
	if (m_iDataSize < 0) {
		return m_iDataSize;
	}
	if (m_szDataBuf != NULL){
		free (m_szDataBuf);
		m_szDataBuf = NULL;
	}

	// デコードしたデータの格納用配列
	m_szDataBuf = (char *)malloc (m_iDataSize + 1);
	if (m_szDataBuf == NULL){
		return RESULT_ERROR_SECURE_MEMORY;
	}
	memset (m_szDataBuf, '\0', m_iDataSize + 1);

	// デコードしたデータを格納します。
	iRet = m_cPMCodeReader.GetDecodeData (m_szDataBuf, m_iDataSize);			// 復号データの取得
	if (iRet != RESULT_OK){
		return iRet;
	}

	// デコードしたデータの各種データを取得します。
	// 拡張子の取得
	memset (szExt, '\0', sizeof (szExt));
	memcpy (szExt, m_szDataBuf, EXT_SIZE);
	// サイズの取得
	memset (szSizeBuf, '\0', sizeof (szSizeBuf));
	memcpy (szSize, &m_szDataBuf [EXT_SIZE], SIZE_SIZE);
	// 備考の設定
	memset (szReserve, '\0', sizeof (szReserve));
	memcpy (szReserve, &m_szDataBuf [EXT_SIZE + SIZE_SIZE], RESERVE_SIZE);

	// PMコードのデータ数きっちりデータが入っているとは限らないので、
	// ヘッダー情報にあるサイズを保存サイズとします。
	for (int i = 0; i < SIZE_SIZE; i ++ ){
		b1th2(szSize[i], &szSizeBuf [i * 2] );
	}
	uiSize = AscHexToLong(szSizeBuf, strlen (szSizeBuf));

	// これが、格納したデータです。
	printf("Load Data = %s\n", &m_szDataBuf[HEADER_SIZE]);

	// あとしまつ。
	free(m_szDataBuf);
	if(&m_cPMCodeReader != NULL)
		delete &m_cPMCodeReader;
	return iRet;

}



int main() {
	int ret = decodePMCode();
	printf("ret = %i\n", ret);
	return 0;
}

