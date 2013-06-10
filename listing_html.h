#ifndef QS_LISTING_HTML_H
#define QS_LISTING_HTML_H

/*
���� �������� ����������� �������-��������, ������������
������ �������� � ���� ������� HTML.
*/

#include "listing.h"

class CListingHTML : public CListing
{
protected:
	//���� ��� ������
	HANDLE mhFile;

public:
	VOID SetFile(HANDLE hFile);										//������� ���� ��� ������
	VOID BeginFile();															//������ ����� ��������
	VOID EndFile();																//��������� ����� ��������

	virtual VOID EndLine();												//��������� ������
	virtual UINT PrintOffset(UINT Address);				//�������� ��������� ������������� ������
};

#endif
