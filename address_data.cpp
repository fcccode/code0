#include "address_data.h"
#include "subroutine.h"

// class CAddressData realization

CAddressData::CAddressData()
{
	//���������������� ������ ������ (�� ��������� 1)
	mSize=1;
	mPreviousSize=1;
}

CAddressData::~CAddressData()
{
	//�������� ������
	for(ADELI i=mElementsMap.begin();i!=mElementsMap.end();i++)
		(*i).second->Release();
	mElementsMap.clear();
}

VOID CAddressData::SetIdentityElement(CAddressDataElement* pElement,DWORD ElementType)
{
	//������� �������, ���� ����� ��� ����
	ADELI i=mElementsMap.find(ElementType);
	if(i!=mElementsMap.end())
	{
		(*i).second->Release();
		mElementsMap.erase(i);
	}

	//���� ����� ������� ������, �������� ��� � ����� ������
	if(pElement)
		AddElement(pElement);
}

BOOL CAddressData::IsEmpty()
{
	//������� ������ �������, ���� � ������ ��������� ����
	return !mElementsMap.size();
}

VOID CAddressData::SetSize(DWORD Size)
{
	//��������� ������ ������
	mSize=Size;
}

DWORD CAddressData::GetSize()
{
	//������� ������ ������
	return mSize;
}

VOID CAddressData::SetPreviousSize(DWORD PreviousSize)
{
	//��������� ������ ����������� ������
	mPreviousSize=PreviousSize;
}

DWORD CAddressData::GetPreviousSize()
{
	//������� ������ ����������� ������
	return mPreviousSize;
}

VOID CAddressData::SetBlockAddress(CYBER_ADDRESS Address)
{
	//��������� ����� �����
	mBlockAddress=Address;
}

CYBER_ADDRESS CAddressData::GetBlockAddress()
{
	//������� ����� �����
	return mBlockAddress;
}

VOID CAddressData::AddElement(CAddressDataElement* pElement)
{
	//�������� ������� � �����
	pElement->AddRef();
	mElementsMap.insert(std::pair<DWORD,CAddressDataElement*>(pElement->GetElementType(),pElement));
}

CAddressData::ADEL* CAddressData::GetElements()
{
	//������� ��������� �� �����
	return &mElementsMap;
}

VOID CAddressData::Print(CListing* pListing)
{
#ifdef NIKITIN
	if(0)
#endif
	//���� � ����� ������ ���� �����
	if(mElementsMap.find(ADET_LABEL)!=mElementsMap.end())
	{
		//������� ������ ������
		pListing->BeginLine();
		pListing->EndLine();
	}

	//���� �� ��������� � ����� ������
	BOOL TooManyElements=mElementsMap.size()>10;
	DWORD LastElementType=0xFFFFFFFF;
	BOOL LastElementMessage=FALSE;
	for(ADELI i=mElementsMap.begin();i!=mElementsMap.end();i++)
	{
		//�������� �������
		CAddressDataElement* pElement=(*i).second;
		if(TooManyElements)
		{
			DWORD ElementType=(*i).first;
			if(ElementType==LastElementType)
			{
				if(LastElementMessage)
				{
					pListing->BeginLine();
					pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_COMMENTS),TEXT("; ... ... ..."));
					pListing->EndLine();
					LastElementMessage=FALSE;
				}
				continue;
			}
			LastElementType=ElementType;
			LastElementMessage=TRUE;
		}
		//���������� �������
		pElement->Print(pListing);
	}
	//���� ������������� ���
	if(mElementsMap.find(ADET_INTERPRETATION)==mElementsMap.end())
	{
		//����������, ��� ������������� ���
		pListing->BeginLine();
		pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_NAME),TEXT("(no interpretation)"));
		pListing->EndLine();
	}
}

CInterpretation* CAddressData::GetInterpretation()
{
	//������� �������������, ���� ��� ����
	ADELI i=mElementsMap.find(ADET_INTERPRETATION);
	if(i!=mElementsMap.end())
	{
		CInterpretation* pInterpretation=(CInterpretation*)(*i).second;
		pInterpretation->AddRef();
		return pInterpretation;
	}
	return NULL;
}

VOID CAddressData::SetInterpretation(CInterpretation* pInterpretation)
{
	SetIdentityElement(pInterpretation,ADET_INTERPRETATION);
}

CLabel* CAddressData::GetLabel()
{
	//������� �����, ���� ��� ����
	ADELI i=mElementsMap.find(ADET_LABEL);
	if(i!=mElementsMap.end())
	{
		CLabel* pLabel=(CLabel*)(*i).second;
		pLabel->AddRef();
		return pLabel;
	}
	return NULL;
}

VOID CAddressData::SetLabel(CLabel* pLabel)
{
	SetIdentityElement(pLabel,ADET_LABEL);
}

CComment* CAddressData::GetComment()
{
	//������� �����������, ���� �� ����
	ADELI i=mElementsMap.find(ADET_COMMENT);
	if(i!=mElementsMap.end())
	{
		CComment* pComment=(CComment*)(*i).second;
		pComment->AddRef();
		return pComment;
	}
	return NULL;
}

VOID CAddressData::SetComment(CComment* pComment)
{
	SetIdentityElement(pComment,ADET_COMMENT);
}

VOID CAddressData::CreateSubroutine()
{
	//���� ������� ���, ������� �
	if(mElementsMap.find(ADET_SUBROUTINE)==mElementsMap.end())
	{
		CSubroutine* pSubroutine=new CSubroutine();
		AddElement(pSubroutine);
		pSubroutine->Release();
	}
}
