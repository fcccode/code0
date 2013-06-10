#include "danglewindow_danglegroup.h"

// class CDangleWindowDangleGroup

CDangleWindowDangleGroup::CDangleWindowDangleGroup(LPCTSTR szName,UINT Pane)
{
	//�������� ����������
	mpDangleGroup=NULL;
	//������� ���
	mszName=(LPTSTR)szName;
	//��������� ��� ������
	mPane=Pane;
	//���������������� ��������� �� ������
	mpPaneMain=NULL;
	mpPaneLeft=NULL;
	mpPaneRight=NULL;
}

CDangleWindowDangleGroup::~CDangleWindowDangleGroup()
{
	//���������� ��������� �� ������
	SafeRelease(mpPaneMain);
	SafeRelease(mpPaneLeft);
	SafeRelease(mpPaneRight);
	//������� ������ �������
	SafeRelease(mpDangleGroup);
}

LRESULT CDangleWindowDangleGroup::DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//��������� ��������� ������������
	switch(Msg)
	{
	case WM_CREATE:
		{
			//������� ������ �������
			RECT Rect;
			GetClientRect(mhWindow,&Rect);
			mpDangleGroup=new CDangleGroup(mhWindow,Rect);
		}
		return 0;
	case WM_WINDOWPOSCHANGING:
		{
			//�������� ��������� � �����������
			WINDOWPOS* pWindowPos=(WINDOWPOS*)lParam;
			//�������� ������� ������������� ����
			RECT Rect;
			GetClientRect(GetParent(mhWindow),&Rect);
			//�������� ����� ������ � ������������ � ����� ������
			switch(mPane)
			{
			case DWDG_PANE_FLOAT:
				//�������� ����������� � ��������� �������
				pWindowPos->flags|=SWP_NOMOVE | SWP_NOSIZE;
				break;
			case DWDG_PANE_FULL:
				pWindowPos->cx=Rect.right;
				pWindowPos->cy=Rect.bottom;
				break;
			case DWDG_PANE_MAIN:
				pWindowPos->x=Rect.right/4;
				pWindowPos->cx=Rect.right*3/4;
				break;
			case DWDG_PANE_LEFT:
				pWindowPos->cx=Rect.right/4;
				break;
			case DWDG_PANE_RIGHT:
				pWindowPos->x=Rect.right*3/4;
				pWindowPos->cx=Rect.right/4;
				break;
			}
		}
		return 0;
	case WM_SIZE:
		//�������� ������ ������ �������
		if(mpDangleGroup)
			MoveWindow(mpDangleGroup->GetWindowHandle(),0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
		return 0;
	case WM_NOTIFY:
		{
			//�������� ��������� �����������
			NMHDR* pNotify=(NMHDR*)lParam;
			//����� �� ���� �����������
			switch(pNotify->code)
			{
			case NDG_CHANGEWINDOW:					//�� ��������� ������ ������� ����������� �������
				{
					//�������� ����������� ���������
					NDG_CHANGEWINDOW_STRUCT* pStruct=(NDG_CHANGEWINDOW_STRUCT*)pNotify;

					//��������� ��������� UI
					mUI.hMenu=pStruct->hMenu;
					mUI.hAccelerators=pStruct->hAccelerators;
					
					//��������� ����������� ������������ ������ ������� �� ����������
					NDG_WINDOWUPDATED_STRUCT Notify;
					Notify.Header.code=NDG_WINDOWUPDATED;
					Notify.Header.hwndFrom=mhWindow;
					Notify.Header.idFrom=0;
					Notify.hWindow=mhWindow;
					::SendMessage(GetParent(mhWindow),WM_NOTIFY,0,(LPARAM)&Notify);
				}
				return 0;
			}
		}
	}
	//��� ��������� ��������� ���������������� ��������� ������ �������
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
	//��������� ����������� ���������
	return DefWindowProc(mhWindow,Msg,wParam,lParam);
}

VOID CDangleWindowDangleGroup::Paint(HDC hdc)
{
	//������ �� ��������
}

CDangleGroup* CDangleWindowDangleGroup::GetGroup()
{
	//������� ������
	mpDangleGroup->AddRef();
	return mpDangleGroup;
}

VOID CDangleWindowDangleGroup::CreateStandardPanes()
{
	//����� ������
	mpPaneLeft=new CDangleWindowDangleGroup(TEXT("LeftPane"),DWDG_PANE_LEFT);
	mpDangleGroup->AddWindow(mpPaneLeft);
	//������ ������
	mpPaneRight=new CDangleWindowDangleGroup(TEXT("RightPane"),DWDG_PANE_RIGHT);
	mpDangleGroup->AddWindow(mpPaneRight);
	//������� ������
	mpPaneMain=new CDangleWindowDangleGroup(TEXT("MainPane"),DWDG_PANE_MAIN);
	mpDangleGroup->AddWindow(mpPaneMain);
}

CDangleGroup* CDangleWindowDangleGroup::GetMainPane()
{
	//������� �������
	return mpPaneMain->GetGroup();
}

CDangleGroup* CDangleWindowDangleGroup::GetLeftPane()
{
	//������� �������
	return mpPaneLeft->GetGroup();
}

CDangleGroup* CDangleWindowDangleGroup::GetRightPane()
{
	//������� �������
	return mpPaneRight->GetGroup();
}
