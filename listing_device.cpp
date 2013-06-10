#include "listing_device.h"
#include "application.h"

// class CListingDevice

HPEN CListingDevice::mhDashPen=CreatePen(PS_DASH,1,0);

CListingDevice::CListingDevice()
{
	//���������������� ����������
	mControlGroup=FALSE;
}

VOID CListingDevice::SetSelecting(BOOL Select)
{
	//��������� ���� ���������
	mSelect=Select;
}

VOID CListingDevice::SetContext(LISTING_DEVICE_CONTEXT Context)
{
	//��������� ��������� ���������
	mContext=Context;

	//�������� ������ � ������ ������ �������
	GetTextExtentPoint32(mContext.hdc,TEXT("W"),1,&mCharSize);

	//���������� ��������� ��������� ���������
	SetBkMode(mContext.hdc,TRANSPARENT);
	SetTextColor(mContext.hdc,0);

	//�������� ���������� ��� ������������ ����� � ����������� �����
	mPrintedCount=0;
	mGroupCount=0;
	//�������� ���� ���������
	mSelect=FALSE;
}

VOID CListingDevice::SetControlGrouping()
{
	//���������� ���� ����������� ������
	mControlGroup=TRUE;
	//�������� ����� ���������� �������
	mVisesSet.clear();
}

VOID CListingDevice::EndLine()
{
	//���� ��� ����� ����������� ������
	if(!mControlGroup)
	{
		//���������� �������������
		RECT Rect=mContext.Rect;

		//��������� ���������� Y
		INT Y=Rect.top+mPrintedCount*mCharSize.cy;

		//���� ������ ��������
		if(mSelect)
			//���������� ���������� �������������
			PatBlt(mContext.hdc,Rect.left,Y,Rect.right-Rect.left,mCharSize.cy,PATCOPY);

		//���������� ������
		TextOut(mContext.hdc,Rect.left,Y,mszLineText,mLineTextLength);

		//��������� ���������� ������������ �����
		mPrintedCount++;
		//��������� ���������� ����� � ������
		mGroupCount++;
	}
}

VOID CListingDevice::GroupLines()
{
	//���� ���� ���� ����������� ������
	if(mControlGroup)
	{
		//�������� ����
		mControlGroup=FALSE;
	}
	else
		//���� ��������� ������ ��������,
		//��� ����� ������ ���� � ������ ���������� �������
		if(mSelect || (mVisesSet.find(mAddress)!=mVisesSet.end()))
		{
			//���������� ������������ �������������
			HBRUSH hLastBrush=SelectBrush(mContext.hdc,GetStockBrush(NULL_BRUSH));
			HPEN hLastPen=SelectPen(mContext.hdc,mSelect ? GetStockPen(BLACK_PEN) : mhDashPen);
			Rectangle(mContext.hdc,
				mContext.Rect.left,mContext.Rect.top+(mPrintedCount-mGroupCount)*mCharSize.cy,
				mContext.Rect.right,mContext.Rect.top+mPrintedCount*mCharSize.cy
/*				mContext.Rect.left+mCharSize.cx*(mColumnMargins[LISTING_COLUMN_LABEL]-1),
				mContext.Rect.top+(mPrintedCount-mGroupCount)*mCharSize.cy,
				mContext.Rect.left+mCharSize.cx*mColumnMargins[LISTING_COLUMN_LABEL],
				mContext.Rect.top+mPrintedCount*mCharSize.cy*/
				);
			SelectBrush(mContext.hdc,hLastBrush);
			SelectPen(mContext.hdc,hLastPen);
		}
	//�������� ������
	mGroupCount=0;
}

BOOL CListingDevice::IsOverflow()
{
	//�������, ���� �� ������������
	return (mPrintedCount*mCharSize.cy)>=(mContext.Rect.bottom-mContext.Rect.top);
}

UINT CListingDevice::PrintOffset(UINT Address)
{
	//���� ������� ���� ����������� ������
	if(mControlGroup)
		//�������� ��������� ����� � ����� ���������� �������
		mVisesSet.insert(Address);
	//������� �����
	return CListing::PrintOffset(Address);
}
