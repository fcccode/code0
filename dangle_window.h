#ifndef QS_DANGLE_WINDOW_H
#define QS_DANGLE_WINDOW_H

/*
���� �������� ����������� ������ "����������� ����",
������� ������������ ����� ���� ���� ���������.

����������� ������ ������ ����������� ������� Paint,
������� ��������� �� ������ CPaintedWindow.

�������� ���� ���������� ��� ������ Create(),
�������� - ��� �������� �������.
*/

#include "painted_window.h"

//������������ ������������� ����� ����� ����
#define MAX_DANGLE_NAME_LENGTH			30

//��������� ���������� �� ���������� �������
struct DANGLE_WINDOW_UI
{
	HMENU hMenu;										//���� �������
	HACCEL hAccelerators;						//������� ������������� �������
};

class CDangleWindow : public CPaintedWindow
{
protected:
	//��� ����
	LPTSTR mszName;
	//���������� �� ���������� �������
	DANGLE_WINDOW_UI mUI;

protected:
	//��������� (������� ���������������)
	virtual VOID Paint(HDC hdc) PURE;
	//������� ���������
	virtual LRESULT PaintedWindowProc(UINT Msg,WPARAM wParam,LPARAM lParam);

	//������������� ������� ���������
	virtual LRESULT DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam) PURE;

public:
	//����������� � ����������
	CDangleWindow();
	virtual ~CDangleWindow();

	//�������� ��� ����
	LPCTSTR GetName();
	//�������� ���������� �� ����������
	VOID GetUI(DANGLE_WINDOW_UI* pUI);

	//������� ����
	VOID Create(HWND hWndParent,RECT Rect);
	//������� ����
	VOID Delete();
};

#endif
