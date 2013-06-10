#ifndef QS_WINDOW_H
#define QS_WINDOW_H

/*
���� �������� ���������� ������, ����������� ���������� ������.
*/

#include "object.h"

class CWindow : public CObject
{
protected:
	//���������� ����
	HWND mhWindow;

public:
	//����������� ����������
	virtual ~CWindow();

	//������� ���������
	static LRESULT CALLBACK StaticWndProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);

	//�������� ����
	HWND GetWindowHandle();
	//��������� ���� ���������
	VOID PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam);

protected:
	//������������� ������� ���������
	virtual LRESULT WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam) PURE;
};

#endif
