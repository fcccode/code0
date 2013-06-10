#ifndef QS_DIALOG_LIST_IMPORTS_H
#define QS_DIALOG_LIST_IMPORTS_H

/*
���� �������� ����������� ������ ������� ������ �������.
*/

#include "dialog.h"
#include "usercontrol_list.h"
#include "explored_image.h"
#include "image_list_imports.h"

class CDialogListImports : public CDialog
{
protected:
	//����� �������� ������
	class CListElement : public CUserControlListElement
	{
	public:
		//���� - ��� ����������?
		BOOL mIsLibrary;
		union
		{
			//��������� �� ����������
			CImageListImports::CImportLibrary* mpLibrary;
			//��������� �� �������
			CImageListImports::CImportFunction* mpFunction;
		};

		//������������ � ����������
		CListElement(CImageListImports::CImportFunction* pFunction);
		CListElement(CImageListImports::CImportLibrary* pLibrary);
		virtual ~CListElement();

		//���������� ������� ������
		virtual VOID Draw(HDC hdc,RECT* pRect);
		//�������� ������ ��������
		virtual UINT GetHeight();
	};

protected:
	//��������� �� ������ ������ �������
	CImageListImports* mpImportsList;
	//������ �������
	CUserControlList* mpList;
	//��������� �� �����
	CExploredImage* mpExploredImage;
	//��������� �� �����
	CYBER_ADDRESS* mpAddress;

protected:
	//������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//��������� ���������� ������
	VOID FillList(LPCTSTR szFilter);

public:
	//����� �������
	//������������ �����, � ������� ����� �������
	BOOL Show(CExploredImage* pExploredImage,CYBER_ADDRESS* pAddress,HWND hWndParent);
};

#endif
