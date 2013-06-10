#ifndef QS_CODE_EXPLORER_WINDOW_H
#define QS_CODE_EXPLORER_WINDOW_H

/*
���� �������� ����������� ������ ���� ���������.
������ ����������� ���� ����� ����� ��������� ���� ���������.
*/

#include "dangle_window.h"
#include "address_map.h"
#include "cyber_memory.h"
#include "code_parser.h"
#include "explored_image.h"
#include "listing_device.h"
#include <list>

//����� ��������� ���� ���������
class CDangleWindowCode : public CDangleWindow
{
protected:
	//��������� �� ����������� ����
	CExploredImage* mpExploredImage;
	//��������� �� ����� �������
	CAddressMap* mpAddressMap;
	//��������� �� �����������
	CCyberMemory* mpMemory;
	//��������� �� ���������� ����
	CCodeParser* mpCodeParser;

	//��������� �� �������, �������������� ��� ������
	CListingDevice* mpListing;
	//������� ����� ���������
	CYBER_ADDRESS mAddress;
	//���������� �����
	CYBER_ADDRESS mSelectedAddress;
	//�������� ����� ��������� (��������� �����, ������� ��������� � ����)
	CYBER_ADDRESS mEndAddress;
	//��� ����
	TCHAR mszNameBuffer[MAX_DANGLE_NAME_LENGTH+1];

	//������ ������� ���������
	std::list<CYBER_ADDRESS> mHistoryList;
	//�������� ������ ��������� � ������
	std::list<CYBER_ADDRESS>::iterator mHistoryCurrentIterator;

protected:
	//��������� ��������� ���������
	VOID OnKey(UINT Key);
	VOID OnCommand(UINT CommandID,UINT Code,HWND hControl);
	VOID OnMouseWheel(INT Delta,UINT Keys);

	//��������������� ���������

	//�������� ���
	VOID OffsetView(INT Offset);
	//�������� ���������
	VOID OffsetSelection(INT Offset);
	//������� � ��������� ������
	VOID GoToAddress(CYBER_ADDRESS Address);
	//������� ����� � ������� ���������
	VOID GoHistoryBack();
	//������� ������ � ������� ���������
	VOID GoHistoryForward();
	//��������� ������� ����� � ������� ������ ������� ���������
	VOID KeepHistoryCurrent();

public:
	//����������� � ����������
	CDangleWindowCode(CExploredImage* pExploredImage,CYBER_ADDRESS Address,LPCTSTR szFileName);
	virtual ~CDangleWindowCode();

	//������� ���������
	virtual LRESULT DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam);
	//���������
	virtual VOID Paint(HDC hdc);
};

#endif
