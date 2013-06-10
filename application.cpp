#include "application.h"
#include "window.h"

#include "pe_parser.h"
#include "com_parser.h"
#include "dumpfile_parser.h"
#include "binary_parser.h"

#include "dialog_messagebox.h"
#include <commctrl.h>
#include "resource.h"

// ���������� ������ Application
CApplication gApplication;

// Globals

INT QsMessageBox(HWND hWndParent,LPCTSTR szText,LPCTSTR szCaption,UINT Flags)
{
	//������� ������
	static LPCTSTR szButtonNames[7][3]={
		{ TEXT("OK"),NULL,NULL },
		{ TEXT("OK"),TEXT("Cancel"),NULL },
		{ TEXT("Abort"),TEXT("Retry"),TEXT("Ignore") },
		{ TEXT("Yes"),TEXT("No"),TEXT("Cancel") },
		{ TEXT("Yes"),TEXT("No"),NULL },
		{ TEXT("Retry"),TEXT("Cancel"),NULL },
		{ TEXT("Cancel"),TEXT("Try"),TEXT("Continue") }
	};
	static const UINT ButtonIDs[7][3]={
		{ IDOK,0,0 },
		{ IDOK,IDCANCEL,0 },
		{ IDABORT,IDRETRY,IDIGNORE },
		{ IDYES,IDNO,IDCANCEL },
		{ IDYES,IDNO,0 },
		{ IDRETRY,IDCANCEL,0 },
		{ IDCANCEL,/*IDTRY*/0,IDCONTINUE },
	};
	static const UINT ButtonCounts[7]={ 1,2,3,3,2,2,3 };

	//������� ������ ������
	UINT Buttons=(Flags & 0x0F);
	//������� ������
	UINT Bitmap;
	switch(Flags & 0xF0)
	{
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L
	case MB_ICONHAND:
		Bitmap=IDB_BITMAP_ICONSTOP;
		break;
	case MB_ICONQUESTION:
		Bitmap=IDB_BITMAP_ICONQUESTION;
		break;
	case MB_ICONWARNING:
		Bitmap=IDB_BITMAP_ICONWARNING;
		break;
	case MB_ICONASTERISK:
		Bitmap=0;
		break;
	default:
		Bitmap=NULL;
		break;
	}
	//������� ������� ������
	UINT DefaultButton=((Flags & 0x0F00)>>8);
	//������� ������, � ������� ������������� ������� ������
	CDialogMessageBox Dialog;
	return ButtonIDs[Buttons][Dialog.Show(hWndParent ? hWndParent : gApplication.mhMainWindow,szText,szCaption,ButtonCounts[Buttons],szButtonNames[Buttons],LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(Bitmap),IMAGE_BITMAP,0,0,0),DefaultButton)];
}

// class CApplication

const COLORREF CApplication::mBrushSelectColor=RGB(0xE0,0xE0,0xE0);
const COLORREF CApplication::mBrushSlackSelectColor=RGB(0xF0,0xF0,0xF0);

VOID CApplication::Initialize()
{
	//���������������� ���������� �������� ����
	mhMainWindow=NULL;

	//�������, ��� ���� ������ ���� low-fragmentation
	{
		ULONG fragValue = 2;
		HeapSetInformation(GetProcessHeap(), HeapCompatibilityInformation, &fragValue, sizeof(fragValue));
	}

	//���������������� ����������� �������� ����������
	INITCOMMONCONTROLSEX InitCommonControls;
	InitCommonControls.dwSize=sizeof(InitCommonControls);
	InitCommonControls.dwICC=ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS;
	InitCommonControlsEx(&InitCommonControls);

	// ���������������� ������� GDI

	//�������� ��������� ������������ ������
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT),sizeof(lf),&lf);
	//�������� ��� ������
	_tcscpy(lf.lfFaceName,TEXT("Courier New"));
	//�������� ������ ������
	lf.lfHeight=16;
	//������� ����� �����
	mhFontCode=CreateFontIndirect(&lf);
	//������� ����� �� �����, �� ������
	lf.lfWeight=FW_BOLD;
	mhFontCodeBold=CreateFontIndirect(&lf);

	//��������� ������ ������
	mFontCodeHeight=lf.lfHeight;

	//������� ����� ��� ���������
	mhBrushSelect=CreateSolidBrush(mBrushSelectColor);
	//������� ����� ��� ������� ���������
	mhBrushSlackSelect=CreateSolidBrush(mBrushSlackSelectColor);
	//������� ���� ��� ���������
	mhPenSelect=GetStockPen(BLACK_PEN);

	//���������������� ����� �������� ����
	WNDCLASS WndClass;
	mszMainWindowClass=TEXT("QuyseMainWindow");
	ZeroMemory(&WndClass,sizeof(WndClass));
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc=CWindow::StaticWndProc;
	WndClass.hInstance=GetModuleHandle(NULL);
	WndClass.hIcon=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON_APP));
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hbrBackground=mhBrushSelect;
	WndClass.lpszClassName=mszMainWindowClass;
	RegisterClass(&WndClass);
	//���������������� ����� ���� CDangleGroup
	mszDangleGroupWindowClass=TEXT("QuyseDangleGroup");
	ZeroMemory(&WndClass,sizeof(WndClass));
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	WndClass.hbrBackground=mhBrushSelect;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hInstance=GetModuleHandle(NULL);
	WndClass.lpszClassName=mszDangleGroupWindowClass;
	WndClass.lpfnWndProc=CWindow::StaticWndProc;
	RegisterClass(&WndClass);
	//���������������� ����� ���� CDangleWindow
	mszDangleWindowClass=TEXT("QuyseDangleWindow");
	ZeroMemory(&WndClass,sizeof(WndClass));
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	WndClass.hbrBackground=NULL;//GetStockBrush(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hInstance=GetModuleHandle(NULL);
	WndClass.lpszClassName=mszDangleWindowClass;
	WndClass.lpfnWndProc=CWindow::StaticWndProc;
	RegisterClass(&WndClass);

	//������� ��������� �����
	{
		HDC hdc=GetDC(NULL);
		mhdcBackBuffer=CreateCompatibleDC(hdc);
		mbmpBackBuffer=CreateCompatibleBitmap(hdc,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
		SelectBitmap(mhdcBackBuffer,mbmpBackBuffer);
		ReleaseDC(NULL,hdc);
	}

	//��������� ������ ����������
	LoadInstructions();
}

VOID CApplication::Uninitialize()
{
	//��������� ��� ����������� �����
	for(std::list<CExploredImage*>::iterator i=mExploredImagesList.begin();i!=mExploredImagesList.end();i++)
		(*i)->Release();
	mExploredImagesList.clear();

	//������� ��������� �����
	DeleteDC(mhdcBackBuffer);
	DeleteBitmap(mbmpBackBuffer);
}

VOID CApplication::GetFileParsers(std::list<CFileParser*>* pList)
{
	//������������ ������ ������������ ������
	pList->push_back(new CPEParser);
	pList->push_back(new CDumpFileParser);
	pList->push_back(new CCOMParser);
	pList->push_back(new CBinaryParser);
}

VOID CApplication::AddExploredImage(CExploredImage* pImage)
{
	//�������� ���� � ������
	pImage->AddRef();
	mExploredImagesList.push_back(pImage);
}

std::list<CExploredImage*>* CApplication::GetExploredImagesList()
{
	//������� ��������� �� ������ ����������� ������
	return &mExploredImagesList;
}
