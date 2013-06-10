#include "address_map.h"
#include "xreference.h"
#include "label.h"

// class CAddressMap realization

CAddressMap::~CAddressMap()
{
	//������� ������ ���� �������
	for(AM::iterator i=mAddressDataMap.begin();i!=mAddressDataMap.end();i++)
		(*i).second->Release();
	//�������� ����� �������
	mAddressDataMap.clear();
}

CAddressData* CAddressMap::OpenAddress(CYBER_ADDRESS Address)
{
	//����������� ����� ��������� ������ ������
	AM::iterator i=mAddressDataMap.find(Address);
	//���� �������, ������� ���������
	if(i!=mAddressDataMap.end())
	{
		CAddressData* pAddressData=(*i).second;
		pAddressData->AddRef();
		return pAddressData;
	}
	//���� �� �������, ������� ����� ��������� ��������� ������
	CAddressData* pAddressData=new CAddressData;
	//������� ����� ����� �� ���������
	pAddressData->SetBlockAddress(Address);
	//�������� ��� � �����
	mAddressDataMap.insert(std::pair<CYBER_ADDRESS,CAddressData*>(Address,pAddressData));
	//������� ���������
	pAddressData->AddRef();
	return pAddressData;
}

CAddressData* CAddressMap::GetAddress(CYBER_ADDRESS Address)
{
	//����� ��������� ������ ������
	AM::iterator i=mAddressDataMap.find(Address);
	//���� �������, ������� ���������
	if(i!=mAddressDataMap.end())
	{
		CAddressData* pAddressData=(*i).second;
		pAddressData->AddRef();
		return pAddressData;
	}
	//����� �� �������
	return NULL;
}

VOID CAddressMap::AddXReference(CYBER_ADDRESS AddressFrom,CYBER_ADDRESS AddressTo,BYTE ReferenceFlags)
{
	//�������� ������������ ������
	CAddressData* pAddressData=OpenAddress(AddressTo);
	CXReference* pXReference=new CXReference(AddressFrom,ReferenceFlags);
	pAddressData->AddElement(pXReference);
	pXReference->Release();
	pAddressData->Release();
}

VOID CAddressMap::SetLabel(CYBER_ADDRESS Address,LPCTSTR szName)
{
	//������� �����, ���� � ��� ���
	CAddressData* pAddressData=OpenAddress(Address);
	CLabel* pLabel=pAddressData->GetLabel();
	if(pLabel)
		//���������� ������������ �����
		pLabel->Release();
	else
	{
		//������� ����� �����
		pLabel=new CLabel(szName);
		//������� � ����� �������
		pAddressData->SetLabel(pLabel);
		//���������� �����
		pLabel->Release();
	}
	//���������� ������ ������
	pAddressData->Release();
}

CLabel* CAddressMap::GetLabel(CYBER_ADDRESS Address)
{
	//�������� ��������� ������
	CAddressData* pData=GetAddress(Address);
	if(pData)
	{
		//�������� � ������� �����
		CLabel* pLabel=pData->GetLabel();
		pData->Release();
		return pLabel;
	}
	return NULL;
}

VOID CAddressMap::SetComment(CYBER_ADDRESS Address,LPCTSTR szName)
{
	//������� �����������, ���� ��� ��� ���
	CAddressData* pAddressData=OpenAddress(Address);
	CComment* pComment=pAddressData->GetComment();
	if(pComment)
		//���������� ������������ �����������
		pComment->Release();
	else
	{
		//������� ����� �����������
		pComment=new CComment(szName);
		//������� � ����� �������
		pAddressData->SetComment(pComment);
		//���������� �����������
		pComment->Release();
	}
	//���������� ������ ������
	pAddressData->Release();
}

CComment* CAddressMap::GetComment(CYBER_ADDRESS Address)
{
	//�������� ��������� ������
	CAddressData* pData=GetAddress(Address);
	if(pData)
	{
		//�������� � ������� �����������
		CComment* pComment=pData->GetComment();
		pData->Release();
		return pComment;
	}
	return NULL;
}

