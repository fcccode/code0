#ifndef QS_DIALOG_LIST_EXPORTS_H
#define QS_DIALOG_LIST_EXPORTS_H

/*
���� �������� ����������� ������ ������� ������ ��������.
*/

#include "dialog.h"
#include "usercontrol_list.h"
#include "explored_image.h"
#include "image_list_exports.h"

class CDialogListExports : public CDialog
{
protected:
	//����� �������� ������
	class CListElement : public CUserControlListElement
	{
	public:
		//��������� �� �������
		CImageListExports::CExportFunction* mpFunction;

		//����������� � ����������
		CListElement(CImageListExports::CExportFunction* pFunction);
		virtual ~CListElement();

		//���������� ������� ������
		virtual VOID Draw(HDC hdc,RECT* pRect);
		//�������� ������ ��������
		virtual UINT GetHeight();
	};

protected:
	//��������� �� ������ ������ ��������
	CImageListExports* mpExportsList;
	//������ ��������
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
