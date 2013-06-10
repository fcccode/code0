#include "windows.h"

#ifdef CODE0_COMPILE_DUMPING_TOOL

#include "tool_dumper.h"
#include "fileformat_dump.h"
#include "application.h"
#include "resource.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <list>

//����� ������� ������ ��������
class CToolDumper::CSelectProcessDialog : public CDialog
{
protected:
	//����� �������� ������
	class CListElement;
protected:
	//������ ���������
	CUserControlList* mpList;
	//������������� ���������� ��������
	DWORD mProcessID;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//�������� ������
	VOID UpdateList();

public:
	//�������� ������
	BOOL Show(HWND hWndParent,DWORD* pProcessID);
};

//����� �������� ������
class CToolDumper::CSelectProcessDialog::CListElement : public CUserControlListElement
{
public:
	//���������� � ��������
	DWORD mProcessID;																	//������������� ��������
	DWORD mParentProcessID;														//������������� ������������� ��������
	TCHAR mszBaseName[MAX_PATH];											//��� ����� ��������
	TCHAR mszFullName[MAX_PATH];											//������ ��� ����� ��������

	TCHAR mszAccountDomainName[MAX_PATH];							//��� ������ ������� ������
	TCHAR mszAccountName[MAX_PATH];										//��� ������� ������ ������������

	PROCESS_MEMORY_COUNTERS mMemoryInfo;							//���������� �� ������������� ������
	BOOL mMemoryInfoExist;														//������������ �� ���������� � ������

public:
	//�����������
	CListElement(PROCESSENTRY32* pProcess);
	//���������� �������
	virtual VOID Draw(HDC hdc,RECT* pRect);
	//�������� ������ ��������
	virtual UINT GetHeight();
	//������� ���������� � �������� � ����
	VOID ViewInformation(HWND hWnd);
};

//���������� ���������
BOOL CToolDumper::CSelectProcessDialog::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ������ ���������
		mpList=new CUserControlList(IDC_LIST_PROCESSES,GetDlgItem(mhWindow,IDC_LIST_PROCESSES));
		//�������� ������
		UpdateList();
		return TRUE;
	case WM_CLOSE:
		//��������� ������
		EndDialog(mhWindow,FALSE);
		return TRUE;
	case WM_DESTROY:
		//������� ������ ���������
		SafeDelete(mpList);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_DUMP_PROCESS_MEMORY:	//������ "Dump"
			//��������� ������������� ���������� ��������
			mProcessID=((CListElement*)mpList->GetSelectedElement())->mProcessID;
			//��������� ������
			EndDialog(mhWindow,TRUE);
			return TRUE;
		case IDCANCEL:								//������ "Cancel"
			//��������� ������
			EndDialog(mhWindow,FALSE);
			return TRUE;
		case IDC_BUTTON_UPDATE:				//������ "Update"
			//�������� ������ ���������
			UpdateList();
			return TRUE;
		case IDC_LIST_PROCESSES:			//������ ���������
			//���� ����������� � ���, ��� ��������� � ������ ����������
			if(HIWORD(wParam)==LBN_SELCHANGE && mpList->IsExistSelection())
			{
				//�������� ��������� �� ������� ������
				CListElement* pElement=(CListElement*)mpList->GetSelectedElement();
				//������� �������������� ����������
				HWND hLabelInformation=GetDlgItem(mhWindow,IDC_STATIC_PROCESS_INFORMATION);
				pElement->ViewInformation(hLabelInformation);
				//�������� �����
				EnableWindow(hLabelInformation,TRUE);
				//�������� ������ "Dump"
				EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_DUMP_PROCESS_MEMORY),TRUE);
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
	//�����, ��������� ��������� � ������
	if(mpList) return mpList->ProcessMessages(Msg,wParam,lParam);
	//����� ������ �� ������
	return FALSE;
}

