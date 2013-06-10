#ifndef QS_DANGLE_GROUPER_H
#define QS_DANGLE_GROUPER_H

/*
���� �������� ����������� ������, ������������ ����������
����������� �������� ������� DangleGroup.
*/

#include "dangle_group.h"
#include <list>

class CDangleGrouper : public CObject
{
protected:
	//������ ����� �������
	std::list<CDangleGroup*> mGroupsList;
	//������� ������ �������
	std::list<CDangleGroup*>::iterator mSelectedGroupIterator;
	//������� ����
	HWND mhWindow;

public:
	//����������� � ����������
	CDangleGrouper(HWND hWindow);
 	virtual ~CDangleGrouper();

	//��������� ��������� ���������
	LRESULT WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam);

	//�������� ������� ������
	CDangleGroup* GetCurrentGroup();
};

#endif