VOID CAddressMap::SetAddressSize(CYBER_ADDRESS Address,DWORD Size)
{
	//������� ������ ��� ������
	CAddressData* pAddressData=OpenAddress(Address);
	pAddressData->SetSize(Size);
	pAddressData->Release();
	//������� ������ ����������� ������ ��� ���������� ������
	pAddressData=OpenAddress(Address+Size);
	pAddressData->SetPreviousSize(Size);
	pAddressData->Release();
}

LPTSTR CAddressMap::GetAddressString(CYBER_ADDRESS Address)
{
	//�������� ������ ������
	CAddressData* pAddressData=GetAddress(Address);
	if(pAddressData)
	{
		//��������, ���� �� � ������ �����
		CLabel* pLabel=pAddressData->GetLabel();
		if(pLabel)
		{
			pAddressData->Release();
			//������� ������ � ������
			LPCTSTR szLabel=pLabel->GetName();
			LPTSTR szAddress=new TCHAR[_tcslen(szLabel)+1];
			_tcscpy(szAddress,szLabel);
			pLabel->Release();
			return szAddress;
		}
		//��������, ���� �� � ������ ����� �����
		CYBER_ADDRESS BlockAddress=pAddressData->GetBlockAddress();
		pLabel=GetLabel(BlockAddress);
		if(pLabel)
		{
			pAddressData->Release();
			//������� � ����� �������� �� ������ �����
			LPCTSTR szLabel=pLabel->GetName();
			LPTSTR szAddress=new TCHAR[_tcslen(szLabel)+11];
			int offset=Address-BlockAddress;
			_stprintf(szAddress,offset >0 ? TEXT("%s+%Xh") : TEXT("%s-%Xh"),szLabel,offset>0 ? offset : -offset);
			pLabel->Release();
			return szAddress;
		}
		pAddressData->Release();
	}
	//������� ������ � �������-������
	LPTSTR szAddress=new TCHAR[10];
	_stprintf(szAddress,TEXT("0%X"),Address);
	return szAddress;
}

#include "listing_textfile.h"
#include "listing_device.h"
#include "application.h"
#include <math.h>

//TEST
VOID CAddressMap::ExplortListingToFile()
{
/*	//������� ��������
	HDC hdc=CreateEnhMetaFile(NULL,TEXT("listing.emf"),NULL,NULL);

	//������� �������
	CListingDevice Listing;
	LISTING_DEVICE_CONTEXT Context;
	Context.hdc=hdc;
//	SetRect(&Context.Rect,0,0,1024,0);
	Listing.SetAddressMap(this);
//	Listing.SetContext(Context);

	//������� �����
	SelectFont(hdc,gApplication.mhFontCode);

	//������� ������ ���� �������
	// 1024*xcount=ycount*h
	// xcount*ycount=size

	// xcount=size/ycount;
	// 1024*xcount=size/xcount*h
	// ycount=sqrt(size*1024/h);
	UINT CountY=(UINT)sqrt(mAddressDataMap.size()*1024/gApplication.mFontCodeHeight);
	UINT j=0;
	for(AM::iterator i=mAddressDataMap.begin();i!=mAddressDataMap.end();i++,j++)
	{
		//������� ������ ������
		if(j%CountY==0)
		{
			Context.Rect.left=(j/CountY)*1024;
			Context.Rect.top=0;
			Context.Rect.right=Context.Rect.left+1024;
			Context.Rect.bottom=gApplication.mFontCodeHeight;
			Listing.SetContext(Context);
		}
		Listing.SetAddress((*i).first);
		(*i).second->Print(&Listing);
	}

	//������� ��������
	DeleteEnhMetaFile(CloseEnhMetaFile(hdc));*/

	//������� ����
	HANDLE hFile=CreateFile(TEXT("listing.txt"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return;

	//������� �������
	CListingTextFile Listing;
	Listing.SetFile(hFile);
	Listing.SetAddressMap(this);

	//������� ������ ���� �������
	for(AM::iterator i=mAddressDataMap.begin();i!=mAddressDataMap.end();i++)
	{
		//������� ������ ������
		Listing.SetAddress((*i).first);
		(*i).second->Print(&Listing);
	}

	//������� ����
	CloseHandle(hFile);

	//���������
	MessageBox(NULL,TEXT(""),TEXT(""),0);
}
