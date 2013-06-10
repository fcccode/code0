#ifndef QS_XREFERENCE_H
#define QS_XREFERENCE_H

/*
���� �������� ����������� ������ �������� ������ ������ - ������������ ������.
*/

#include "address_data_element.h"
#include "cyber_memory.h"

class CXReference : public CAddressDataElement
{
protected:
	CYBER_ADDRESS mAddressFrom;
	BYTE mReferenceFlags;

public:
	CXReference(CYBER_ADDRESS AddressFrom,BYTE ReferenceFlags);

	virtual DWORD GetElementType();										//�������� ��� �������� ������ ������
	virtual VOID Print(CListing*);										//���������� ��������� ������������� �������� ������

	//����������� ������� ������������ ������
	CYBER_ADDRESS GetAddressFrom();										//�������� �������� ����� ������������ ������
};

#endif
