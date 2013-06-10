#include "main_window.h"
#include "application.h"
#include "danglewindow_code.h"
#include "danglewindow_hex.h"
#include "danglewindow_danglegroup.h"
#include "dialog_about.h"
#include "dialog_splash.h"
#include "tool_dumper.h"
#include "resource.h"
#include <commctrl.h>

//���������� ��������� � ������� ���� � ������� ����
#define MAIN_MENU_RIGHT_ITEMS_COUNT 3

// class CMainWindow

BOOL CMainWindow::OnCreate(CREATESTRUCT*)
{
	//������� ������� ������ �������
	RECT Rect;
	GetClientRect(mhWindow,&Rect);
	mpDangleGroup=new CDangleGroup(mhWindow,Rect);

	//������� ������� Application ������� ����
	gApplication.mhMainWindow=mhWindow;

	return TRUE;
}

VOID CMainWindow::OnClose()
{
	//������� ������ �������
	if(mpDangleGroup)
	{
		CDangleGroup* pGroup=mpDangleGroup;
		mpDangleGroup=NULL;
		pGroup->Release();
	}
	//������� ����
	DestroyWindow(mhWindow);
}

VOID CMainWindow::OnDestroy()
{
	//��������� ��������� ��� ��������� ������
	PostQuitMessage(0);
}

VOID CMainWindow::OnSize(UINT Width,UINT Height)
{
	//�������� ������ ���� �������
	if(mpDangleGroup)
		MoveWindow(mpDangleGroup->GetWindowHandle(),0,0,Width,Height,TRUE);
}

