#ifndef QS_DATABASE_H
#define QS_DATABASE_H

/*
���� �������� ����������� �����, ������������ � ������ ��� ������ �������������, �
����� �����, ����������� ����������� � ��������� ������.
*/

#include "object.h"
#include <map>

/*
������ ���� ������ �������������:

+----------------------+--------------+
| FILE_DATABASE_HEADER | ����� ������ |
+----------------------+--------------+
*/

//��������� �����
#define FILE_DATABASE_MAGIC1							'edoC'
#define FILE_DATABASE_MAGIC2							'\0bd0'

//��������� �����
struct FILE_DATABASE_HEADER
{
	//��������� �����
	DWORD Magic[2];
};

class CDatabase
{
protected:
	//��������� ����� ���� ������
	HANDLE mhFile;
	//����� �������� (���� - ������������� � �����, ���������� - ����� �������)
	std::map<DWORD,CObject*> mObjectsMap;

	//�����������
	CDatabase();

public:
	//����������
	~CDatabase();

	//������� ���� ������
	static CDatabase* Create(LPCTSTR szFile);
	//������� ���� ������
	static CDatabase* Open(LPCTSTR szFile);

	//������� ������
	BOOL ReadData(PVOID pData,DWORD Size);
	BOOL WriteData(const PVOID pData,DWORD Size);
	BOOL ReadValue(PDWORD Value);
	BOOL ReadValue(PVOID* Value);
	BOOL WriteValue(DWORD Value);
	BOOL WriteValue(PVOID Value);

	//������� ������
	BOOL ReadObject(CObject** pObject);
	//�������� ������
	BOOL WriteObject(const CObject* pObject);
	//�������� ��������� � ��������
	BOOL UpdatePointers();
	//�������� ��������� �� ������ �� ��� ��������������
	CObject* RetrieveObject(DWORD ID);
};

#endif
