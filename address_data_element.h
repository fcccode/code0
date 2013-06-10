#ifndef QS_ADDRESS_DATA_ELEMENT_H
#define QS_ADDRESS_DATA_ELEMENT_H

/*
���� �������� ����������� ������������ ������ �������� ������ ������.
*/

#include "object.h"
#include "listing.h"

// ���� ��������� ������ ������
// ������� ������������ ����� ��������� ���������� ������� �� ������ � ���������
#define ADET_COMMENT											0
#define ADET_XREFERENCE										1
#define ADET_SUBROUTINE										2
#define ADET_LABEL												3
#define ADET_INTERPRETATION								4

//������� ������ ������
class CAddressDataElement : public CObject
{
public:
	virtual ~CAddressDataElement();										//����������

	virtual DWORD GetElementType() PURE;							//�������� ��� �������� ������ ������
	virtual VOID Print(CListing*) PURE;								//���������� ��������� ������������� �������� ������
};

#endif
