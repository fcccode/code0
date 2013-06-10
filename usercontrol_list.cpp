#include "usercontrol_list.h"
#include "application.h"

// class CUserControlListElement

CUserControlListElement::~CUserControlListElement()
{
	//������ �� ������, ��� ��������
}

// class CUserControlList

CUserControlList::CUserControlList(UINT ControlID,HWND hWindow)
{
	//��������� ������������� �������� ����������, ��� ���� � ����� ��������� ����������
	mControlID=ControlID;
	mhWindow=hWindow;
}

CUserControlList::~CUserControlList()
{
	//�������� ������
	Clear();
}

VOID CUserControlList::AddElement(CUserControlListElement* pElement)
{
	//�������� ������� � ������, ������ ����� �������� � �������� ������ ��������
	ListBox_AddString(mhWindow,pElement);
}

CUserControlListElement* CUserControlList::GetSelectedElement()
{
	//�������� ������ ����������� ��������
	UINT ElementIndex=ListBox_GetCurSel(mhWindow);
	if(ElementIndex==LB_ERR) return NULL;
	return (CUserControlListElement*)ListBox_GetItemData(mhWindow,ElementIndex);
}

VOID CUserControlList::DeleteSelectedElement()
{
	//�������� ���������� �������
	UINT ElementIndex=ListBox_GetCurSel(mhWindow);
	if(ElementIndex!=LB_ERR)
	{
		//������� �������
		delete (CUserControlListElement*)ListBox_GetItemData(mhWindow,ElementIndex);
		ListBox_DeleteString(mhWindow,ElementIndex);
	}
}

VOID CUserControlList::Clear()
{
	//������� ��� �������� � ������
	while(ListBox_GetCount(mhWindow))
	{
		//������� �������
		delete (CUserControlListElement*)ListBox_GetItemData(mhWindow,0);
		ListBox_DeleteString(mhWindow,0);
	}
}

BOOL CUserControlList::IsExistSelection()
{
	//������� �����
	return ListBox_GetCurSel(mhWindow)!=LB_ERR;
}

BOOL CUserControlList::IsEmpty()
{
	//������� �����
	return ListBox_GetCount(mhWindow)==0;
}

void CUserControlList::BeginUpdate()
{
	SendMessage(mhWindow, WM_SETREDRAW, FALSE, 0);
}

void CUserControlList::EndUpdate()
{
	SendMessage(mhWindow, WM_SETREDRAW, TRUE, 0);
}

BOOL CUserControlList::ProcessMessages(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//���������, ��� ��������� ��������� ������ � ����� ������
	if(wParam!=mControlID) return FALSE;

	//����� �� ���������
	switch(Msg)
	{
	case WM_DRAWITEM:				//���������
		{
			//�������� ��������� � ����������� � ���������
			DRAWITEMSTRUCT* pDrawItem=(DRAWITEMSTRUCT*)lParam;
			//���� ��������� ���, ���������
			if(pDrawItem->itemID==LB_ERR) return TRUE;

			//���������� ��������� ���������
			HDC hdc=pDrawItem->hDC;
			RECT Rect=pDrawItem->rcItem;

			//������� �����
			HFONT hLastFont=SelectFont(hdc,gApplication.mhFontCode);
			//���������� ����� ������������
			SetBkMode(hdc,TRANSPARENT);
			//�������� ���� � ����� � ��������
			HPEN hLastPen=SelectPen(hdc,gApplication.mhPenSelect);
			HBRUSH hLastBrush=SelectBrush(hdc,gApplication.mhBrushSelect);

			//���� ������� �������
			if(pDrawItem->itemState & ODS_SELECTED)
				//���������� ���������� �������������
				Rectangle(hdc,Rect.left,Rect.top,Rect.right,Rect.bottom);
			//����� �������� �������������
			else
				FillRect(hdc,&Rect,GetStockBrush(WHITE_BRUSH));

			//���������� �������
			((CUserControlListElement*)pDrawItem->itemData)->Draw(hdc,&Rect);

			//������� ������� ���� � �����
			SelectPen(hdc,hLastPen);
			SelectBrush(hdc,hLastBrush);
			//������� �����
			SelectFont(hdc,hLastFont);
		}
		return TRUE;
	case WM_MEASUREITEM:		//�������� ���������
		{
			//�������� ���������
			MEASUREITEMSTRUCT* pMeasureItem=(MEASUREITEMSTRUCT*)lParam;
			//������� ������ ��������
			pMeasureItem->itemHeight=((CUserControlListElement*)pMeasureItem->itemData)->GetHeight();
		}
		return TRUE;
	}
	//��������� �� ����������
	return FALSE;
}
