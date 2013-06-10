#include "danglewindow_hex.h"
#include "application.h"

//���������� ���� � ������
#define LINE_COUNT 0x10

CDangleWindowHex::CDangleWindowHex(CEditedFile* pEditedFile)
{
	//��������� ��������� �����
	mpEditedFile=pEditedFile;
	mpEditedFile->AddRef();
	//�������������
	mWindowOffset=0;
	mSelectionOffset=0;
	mSelectionLength=0;
	mWindowLinesCount=0;

	//������������ ��� ����
	LPCTSTR szFileName=mpEditedFile->GetFileName();
	//�������� ��� ��� ����
	TCHAR szFileTitle[MAX_PATH];
	GetFileTitle(szFileName,szFileTitle,MAX_PATH);
	//���� ��� ������� �������
	if(_tcslen(szFileTitle)>MAX_DANGLE_NAME_LENGTH)
	{
		//�������� ���
		CopyMemory(mszNameBuffer,szFileTitle,MAX_DANGLE_NAME_LENGTH*sizeof(TCHAR));
		mszNameBuffer[MAX_DANGLE_NAME_LENGTH]=0;
	}
	//����� ����� �������
	else
		_tcscpy(mszNameBuffer,szFileTitle);

	//���������� ��������� �� ���
	mszName=mszNameBuffer;
}

CDangleWindowHex::~CDangleWindowHex()
{
	SafeRelease(mpEditedFile);
}

VOID CDangleWindowHex::OnKey(UINT Key)
{
	//����� �� �������
	switch(Key)
	{
	case VK_DOWN:
		{
			//�������� ������ ����
			mSelectionOffset+=LINE_COUNT;
			UINT FileSize=mpEditedFile->GetSize();
			if(mSelectionOffset>=FileSize) mSelectionOffset=FileSize-1;
			//���� ������ �� ���������� � ����, �������� ����
			if((mSelectionOffset-mWindowOffset)/LINE_COUNT>=mWindowLinesCount)
				mWindowOffset+=LINE_COUNT;
			//�������� ����
			InvalidateRect(mhWindow,NULL,FALSE);
		}
		break;
	case VK_UP:
		{
			//�������� ������ �����
			if(mSelectionOffset<LINE_COUNT) mSelectionOffset=0;
			else mSelectionOffset-=LINE_COUNT;
			//���� ������ �� ���������� � ����, �������� ����
			if(mSelectionOffset<mWindowOffset)
				if(mWindowOffset<LINE_COUNT) mWindowOffset=0;
				else mWindowOffset-=LINE_COUNT;
			//�������� ����
			InvalidateRect(mhWindow,NULL,FALSE);
		}
		break;
	case VK_RIGHT:
		{
			//�������� ������ ������
			UINT FileSize=mpEditedFile->GetSize();
			if(mSelectionOffset<FileSize-1) mSelectionOffset++;
			//���� ������ �� ���������� � ����, �������� ����
			if((mSelectionOffset-mWindowOffset)/LINE_COUNT>=mWindowLinesCount)
				mWindowOffset+=LINE_COUNT;
			//�������� ����
			InvalidateRect(mhWindow,NULL,FALSE);
		}
		break;
	case VK_LEFT:
		{
			//�������� ������ �����
			if(mSelectionOffset>=1) mSelectionOffset--;
			//���� ������ �� ���������� � ����, �������� ����
			if(mSelectionOffset<mWindowOffset)
				if(mWindowOffset<LINE_COUNT) mWindowOffset=0;
				else mWindowOffset-=LINE_COUNT;
			//�������� ����
			InvalidateRect(mhWindow,NULL,FALSE);
		}
		break;
	case VK_NEXT:
		{
			for(UINT i=0;i<mWindowLinesCount;++i)
				OnKey(VK_DOWN);
		}
		break;
	case VK_PRIOR:
		{
			for(UINT i=0;i<mWindowLinesCount;++i)
				OnKey(VK_UP);
		}
		break;
	case VK_HOME:
		{
			mSelectionOffset=0;
			mWindowOffset=0;
			InvalidateRect(mhWindow,NULL,FALSE);
		}
		break;
	case VK_END:
		{
			mSelectionOffset=mpEditedFile->GetSize()-1;
			mWindowOffset=mSelectionOffset/LINE_COUNT*LINE_COUNT;
			InvalidateRect(mhWindow,NULL,FALSE);
		}
		break;
	}
}

