#include "object.h"

// class CObject

CObject::CObject()
{
	//���������� ��������� ���������� ������
	mObjectReferencesCount=1;
}

CObject::~CObject()
{
	//������ �� ������, ��� ����������
	//��� ��������� ����������� ������������
}

VOID CObject::AddRef()
{
	//��������� ���������� ������
	mObjectReferencesCount++;
}

VOID CObject::Release()
{
	_ASSERTE(mObjectReferencesCount!=0);
	//��������� ���������� ������
	mObjectReferencesCount--;
	//���� ���������� ������ ����� 0
	if(!mObjectReferencesCount)
		//������� ������
		delete this;
}
