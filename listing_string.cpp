#include "listing_string.h"

// class CListingString

LPCTSTR CListingString::GetString()
{
	//������� ��������� �� ������
	return mszLineText;
}

VOID CListingString::EndLine()
{
	//���������� ����������� ������
	mszLineText[mLineTextLength]=0;
}
