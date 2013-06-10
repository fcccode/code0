#ifndef QS_PAINTED_WINDOW_H
#define QS_PAINTED_WINDOW_H

/*
���� �������� ����������� ������-���������� ��� ������� ���� CWindow,
������������ ���������� ��� �������� (� ������� �������).
*/

#include "window.h"

class CPaintedWindow : public CWindow
{
protected:
	//������������� ������� ���������
	virtual VOID Paint(HDC hdc) PURE;

	//������� ��������� ���������, ������������� �� CWindow
	virtual LRESULT WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam);

	//������������� ������� ��������� ���������
	virtual LRESULT PaintedWindowProc(UINT Msg,WPARAM wParam,LPARAM lParam) PURE;

public:
	//����������
	virtual ~CPaintedWindow();
};

#endif
