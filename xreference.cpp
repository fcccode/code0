#include "xreference.h"
#include "disassembly.h"

// class CXReference realization

CXReference::CXReference(CYBER_ADDRESS AddressFrom,BYTE ReferenceFlags)
{
	mAddressFrom=AddressFrom;
	mReferenceFlags=ReferenceFlags;
}

DWORD CXReference::GetElementType()
{
	return ADET_XREFERENCE;
}

VOID CXReference::Print(CListing* pListing)
{
	//������������ �����

	//������ ������
	pListing->BeginLine();

	//���������� ��������� �������
	pListing->SetPosition(pListing->GetColumnMargin(LISTING_COLUMN_COMMENTS));
	//������� ������ �����������
	pListing->Print(TEXT("; "));

	//���������� ����� �����
	if(mReferenceFlags & OIR_REFERENCE_READ)
		pListing->Print(TEXT("read "));
	if(mReferenceFlags & OIR_REFERENCE_WRITE)
		pListing->Print(TEXT("write "));
	if(mReferenceFlags & OIR_REFERENCE_EXECUTE_CALL)
		pListing->Print(TEXT("call "));
	if(mReferenceFlags & OIR_REFERENCE_EXECUTE)
		pListing->Print(TEXT("execute "));

	//���������� ���������� ������
	pListing->Print(TEXT("reference from "));
	//���������� �����
	pListing->PrintOffset(mAddressFrom);

	//������� ������
	pListing->EndLine();
}

CYBER_ADDRESS CXReference::GetAddressFrom()
{
	//������� �������� �����
	return mAddressFrom;
}
