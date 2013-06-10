#ifndef QS_USERCONTROL_LIST_H
#define QS_USERCONTROL_LIST_H

/*
���� �������� ����������, ��������� � ���������� "����������" ��������
���������� "������".
����� CUserControlList ������������ ��������� � ��������� ���������
��������� ������. ���� ��������� ��������� ����������� ��� ����� ������
��������� ������.

�� ������� ����������� ������������ ����������� ����������� ���������,
����� ���������� � �������� ���������.
����� ����������� ������������ ������ ������ �� ������ LBS_OWNERDRAWVARIABLE.
*/

#include "windows.h"

//������������� ������� ����� ��� ��������� �������
#define LIST_ELEMENT_MARGIN_HEADER		20
#define LIST_ELEMENT_MARGIN_TEXT			40

//����������� ����� �������� ������
class CUserControlListElement
{
public:
	//����������
	virtual ~CUserControlListElement();

	//���������� �������
	virtual VOID Draw(HDC hdc,RECT* pRect) PURE;
	//�������� ������ ��������
	virtual UINT GetHeight() PURE;
};

//����� ������
class CUserControlList
{
protected:
	//������������� �������� ����������
	UINT mControlID;
	//���� �������� ����������
	HWND mhWindow;

public:

	CUserControlList(UINT ControlID,HWND hWindow);
	virtual ~CUserControlList();
	
	//�������� ������� � ������
	VOID AddElement(CUserControlListElement* pElement);
	//�������� ���������� ������� ������
	CUserControlListElement* GetSelectedElement();
	//������� ���������� ������� �� ������
	VOID DeleteSelectedElement();
	//�������� ������
	VOID Clear();
	//���� �� ���������� �������
	BOOL IsExistSelection();
	//���� �� ������
	BOOL IsEmpty();
	//������ ���������� ������ (������������� ���������� �����������)
	void BeginUpdate();
	//��������� ���������� ������
	void EndUpdate();

	//���������� ���������; TRUE, ���� ����������
	BOOL ProcessMessages(UINT Msg,WPARAM wParam,LPARAM lParam);
};

#endif
