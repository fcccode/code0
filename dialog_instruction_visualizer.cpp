#include "dialog_instruction_visualizer.h"
#include "application.h"
#include "consts.h"
#include "resource.h"
#include <list>

// class CDialogInstructionVisualizer

#define SHADOW_WIDTH					3

HFONT CDialogInstructionVisualizer::mhFontBig=NULL;

BOOL CDialogInstructionVisualizer::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//�������� �����������
		Update();
		return TRUE;
	case WM_DRAWITEM:							//����������� �����������
		//������� �������� �����������
		PlayEnhMetaFile(((DRAWITEMSTRUCT*)lParam)->hDC,mhImageMetaFile,&((DRAWITEMSTRUCT*)lParam)->rcItem);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			//������� ������
			EndDialog(mhWindow,0);
			return TRUE;
		}
		return FALSE;
	case WM_DESTROY:
		//������� ��������
		if(mhImageMetaFile)
			DeleteEnhMetaFile(mhImageMetaFile);
		//���������� ������
		SafeDeleteMassive(mpCode);
		return TRUE;
	}
	return FALSE;
}

//��������� ���������� �����������
VOID CDialogInstructionVisualizer::Update()
{
	//�������� ������������� �������� ���������� - ��������
	RECT rect;
	GetClientRect(GetDlgItem(mhWindow,IDC_PICTURE_VISUALIZATION),&rect);
	INT Width=rect.right-rect.left;
	INT Height=rect.bottom-rect.top;

	//������� ���������� ��������
	if(mhImageMetaFile)
		DeleteEnhMetaFile(mhImageMetaFile);

	//������� ��������
	HDC hdc=CreateEnhMetaFile(NULL,NULL,NULL,NULL);

	//���������� ����
	SelectPen(hdc,GetStockPen(BLACK_PEN));
	SelectBrush(hdc,GetStockBrush(BLACK_BRUSH));
	Rectangle(hdc,SHADOW_WIDTH,SHADOW_WIDTH,Width,Height);
	//���������� �����
	SelectBrush(hdc,GetStockBrush(WHITE_BRUSH));
	Rectangle(hdc,0,0,Width-SHADOW_WIDTH,Height-SHADOW_WIDTH);

	//���� ��� �����
	if(mCodeLength)
		//������������ �����������
		MakeImage(hdc);
	//����� ��� �� �����
	else
	{
		//������� ���������
		SelectFont(hdc,gApplication.mhFontCode);
		DrawText(hdc,TEXT("Enter text into \"code\" edit box."),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	//������� ��������
	mhImageMetaFile=CloseEnhMetaFile(hdc);
}

//������������ ����������� ���� ����������
VOID CDialogInstructionVisualizer::MakeImage(HDC hdc)
{
	//������ "������"
	std::list<CHUNK> ChunksList;

	//������� ������� ���������
	INT CurrentMargin=SHADOW_WIDTH;

	//����� ��������
	for(UINT i=0;i<mCodeLength;++i)
	{
		//�������� ����
		BYTE Byte=mpCode[i];
		//���� ���� ���� - �������
		if(mpInstructionSet->mInstructionBytes[i].bFlags & (IBF_OK | IBF_PREFIX))
		{
			//�������� ���� � ����� ��������
			DWORD PrefixFlag=mpInstructionSet->mInstructionBytes[i].dwPrefixFlag;
			for(BYTE Prefix=0;Prefix<PREFIXES_COUNT;++Prefix)
				if(PrefixFlag==(1<<Prefix))
					break;
			//�������� �����
			CHUNK Chunk;
			Chunk.FirstByte=i;
			Chunk.BytesCount=1;
			Chunk.szCode=new TCHAR[3];
			_stprintf(Chunk.szCode,TEXT("%02x"),Byte);
			Chunk.szMessage=new TCHAR[26+_tcslen(gcszPrefixNames[Prefix])];
			_stprintf(Chunk.szMessage,TEXT("prefix byte for \'%s\' prefix"),gcszPrefixNames[Prefix]);

			ChunksList.push_back(Chunk);
		}
	}

	//���������� ��� ����������
}




























CDialogInstructionVisualizer::CDialogInstructionVisualizer()
{
	//���������������� ����������
	mhImageMetaFile=NULL;

	//������� �����, ���� �� ��� �� ������
	if(!mhFontBig)
	{
		LOGFONT lf;
		GetObject(gApplication.mhFontCodeBold,sizeof(lf),&lf);
		lf.lfHeight*=3;
		mhFontBig=CreateFontIndirect(&lf);
	}
}

VOID CDialogInstructionVisualizer::Show(HWND hWndParent)
{
	//���������������� ���
	mpCode=NULL;
	mCodeLength=0;
	//������� ������
	Ask(MAKEINTRESOURCE(IDD_DIALOG_INSTRUCTION_VISUALIZER),hWndParent);
}

VOID CDialogInstructionVisualizer::Show(CCyberMemory* pMemory,CYBER_ADDRESS Address,UINT Size,HWND hWndParent)
{
	//������� ��� ����������
	mpCode=new BYTE[Size];
	try
	{
		pMemory->Data(mpCode,Address,Size);
	}
	catch(CCyberMemoryPageFaultException)
	{
		_ASSERTE(("����������� ���������� page fault",TRUE));
	}
	//��������� �����
	mCodeLength=0;
	//������� ������
	Ask(MAKEINTRESOURCE(IDD_DIALOG_INSTRUCTION_VISUALIZER),hWndParent);
}
