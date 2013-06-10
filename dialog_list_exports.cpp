#include "dialog_list_exports.h"
#include "image_extend_object.h"
#include "dialog_gotoxreference.h"
#include "application.h"
#include "strings.h"
#include "resource.h"

// subclass CDialogListExports::CListElement

CDialogListExports::CListElement::CListElement(CImageListExports::CExportFunction* pFunction)
{
	//���������� ���������
	mpFunction=pFunction;
	mpFunction->AddRef();
}

CDialogListExports::CListElement::~CListElement()
{
	//���������� ������
	mpFunction->Release();
}

VOID CDialogListExports::CListElement::Draw(HDC hdc,RECT* pRect)
{
	//���������� ��� �������
	SelectFont(hdc,gApplication.mhFontCode);
	//�������� � ���������� ������� � ���
	static TCHAR szText[LISTING_LINE_LENGTH];
	LPCTSTR szName=mpFunction->GetName();
	if(szName)
		_stprintf(szText,TEXT("% 5u  %s"),mpFunction->GetOrdinal(),szName);
	else
		_stprintf(szText,TEXT("% 5u  (no name)"),mpFunction->GetOrdinal());
	TextOut(hdc,pRect->left,pRect->top,szText,_tcslen(szText));

	//�������� ������ �� ���� ������
	SIZE Size;
	GetTextExtentPoint32(hdc,szText,_tcslen(szText),&Size);
	Size.cx+=pRect->left;

	//���� ������� - ���������
	if(mpFunction->IsLocal())
		//���������� �����
		_stprintf(szText,TEXT(" %08X"),mpFunction->GetAddress());
	//����� ������� - �������
	else
		//���������� �������
		_stprintf(szText,TEXT(" - forward to %s"),mpFunction->GetForwardName());
	TextOut(hdc,Size.cx,pRect->top,szText,_tcslen(szText));
}

UINT CDialogListExports::CListElement::GetHeight()
{
	//������� ������ ������
	return gApplication.mFontCodeHeight;
}

// class CDialogListExports

