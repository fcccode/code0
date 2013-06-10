#include "interpretations.h"

// class CInterpretation

DWORD CInterpretation::GetElementType()
{
	return ADET_INTERPRETATION;
}

BOOL CInterpretation::GetReference(CYBER_ADDRESS* pAddressReference)
{
	//������ ���, ��� ��������
	return FALSE;
}

// class CInterpretationData

CInterpretationData::CInterpretationData(CCyberMemory* pMemory,CYBER_ADDRESS Address,DWORD Size)
{
	//��������� ���������
	mpMemory=pMemory;
	mpMemory->AddRef();
	mAddress=Address;
	mSize=Size;
}

CInterpretationData::~CInterpretationData()
{
	//���������� ������
	mpMemory->Release();
}

DWORD CInterpretationData::GetInterpretationType()
{
	return ADEIT_DATA;
}

VOID CInterpretationData::Print(CListing* pListing)
{
	//������ ������
	pListing->BeginLine();

	//���������� ���������
	pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_NAME),TEXT("db"));

	//���������� ��� �����, �� �� �����, ��� 0x10
	DWORD Size=mSize;
	if(Size>0x10) Size=0x10;
	DWORD i;
	for(i=0;i<Size;++i)
	{
		try
		{
			//�������� �������� �����
			BYTE Byte=mpMemory->Byte(mAddress+i);
			//���������� �������� �����
			TCHAR s[4];
			_stprintf(s,TEXT(" %02X"),Byte);
			pListing->Print(s);
		}
		catch(CCyberMemoryPageFaultException)
		{
			//���������� ������� ���������� ������
			pListing->Print(TEXT(" ##"));
		}
	}

	//���� ���������� �� ��� �������
	if(Size<mSize)
		//������� ����������
		pListing->Print(TEXT(" ..."));

	//��������� ������
	pListing->EndLine();
}

// class CInterpretationCode

CInterpretationCode::CInterpretationCode(CInstruction* pInstruction)
{
	//��������� ��������� �� ����������
	mpInstruction=pInstruction;
	if(mpInstruction) mpInstruction->AddRef();
	//�������� �����
	mFlags=0;
}

CInterpretationCode::~CInterpretationCode()
{
	//������� ����������
	SafeRelease(mpInstruction);
}

VOID CInterpretationCode::SetFlags(DWORD Flags)
{
	//���������� �����
	mFlags|=Flags;
}

DWORD CInterpretationCode::GetInterpretationType()
{
	return ADEIT_CODE;
}

VOID CInterpretationCode::Print(CListing* pListing)
{
	//���� ���������� ����
	if(mpInstruction)
		//���������� ����������
		mpInstruction->Print(pListing);
	else
	{
		//����������, ��� ���������� - ����������
		pListing->BeginLine();
		pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_NAME),TEXT("(unknown instruction)"));
		pListing->EndLine();
	}

#ifdef NIKITIN
	if(0)
	{
#endif
	//���� ����� - ����� �������
	if(mFlags & ICF_SUBROUTINE_END)
	{
		pListing->BeginLine();
		pListing->EndLine();
		for(DWORD i=0;i<2;++i)
		{
			pListing->BeginLine();
			pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_LABEL)+10,TEXT("; *******************************************************************"));
			pListing->EndLine();
		}
		pListing->BeginLine();
		pListing->EndLine();
	}
/*		//���������� ������ ������
		for(DWORD i=0;i<2;++i)
		{
			pListing->BeginLine();
			pListing->EndLine();
		}*/
	//���� ���������� - �����������
	else if(mFlags & ICF_SPACE)
	{
		pListing->BeginLine();
		pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_LABEL)+10,TEXT("; -------------------------------------------------------------------"));
		pListing->EndLine();
		pListing->BeginLine();
		pListing->EndLine();
	}
/*		//���������� ������ ������
		for(DWORD i=0;i<2;++i)
		{
			pListing->BeginLine();
			pListing->EndLine();
		}*/
#ifdef NIKITIN
	}
#endif
}

BOOL CInterpretationCode::GetReference(CYBER_ADDRESS* pAddressReference)
{
	//���� ���������� ����
	if(mpInstruction)
		//�������� ������ ������
		return mpInstruction->GetReference(pAddressReference);
	//����� ������ ���
	return FALSE;
}

// class CIntepretationString

CInterpretationString::~CInterpretationString()
{
	//���������� ������
	mpMemory->Release();
}

DWORD CInterpretationString::GetInterpretationType()
{
	return ADEIT_STRING;
}

// class CInterpretationStringUnicode

