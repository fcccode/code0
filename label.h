#ifndef QS_LABEL_H
#define QS_LABEL_H

/*
���� �������� ���������� ������ �����.
*/

#include "address_data_element.h"

class CLabel : public CAddressDataElement
{
protected:
	LPTSTR mszName;																		//��������� �� ��� �����

public:
	CLabel(LPCTSTR szName);
	virtual ~CLabel();

	LPCTSTR GetName();																//�������� ��������� �� ��� ����� (������������, ������ ���� ����� ����������)

	virtual DWORD GetElementType();										//�������� ��� �������� ������ ������
	virtual VOID Print(CListing*);										//���������� ��������� ������������� �������� ������
};

#endif