VOID CDangleWindowHex::OnCommand(UINT CommandID,UINT Code,HWND hControl)
{
}

VOID CDangleWindowHex::OnMouseWheel(INT Delta,UINT Keys)
{
}

LRESULT CDangleWindowHex::DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_KEYDOWN:
		return OnKey(wParam),0;
	case WM_COMMAND:
		return OnCommand(LOWORD(wParam),HIWORD(wParam),(HWND)lParam),0;
	case WM_MOUSEWHEEL:
		return OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA,GET_KEYSTATE_WPARAM(wParam)),0;
	default:
		return DefWindowProc(mhWindow,Msg,wParam,lParam);
	}
}

VOID CDangleWindowHex::Paint(HDC hdc)
{
	//�������� ������������� ����
	RECT rect;
	GetClientRect(mhWindow,&rect);
	//������� �������� ��� ��������
	LISTING_DEVICE_CONTEXT Context;
	Context.hdc=hdc;
	Context.Rect=rect;
	mListing.SetContext(Context);
	//�������� ������ �����
	UINT FileSize=mpEditedFile->GetSize();
	PBYTE pFile=(PBYTE)mpEditedFile->GetMemory();
	UINT k=mWindowOffset;

	//�������� �����
	SelectBrush(hdc,GetStockBrush(WHITE_BRUSH));
	//�������� ����
	PatBlt(hdc,rect.left,rect.top,rect.right,rect.bottom,PATCOPY);
	//�������� �����
	SelectFont(hdc,gApplication.mhFontCode);

	//��������� ���������� ������������ �����
	mWindowLinesCount=(rect.bottom-rect.top)/gApplication.mFontCodeHeight;

	//���������� ���������, ���� ��� ����
	if(mSelectionLength)
	{
	}
	else if(mSelectionOffset>=mWindowOffset && mSelectionOffset<mWindowOffset+(mWindowLinesCount+1)*LINE_COUNT)
	{
		//�������� ���������� ����
		SelectBrush(hdc,gApplication.mhBrushSelect);
		SelectPen(hdc,gApplication.mhPenSelect);
		//�������� ������ ������ �������
		SIZE CharSize;
		GetTextExtentPoint32(hdc,TEXT("W"),1,&CharSize);
		INT x=(0x10+(mSelectionOffset-mWindowOffset)%LINE_COUNT*3)*CharSize.cx;
		INT y=(mSelectionOffset-mWindowOffset)/LINE_COUNT*CharSize.cy;
		Rectangle(hdc,x,y,x+CharSize.cx*2,y+CharSize.cy);
		x=(0x10+LINE_COUNT*3+0x8+(mSelectionOffset-mWindowOffset)%LINE_COUNT)*CharSize.cx;
		Rectangle(hdc,x,y,x+CharSize.cx,y+CharSize.cy);
	}

	//���� ������ �����
	TCHAR s[3];
	for(UINT i=mWindowOffset;i<FileSize;i+=LINE_COUNT)
	{
		//������� ����� ��� ��������
		mListing.SetAddress(i);
		//������ ������
		mListing.BeginLine();
		//��������� ���������� ���������� ��������
		UINT Count=FileSize-i;
		if(Count>LINE_COUNT) Count=LINE_COUNT;
		//������� �������
		for(UINT j=0;j<Count;++j)
		{
			_stprintf(s,TEXT("%02X"),pFile[i+j]);
			mListing.Print(0x10+j*3,s);
#ifdef UNICODE
			WCHAR ws[2];
			MultiByteToWideChar(CP_ACP,0,(LPCSTR)(pFile+i+j),1,ws,1);
			ws[1]=0;
			mListing.Print(0x10+LINE_COUNT*3+0x8+j,ws);
#else
			CHAR s[2];
			s[0]=pFile[i+j];
			s[1]=0;
			mListing.Print(0x10+LINE_COUNT*3+0x8+j,s);
#endif
		}

		//��������� ������
		mListing.EndLine();

		//���� ���� �����������, ���������
		if(mListing.IsOverflow()) break;
	}
}