CInterpretationStringUnicode::CInterpretationStringUnicode(CCyberMemory* pMemory,CYBER_ADDRESS Address)
{
	//��������� ���������
	mpMemory=pMemory;
	mpMemory->AddRef();
	mAddress=Address;
}

CInterpretationStringUnicode::~CInterpretationStringUnicode()
{
}

VOID CInterpretationStringUnicode::Print(CListing* pListing)
{
	//������ ������
	pListing->BeginLine();
	//�������� ������ ��� ������
	UINT Margin=pListing->GetColumnMargin(LISTING_COLUMN_NAME);
	//�������� ������������ ����� ������
	UINT MaxLength=LISTING_LINE_LENGTH-Margin-0x10;

	//������� ���������
	pListing->Print(Margin,TEXT(".unicode \""));

	//���� ������ ����������� ������������������
	//���� �� ��������
	CYBER_ADDRESS Address=mAddress;
	for(UINT i=10;i<=MaxLength;++i)
	{
		try
		{
			//�������� ������
			WCHAR Chars[2];
			Chars[0]=mpMemory->Word(Address);
			Chars[1]=0;
			//���� ������ - ��������, �� ���������
			if(!Chars[0])
			{
				pListing->Print(TEXT("\""));
				break;
			}
			//������� ������, ���� �� �� ����� �� ������� ������
			if(i<MaxLength)
				pListing->Print(Chars);
			else
				//������� ����������
				pListing->Print(TEXT("\"..."));
		}
		catch(CCyberMemoryPageFaultException)
		{
			//���������� ������� ��������� ������
			pListing->Print(TEXT("\"##"));
			//����� �� �����
			break;
		}
		//������� � ���������� �������
		Address+=sizeof(WCHAR);
	}

	//��������� ������
	pListing->EndLine();
}

UINT CInterpretationStringUnicode::GetStringSize()
{
	//��������� ����� ������
	CYBER_ADDRESS i;
	for(i=mAddress;;i+=sizeof(WCHAR))
		try
		{
			//������� ������
			WCHAR Char=mpMemory->Word(i);
			//���� ������ �������, �� ��� ����� ������
			if(!Char) break;
		}
		catch(CCyberMemoryPageFaultException)
		{
			//�� ����� �� ���������� ������
			break;
		}
	//������� ������ ������
	return i-mAddress+sizeof(WCHAR);
}

// class CInterpretationStringASCII

CInterpretationStringASCII::CInterpretationStringASCII(CCyberMemory* pMemory,CYBER_ADDRESS Address)
{
	//��������� ���������
	mpMemory=pMemory;
	mpMemory->AddRef();
	mAddress=Address;
}

CInterpretationStringASCII::~CInterpretationStringASCII()
{
}

VOID CInterpretationStringASCII::Print(CListing* pListing)
{
	//������ ������
	pListing->BeginLine();
	//�������� ������ ��� ������
	UINT Margin=pListing->GetColumnMargin(LISTING_COLUMN_NAME);
	//�������� ������������ ����� ������
	UINT MaxLength=LISTING_LINE_LENGTH-Margin-0x10;

	//������� ���������
	pListing->Print(Margin,TEXT(".ascii \""));

	//���� ������ ����������� ������������������
	//���� �� ��������
	CYBER_ADDRESS Address=mAddress;
	for(UINT i=10;i<=MaxLength;++i)
	{
		try
		{
			//�������� ������
			TCHAR Chars[2];
			Chars[0]=(TCHAR)mpMemory->Byte(Address);
			Chars[1]=0;
			//���� ������ - ��������, �� ���������
			if(!Chars[0])
			{
				pListing->Print(TEXT("\""));
				break;
			}
			//������� ������, ���� �� �� ����� �� ������� ������
			if(i<MaxLength)
				pListing->Print(Chars);
			else
				//������� ����������
				pListing->Print(TEXT("\"..."));
		}
		catch(CCyberMemoryPageFaultException)
		{
			//���������� ������� ��������� ������
			pListing->Print(TEXT("\"##"));
			//����� �� �����
			break;
		}
		//������� � ���������� �������
		Address+=sizeof(CHAR);
	}

	//��������� ������
	pListing->EndLine();
}

UINT CInterpretationStringASCII::GetStringSize()
{
	//��������� ����� ������
	CYBER_ADDRESS i;
	for(i=mAddress;;i+=sizeof(CHAR))
		try
		{
			//������� ������
			CHAR Char=mpMemory->Byte(i);
			//���� ������ �������, �� ��� ����� ������
			if(!Char) break;
		}
		catch(CCyberMemoryPageFaultException)
		{
			//�� ����� �� ���������� ������
			break;
		}
	//������� ������ ������
	return i-mAddress+sizeof(CHAR);
}
