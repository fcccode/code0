#include "dangle_grouper.h"

// class CDangleGrouper

CDangleGrouper::CDangleGrouper(HWND hWindow)
{
	//��������� ���������� ����
	mhWindow=hWindow;
}

CDangleGrouper::~CDangleGrouper()
{
	//���������� ��� ������
	for(std::list<CDangleGroup*>::iterator i=mGroupsList.begin();i!=mGroupsList.end();i++)
		(*i)->Release();
	mGroupsList.clear();
}

LRESUTL CDangleGrouper::WindowProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_SIZE:
		// Haven't done.
		return 0;
	}

	//��� ���������, ������� �� ����������, ���������� ���������� ������
	if(mGroupsList.size())
		switch(Msg)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_COMMAND:
			(*mSelectedGroupIterator)->PostMessage(Msg,wParam,lParam);
		}
	//��������� ����������� ���������
	return DefWindowProc(mhWindow,Msg,wParam,lParam);
}

CDangleGroup* CDangleGrouper::GetCurrentGroup()
{
	//���� ������ ����
	if(mGroupsList.size())
		//������� ���������� ������
		return *mSelectedGroupIterator;
	//����� ����� ���
	return NULL;
}
