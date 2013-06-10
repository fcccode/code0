#include "dangle_group.h"
#include "application.h"
#include <commctrl.h>

// ���������

//������ �� ����� ���������� �������
#define DANGLE_GROUP_TAB_BORDER					10
//������ ��������� �������
#define DANGLE_GROUP_TAB_HEIGHT					20

// class CDangleGroup

VOID CDangleGroup::OnCreate(CREATESTRUCT* pCreateStruct)
{
	//������� ������ ������������
	RECT Rect;
	GetClientRect(mhWindow,&Rect);
	mhTabbar=CreateWindowEx(TBSTYLE_EX_MIXEDBUTTONS | WS_EX_WINDOWEDGE,TOOLBARCLASSNAME,TEXT("Quyse Toolbar"),WS_CHILD | TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_WRAPABLE | (mTabbarVisible ? WS_VISIBLE : 0),0,0,Rect.right,mTabbarVisible ? Rect.bottom : 0,mhWindow,NULL,GetModuleHandle(NULL),NULL);
	//��������� ��������� ��� ������������� ������
	SendMessage(mhTabbar,TB_BUTTONSTRUCTSIZE,sizeof(TBBUTTON),0);
}

VOID CDangleGroup::OnSize(UINT Width,UINT Height)
{
	///�������� ������ ������ �������
	MoveWindow(mhTabbar,0,0,Width,Height,TRUE);
	//�������� ������� ������ �������
	RECT Rect;
	GetWindowRect(mhTabbar,&Rect);
	UINT TabbarHeight=Rect.bottom-Rect.top;
	//�������� ������� ���� �������
	for(DGWLI i=mWindowsList.begin();i!=mWindowsList.end();i++)
		MoveWindow((*i)->GetWindowHandle(),0,TabbarHeight,Width,Height-TabbarHeight,TRUE);
}

BOOL CDangleGroup::OnCommand(UINT CommandID,UINT Code,HWND hControl)
{
	//���� ��������� ��������� �� ������ �������
	if(hControl==mhTabbar)
	{
		//�������� ��������� �� ������� (������������� ������ - ����� ����)
		DGWLI i=mWindowsList.begin();
		std::advance(i,CommandID);
		CDangleWindow* pWindow=*i;
		//���� ������� ������ �������������, �.�. ������� ���� �� ��������� � ���������
		if(mSelectedWindowIterator==mWindowsList.end() || pWindow!=*mSelectedWindowIterator)
		{
			//������� ���� ��������� �������
			SetWindowPos(pWindow->GetWindowHandle(),HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

			//���������� ����� ������� ����
			mSelectedWindowIterator=i;

			//�������� ���������� � ���������������� ���������� �������
			DANGLE_WINDOW_UI WindowUI;
			pWindow->GetUI(&WindowUI);
			//��������� ����������� �������� ����, ����� ��� �������� ���������
			NDG_CHANGEWINDOW_STRUCT Notify;
			Notify.Header.code=NDG_CHANGEWINDOW;
			Notify.Header.hwndFrom=mhWindow;
			Notify.Header.idFrom=0;
			Notify.hMenu=WindowUI.hMenu;
			Notify.hAccelerators=WindowUI.hAccelerators;
			::SendMessage(GetParent(mhWindow),WM_NOTIFY,0,(LPARAM)&Notify);
		}

		//��������� ����������
		return TRUE;
	}
	//����� ��������� �� ����������
	return FALSE;
}

VOID CDangleGroup::OnNotify(NMHDR* pNotify)
{
	//����� �� ���� �����������
	switch(pNotify->code)
	{
	case NDG_WINDOWUPDATED:													//���������� �������
		{
			//�������� ����������� ���������
			NDG_WINDOWUPDATED_STRUCT* pStruct=(NDG_WINDOWUPDATED_STRUCT*)pNotify;
			//�������� ���� �������
			HWND hDangleWindow=pStruct->hWindow;

			//���� ���� �� ��������, �� ��������� (������ ������������� ��������� ��� ������������ �� ��� ����)
			if(!mWindowsList.size() || (*mSelectedWindowIterator)->GetWindowHandle()!=hDangleWindow) return;

			//����� ���� ��������

			//�������� ����� ����
			UINT j=0;
			for(DGWLI i=mWindowsList.begin();i!=mWindowsList.end();i++,j++)
				if(i==mSelectedWindowIterator)
					break;
			//���������� ������� ���� �� ������������ ��������, ����� ���� ��������� ������������
			mSelectedWindowIterator=mWindowsList.end();
			//��������� ������ ���� ��������� � ������� ������
			SendMessage(mhWindow,WM_COMMAND,MAKELONG(j,0),(LPARAM)mhTabbar);
		}
		break;
	}
}

//������� ���������
LRESULT CDangleGroup::WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_CREATE:
		return OnCreate((CREATESTRUCT*)lParam),0;
	case WM_SIZE:
		return OnSize(LOWORD(lParam),HIWORD(lParam)),0;
	case WM_COMMAND:
		if(OnCommand(LOWORD(wParam),HIWORD(wParam),(HWND)lParam))
			return 0;
		break;
	case WM_NOTIFY:
		return OnNotify((NMHDR*)lParam),0;
	}

	//������ ���������, ������� �� ���� ����������, �������� ���������� �������
	if(mWindowsList.size())
		switch(Msg)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_COMMAND:
			(*mSelectedWindowIterator)->PostMessage(Msg,wParam,lParam);
		}
	//��������� ����������� ���������
	return DefWindowProc(mhWindow,Msg,wParam,lParam);
}