//�������� ������ ���������
VOID CToolDumper::CSelectProcessDialog::UpdateList()
{
	//�������� ������
	mpList->Clear();
	//������� ����� ��� ����� ���������� � ��������
	HWND hLabelInformation=GetDlgItem(mhWindow,IDC_STATIC_PROCESS_INFORMATION);
	SetWindowText(hLabelInformation,TEXT("(no process is being selected)"));
	//��������� �����
	EnableWindow(hLabelInformation,FALSE);
	//��������� ������ "Dump"
	EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_DUMP_PROCESS_MEMORY),FALSE);

	//�������� ������ ��� ������������ ���������
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hSnapshot==INVALID_HANDLE_VALUE) return;

	//��������� ��� ������������ ���������
	PROCESSENTRY32 Process;
	Process.dwSize=sizeof(PROCESSENTRY32);

	//�������� ������ �������
	if(Process32First(hSnapshot,&Process))
		do
		{
			//�������� ���������� � �������� � ������
			mpList->AddElement(new CListElement(&Process));
		}
		while(Process32Next(hSnapshot,&Process));

	//��������� ������������
	CloseHandle(hSnapshot);
}

//�������� ������
BOOL CToolDumper::CSelectProcessDialog::Show(HWND hWndParent,DWORD* pProcessID)
{
	//���������������� ��������� �� ������ ���������
	mpList=NULL;
	//������� ������
	if(Ask(MAKEINTRESOURCE(IDD_DIALOG_TOOL_DUMPER_SELECT_PROCESS),hWndParent))
	{
		//������� ������������� ���������� ��������
		*pProcessID=mProcessID;
		//������ �������� �������
		return TRUE;
	}
	//������� ������
	return FALSE;
}

CToolDumper::CSelectProcessDialog::CListElement::CListElement(PROCESSENTRY32* pProcess)
{
	//��������� ����������
	mProcessID=pProcess->th32ProcessID;
	mParentProcessID=pProcess->th32ParentProcessID;
	_tcscpy(mszBaseName,pProcess->szExeFile);

	//�������� �������������� ����������
	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,mProcessID);
	//���� ������ ��������� ���������� �� ������� ������
	BOOL AccountResulted=FALSE;
	if(hProcess)
	{
		//�������� ������ ��� ����� ��������
		GetProcessImageFileName(hProcess,mszFullName,(MAX_PATH-1)*sizeof(TCHAR));
		mszFullName[MAX_PATH-1]=0;

		//�������� ���������� �� ������������� ������ ���������
		mMemoryInfoExist=GetProcessMemoryInfo(hProcess,&mMemoryInfo,sizeof(mMemoryInfo));

		//�������� ������ ������ ��������
		HANDLE hProcessToken;
		if(OpenProcessToken(hProcess,TOKEN_QUERY,&hProcessToken))
		{
			//�������� ���������� � ������������
			DWORD NeededSize;
			if(!GetTokenInformation(hProcessToken,TokenUser,NULL,0,&NeededSize))
			{
				TOKEN_USER* pUserInfo=(TOKEN_USER*)new BYTE[NeededSize];
				if(GetTokenInformation(hProcessToken,TokenUser,pUserInfo,NeededSize,&NeededSize))
				{
					//�������� ��� ������� ������, ��� ������� �������� �������
					DWORD AccountNameLength=MAX_PATH;
					DWORD AccountDomainNameLength=MAX_PATH;
					SID_NAME_USE AccountUse;
					if(LookupAccountSid(NULL,pUserInfo->User.Sid,mszAccountName,&AccountNameLength,mszAccountDomainName,&AccountDomainNameLength,&AccountUse))
						//���������� ���� ����������
						AccountResulted=TRUE;
				}
				delete [] (BYTE*)pUserInfo;
			}

			CloseHandle(hProcessToken);
		}

		//������� ��������� ��������
		CloseHandle(hProcess);
	}
	else
	{
		mszFullName[0]=0;
		mMemoryInfoExist=FALSE;
	}
	//���� ������� � ���������� ����� ������� ������, �������� ������
	if(!AccountResulted)
	{
		mszAccountName[0]=0;
		mszAccountDomainName[0]=0;
	}
}

