#ifndef QS_LISTING_STRING_H
#define QS_LISTING_STRING_H

/*
���� �������� ����������� �������-��������, ������������
������ �������� � ������.
*/

#include "listing.h"

class CListingString : public CListing
{
public:
	//�������� ������ � �������
	LPCTSTR GetString();

	virtual VOID EndLine();												//��������� ������
};

#endif