CDangleGroup::CDangleGroup(HWND hWndParent,RECT Rect,BOOL TabbarVisible)
{
	//��������� ���� ��������� ������ �������
	mTabbarVisible=TabbarVisible;
	//������� ����
	mhWindow=CreateWindow(gApplication.mszDangleGroupWindowClass,NULL,WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hWndParent,NULL,GetModuleHandle(NULL),this);
}

CDangleGroup::~CDangleGroup()
{
	//���������� ��� �������
	for(DGWLI i=mWindowsList.begin();i!=mWindowsList.end();i++)
	{
		(*i)->Delete();
		(*i)->Release();
	}
	mWindowsList.clear();
	//���������� ����
	DestroyWindow(mhWindow);
}

VOID CDangleGroup::AddWindow(CDangleWindow* pWindow)
{
	//������� ����
	RECT Rect,RectTabbar;
	GetClientRect(mhWindow,&Rect);
	GetWindowRect(mhTabbar,&RectTabbar);
	Rect.top+=RectTabbar.bottom-RectTabbar.top;
	pWindow->Create(mhWindow,Rect);
	//�������� ���� � ������
	pWindow->AddRef();
	mWindowsList.push_back(pWindow);
	//���������� ������� ���� �� ������������ ��������
	//��� �����������, ����� � ����������� WM_COMMAND ���� ������� ������������ ����
	//��������� �� ���������� �������� ����� ��������� � ���� �� �����������
	mSelectedWindowIterator=mWindowsList.end();

	//�������� ������ � ������ �������
	TBBUTTON Button;
	Button.iBitmap=I_IMAGENONE;
	//������������� ������ ������������, ��� ����� ������� ����
	Button.idCommand=mWindowsList.size()-1;
	Button.fsState=TBSTATE_ENABLED;
	Button.fsStyle=BTNS_SHOWTEXT | BTNS_AUTOSIZE | BTNS_CHECKGROUP;
	Button.dwData=0;
	Button.iString=(INT_PTR)pWindow->GetName();
	SendMessage(mhTabbar,TB_ADDBUTTONS,1,(LPARAM)&Button);

	//������ ������
	SendMessage(mhTabbar,TB_CHECKBUTTON,Button.idCommand,(LPARAM)MAKELONG(TRUE,0));
	//��������� ������ ���� ��������� � ������� ������
	SendMessage(mhWindow,WM_COMMAND,MAKELONG(Button.idCommand,0),(LPARAM)mhTabbar);
}

CDangleGroup::DGWL* CDangleGroup::GetWindows()
{
	//������� ��������� �� ������ ����
	return &mWindowsList;
}

VOID CDangleGroup::CloseSelectedWindow()
{
	//���� ���� ����
	if(mWindowsList.size())
	{
		//�������� ����� ������� ���� (��� ����, ����� ����� ���� ������� ������ �� ������)
		UINT j=0;
		for(DGWLI i=mWindowsList.begin();i!=mWindowsList.end();i++,j++)
			if(i==mSelectedWindowIterator)
				break;

		//�������� ��������� �� ����
		CDangleWindow* pWindow=*mSelectedWindowIterator;
		//������� � ���������� ����
		pWindow->Delete();
		pWindow->Release();
		//������� ���� �� ������
		mWindowsList.erase(mSelectedWindowIterator);
		//������� ��������������� ������
		SendMessage(mhTabbar,TB_DELETEBUTTON,(WPARAM)j,0);

		//���������� �������� �������� ���� �� ������������
		mSelectedWindowIterator=mWindowsList.end();

		//��������� ���� �������, ������� ����������� ����� ��������, ������������� �������
		for(;j<mWindowsList.size();j++)
			SendMessage(mhTabbar,TB_SETCMDID,j,j);

		//���� ���� ��� ����
		if(mWindowsList.size())
		{
			//���� ��� ���� - �� ������
			if(j)
				//�������� ���������� ����
				j--;
			//����� ��� ���� ���� ������
			else;
				//�������� ��������� ���� (������ j++ �� ����, ��� ��� �� � ��� ��� �������� ����� ������ ��� ���������� ����)
			//������ ������
			SendMessage(mhTabbar,TB_CHECKBUTTON,j,(LPARAM)MAKELONG(TRUE,0));
			//��������� ������ ���� ��������� � ������� ������
			SendMessage(mhWindow,WM_COMMAND,MAKELONG(j,0),(LPARAM)mhTabbar);
		}
		//����� ���� ������ ���
		else
		{
			//��������� ����������� �������� ����, ����� ��� �������� ���������
			NDG_CHANGEWINDOW_STRUCT Notify;
			Notify.Header.code=NDG_CHANGEWINDOW;
			Notify.Header.hwndFrom=mhWindow;
			Notify.Header.idFrom=0;
			Notify.hMenu=NULL;
			Notify.hAccelerators=NULL;
			::SendMessage(GetParent(mhWindow),WM_NOTIFY,0,(LPARAM)&Notify);
		}
	}
}

CDangleWindow* CDangleGroup::GetCurrentWindow()
{
	//���� ���� ����
	if(mWindowsList.size())
		//������� ����
		return *mSelectedWindowIterator;
	//����� ���� ���
	return NULL;
}
