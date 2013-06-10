#ifndef QS_DANGLEWINDOW_IMPORTS_H
#define QS_DANGLEWINDOW_IMPORTS_H

/*
���� �������� ����������� ������ ���� ������ ������������� �������.
*/

#include "dangle_window.h"
#include <list>

//����� ��������� ���� �������
class CDangleWindowImports : public CDangleWindow
{
public:
	//�������� ������ �������� ������ �������
	class CImportListItem;

protected:
	//������ ������������� �������
	std::list<CImportListItem*> mList;

public:
	//����������� � ����������
	CDangleWindowImports();
	virtual ~CDangleWindowImports();

	//������� ���������
	virtual LRESULT DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//���������
	virtual VOID Paint(HDC hdc);
};

#endif
