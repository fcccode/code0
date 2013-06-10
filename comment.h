#ifndef QS_COMMENT_H
#define QS_COMMENT_H

/*
���� �������� ���������� ������ �����������.
*/

#include "address_data_element.h"

class CComment : public CAddressDataElement
{
protected:
	LPTSTR mszText;																		//��������� �� ����� �����������

public:
	CComment(LPCTSTR szText);
	virtual ~CComment();

	LPCTSTR GetText();																//�������� ��������� �� ����� ����������� (������������, ������ ���� ����������� ����������)

	virtual DWORD GetElementType();										//�������� ��� �������� ������ ������
	virtual VOID Print(CListing*);										//���������� ��������� ������������� �������� ������
};

#endif
