#ifndef QS_OPERANDS_H
#define QS_OPERANDS_H

#include "object.h"
#include "disassembly.h"
#include "listing.h"

// ���������� ���������

// ���� ���������
#define OT_DATA												0								//���������
#define OT_REGISTER										1								//�������
#define OT_MEMORY											2								//������

// ������ ���������

class COperand : public CObject												//������� ����������
{
public:
	virtual DWORD GetType() PURE;												//�������� ��� �������� (����� OT_*)
	virtual VOID Print(CListing*) PURE;									//���������� �������
	virtual BOOL GetReference(DWORD* pReference);				//�������� ������ ������ (���������� ��������)
};

class COperandData : public COperand									//������� - ���������
{
public:
	DWORD mData;																				//�������� ���������
	BYTE mReferenceFlags;																//����� OIR_*, ����������� ��� ������ ������

public:
	COperandData(DWORD);																//������� ��������� � �������� ���������

	virtual DWORD GetType();														//�������� ��� ��������
	virtual VOID Print(CListing*);											//���������� �������
	virtual BOOL GetReference(DWORD* pReference);				//�������� ������ ������
};

class COperandRegister : public COperand							//������� - �������
{
public:
	BYTE mRegister;																			//�������

public:
	COperandRegister(BYTE);															//������� ������� � �������� ���������

	virtual DWORD GetType();														//�������� ��� ��������
	virtual VOID Print(CListing*);											//���������� �������
};

//����� ��������-����������
#define OMF_BASE_REGISTER					1								//������������ ������� �������
#define OMF_INDEX_REGISTER				2								//������������ ��������� �������
#define OMF_MULTIPLIER						4								//������������ ���������
#define OMF_OFFSET								8								//������������ ��������
#define OMF_SEGMENT								16							//������������ ���������� �������
class COperandMemory : public COperand								//������� - ���������� � ������
{
public:
	BYTE mFlags;																				//�����
	BYTE mBaseRegister;																	//������� �������
	BYTE mIndexRegister;																//��������� �������
	BYTE mMultiplier;																		//���������
	DWORD mOffset;																			//��������
	BYTE mSize;																					//������ ��������
	BYTE mSegment;																			//���������� �������

public:
	COperandMemory(MODRM);															//������� �������-���������� �� ���� RM ����� ModRM
	COperandMemory(DWORD);															//������� �������-���������� �� ������������ ������

	VOID SetSegment(BYTE Segment);											//������� ���������� ������� ��� ������

	virtual DWORD GetType();														//�������� ��� ��������
	virtual VOID Print(CListing*);											//���������� �������
	virtual BOOL GetReference(DWORD* pReference);				//�������� ������ ������
};

#endif
