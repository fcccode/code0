#include "dialog_list_imports.h"
#include "image_extend_object.h"
#include "dialog_gotoxreference.h"
#include "application.h"
#include "strings.h"
#include "resource.h"

// subclass CDialogListImports::CListElement

CDialogListImports::CListElement::CListElement(CImageListImports::CImportLibrary* pLibrary)
{
	//���������� ���������
	mpLibrary=pLibrary;
	mpLibrary->AddRef();
	mIsLibrary=TRUE;
}

CDialogListImports::CListElement::CListElement(CImageListImports::CImportFunction* pFunction)
{
	//���������� ���������
	mpFunction=pFunction;
	mpFunction->AddRef();
	mIsLibrary=FALSE;
}

CDialogListImports::CListElement::~CListElement()
{
	//���������� ������ ������
	if(mIsLibrary)
		mpLibrary->Release();
	else
		mpFunction->Release();
}

VOID CDialogListImports::CListElement::Draw(HDC hdc,RECT* pRect)
{
	//���������� ��� ����������/�������
	if(mIsLibrary)
	{
		SelectFont(hdc,gApplication.mhFontCodeBold);
		//�������� ���
		LPCTSTR szName=mpLibrary->GetName();
		TextOut(hdc,pRect->left+16,pRect->top,szName,_tcslen(szName));
	}
	else
	{
		SelectFont(hdc,gApplication.mhFontCode);
		//�������� � ���������� ���
		LPCTSTR szName=mpFunction->GetName();
		TextOut(hdc,pRect->left+32,pRect->top,szName,_tcslen(szName));
	}
}

UINT CDialogListImports::CListElement::GetHeight()
{
	//������� ������ ������
	return gApplication.mFontCodeHeight;
}

// class CDialogListImports

BOOL CDialogListImports::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			//������� ������ �������
			mpList=new CUserControlList(IDC_LIST_IMPORTS,GetDlgItem(mhWindow,IDC_LIST_IMPORTS));
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
			case IDC_LIST_IMPORTS:
				//���� ����������� � ���, ��� ��������� � ������ ����������
				if(HIWORD(wParam)==LBN_SELCHANGE && mpList->IsExistSelection())
				{
					//������� ��� ������ "Go to import" � ����������� �� ���� ���������� ��������
					HWND hButton=GetDlgItem(mhWindow,IDC_BUTTON_GOTO_IMPORT);
					CListElement* pElement=(CListElement*)mpList->GetSelectedElement();
					SetWindowText(hButton,pElement->mIsLibrary ? TEXT("Go to IAT of library") : TEXT("Go to function address in IAT"));
					//�������� ������ "Go to import"
					EnableWindow(hButton,TRUE);
					//�������� ��� ��������� ������ "X references"
					EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_XREFERENCES),!pElement->mIsLibrary);
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
			case IDC_BUTTON_GOTO_IMPORT:			// ������ "Go to import"
				{
					//�������� ����� ���������� ��������
					CListElement* pElement=(CListElement*)mpList->GetSelectedElement();
					if(pElement->mIsLibrary)
						*mpAddress=pElement->mpLibrary->GetAddressIAT();
					else
						*mpAddress=pElement->mpFunction->GetAddressInIAT();
					//��������� ������
					EndDialog(mhWindow,TRUE);
				}
				return TRUE;
			case IDC_BUTTON_XREFERENCES:			// ������ "X references"
				{
					//�������� ����� ���������� �������� (�� ������ ���� ��������)
					CListElement* pElement=(CListElement*)mpList->GetSelectedElement();

					//�������� ������ ������
					CAddressData* pAddressData=mpExploredImage->GetAddressData(pElement->mpFunction->GetAddressInIAT());
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
					MessageBox(mhWindow,TEXT("Can't find any x-references for this import function."),TEXT("Find x-references"),MB_ICONSTOP);
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
VOID CDialogListImports::FillList(LPCTSTR szFilter)
{
	//������ ����������
	mpList->BeginUpdate();
	//�������� ������
	mpList->Clear();
	//��������� ������
	EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_GOTO_IMPORT),FALSE);
	EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_XREFERENCES),FALSE);
	//��������� ������ ������� � ���������
	std::list<CImageListImports::CImportLibrary*>* pLibrariesList=mpImportsList->GetList();
	for(std::list<CImageListImports::CImportLibrary*>::iterator i=pLibrariesList->begin();i!=pLibrariesList->end();i++)
	{
		//���� ������������� ����������
		BOOL LibraryAddedFlag=FALSE;
		//�������� ����������
		CImageListImports::CImportLibrary* pLibrary=*i;
		//��������� ���� ����������
		if(IsAgreeFilterI(pLibrary->GetName(),szFilter))
		{
			//�������� ���������� � ������
			mpList->AddElement(new CListElement(pLibrary));
			//���������� ���� �������������
			LibraryAddedFlag=TRUE;
		}

		//�������� ������ �������
		std::list<CImageListImports::CImportFunction*>* pFunctionsList=(*i)->GetList();
		//���� �� �������� �� ����������
		for(std::list<CImageListImports::CImportFunction*>::iterator j=pFunctionsList->begin();j!=pFunctionsList->end();j++)
		{
			//�������� �������
			CImageListImports::CImportFunction* pFunction=*j;
			//��������� ���
			if(IsAgreeFilter(pFunction->GetName(),szFilter))
			{
				//���� ���������� �� ���������
				if(!LibraryAddedFlag)
				{
					//�������� ���������� � ������
					mpList->AddElement(new CListElement(pLibrary));
					//���������� ���� �������������
					LibraryAddedFlag=TRUE;
				}
				//�������� �������
				mpList->AddElement(new CListElement(pFunction));
			}
		}
	}
	//��������� ����������
	mpList->EndUpdate();
}

//�������� ������ ������� � ������� ������
BOOL CDialogListImports::Show(CExploredImage* pExploredImage,CYBER_ADDRESS* pAddress,HWND hWndParent)
{
	//�������� ������ �������
	std::map<UINT,CObject*>* pExtendMap=pExploredImage->GetExtendMap();
	std::map<UINT,CObject*>::iterator i=pExtendMap->find(IMAGE_EXTEND_LIST_IMPORTS);
	if(i==pExtendMap->end())
	{
		MessageBox(NULL,TEXT("Imports list for this image is not supported."),TEXT("Imports List"),MB_ICONSTOP);
		return FALSE;
	}
	mpImportsList=(CImageListImports*)(*i).second;

	//��������� ����� ������
	mpExploredImage=pExploredImage;
	//��������� ����� ����������-����������
	mpAddress=pAddress;
	//���������������� ��������� �� ������
	mpList=NULL;
	//������� ������
	return Ask(MAKEINTRESOURCE(IDD_DIALOG_LIST_IMPORTS),hWndParent);
}
