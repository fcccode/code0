#ifndef QS_LISTING_DEVICE_H
#define QS_LISTING_DEVICE_H

/*
���� �������� ����������� �������-��������, ���������������
����� �� �������� DC.

������� ������������ ���� ����������� ������, ������� ���������
��� ������, ��������� � ���, � �������� ��������������� ������.
���� ����������� ������ ������������� ������������ ����� ������
������ GroupLines().
*/

#include "listing.h"
#include <set>

//��������� ���������� ��������� ������
struct LISTING_DEVICE_CONTEXT
{
	HDC hdc;																			//�������� ��� ���������
	RECT Rect;																		//�������������� �������������
};

class CListingDevice : public CListing
{
protected:
	LISTING_DEVICE_CONTEXT mContext;							//��������� ��������� ������
	SIZE mCharSize;																//������� ������ �������

	INT mPrintedCount;														//���������� ��� ������������ ����� � ���� ���������
	INT mGroupCount;															//���������� ����� � ������
	BOOL mSelect;																	//���� ��������� �����
	BOOL mControlGroup;														//���� ����������� ������

	std::set<UINT> mVisesSet;											//����� ���������� �������

	static HPEN mhDashPen;												//���������� ���� ��� ��������� ��������� �������

public:
	//�����������
	CListingDevice();

	//������� ���� ���������
	VOID SetSelecting(BOOL Select);
	//������� ������� ��������
	VOID SetContext(LISTING_DEVICE_CONTEXT Context);

	//������� ���� ����������� ������
	VOID SetControlGrouping();

	//��������� ������
	virtual VOID EndLine();
	//������������� ��������� ��������� �����, ������� � ���������� ������ SetAddress
	VOID GroupLines();

	//��������, ���� �� ������������ ������� ������ �� ���������� �����
	BOOL IsOverflow();

	//�������� ��������� ������������� ������
	//����� ���������� �� CListing ��� ��������� ����������� �����
	virtual UINT PrintOffset(UINT Address);
};

#endif
