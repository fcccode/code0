#include "dialog_selectfileanalizer.h"
#include "application.h"
#include "resource.h"
#include <commctrl.h>

// class CListElement

class CDialogSelectFileAnalizer::CListElement : public CUserControlListElement
{
protected:
	//�������� �� ���������� � ������ ������������
	std::list<CFileParser*>::iterator mFileParserIterator;
	//����������
	CFileParser* mpFileParser;
	//��������� ���������� �� �����������
	FILE_PARSER_ABOUT mFileParserAbout;
	//����������� ����� ������ ��� �����������
	DWORD mBufferSize;

public:
	//�����������
	CListElement(std::list<CFileParser*>::iterator FileParserIterator)
	{
		//��������� ��������� �� ���������� � ��������
		mFileParserIterator=FileParserIterator;
		mpFileParser=*mFileParserIterator;
		//�������� ���������� �� �����������
		mpFileParser->About(&mFileParserAbout);
		//�������� ����������� ����� ������
		mBufferSize=mpFileParser->GetNeededMemorySize();
	}

	virtual VOID Draw(HDC hdc,RECT* pRect)
	{
		//������� ������ �����
		SelectFont(hdc,gApplication.mhFontCodeBold);
		//���������� ��� �����������
		TextOut(hdc,pRect->left+LIST_ELEMENT_MARGIN_HEADER,pRect->top,mFileParserAbout.szName,_tcslen(mFileParserAbout.szName));
		//������� ������� �����
		SelectFont(hdc,gApplication.mhFontCode);
		//���������� ������
		TCHAR s[0x80];
		_stprintf(s,TEXT("Version: %d.%d"),HIWORD(mFileParserAbout.Version),LOWORD(mFileParserAbout.Version));
		TextOut(hdc,pRect->left+LIST_ELEMENT_MARGIN_TEXT,pRect->top+gApplication.mFontCodeHeight,s,_tcslen(s));
		//���������� ����������� ����� ������
		_stprintf(s,TEXT("File memory size needed: %d.%02d Kb"),mBufferSize/1024,mBufferSize%1024*100/1024);
		TextOut(hdc,pRect->left+LIST_ELEMENT_MARGIN_TEXT,pRect->top+gApplication.mFontCodeHeight*2,s,_tcslen(s));
	}

	virtual UINT GetHeight()
	{
		//������ ������ - ��� ��� ������� ������
		return gApplication.mFontCodeHeight*3;
	}

	BOOL IsAskSettingsAvailable()
	{
		//������� �����
		return mpFileParser->IsAskSettingsAvaliable();
	}

	BOOL AskSettings(HWND hWndParent)
	{
		//������� ������ �������������� ��������
		return mpFileParser->AskSettings(hWndParent);
	}

	VOID SetDescriptionText(HWND hWnd)
	{
		//������� ����� - ���������� �� �����������
		SetWindowText(hWnd,mFileParserAbout.szDescription);
	}

	std::list<CFileParser*>::iterator GetIterator()
	{
		return mFileParserIterator;
	}
};

// class CDialogSelectFileAnalizer

