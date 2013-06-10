#ifndef QS_DANGLEWINDOW_DANGLEGROUP_H
#define QS_DANGLEWINDOW_DANGLEGROUP_H

/*
���� �������� ����������� ������ ������� - ������ �������,
������� ������������ ��� ����������� �������� ������������.
*/

#include "dangle_window.h"
#include "dangle_group.h"

//���� �������
#define DWDG_PANE_FLOAT									0
#define DWDG_PANE_FULL									1
#define DWDG_PANE_MAIN									2
#define DWDG_PANE_LEFT									3
#define DWDG_PANE_RIGHT									4

class CDangleWindowDangleGroup : public CDangleWindow
{
protected:
	//��������� ������ �������
	CDangleGroup* mpDangleGroup;
	//��� ������
	UINT mPane;
	//��������� �� ����������� �������
	CDangleWindowDangleGroup* mpPaneMain;
	CDangleWindowDangleGroup* mpPaneLeft;
	CDangleWindowDangleGroup* mpPaneRight;

public:
	//����������� � ����������
	CDangleWindowDangleGroup(LPCTSTR szName,UINT Pane);
	virtual ~CDangleWindowDangleGroup();

	//������� ���������
	virtual LRESULT DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//���������
	virtual VOID Paint(HDC hdc);

	//�������� ��������� ������ �������
	CDangleGroup* GetGroup();

	//������� ����������� ������
	VOID CreateStandardPanes();
	//�������� �������
	CDangleGroup* GetMainPane();
	CDangleGroup* GetLeftPane();
	CDangleGroup* GetRightPane();
};

#endif