//���������� ������� ������
VOID CToolDumper::CSelectProcessDialog::CListElement::Draw(HDC hdc,RECT* pRect)
{
	//���������� ������ ������� ������������� � ��� ��������
	SelectFont(hdc,gApplication.mhFontCodeBold);
	static TCHAR s[MAX_PATH*2+0x30];
	_stprintf(s,TEXT("%s (%X)"),mszBaseName,mProcessID);
	TextOut(hdc,pRect->left+LIST_ELEMENT_MARGIN_HEADER,pRect->top,s,_tcslen(s));
	//����� �������� ������� �������
	SelectFont(hdc,gApplication.mhFontCode);
	//���������� ������ ��� ����� ��������
	if(mszFullName[0])
		_stprintf(s,TEXT("File: \"%s\""),mszFullName);
	else
		_tcscpy(s,TEXT("File: (unknown)"));
	TextOut(hdc,pRect->left,pRect->top+gApplication.mFontCodeHeight,s,_tcslen(s));
	//���������� ���������� �� ������� ������, ���� ��� ����
	if(mszAccountName[0])
		_stprintf(s,TEXT("User account: %s\\%s"),mszAccountDomainName,mszAccountName);
	else
		_tcscpy(s,TEXT("User account information: (unknown)"));
	TextOut(hdc,pRect->left,pRect->top+gApplication.mFontCodeHeight*2,s,_tcslen(s));
}
//�������� ������ ��������
UINT CToolDumper::CSelectProcessDialog::CListElement::GetHeight()
{
	//������� ������ ��� �����
	return gApplication.mFontCodeHeight*3;
}
//������� ���������� � �������� � ����
VOID CToolDumper::CSelectProcessDialog::CListElement::ViewInformation(HWND hWnd)
{
	//������������ ����������
	if(mMemoryInfoExist)
	{
		TCHAR s[0x400];
		_stprintf(s,TEXT("Page fault count:\t\t\t%Xh\nPeak working set size:\t\t\t%Xh\nCurrent working set size:\t\t%Xh\nPeak paged pool usage:\t\t\t%Xh\nCurrent paged pool usage:\t\t%Xh\nPeak nonpaged pool usage:\t\t%Xh\nCurrent nonpaged pool usage:\t\t%Xh\nPage file usage:\t\t\t\t%Xh\nPeak page file usage:\t\t\t%Xh"),
			mMemoryInfo.PageFaultCount,mMemoryInfo.PeakWorkingSetSize,mMemoryInfo.WorkingSetSize,mMemoryInfo.QuotaPeakPagedPoolUsage,mMemoryInfo.QuotaPagedPoolUsage,mMemoryInfo.QuotaPeakNonPagedPoolUsage,mMemoryInfo.QuotaNonPagedPoolUsage,mMemoryInfo.PagefileUsage,mMemoryInfo.PeakPagefileUsage
			);
		//������� ���������� ����
		SetWindowText(hWnd,s);
	}
	else
		//�������, ��� ���������� ���
		SetWindowText(hWnd,TEXT("Information for selected process are not available."));
}

