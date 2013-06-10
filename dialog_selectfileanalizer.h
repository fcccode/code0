#ifndef QS_DIALOG_SELECTFILEANALIZER_H
#define QS_DIALOG_SELECTFILEANALIZER_H

/*
���� �������� ����������� ������� ������ ����������� �����.
*/

#include "dialog.h"
#include "file_parser.h"
#include "usercontrol_list.h"
#include <list>

class CDialogSelectFileAnalizer : public CDialog
{
protected:
	class CListElement;
protected:
	//��� �����
	LPCTSTR mszFileName;
	//��������� �� �������� �����
	PVOID mpFileMapping;
	//������ �����
	DWORD mFileSize;

	//������ ������������
	std::list<CFileParser*> mParsersList;
	//������ ������������ � �������
	CUserControlList* mpList;
	//��������� ���������� (��������)
	std::list<CFileParser*>::iterator mSelectedParserIterator;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

	//�������� ���������� � ������ � ����
	VOID UpdateMemoryInformation();

public:
	//�������� ������ (������������ ����������� ����������, ������� ���������� ��������� ������ ������� � ������� Reelase)
	CFileParser* Show(HWND hWndParent,LPCTSTR szFileName,PVOID pFileMapping,DWORD FileSize);
};

#endif
