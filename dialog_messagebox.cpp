#include "dialog_messagebox.h"
#include "application.h"
#include "resource.h"

// class CDialogMessageBox

BOOL CDialogMessageBox::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			//�������� ������� ������
			HDC hdc=GetDC(NULL);
			SelectFont(hdc,gApplication.mhFontCode);
			SIZE Size;
			Size.cx=0;
			Size.cy=0;
			{
				//��������� ������� ���������
				UINT Length=_tcslen(mszText);
				UINT i,j;
				for(i=0;i<Length;++i)
				{
					for(j=i+1;j<Length;++j)
						if(mszText[j]=='\n')
							break;
					SIZE LineSize;
					GetTextExtentPoint32(hdc,mszText+i,j-i,&LineSize);
					if(Size.cx<LineSize.cx) Size.cx=LineSize.cx;
					Size.cy+=LineSize.cy;
					i=j-1;
				}
			}
			ReleaseDC(NULL,hdc);

			//�������� ������������� �������
			RECT DialogRect;
			GetWindowRect(mhWindow,&DialogRect);
			//�������� ������������� �������
			HWND hStaticText=GetDlgItem(mhWindow,IDC_STATIC_TEXT);
			RECT StaticTextRect;
			GetWindowRect(hStaticText,&StaticTextRect);
			//�������� ������������� ������� ��� ������
			HWND hButtonsArea=GetDlgItem(mhWindow,IDC_BUTTON_AREA);
			RECT ButtonsRect;
			GetWindowRect(hButtonsArea,&ButtonsRect);
			//������� ������-�������
			DestroyWindow(hButtonsArea);
			//�������� ����� �������������� ������������ ������� �� �������� ����
			INT ButtonsOffsetY=ButtonsRect.top-StaticTextRect.bottom;

			//��������� ����� ������ �������
			INT DialogWidth=DialogRect.right-DialogRect.left-(StaticTextRect.right-StaticTextRect.left)+Size.cx;
			INT DialogHeight=DialogRect.bottom-DialogRect.top-(StaticTextRect.bottom-StaticTextRect.top)+Size.cy;
			//�������� ������������� ������������� ����
			RECT ParentRect;
			GetWindowRect(mhWndParent,&ParentRect);
			//������� ������ � ��������� �������
			MoveWindow(mhWindow,ParentRect.left+(ParentRect.right-ParentRect.left-DialogWidth)/2,ParentRect.top+(ParentRect.bottom-ParentRect.top-DialogHeight)/2,DialogWidth,DialogHeight,FALSE);

			//������� ������ �������
			SetWindowPos(hStaticText,NULL,0,0,Size.cx,Size.cy,SWP_NOMOVE | SWP_NOZORDER);
			//������� ����� ��� �������
			FORWARD_WM_SETFONT(hStaticText,gApplication.mhFontCode,FALSE,SendMessage);
			
			//������� �������
			SetWindowText(hStaticText,mszText);
			//������� ��������� ����
			if(mszCaption) SetWindowText(mhWindow,mszCaption);

			//�������� ����� ��������� ������� � �������
			GetWindowRect(hStaticText,&StaticTextRect);
			MapWindowPoints(NULL,mhWindow,(LPPOINT)&StaticTextRect,2);
			//��������� ������� ���������� ��� ������
			INT ButtonsTop=StaticTextRect.bottom+ButtonsOffsetY;
			//������ ������ - ������ � ������������
			INT ButtonWidth=ButtonsRect.right-ButtonsRect.left;
			//��������� ���������� ����� ��������
			INT ButtonSpace=ButtonWidth+ButtonWidth/7;
			//��������� ������ �����
			INT ButtonsLeft=StaticTextRect.left+(StaticTextRect.right-StaticTextRect.left-ButtonSpace*mButtonsCount+ButtonSpace-ButtonWidth)/2;
			//�������� ������ ������
			INT ButtonHeight=ButtonsRect.bottom-ButtonsRect.top;
			//������� ������
			for(UINT i=0;i<mButtonsCount;++i)
			{
				HWND hButton=CreateWindowEx(WS_EX_NOPARENTNOTIFY,TEXT("Button"),mszButtonNames[i],WS_VISIBLE | WS_CHILDWINDOW | WS_TABSTOP | BS_TEXT /*| (i==mDefaultButton ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON)*/ | BS_PUSHBUTTON | (!i ? WS_GROUP : 0),
					ButtonsLeft+i*ButtonSpace,ButtonsTop,ButtonWidth,ButtonHeight,mhWindow,NULL,GetModuleHandle(NULL),NULL);
				SetWindowLong(hButton,GWL_USERDATA,i);
				//������� �����
				FORWARD_WM_SETFONT(hButton,gApplication.mhFontCode,FALSE,SendMessage);
			}
			//��������� TAB
			FORWARD_WM_NEXTDLGCTL(mhWindow,0,TRUE,PostMessage);

			//������� ������
			if(mhIcon)
				SendDlgItemMessage(mhWindow,IDC_STATIC_ICON,STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)mhIcon);
		}
		return TRUE;
	case WM_COMMAND:
		if(lParam)
			//�������� ����� ������� ������ � ������� ������
			EndDialog(mhWindow,GetWindowLong((HWND)lParam,GWL_USERDATA));
		return TRUE;
	}
	return FALSE;
}

UINT CDialogMessageBox::Show(HWND hWndParent,LPCTSTR szText,LPCTSTR szCaption,UINT ButtonsCount,LPCTSTR* szButtonNames,HANDLE hIcon,UINT DefaultButton)
{
	//��������� ���������
	mhWndParent=hWndParent;
	mszText=szText;
	mszCaption=szCaption;
	mButtonsCount=ButtonsCount;
	mszButtonNames=szButtonNames;
	mhIcon=hIcon;
	mDefaultButton=DefaultButton;

	//������� ������
	return Ask(MAKEINTRESOURCE(IDD_DIALOG_MESSAGEBOX),hWndParent);
}
