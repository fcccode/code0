#ifndef QS_ADDRESS_MAP_H
#define QS_ADDRESS_MAP_H

/*
���� �������� ����������� ������, ������������ ��������� ���������� �� ����������� �����.
*/

#include "object.h"
#include "address_data.h"
#include "label.h"
#include "comment.h"
#include "listing.h"
#include <map>

class CAddressMap : public CObject
{
protected:
	typedef std::map<CYBER_ADDRESS,CAddressData*> AM;
	AM mAddressDataMap;																					//����� (map), ���������� ���������� �� �������

public:
	//����������
	virtual ~CAddressMap();

	CAddressData* OpenAddress(CYBER_ADDRESS Address);						//��������� ��� ������� ���������� � �������� ������
	CAddressData* GetAddress(CYBER_ADDRESS Address);						//��������� (�� �������) ���������� � �������� ������

	//������� ������������ ������ (������, ��� ��������� ����� ������ � ��������������� �������������)
	VOID AddXReference(CYBER_ADDRESS AddressFrom,CYBER_ADDRESS AddressTo,BYTE ReferenceFlags);
	//������� ����� ��� ������
	VOID SetLabel(CYBER_ADDRESS Address,LPCTSTR szName);
	//�������� ����� ��� ������ (���� ����)
	CLabel* GetLabel(CYBER_ADDRESS Address);
	//������� ����������� ��� ������
	VOID SetComment(CYBER_ADDRESS Address,LPCTSTR szName);
	//�������� ����������� ��� ������ (���� ����)
	CComment* GetComment(CYBER_ADDRESS Address);

	//������� ������ ������
	VOID SetAddressSize(CYBER_ADDRESS Address,DWORD Size);

	// ��������������� �������

	//������������ ������ ��� ������ (� ���� ����� ��� ��������), ������ ������� ������� delete []
	LPTSTR GetAddressString(CYBER_ADDRESS Address);

	//TEST
	VOID ExplortListingToFile();
};

#endif
