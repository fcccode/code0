#ifndef QS_EDIT_FILE_H
#define QS_EDIT_FILE_H

/*
���� �������� ����������� ������, ������������ ��������� � ��������������� ������.
*/

#include "object.h"

class CEditedFile : public CObject
{
protected:
	//��� �����
	TCHAR mszFileName[MAX_PATH];
	//��������� �� ������ �������� �����
	PVOID mpFileMemory;
	//������ �����
	UINT mFileSize;

	//�����������
	CEditedFile();

public:
	//������� ����
	static CEditedFile* Open(LPCTSTR szFile);

	//����������
	virtual ~CEditedFile();

	//�������� ��� �����
	LPCTSTR GetFileName();

	//�������� ��������� �� ������ �����
	PVOID GetMemory();
	//�������� ������ �����
	UINT GetSize();
};

#endif