BOOL CMainWindow::OnCommand(INT ControlID)
{
	//����� �� ��������������
	switch(ControlID)
	{
		//** ���� "File"
	case MID_MAIN_FILE_OPENIMAGE:
		{
			//������� ���� ������ �����
			TCHAR szFile[MAX_PATH]={0};
			OPENFILENAME ofn={sizeof(OPENFILENAME)};
			ofn.hwndOwner=mhWindow;
			ofn.lpstrFile=szFile;
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrFilter=TEXT("All supported files\0*.exe;*.dll;*.com;*.dump\0Executable images (*.exe)\0*.exe\0Dynamic-link libraries (*.dll)\0*.dll\0MS-DOS .COM files (*.com)\0*.com\0Code0 dump files (*.dump)\0*.dump\0All files (*.*)\0*.*\0");
			ofn.lpstrTitle=TEXT("Select File to Disassembly");
			ofn.Flags=OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			if(!GetOpenFileName(&ofn)) break;

			//��������� ����
			CExploredImage* pImage=new CExploredImage(szFile);
			if(pImage->IsFileOpened())
			{
				//������� � ��������� ���������� �����
				CFileParser* pFileParser=pImage->AskFileParser(mhWindow);
				if(pFileParser)
				{
					//���������� �����-�����
					CSplashWindow SplashWindow;
					SplashWindow.Show(mhWindow,TEXT("Analizing your file..."),TEXT("Opening"));
					//��������� ������ � ������� ��������� � ������� �������, ���� �����
					switch(pImage->AnalizeAndPresent(pFileParser,mpDangleGroup))
					{
					case FILE_PARSER_ERROR_SUCCESS:
						//�� � �������
						break;
					case FILE_PARSER_ERROR_LOADING:
						MessageBox(mhWindow,TEXT("File analizer reported about permanent errors.\nFile can't be loaded with this analizer."),TEXT("Analizing error"),MB_ICONSTOP);
						break;
					case FILE_PARSER_ERROR_ANALIZING:
						MessageBox(mhWindow,TEXT("File analizer reported about availability some errors.\nIt may be mean auto-analizing has failed."),TEXT("Analizing warning"),MB_ICONWARNING);
						break;
					default:
						_ASSERTE(("Unknown file parser error code.",TRUE));
						break;
					}
					//���������� ����������
					pFileParser->Release();
					//������ �����-�����
					SplashWindow.Hide();
				}
			}
			else
				//������� ���������
				MessageBox(NULL,TEXT("Coudn't open this file.\nThis is maybe not enough memory, not enough address space or network problem."),TEXT("Opening executable image"),MB_ICONSTOP);
			//���������� ����
			pImage->Release();
		}
		break;
	case MID_MAIN_FILE_OPENHEX:
		{
			//������� ���� ������ �����
			TCHAR szFile[MAX_PATH]={0};
			OPENFILENAME ofn={sizeof(OPENFILENAME)};
			ofn.hwndOwner=mhWindow;
			ofn.lpstrFile=szFile;
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrFilter=TEXT("All files\0*.*\0");
			ofn.lpstrTitle=TEXT("Select File to HEX edit");
			ofn.Flags=OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			if(!GetOpenFileName(&ofn)) break;

			//��������� ����
			CEditedFile* pFile=CEditedFile::Open(szFile);
			if(pFile)
			{
				//������� ���� �� �����
				CDangleWindowHex* pWindow=new CDangleWindowHex(pFile);
				pFile->Release();
				mpDangleGroup->AddWindow(pWindow);
				pWindow->Release();
			}
			else
				MessageBox(mhWindow,TEXT("File could not be opened."),TEXT("Opening file in HEX"),MB_ICONSTOP);
		}
		break;
	case MID_MAIN_FILE_QUIT:
		//��������� ��������� � �������� ����, ����� ��������� �����������
		PostMessage(WM_CLOSE,0,0);
		break;
		//** ���� "Tools"
	case MID_MAIN_TOOLS_DUMPPROCESSMEMORY:
		{
#ifdef CODE0_COMPILE_DUMPING_TOOL
			//��������� ����������
			CToolDumper Tool;
			Tool.Start();
#else
			//������� ���������
			MessageBox(mhWindow,TEXT("Dumping tool in this version is not supported."),TEXT("Dumping tool"),MB_ICONSTOP);
#endif
		}
		break;
		//** ���� "Window"
	case MID_MAIN_WINDOW_CLOSE:
		{
			//���������� �����-�����, ��� ��� ��� �������� ������� ������ ���� ����������� �����
			CSplashWindow SplashWindow;
			SplashWindow.Show(mhWindow,TEXT("Releasing resources..."),TEXT("Shutdown"));
			//������� ���������� ����
			mpDangleGroup->CloseSelectedWindow();
			//������ �����-�����
			SplashWindow.Hide();
		}
		break;
		//** ���� "Help"
	case MID_MAIN_HELP_ABOUT:								//� ���������
		{
			//������� ������
			CDialogAbout Dialog;
			Dialog.Show(mhWindow);
		}
		break;
	default:			//��� ����������� ��������� �� ������������
		return FALSE;
	}

	//��������� ����������
	return TRUE;
}

