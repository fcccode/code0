#ifndef QS_DIALOG_INSTRUCTIONASSEMBLER_H
#define QS_DIALOG_INSTRUCTIONASSEMBLER_H

/*
���� �������� ���������� ������ CDialogInstructionAssembler,
������������ �������� ���������������.
*/

#include "dialog.h"
#include "usercontrol_list.h"
#include "address_data.h"
#include "address_map.h"
#include "listing_string.h"

class CDialogInstructionAssembler : public CDialog
{
protected:
	//����� �������� ������ ���������
	class CListElement : CUserControlListElement
	{
	public:
		//��� ����������
		PBYTE mpCode;
		//������ ����
		UINT mCodeSize;
		//����������
		CInstruction* mpInstruction;

	public:
		//����������� � ����������
		CListElement();
		virtual ~CListElement();

		//���������� �������
		virtual VOID Draw(HDC hdc,RECT* pRect);
		//�������� ������ ��������
		virtual UINT GetHeight();
	};
protected:
	//������ ��������� ���������������
	CUserControlList* mpList;
	//������� ��� ������
	CListingString mListing;

protected:
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:

	//������� ������
	BOOL Show(HWND hWndParent);
};

#endif