BOOL CDialogSelectFileAnalizer::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:												//������ "Continue"
			//��������� �������� ����������� �����������
			mSelectedParserIterator=((CListElement*)mpList->GetSelectedElement())->GetIterator();
			EndDialog(mhWindow,TRUE);
			return TRUE;
		case IDCANCEL:										//������ "Cancel"
			EndDialog(mhWindow,FALSE);
			return TRUE;
		case IDC_LIST_FILE_ANALIZERS:			//������ ������������
			//���� ����������� � ���, ��� ��������� � ������ ����������
			if(HIWORD(wParam)==LBN_SELCHANGE && mpList->IsExistSelection())
			{
				//�������� ��������� �� ������� ������
				CListElement* pElement=(CListElement*)mpList->GetSelectedElement();
				//�������� ������ "Continue"
				EnableWindow(GetDlgItem(mhWindow,IDOK),TRUE);
				//�������� ������� ����� � ������� "Continue"
				ShowWindow(GetDlgItem(mhWindow,IDC_STATIC_CONTINUE),SW_SHOW);
				//���������� ��������� ������ "Customize"
				BOOL CustomizeAvailable=pElement->IsAskSettingsAvailable();
				EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_CUSTOMIZE),CustomizeAvailable);
				//������� ����� ��� ����� ����� � "Customize"
				SetDlgItemText(mhWindow,IDC_STATIC_CUSTOMIZE,CustomizeAvailable ? TEXT("To specify additional options for this analizer, click \"Customize...\" button.") : TEXT("This analizer is no have additional options."));
				//������� ����� ��� ����� �������� ����������� � �������� �
				HWND hDescriptionLabel=GetDlgItem(mhWindow,IDC_STATIC_DESCRIPTION);
				pElement->SetDescriptionText(hDescriptionLabel);
				EnableWindow(hDescriptionLabel,TRUE);
				return TRUE;
			}
			return FALSE;
		case IDC_BUTTON_CUSTOMIZE:				//������ "Customize"
			//��������� ������ ��������
			((CListElement*)mpList->GetSelectedElement())->AskSettings(mhWindow);
			return TRUE;
		case IDC_BUTTON_UPDATEMEMORYINFORMATION:			//������ "Update"
			UpdateMemoryInformation();
			return TRUE;
		}
		return FALSE;
	case WM_INITDIALOG:
		{
			//������� ������ ������������
			mpList=new CUserControlList(IDC_LIST_FILE_ANALIZERS,GetDlgItem(mhWindow,IDC_LIST_FILE_ANALIZERS));
			//�������� � ���� �����������
			for(std::list<CFileParser*>::iterator i=mParsersList.begin();i!=mParsersList.end();i++)
				mpList->AddElement(new CListElement(i));

			//�������� ���������� � ������
			UpdateMemoryInformation();
		}
		return TRUE;
	case WM_DESTROY:
		//������� ������ ������������
		SafeDelete(mpList);
		return TRUE;
	}
	//�����, ��������� ��������� � ������
	if(mpList) return mpList->ProcessMessages(Msg,wParam,lParam);
	//����� ������ �� ������
	return FALSE;
}

VOID CDialogSelectFileAnalizer::UpdateMemoryInformation()
{
	//�������� ���������� � ������
	MEMORYSTATUS MemoryStatus;
	GlobalMemoryStatus(&MemoryStatus);
	//������������ �����
	TCHAR s[0x200];
	_stprintf(s,TEXT("Total Physical Memory: %d Mb\nFree Physical Memory: %d Mb\nVirtual Address Space: %d Mb"),MemoryStatus.dwTotalPhys>>20,MemoryStatus.dwAvailPhys>>20,MemoryStatus.dwAvailVirtual>>20);
	SetDlgItemText(mhWindow,IDC_STATIC_MEMORYINFORMATION,s);
	//������� ��������� ������
	HWND hProgressBar=GetDlgItem(mhWindow,IDC_PROGRESS_USINGMEMORY);
	SendMessage(hProgressBar,PBM_SETRANGE,0,MAKELPARAM(0,100));
	SendMessage(hProgressBar,PBM_SETPOS,MemoryStatus.dwMemoryLoad,0);
	//������� ������� �������������
	_stprintf(s,TEXT("%d%%"),MemoryStatus.dwMemoryLoad);
	SetDlgItemText(mhWindow,IDC_STATIC_MEMORYUSINGPERCENT,s);
}

CFileParser* CDialogSelectFileAnalizer::Show(HWND hWndParent,LPCTSTR szFileName,PVOID pFileMapping,DWORD FileSize)
{
	//�������� ������ ���� ������������ ������
	gApplication.GetFileParsers(&mParsersList);

	//������� ��� �����������, ������� �� ����� ���������� ����
	std::list<CFileParser*> newList;
	std::list<CFileParser*>::iterator i;
	for(i=mParsersList.begin();i!=mParsersList.end();++i)
	{
		//�������� ����������
		CFileParser* pFileParser=*i;
		//������� ����������� �������� ������������ �����
		pFileParser->SetFile(pFileMapping,FileSize);
		//���� ���������� ����� ���������� ���� ����
		if(pFileParser->PrepareAnalize())
			//�������� � ����� ������
			newList.push_back(pFileParser);
		else
			//������� ���
			pFileParser->Release();
	}
	//�������� ������
	mParsersList=newList;

	//������ � ������ �������� ������ �����������, ������� ����� ���������� ����

	//���������������� ��������� �� ������ ������������ � �������
	mpList=NULL;
	//��������� ����������
	CFileParser* pFileParser;
	//�������� ������
	if(Ask(MAKEINTRESOURCE(IDD_DIALOG_SELECTFILEANALIZER),hWndParent))
	{
		//�������� ��������� ����������
		pFileParser=*mSelectedParserIterator;
		//������� ��� �� ������
		mParsersList.erase(mSelectedParserIterator);
	}
	else
		pFileParser=NULL;
	//������� ��� ����������� �� ������
	for(i=mParsersList.begin();i!=mParsersList.end();i++)
		(*i)->Release();
	mParsersList.clear();

	//������� ����������
	return pFileParser;
}
