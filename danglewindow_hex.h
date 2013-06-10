#ifndef QS_DANGLEWINDOW_HEX_H
#define QS_DANGLEWINDOW_HEX_H

/*
���� �������� ����������� ������ ���� ��������� ����� � HEX-������.
*/

#include "dangle_window.h"
#include "edited_file.h"
#include "listing_device.h"

class CDangleWindowHex : public CDangleWindow
{
protected:
	//��������� �� ������������� ����
	CEditedFile* mpEditedFile;
	//����� �� ������ �����, ��������������� ������ ����
	UINT mWindowOffset;
	//����� ����������� ����� ��� ������ ���������
	UINT mSelectionOffset;
	//����� ���������
	UINT mSelectionLength;
	//������� ��� ������
	CListingDevice mListing;
	//���������� ������������� � ���� �����
	UINT mWindowLinesCount;

	//��� ����
	TCHAR mszNameBuffer[MAX_DANGLE_NAME_LENGTH+1];

protected:
	//��������� ��������� ���������
	VOID OnKey(UINT Key);
	VOID OnCommand(UINT CommandID,UINT Code,HWND hControl);
	VOID OnMouseWheel(INT Delta,UINT Keys);

public:
	//����������� � ����������
	CDangleWindowHex(CEditedFile* pEditedFile);
	virtual ~CDangleWindowHex();

	//������� ���������
	virtual LRESULT DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//���������
	virtual VOID Paint(HDC hdc);
};

#endif