VOID CToolDumper::Start()
{
	//�������� ������
	CSelectProcessDialog Dialog;
	if(!Dialog.Show(gApplication.mhMainWindow,&mProcessID)) return;

	//������� �������
	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,mProcessID);
	if(!hProcess)
	{
		MessageBox(NULL,TEXT("Could not open handle of selected process.\nNo rights to perform the operation."),TEXT("Dumping process"),MB_ICONSTOP);
		return;
	}

	//��������� ���� ��� ���������� �����
	TCHAR szFile[MAX_PATH]={0};
	{
		OPENFILENAME ofn={sizeof(OPENFILENAME)};
		ofn.hwndOwner=gApplication.mhMainWindow;
		ofn.lpstrFile=szFile;
		ofn.nMaxFile=MAX_PATH;
		ofn.lpstrFilter=TEXT("Code0 Dump Files (*.dump)\0*.dump\0");
		ofn.lpstrTitle=TEXT("Select File to Save Dump File");
		ofn.Flags=OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST |OFN_HIDEREADONLY;
		if(!GetSaveFileName(&ofn))
		{
			//������� ������� � ���������
			CloseHandle(hProcess);
			return;
		}
	}

	//������� ���� �����
	HANDLE hFile=CreateFile(szFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		MessageBox(NULL,TEXT("Could not create file on specified location."),TEXT("Dumping process"),MB_ICONSTOP);
		return;
	}

	//�������� ������ �������� � ������� ������ ��������
	std::list<FILE_DUMP_REGION> RegionsList;
	DWORD MemorySize=0;
	{
		//���� �� ��������
		PBYTE pRegion=0;
		do
		{
			//�������� ���������� � �������
			MEMORY_BASIC_INFORMATION RegionInfo;
			if(!VirtualQueryEx(hProcess,pRegion,&RegionInfo,sizeof(RegionInfo))) break;

			//�������� ������ � ������
			FILE_DUMP_REGION Region;
			Region.Flags=0;
			Region.BaseAddress=(DWORD)pRegion;
			Region.Size=RegionInfo.RegionSize;
			Region.State=RegionInfo.State;
			Region.Protect=RegionInfo.Protect;
			Region.DataOffset=RegionInfo.State==MEM_COMMIT ? MemorySize : 0;
			RegionsList.push_back(Region);

			//��������� ������� ����������� ������, ���� ������ ������� �������������
			if(RegionInfo.State==MEM_COMMIT)
				MemorySize+=RegionInfo.RegionSize;

			//������� � ���������� �������
			pRegion+=RegionInfo.RegionSize;
		}
		while(pRegion);
	}

	//�������� ������ �������
	std::list<FILE_DUMP_THREAD> ThreadsList;
	{
		//������� ������ ��������
		HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
		if(hSnapshot==INVALID_HANDLE_VALUE)
			MessageBox(NULL,TEXT("Thread list in dump is not available."),TEXT("Dumping process"),MB_ICONEXCLAMATION);
		else
		{
			//���� �� �������
			THREADENTRY32 ThreadEntry;
			ThreadEntry.dwSize=sizeof(ThreadEntry);

			if(Thread32First(hSnapshot,&ThreadEntry))
				do
				{
					//���� ����� ����������� ������� �������� (��� ��� ������������� ��� ������)
					if(ThreadEntry.th32OwnerProcessID==mProcessID)
					{
						//�������� ���������� � ������ � ������
						FILE_DUMP_THREAD Thread;
						Thread.Flags=0;
						Thread.ThreadID=ThreadEntry.th32ThreadID;
						Thread.UsageCount=ThreadEntry.cntUsage;
						Thread.PriorityBase=ThreadEntry.tpBasePri;
						Thread.PriorityDelta=ThreadEntry.tpDeltaPri;
						ThreadsList.push_back(Thread);
					}
				}
				while(Thread32Next(hSnapshot,&ThreadEntry));
			//������� ������ ��������
			CloseHandle(hSnapshot);
		}
	}

	//�������� ������ �������
	std::list<FILE_DUMP_MODULE> ModulesList;
	{
		//������� ������ ��������
		HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,mProcessID);
		if(hSnapshot==INVALID_HANDLE_VALUE)
			MessageBox(NULL,TEXT("Modules list in dump is not available."),TEXT("Dumping process"),MB_ICONEXCLAMATION);
		else
		{
			//���� �� �������
			MODULEENTRY32 ModuleEntry;
			ModuleEntry.dwSize=sizeof(ModuleEntry);

			if(Module32First(hSnapshot,&ModuleEntry))
				do
				{
					//�������� ���������� � ������ � ������
					FILE_DUMP_MODULE Module;
					ZeroMemory(&Module,sizeof(Module));
					Module.Flags=0;
					Module.BaseAddress=(DWORD)ModuleEntry.modBaseAddr;
					Module.BaseSize=ModuleEntry.modBaseSize;
					Module.UsageGlobalCount=ModuleEntry.GlblcntUsage;
					Module.UsageProcessCount=ModuleEntry.ProccntUsage;
					_tcscpy(Module.ModuleName,ModuleEntry.szModule);
					_tcscpy(Module.ModulePath,ModuleEntry.szExePath);
					ModulesList.push_back(Module);
				}
				while(Module32Next(hSnapshot,&ModuleEntry));
			//������� ������ ��������
			CloseHandle(hSnapshot);
		}
	}

	//������������ � �������� ��������� ����� �����
	DWORD Written;
	DWORD HeadersSize;
	{
		FILE_DUMP_HEADER Header;
		Header.Magics[0]=FILE_DUMP_MAGIC0;
		Header.Magics[1]=FILE_DUMP_MAGIC1;
		Header.RegionsCount=RegionsList.size();
		Header.ThreadsCount=ThreadsList.size();
		Header.ModulesCount=ModulesList.size();
		HeadersSize=sizeof(FILE_DUMP_HEADER)+Header.RegionsCount*sizeof(FILE_DUMP_REGION)+Header.ThreadsCount*sizeof(FILE_DUMP_THREAD)+Header.ModulesCount*sizeof(FILE_DUMP_MODULE);
		Header.FileSize=HeadersSize+MemorySize;
		WriteFile(hFile,&Header,sizeof(Header),&Written,NULL);
	}

	//�������� ������ ��������, �������� ������� � �������� ������� ����������
	{
		for(std::list<FILE_DUMP_REGION>::iterator i=RegionsList.begin();i!=RegionsList.end();i++)
		{
			(*i).DataOffset+=HeadersSize;
			WriteFile(hFile,&*i,sizeof(FILE_DUMP_REGION),&Written,NULL);
		}
	}
	//�������� ������ �������
	{
		for(std::list<FILE_DUMP_THREAD>::iterator i=ThreadsList.begin();i!=ThreadsList.end();i++)
			WriteFile(hFile,&*i,sizeof(FILE_DUMP_THREAD),&Written,NULL);
	}
	//�������� ������ �������
	{
		for(std::list<FILE_DUMP_MODULE>::iterator i=ModulesList.begin();i!=ModulesList.end();i++)
			WriteFile(hFile,&*i,sizeof(FILE_DUMP_MODULE),&Written,NULL);
	}

	//�������� ������ ��������
	{
		//���� �� ��������
		for(std::list<FILE_DUMP_REGION>::iterator i=RegionsList.begin();i!=RegionsList.end();i++)
		{
			//�������� ������
			FILE_DUMP_REGION Region=*i;
			//���� ������ ������������
			if(Region.State==MEM_COMMIT)
			{
				//�������� � �������� ������ ������� �����������
				PBYTE pEndAddress=(PBYTE)(Region.BaseAddress+Region.Size);
				static BYTE Buffer[0x1000];
				for(PBYTE pAddress=(PBYTE)Region.BaseAddress;pAddress<pEndAddress;pAddress+=0x1000)
				{
					ReadProcessMemory(hProcess,pAddress,Buffer,0x1000,NULL);
					WriteFile(hFile,Buffer,0x1000,&Written,NULL);
				}
			}
		}
	}

	//������� ����
	CloseHandle(hFile);

	//������� ��������� ��������
	CloseHandle(hProcess);

	//�������� �� �������� ����������
	MessageBox(NULL,TEXT("Dumping process has been finished succesfully."),TEXT("Dumping process"),MB_ICONINFORMATION);
}

#endif
