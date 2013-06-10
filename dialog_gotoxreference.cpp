#include "dialog_gotoxreference.h"
#include "xreference.h"
#include "usercontrol_list.h"
#include "application.h"
#include "listing_string.h"
#include "resource.h"

// class CListElement

class CDialogGoToXReferenceListElement : public CUserControlListElement
{
	friend class CDialogGoToXReference;
protected:
	//��������� �� ����� �������
	CDialogGoToXReference* mpDialog;
	//�������� ������������ ������
	CAddressData::ADELI mXReferenceIterator;

public:
	CDialogGoToXReferenceListElement(CDialogGoToXReference* pDialog,CAddressData::ADELI XReferenceIterator)
	{
		//��������� ��������� � ��������
		mpDialog=pDialog;
		mXReferenceIterator=XReferenceIterator;
	}

	virtual VOID Draw(HDC hdc,RECT* pRect)
	{
		//�������� ������������ ������
		CXReference* pXReference=(CXReference*)(*mXReferenceIterator).second;

		//������������ ����� ��� ������������ ������
		pXReference->Print(&mpDialog->mListing);
		//���������� �����
		LPCTSTR szText=mpDialog->mListing.GetString()+mpDialog->mListing.GetColumnMargin(LISTING_COLUMN_COMMENTS)+2;
		TextOut(hdc,pRect->left,pRect->top,szText,_tcslen(szText));
	}

	virtual UINT GetHeight()
	{
		//������� ������ ������
		return gApplication.mFontCodeHeight;
	}
};

// class CDialogGoToXReference

BOOL CDialogGoToXReference::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			//������� ������
			mpList=new CUserControlList(IDC_LIST_XREFERENCES,GetDlgItem(mhWindow,IDC_LIST_XREFERENCES));
			//��������� ������
			CAddressData::ADEL* pElements=mpAddressData->GetElements();
			CAddressData::ADELI Begin=pElements->lower_bound(ADET_XREFERENCE),End=pElements->upper_bound(ADET_XREFERENCE);
			for(CAddressData::ADELI i=Begin;i!=End;i++)
				mpList->AddElement(new CDialogGoToXReferenceListElement(this,i));
		}
		return TRUE;
	case WM_DESTROY:
		//������� ������
		SafeDelete(mpList);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_GO:								//������ "Go"
			//�������� ����� ���������� ������
			*mpAddress=((CXReference*)(*((CDialogGoToXReferenceListElement*)mpList->GetSelectedElement())->mXReferenceIterator).second)->GetAddressFrom();
			EndDialog(mhWindow,TRUE);
			return TRUE;
		case IDCANCEL:										//������ Cancel
			EndDialog(mhWindow,FALSE);
			return TRUE;
		case IDC_LIST_XREFERENCES:				//������ ������
			//���� ����������� � ���, ��� ��������� � ������ ����������
			if(HIWORD(wParam)==LBN_SELCHANGE && mpList->IsExistSelection())
				//�������� ������ "Go"
				EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_GO),TRUE);
			return TRUE;
		}
		return FALSE;
	}
	//���� �� �� ������������ ���������, ��, �����, ��� ���������� ������
	if(mpList) return mpList->ProcessMessages(Msg,wParam,lParam);
	//����� ������ �� ������
	return FALSE;
}

BOOL CDialogGoToXReference::Show(HWND hWndParent,CAddressData* pAddressData,CAddressMap* pAddressMap,CYBER_ADDRESS* pAddress)
{
	//���������������� ��������� �� ������
	mpList=NULL;
	//���������������� �������
	mListing.SetAddressMap(pAddressMap);
	//��������� ���������
	mpAddressData=pAddressData;
	mpAddress=pAddress;

	//������� ������
	return Ask(MAKEINTRESOURCE(IDD_DIALOG_GO_TO_XREFERENCE),hWndParent);
}
