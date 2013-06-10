#include "painted_window.h"
#include "application.h"

// class CPaintedWindow

CPaintedWindow::~CPaintedWindow()
{
}

LRESULT CPaintedWindow::WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//���� ��������� - ���������
	if(Msg==WM_PAINT)
	{
		//��������� ���������
		Paint(gApplication.mhdcBackBuffer);
		//����������� ������������ �� ����
		PAINTSTRUCT ps;
		HDC hdc=BeginPaint(mhWindow,&ps);
		BitBlt(hdc,ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top,gApplication.mhdcBackBuffer,ps.rcPaint.left,ps.rcPaint.top,SRCCOPY);
		EndPaint(mhWindow,&ps);

		//������� �����
		return 0;
	}
	else
		//�������� ��������� ������������� �������
		return PaintedWindowProc(Msg,wParam,lParam);
}