VOID CMainWindow::OnNotify(NMHDR* pNotify)
{
	//����� �� ���� ���������
	switch(pNotify->code)
	{
	case NDG_CHANGEWINDOW:					//����������� �������
		{
			//�������� ����������� ���������
			NDG_CHANGEWINDOW_STRUCT* pStruct=(NDG_CHANGEWINDOW_STRUCT*)pNotify;

			//��������� ������� �������������
			mhCurrentWindowAccelerators=pStruct->hAccelerators;

			//�������������� ����
			{
				//�������� ���� ����
				HMENU hMainMenu=GetMenu(mhWindow);
				//�������� ������� ���������� ����
				UINT MainMenuItemsCount=GetMenuItemCount(hMainMenu);
				//�������� ���������� ����, ������� ����� �������
				UINT ItemsToDeleteCount=MainMenuItemsCount-mMainMenuItemsCount;
				//���� �� ���� ���������
				for(UINT i=0;i<ItemsToDeleteCount;++i)
					//������� ������� (��� �������� ���������������� �������, � ������� �� DeleteMenu)
					RemoveMenu(hMainMenu,mMainMenuItemsCount-MAIN_MENU_RIGHT_ITEMS_COUNT,MF_BYPOSITION);

				//�������� ���� ��� �������
				HMENU hMenu=pStruct->hMenu;
				//���� ��� ����
				if(hMenu)
				{
					//�������� ���������� ��������� � ����� ����
					UINT MenuItemsCount=GetMenuItemCount(hMenu);
					//�������� �� � ������� ����
					for(UINT i=0;i<MenuItemsCount;++i)
					{
						//�������� ���������� �� �������� ���� (������������ �������������� ����� ������ ���� ����������)
						MENUITEMINFO ItemInfo;
						ZeroMemory(&ItemInfo,sizeof(ItemInfo));
						ItemInfo.cbSize=sizeof(ItemInfo);
						ItemInfo.fMask=MIIM_DATA | MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
						static TCHAR szMenuItemName[0x100];
						ItemInfo.cch=0x100-1;
						ItemInfo.dwTypeData=szMenuItemName;
						INT k=GetMenuItemInfo(hMenu,i,TRUE,&ItemInfo);
						//�������� �������
						//����� ������� -1, ��� ��� ��������� ���������� ������������� ��������,
						//����� ������� ��������� �������� �������
						k=InsertMenuItem(hMainMenu,mMainMenuItemsCount-MAIN_MENU_RIGHT_ITEMS_COUNT+i,TRUE,&ItemInfo);
					}
				}

				//������������ ������ ����
				DrawMenuBar(mhWindow);
			}

			//���������� ����� ����
		}
		break;
	}
}

LRESULT CMainWindow::WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_CREATE:
		return OnCreate((CREATESTRUCT*)lParam) ? 0 : -1;
	case WM_CLOSE:
		return OnClose(),0;
	case WM_DESTROY:
		return OnDestroy(),0;
	case WM_SIZE:
		return OnSize(LOWORD(lParam),HIWORD(lParam)),0;
	case WM_COMMAND:
		if(OnCommand(LOWORD(wParam)))
			return 0;
		break;
	case WM_NOTIFY:
		return OnNotify((NMHDR*)lParam),0;
	}
	//������ ���������, ������� �� ����������, ������������� ������ �������
	if(mpDangleGroup)
		switch(Msg)
		{
		//���������������� ���������
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_COMMAND:
			mpDangleGroup->PostMessage(Msg,wParam,lParam);
		}

	//��� ���� �� ������������ ��������� (� ��� ����� ����������������) ��������� ����������� ���������
	return DefWindowProc(mhWindow,Msg,wParam,lParam);
}

CMainWindow::CMainWindow()
{
	//���������������� ����������
	HMENU hMainMenu=LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_MENU_MAIN));
	mMainMenuItemsCount=GetMenuItemCount(hMainMenu);
	mhCurrentWindowAccelerators=NULL;

	//������� ����
	mhWindow=CreateWindowEx(WS_EX_APPWINDOW,gApplication.mszMainWindowClass,TEXT("Quyse Code0 Disassembly"),WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,0,0,500,400,NULL,hMainMenu,GetModuleHandle(NULL),this);
	ShowWindow(mhWindow,SW_MAXIMIZE);
}

CMainWindow::~CMainWindow()
{
}

//������� ������� ���������
VOID CMainWindow::Show()
{
	//��������� ������� �������������
	HACCEL hAccelerators=LoadAccelerators(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_ACCELERATOR_MAIN));
	//������� ����
	MSG Msg;
	//�������� ���������
	while(GetMessage(&Msg,NULL,0,0))
	{
		//���� ���� ������� ������������� ���������� �������
		if(mhCurrentWindowAccelerators)
			//���������, ������������� �� ��� ���������
			if(TranslateAccelerator(mhWindow,mhCurrentWindowAccelerators,&Msg))
				continue;
		//������ ��������� �� ���������� ������� �������������
		if(TranslateAccelerator(mhWindow,hAccelerators,&Msg))
			continue;

		//������� ������������� �� ������������� ���������; �������� ��� �� ���������
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}
