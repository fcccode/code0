#ifndef QS_SUBROUTINE_H
#define QS_SUBROUTINE_H

/*
���� �������� ����������� ������� - �������� ����� ������ ������,
������� �������� ���������� � ��������� ����.
*/

#include "address_data_element.h"

class CSubroutine : public CAddressDataElement
{
protected:
	//���� ��� ����������

public:
	//�����������
	CSubroutine();

	//������������� �������
	virtual DWORD GetElementType();										//�������� ��� �������� ������ ������
	virtual VOID Print(CListing*);										//���������� ��������� ������������� �������� ������
};

#endif
