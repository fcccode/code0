#ifndef QS_DIALOG_GOTOXREFERENCE_H
#define QS_DIALOG_GOTOXREFERENCE_H

/*
���� �������� ���������� ������ CDialogGoToXReference,
��������������� ������ �� ������� ������������ ������.
*/

#include "dialog.h"
#include "usercontrol_list.h"
#include "address_data.h"
#include "address_map.h"
#include "listing_string.h"

class CDialogGoToXReferenceListElement;

class CDialogGoToXReference : public CDialog
{
	friend class CDialogGoToXReferenceListElement;
protected:
	//������ ������
	CAddressData* mpAddressData;
	//��������� �� ���������� ��� ������ ����������
	CYBER_ADDRESS* mpAddress;
	//������ ������������ ������
	CUserControlList* mpList;
	//������� ��� ������
	CListingString mListing;

protected:
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:

	//������� ������
	BOOL Show(HWND hWndParent,CAddressData* pAddressData,CAddressMap* pAddressMap,CYBER_ADDRESS* pAddress);
};

#endif

