#ifndef QS_DANGLE_GROUP_H
#define QS_DANGLE_GROUP_H

/*
���� �������� ����� CDangleGroup, ����������� �������
���� � �������� ������ CDangleWindow.
����� ������, ��� ���� � ������� �������, ��� ������� ��������� ���������� ��������� �������.

�����, ��� ���������� ������ (HWND) ��������� ������ CDandleGroup, � ������ �� ������
��������� ���� �������� CDandleWindow.
*/

#include "window.h"
#include "dangle_window.h"
#include <list>

//�����������
#define NDG_CHANGEWINDOW						0x7777			//��������� ������������ ����
//��������� ����������� � ������������ ����
struct NDG_CHANGEWINDOW_STRUCT
{
	NMHDR Header;
	HMENU hMenu;													//���� ������ ���� (NULL, ���� ���)
	HACCEL hAccelerators;									//������� ������������� ������ ����
};
#define NDG_WINDOWUPDATED						0x7778			//������� ����������, ����� �������� ����, ������� ������������� � �.�.
struct NDG_WINDOWUPDATED_STRUCT
{
	NMHDR Header;
	HWND hWindow;													//���� �������, ����� ��� ����� ���� �����
};

class CDangleGroup : public CWindow
{
protected:
	typedef std::list<CDangleWindow*> DGWL;
	typedef DGWL::iterator DGWLI;

	//������ ������������ �������
	HWND mhTabbar;
	//���� ��������� ������ �������
	BOOL mTabbarVisible;

	//������ ����
	DGWL mWindowsList;
	//������� ��������� ����
	DGWLI mSelectedWindowIterator;

protected:
	//����������� ���������
	VOID OnCreate(CREATESTRUCT* pCreateStruct);
	VOID OnSize(UINT Width,UINT Height);
	BOOL OnCommand(UINT CommandID,UINT Code,HWND hControl);
	VOID OnNotify(NMHDR* pNotify);

	//��������� ��������� ���������
	virtual LRESULT WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//����������� � ����������
	CDangleGroup(HWND hWndParent,RECT Rect,BOOL TabbarVisible=TRUE);
	~CDangleGroup();

	//�������� ���� � ������
	VOID AddWindow(CDangleWindow* pWindow);
	//�������� ��������� �� ������ ����
	DGWL* GetWindows();
	//������� ������� ����
	VOID CloseSelectedWindow();

	//�������� ������� ����
	CDangleWindow* GetCurrentWindow();
};

#endif
