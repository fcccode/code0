#ifndef QS_INTERPRETATIONS_H
#define QS_INTERPRETATIONS_H

/*
���� �������� ���������� ������� �������������.
*/

#include "address_data_element.h"
#include "cyber_memory.h"
#include "instruction.h"

// ���� �������������

#define ADEIT_DATA									0
#define ADEIT_CODE									1
#define ADEIT_STRING								2

// ����� ����� �������������
class CInterpretation : public CAddressDataElement
{
public:
	virtual DWORD GetElementType();									//�������� ��� �������� ������ ������
	virtual DWORD GetInterpretationType() PURE;			//�������� ��� �������������
	virtual BOOL GetReference(CYBER_ADDRESS* pAddressReference);	//�������� ������ ������ (����������� ��������)
};

// ����� ������������� - ������

class CInterpretationData : public CInterpretation
{
protected:
	CCyberMemory* mpMemory;													//��������� �� �����������
	CYBER_ADDRESS mAddress;													//����� ������
	DWORD mSize;																		//������ ������

public:
	CInterpretationData(CCyberMemory* pMemory,CYBER_ADDRESS Address,DWORD Size);
	virtual ~CInterpretationData();

	virtual DWORD GetInterpretationType();					//�������� ��� �������������
	virtual VOID Print(CListing* pListing);					//���������� ��������� ������������� �������� ������
};

// ����� ������������� - ����

//����� ����
#define ICF_SUBROUTINE_END							1							//����� �������
#define ICF_SPACE												2							//�����������

class CInterpretationCode : public CInterpretation
{
protected:
	CInstruction* mpInstruction;										//��������� �� ����������
	DWORD mFlags;																		//����� ����

public:
	CInterpretationCode(CInstruction* pInstruction);
	virtual ~CInterpretationCode();

	VOID SetFlags(DWORD Flags);											//���������� �����
	virtual DWORD GetInterpretationType();					//�������� ��� �������������
	virtual VOID Print(CListing*);									//���������� ��������� ������������� �������� ������
	virtual BOOL GetReference(CYBER_ADDRESS* pAddressReference);	//�������� ������ ������
};

// ����������� ����� �������������-������

class CInterpretationString : public CInterpretation
{
protected:
	CCyberMemory* mpMemory;													//��������� �� �����������
	CYBER_ADDRESS mAddress;													//����� ������

public:
	//����������
	virtual ~CInterpretationString();

	virtual DWORD GetInterpretationType();					//�������� ��� �������������
	virtual UINT GetStringSize() PURE;							//�������� ������ �������������-������ � ������ (������������� � ����������� �������)
};

// ����� ������������� - Unicode-������

class CInterpretationStringUnicode : public CInterpretationString
{
protected:

public:
	//����������� � ����������
	CInterpretationStringUnicode(CCyberMemory* pMemory,CYBER_ADDRESS Address);
	virtual ~CInterpretationStringUnicode();

	virtual VOID Print(CListing* pListing);					//���������� ������
	virtual UINT GetStringSize();										//�������� ������ �������������-������ � ������
};

// ����� ������������� - ASCII-������

class CInterpretationStringASCII : public CInterpretationString
{
protected:

public:
	//����������� � ����������
	CInterpretationStringASCII(CCyberMemory* pMemory,CYBER_ADDRESS Address);
	virtual ~CInterpretationStringASCII();

	virtual VOID Print(CListing* pListing);					//���������� ������
	virtual UINT GetStringSize();										//�������� ������ �������������-������ � ������
};

#endif
