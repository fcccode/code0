#ifndef QS_LISTING_TEXTFILE_H
#define QS_LISTING_TEXTFILE_H

/*
���� �������� ����������� �������-��������, ������������
������ �������� � ����.
*/

#include "listing.h"

class CListingTextFile : public CListing
{
protected:
	//���� ��� ������
	HANDLE mhFile;

public:
	VOID SetFile(HANDLE hFile);										//������� ���� ��� ������

	virtual VOID EndLine();												//��������� ������
};

#endif
