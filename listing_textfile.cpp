#include "listing_textfile.h"

// class CListingTextFile

VOID CListingTextFile::SetFile(HANDLE hFile)
{
	//��������� ���� ��� ������
	mhFile=hFile;
}

VOID CListingTextFile::EndLine()
{
	//������� ������
	DWORD Written;
	WriteFile(mhFile,mszLineText,mLineTextLength*sizeof(TCHAR),&Written,NULL);
	//������� ������ �������� ������
	WriteFile(mhFile,TEXT("\r\n"),2*sizeof(TCHAR),&Written,NULL);
}
