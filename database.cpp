#include "database.h"

// class CDatabase

CDatabase::CDatabase()
{
}

CDatabase::~CDatabase()
{
	//������� ����
	if(mhFile)
	{
		CloseHandle(mhFile);
		mhFile=NULL;
	}
}

CDatabase* CDatabase::Create(LPCTSTR szFile)
{
	//������� ����
	HANDLE hFile=CreateFile(szFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return NULL;

	//�������� ���������
	FILE_DATABASE_HEADER Header;
	Header.Magic[0]=FILE_DATABASE_MAGIC1;
	Header.Magic[1]=FILE_DATABASE_MAGIC2;
	DWORD Written;
	if(!WriteFile(hFile,&Header,sizeof(Header),&Written,NULL) || Written<sizeof(Header))
	{
		CloseHandle(hFile);
		return NULL;
	}

	//������� ������ ������
	CDatabase* pDatabase=new CDatabase;
	pDatabase->mhFile=hFile;
	return pDatabase;
}

CDatabase* CDatabase::Open(LPCTSTR szFile)
{
	//������� ����
	HANDLE hFile=CreateFile(szFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return NULL;

	//������� ������ ������
	CDatabase* pDatabase=new CDatabase;
	pDatabase->mhFile=hFile;
	return pDatabase;
}

BOOL CDatabase::ReadData(PVOID pData,DWORD Size)
{
	//������� ������
	DWORD Read;
	return ReadFile(mhFile,pData,Size,&Read,NULL) && Size==Read;
}

BOOL CDatabase::ReadValue(PDWORD Value)
{
	//������� �����
	DWORD Read;
	return ReadFile(mhFile,Value,sizeof(DWORD),&Read,NULL) && Read==sizeof(DWORD);
}

BOOL CDatabase::ReadValue(PVOID* Value)
{
	//������� �����
	DWORD Read;
	return ReadFile(mhFile,Value,sizeof(PVOID),&Read,NULL) && Read==sizeof(PVOID);
}

BOOL CDatabase::WriteData(const PVOID pData,DWORD Size)
{
	//�������� ������
	DWORD Written;
	return WriteFile(mhFile,pData,Size,&Written,NULL) && Size==Written;
}

BOOL CDatabase::WriteValue(DWORD Value)
{
	//�������� �����
	DWORD Written;
	return WriteFile(mhFile,&Value,sizeof(DWORD),&Written,NULL) && Written==sizeof(DWORD);
}

BOOL CDatabase::WriteValue(PVOID Value)
{
	//�������� �����
	DWORD Written;
	return WriteFile(mhFile,&Value,sizeof(PVOID),&Written,NULL) && Written==sizeof(PVOID);
}

BOOL CDatabase::ReadObject(CObject** pObject)
{
	//������� ������������� �������
	DWORD ID,Read;
	if(!ReadFile(mhFile,&ID,sizeof(ID),&Read,NULL) || Read<sizeof(ID)) return FALSE;

	//���� ������ ��� �� ��� ������
	std::map<DWORD,CObject*>::iterator i=mObjectsMap.find(ID);
	if(i==mObjectsMap.end())
	{
		//�������� ������ � �������
		mObjectsMap.insert(std::make_pair(ID,*pObject));
		//��������� ������
		return (*pObject)->ReadFromDatabase(this);
	}
	//����� ������ ��� ��� ������
	else
	{
		//������� ������
		delete *pObject;
		*pObject=(*i).second;
		return TRUE;
	}
}

BOOL CDatabase::WriteObject(const CObject* pObject)
{
	//�������� ������������� ������� (������ ��� ������)
	DWORD ID,Written;
	ID=(DWORD)pObject;
	if(!WriteFile(mhFile,&ID,sizeof(ID),&Written,NULL) || Written<sizeof(ID)) return FALSE;

	//���� ������ ��� �� ��� �������
	if(mObjectsMap.find((DWORD)pObject)==mObjectsMap.end())
	{
		//�������� ������ � �����
		mObjectsMap.insert(std::make_pair((DWORD)pObject,(CObject*)pObject));
		//�������� ������
		return pObject->WriteToDatabase(this);
	}
	//����� ������ ��� ��� �������
	else
		//��� ��� �� ����������
		return TRUE;
}

BOOL CDatabase::UpdatePointers()
{
	//���� �� ��������
	for(std::map<DWORD,CObject*>::iterator i=mObjectsMap.begin();i!=mObjectsMap.end();i++)
		//�������� ��������� � �������
		if((*i).second->UpdatePointersWithDatabase(this)) return FALSE;
	return TRUE;
}

CObject* CDatabase::RetrieveObject(DWORD ID)
{
	//�������� ������
	std::map<DWORD,CObject*>::iterator i=mObjectsMap.find(ID);
	if(i!=mObjectsMap.end())
		return (*i).second;
	return NULL;
}
