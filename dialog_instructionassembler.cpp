#include "dialog_instructionassembler.h"
#include "usercontrol_list.h"
#include "application.h"
#include "listing_string.h"
#include "resource.h"

// class CDialogInstructionAssembler::CListElement

CDialogInstructionAssembler::CListElement::CListElement()
{
	//�������������
	mpCode=NULL;
	mpInstruction=NULL;
}

CDialogInstructionAssembler::CListElement::~CListElement()
{
	//���������� �������
	SafeDeleteMassive(mpCode);
	SafeRelease(mpInstruction);
}

VOID CDialogInstructionAssembler::CListElement::Draw(HDC hdc,RECT* pRect)
{
}

UINT CDialogInstructionAssembler::CListElement::GetHeight()
{
	//������� ������ ������
	return gApplication.mFontCodeHeight;
}

// class CDialogInstructionAssembler

BOOL CDialogInstructionAssembler::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ������
		mpList=new CUserControlList(IDC_LIST_XREFERENCES,GetDlgItem(mhWindow,IDC_LIST_XREFERENCES));
		return TRUE;
	case WM_DESTROY:
		//������� ������
		SafeDelete(mpList);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:										//������ Cancel
			EndDialog(mhWindow,FALSE);
			return TRUE;
/*		case IDC_LIST_XREFERENCES:				//������ ���������
			//���� ����������� � ���, ��� ��������� � ������ ����������
			if(HIWORD(wParam)==LBN_SELCHANGE && mpList->IsExistSelection())
				//���-�� �������
				;
			return TRUE;*/
		}
		return FALSE;
	}
	//���� �� �� ������������ ���������, ��, �����, ��� ���������� ������
	if(mpList) return mpList->ProcessMessages(Msg,wParam,lParam);
	//����� ������ �� ������
	return FALSE;
}

BOOL CDialogInstructionAssembler::Show(HWND hWndParent)
{
	//���������������� ��������� �� ������
	mpList=NULL;

	//������� ������
	return Ask(MAKEINTRESOURCE(IDD_DIALOG_INSTRUCTIONASSEMBLER),hWndParent);
}
