#ifndef QS_ADDRESS_DATA_H
#define QS_ADDRESS_DATA_H

#include "object.h"
#include "cyber_memory.h"
#include "address_data_element.h"
#include "interpretations.h"
#include "label.h"
#include "comment.h"
#include "listing.h"
#include <map>

class CAddressData : public CObject									//�����, �������� ���������� �� ����� ������
{
public:
	typedef std::multimap<DWORD,CAddressDataElement*> ADEL;
	typedef ADEL::iterator ADELI;
protected:
	ADEL mElementsMap;																//����� ��������� ������ ������
	DWORD mSize;																			//������ ����� ������
	DWORD mPreviousSize;															//������ ����������� �����
	CYBER_ADDRESS mBlockAddress;											//����� ������ ���������/������-�� ����� ����

protected:
	//��������������� �������

	//������� �������, ������� ����� ���� ������ ����
	VOID SetIdentityElement(CAddressDataElement* pElement,DWORD ElementType);

public:
	CAddressData();																		//�����������
	virtual ~CAddressData();													//����������

	//���������, �������� �� ������ �������, �.�. �� �������� ����������
	BOOL IsEmpty();

	//���������� ������ ������
	VOID SetSize(DWORD Size);
	//�������� ������ ������
	DWORD GetSize();
	//���������� ������ ����������� ������
	VOID SetPreviousSize(DWORD PreviousSize);
	//�������� ������ ����������� ������
	DWORD GetPreviousSize();
	//���������� ����� ������ ���������
	VOID SetBlockAddress(CYBER_ADDRESS Address);
	//�������� ����� ������ ���������
	CYBER_ADDRESS GetBlockAddress();

	//�������� ������� � ���������� �� ������
	VOID AddElement(CAddressDataElement* pElement);
	//�������� ��������� �� ����� ��������� ������ ������
	ADEL* GetElements();
	//���������� ������ ������
	VOID Print(CListing* pListing);

	//������� ���������� ������� ����������
	//�������� �������������
	CInterpretation* GetInterpretation();
	//���������� ������������� (NULL - ������� �������������)
	VOID SetInterpretation(CInterpretation* pInterpretation);
	//�������� �����
	CLabel* GetLabel();
	//���������� ����� (NULL - ������� �����)
	VOID SetLabel(CLabel* pLabel);
	//�������� �����������
	CComment* GetComment();
	//���������� ����������� (NULL - ������� �����������)
	VOID SetComment(CComment* pComment);
	//������� �������
	VOID CreateSubroutine();
};

#endif
