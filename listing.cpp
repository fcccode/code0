#include "listing.h"
#include "address_map.h"

// class CListing

const UINT CListing::mColumnMargins[LISTING_COLUMNS_COUNT]={ 0, 10, 32, 48, 40 };

CListing::CListing()
{
	//��������������� ��������� �� ����� �������
	mpAddressMap=NULL;
}

CListing::~CListing()
{
	//���������� ����� �������
	SafeRelease(mpAddressMap);
}

VOID CListing::SetAddress(DWORD Address)
{
	//��������� ������� �����
	mAddress=Address;
}

VOID CListing::SetAddressMap(CAddressMap* pAddressMap)
{
	//���������� ������� ����� �������
	SafeRelease(mpAddressMap);
	//��������� ��������� �� ����� ����� �������
	mpAddressMap=pAddressMap;
	mpAddressMap->AddRef();
}

VOID CListing::BeginLine()
{
	//�������� ������
	mLineTextLength=0;
	//�������� ���������
	mLineCurrentPosition=0;
	//���������� ����� � ������ ������
	TCHAR s[0x10];
	_stprintf(s,TEXT("%08X:"),mAddress);
	Print(0,s);
}

UINT CListing::Print(UINT Position,LPCTSTR szText)
{
	//�������� ����� ������������ ������
	UINT TextLength=_tcslen(szText);

	//���� ��������� ������� �� ����� ������������ ����� ������, �� ���������
	if(Position>=LISTING_LINE_LENGTH) return LISTING_LINE_LENGTH;
	//���� ����� ������� �� ����� ������������ ����� ������, �������� ������
	if(Position+TextLength>=LISTING_LINE_LENGTH) TextLength=LISTING_LINE_LENGTH-Position;
	//���� ������ ����������� ������ � ��������� ������� ����
	if(Position>mLineTextLength)
		//��������� ��������� ��� ������������
		for(UINT i=mLineTextLength;i<Position;++i)
			mszLineText[i]=TEXT(' ');

	//�������� ������
	CopyMemory(mszLineText+Position,szText,TextLength*sizeof(TCHAR));
	//��������� ����� ������� ��������� �������
	mLineCurrentPosition=Position+TextLength;
	if(mLineCurrentPosition>mLineTextLength) mLineTextLength=mLineCurrentPosition;
	//������� ����� �������
	return mLineCurrentPosition;
}

UINT CListing::Print(LPCTSTR szText)
{
	//���������� �����, ������� � ������� �������, � ������� ����� ������� �������
	return Print(mLineCurrentPosition,szText);
}

VOID CListing::SetPosition(UINT Position)
{
	//���� ��������� ������� �� ����� ������������ ����� ������, �� ���������
	if(Position>=LISTING_LINE_LENGTH) return;
	//���������� ������� ��������� �������
	mLineCurrentPosition=Position;
}

UINT CListing::GetColumnMargin(DWORD ColumnIndex)
{
	//���� ����� ������� �������, ���������
	if(ColumnIndex>=LISTING_COLUMNS_COUNT) return 0;

	//������� �����
	return mColumnMargins[ColumnIndex];
}

UINT CListing::PrintOffset(UINT Address)
{
#ifdef NIKITIN
	if(0)
#endif
	//���� ������� ����� �������
	if(mpAddressMap)
	{
		//�������� ������ ��� ������ � ������� ���
		LPTSTR szAddress=mpAddressMap->GetAddressString(Address);
		UINT Result=Print(szAddress);
		delete [] szAddress;
		return Result;
	}

	//����� ������ ������� �����
	TCHAR szAddress[0x10];
	if(Address & 0x80000000)
		_stprintf(szAddress,TEXT("-0%X"),-(int)Address);
	else
		_stprintf(szAddress,TEXT("0%X"),Address);
//	_stprintf(szAddress,TEXT("0%+Xh"),Address);
	return Print(szAddress);
}
