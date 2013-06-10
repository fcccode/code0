#ifndef QS_MAIN_WINDOW_H
#define QS_MAIN_WINDOW_H

/*
���� �������� ���������� ������ CMainWindow,
������������ ������� �����.
*/

#include "window.h"
#include "dangle_group.h"

class CMainWindow : public CWindow
{
protected:
	//������ "�����������" ����
	CDangleGroup* mpDangleGroup;

	//���������� ���� � �������� ����
	UINT mMainMenuItemsCount;
	//������� ������������� ������� �������
	HACCEL mhCurrentWindowAccelerators;

protected:
	//����������� ���������
	BOOL OnCreate(CREATESTRUCT* pCreateStruct);
	VOID OnClose();
	VOID OnDestroy();
	VOID OnSize(UINT Width,UINT Height);
	BOOL OnCommand(INT ControlID);
	VOID OnNotify(NMHDR* pNotify);
	//������� �������
	virtual LRESULT WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	CMainWindow();
	~CMainWindow();

	//������� ������� ���������
	VOID Show();
};

#endif
