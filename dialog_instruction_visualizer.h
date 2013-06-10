#ifndef QS_DIALOG_INSTRUCTION_VISUALIZER_H
#define QS_DIALOG_INSTRUCTION_VISUALIZER_H

/*
���� �������� ����������� ������ ������� - ������������� ����������.
*/

#include "dialog.h"
#include "cyber_memory.h"
#include "instruction_set.h"

class CDialogInstructionVisualizer : public CDialog
{
protected:
	//��������������� ��������� "����� ����"
	struct CHUNK
	{
		INT FirstByte;						//������ ���������� ����
		INT BytesCount;						//���������� ���������� ������

		INT Left;									//����� ������� ���������
		INT Right;								//������ ������� ���������

		LPTSTR szCode;						//���
		LPTSTR szMessage;					//���������
	};
protected:
	//�������� ��� ����������
	PBYTE mpCode;
	//����� ����
	UINT mCodeLength;
	//�������� ����������� ��� �����������
	HENHMETAFILE mhImageMetaFile;

	//����� ����������
	CInstructionSet* mpInstructionSet;

	//����������� ������� ����� ��� ���������
	static HFONT mhFontBig;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//�������� �����������
	VOID Update();
	//������������ �����������
	VOID MakeImage(HDC hdc);

public:
	//�����������
	CDialogInstructionVisualizer();

	//������� ������
	VOID Show(HWND hWndParent);
	//������� ������ � �������� �����������
	VOID Show(CCyberMemory* pMemory,CYBER_ADDRESS Address,UINT Size,HWND hWndParent);
};

#endif