BOOL CDialogListExports::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			//������� ������ ��������
			mpList=new CUserControlList(IDC_LIST_EXPORTS,GetDlgItem(mhWindow,IDC_LIST_EXPORTS));
			//��������� ������
			FillList(TEXT(""));
		}
		return TRUE;
	case WM_DESTROY:
		//������� ������
		SafeDelete(mpList);
		return TRUE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_LIST_EXPORTS:
				//���� ����������� � ���, ��� ��������� � ������ ����������
				if(HIWORD(wParam)==LBN_SELCHANGE && mpList->IsExistSelection())
				{
					CListElement* pElement=(CListElement*)mpList->GetSelectedElement();
					//�������� ���� �����������
					BOOL IsLocal=pElement->mpFunction->IsLocal();
					//�������� ��� ��������� ������ "Go to export"
					EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_GOTO_EXPORT),IsLocal);
					//�������� ��� ��������� ������ "X references"
					EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_XREFERENCES),IsLocal);
				}
				return TRUE;
			case IDC_EDIT_FILTER:							// ���� �������
				//���� ����������� � ���, ��� ����� ���������
				if(HIWORD(wParam)==EN_CHANGE)
				{
					//�������� ����� �������
					HWND hEdit=GetDlgItem(mhWindow,IDC_EDIT_FILTER);
					INT Length=GetWindowTextLength(hEdit)+1;
					LPTSTR szFilter=new TCHAR[Length];
					GetWindowText(hEdit,szFilter,Length);
					//��������� ������
					FillList(szFilter);
					//���������� ������
					SafeDeleteMassive(szFilter);
				}
				return TRUE;
			case IDC_BUTTON_GOTO_EXPORT:			// ������ "Go to export"
				{
					//�������� ����� ���������� �������� (�� ������ ���� ��������� ��������)
					CListElement* pElement=(CListElement*)mpList->GetSelectedElement();
					*mpAddress=pElement->mpFunction->GetAddress();
					//��������� ������
					EndDialog(mhWindow,TRUE);
				}
				return TRUE;
			case IDC_BUTTON_XREFERENCES:			// ������ "X references"
				{
					//�������� ����� ���������� �������� (�� ������ ���� ��������� ��������)
					CListElement* pElement=(CListElement*)mpList->GetSelectedElement();

					//�������� ������ ������
					CAddressData* pAddressData=mpExploredImage->GetAddressData(pElement->mpFunction->GetAddress());
					if(pAddressData)
					{
						//���������, ��� ���� �� ���� ������ ����
						std::multimap<DWORD,CAddressDataElement*>* pAddressElements=pAddressData->GetElements();
						pAddressData->Release();
						if(pAddressElements->find(ADET_XREFERENCE)!=pAddressElements->end())
						{
							//������� ������ ������������ ������
							CDialogGoToXReference Dialog;
							CAddressMap* pAddressMap=mpExploredImage->GetAddressMap();
							if(Dialog.Show(mhWindow,pAddressData,pAddressMap,mpAddress))
								//��������� ������
								EndDialog(mhWindow,TRUE);
							pAddressMap->Release();
							//��������� ���������
							return TRUE;
						}
					}
					//������� ��������� � ���, ��� � ������ ��� ������������ ������
					MessageBox(mhWindow,TEXT("Can't find any x-references for this export function."),TEXT("Find x-references"),MB_ICONSTOP);
				}
				return TRUE;
			case IDCANCEL:										// ������ "Close"
				EndDialog(mhWindow,FALSE);
				return TRUE;
			}
			return FALSE;
		}
		return TRUE;
	}
	//���� �� �� ������������ ���������, ��, �����, ��� ���������� ������
	if(mpList) return mpList->ProcessMessages(Msg,wParam,lParam);
	//����� ������ �� ������
	return FALSE;
}

//��������� ���������� ������
VOID CDialogListExports::FillList(LPCTSTR szFilter)
{
	//�������� ������
	mpList->Clear();
	//��������� ������
	EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_GOTO_EXPORT),FALSE);
	EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_XREFERENCES),FALSE);
	//��������� ������ �������
	std::list<CImageListExports::CExportFunction*>* pFunctionsList=mpExportsList->GetList();
	for(std::list<CImageListExports::CExportFunction*>::iterator i=pFunctionsList->begin();i!=pFunctionsList->end();i++)
	{
		//�������� �������
		CImageListExports::CExportFunction* pFunction=*i;
		//��������� �������
		LPCTSTR szName=pFunction->GetName();
		if(!*szFilter || (szName && IsAgreeFilter(szName,szFilter)))
				//�������� ������� � ������
				mpList->AddElement(new CListElement(pFunction));
	}
}

//�������� ������ �������� � ������� ������
BOOL CDialogListExports::Show(CExploredImage* pExploredImage,CYBER_ADDRESS* pAddress,HWND hWndParent)
{
	//�������� ������ ��������
	std::map<UINT,CObject*>* pExtendMap=pExploredImage->GetExtendMap();
	std::map<UINT,CObject*>::iterator i=pExtendMap->find(IMAGE_EXTEND_LIST_EXPORTS);
	if(i==pExtendMap->end())
	{
		MessageBox(NULL,TEXT("Exports list for this image is not supported."),TEXT("Exports List"),MB_ICONSTOP);
		return FALSE;
	}
	mpExportsList=(CImageListExports*)(*i).second;

	//��������� ����� ������
	mpExploredImage=pExploredImage;
	//��������� ����� ����������-����������
	mpAddress=pAddress;
	//���������������� ��������� �� ������
	mpList=NULL;
	//������� ������
	return Ask(MAKEINTRESOURCE(IDD_DIALOG_LIST_EXPORTS),hWndParent);
}
