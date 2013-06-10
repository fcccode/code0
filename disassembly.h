#ifndef QS_DISASSEMBLY_H
#define QS_DISASSEMBLY_H

#include "windows.h"

/* ����, ����������� ��������� ��� ������������������.
*/

//������������ ���������� ��������� � ����������
#define MAX_INSTRUCTION_OPERANDS_COUNT		3
//������������ ����� ����� ����������
#define MAX_INSTRUCTION_NAME_LENGTH				16

//����������� ������������� ��������� ����������
#define INSTRUCTION_ID_ERROR							0xFFFFFFFF

/******************* ��������� ���������� *******************/

/*** ����� ���������� �� �������� ���������� ***/

//����� ��������� ��������
#define OIS_DATA												0					//���������
#define OIS_SIGNED_DATA									1					//��������� �� ������ (���� ����� �������� ��� ������� ������ 32-�������)
#define OIS_REGISTER_BYTE								2					//������� �� ������������ �����
#define OIS_MODRM_RO										3					//������� �� ���� RO ����� ModR/M
#define OIS_MODRM_RM										4					//������� ��� ���������� �� ���� RM ����� ModR/M
#define OIS_MEMORY_IM										5					//���������� �� ������������ 32-������� ������
#define OIS_CONST_REGISTER							6					//���������� �������
#define OIS_CONST_DATA									7					//���������� ���������

//����� ���� ������
/* ��������� ������ ������ - ���������������� �����, ������� �� ����������� ��� ������ ������ */
#define OIR_ABSOLUTE_POINTER						1					//����, ����������� �� ��, ��� ������ ������� ���������� ��� ���������� ��������� � ������
#define OIR_RELATIVE_POINTER						2					//����, ����������� �� ��, ��� ������ ������� ���������� ��� ������������� ��������� � ������
/* ��������� ������ ������ - ����� ���������������� �����, ������� �����������, ������ ���� ���������� ���� �� ������ ������ ������ */
#define OIR_REFERENCE_WRITE							4					//���� ������ ������
#define OIR_REFERENCE_READ							8					//���� ������ ������
#define OIR_REFERENCE_READWRITE					(OIR_REFERENCE_READ | OIR_REFERENCE_WRITE)	//���� ������/������ ������
#define OIR_REFERENCE_EXECUTE						16				//���� ����������
/* ��������� ������ ������ - ����� ���������������� �����, ������� �����������, ������ ���� ���������� ���� OIR_REFERENCE_EXECUTE */
#define OIR_REFERENCE_EXECUTE_CALL			32				//���� ����, ��� � ���� ���������� ��� � �������

/*
���������� �������: ��������� �������� ������� �� bRegisterCategory, � ����� - �� bConstRegister.
��� ��������� ������������ ��������� RC_WORDDWORD.
*/

struct OPERAND_INFORMATION												//���������� �� �������� ����������
{
	BYTE bSource;																		//�������� �������� - ������ ��������� �������
	/* ������ ���� ������ �������������� ������ ��� ������� ����������. */

	BYTE bSize;																			//������ ������ �������� (��� ���������� � ������ � ���������������� ��������)
	BYTE bAddressReference;													//��� ������ ������ �������� (�.�. �� ������ ������, �� ������� ��������� �����)
	BYTE bDataReference;														//��� ������ ������ �������� (�.�. ���������� ������ ������)
	BYTE bRegisterCategory;													//��������� ������������ ��������� (���� ������� - �������)
	union
	{
		BYTE bConstRegister;													//���������� ������� (������ ���� bSource=OIS_CONST_REGISTER)
		BYTE bConstData;															//���������� ��������� (������ ���� bSource=OIS_CONST_DATA)
	};
};

/*** ����� ���������� �� ���������� ***/
#define IIF_MODRM									1								//��������� ���� Mod/RM
#define IIF_REGBYTE								2								//��������� ����������� ����
#define IIF_ENDPROC								4								//���������� �������� ���������� ���������
#define IIF_SPACE									8								//���������� �������� "�������� ��������", � ������ ���� �������� �����������

struct INSTRUCTION_INFORMATION										//���������� �� ����������
{
	BYTE bFlags;																		//�����
	BYTE bCodeSize;																	//������ ����
	BYTE bRegisterByteOffset;												//�������� �� ������ ���������� ������������ �����
	BYTE bOperandsCount;														//���������� ���������
	DWORD dwSense;																	//"�����" ����������, � �������� ��� ��������� ������� ����
	OPERAND_INFORMATION Operands[MAX_INSTRUCTION_OPERANDS_COUNT];	//���������� �� ���������
	TCHAR szName[MAX_INSTRUCTION_NAME_LENGTH];				//��� ����������
};

/*** ����� ����� ���������� ***/
#define IBF_OK										1								//���� ����; ��� ���������� ����� ���������� ��������� ��������������
#define IBF_END										2								//���� ���� - ��������
#define IBF_PREFIX								4								//���� ���� - �������

struct INSTRUCTION_BYTE														//���� ����������
{
	BYTE bFlags;																		//�����
	BYTE bMask;																			//����� ���� ��� ���������� �����
	union
	{
		DWORD dwInstructionID;												//���� ���� IBF_END ����������, �� - ������������� ����������
		DWORD dwNextByteGroup;												//���� ���� IBF_END �� ����������, �� - ������ �� ��������� ��������� ������
		DWORD dwPrefixFlag;														//���� ���� IBF_PREFIX ����������, �� - ���� ��������
	};
};

/******************* ��������� ����������� *******************/

//����� ���� RM �� ModRM
//������ 4 ��������� � ������� OMF_ �� code.h!
#define MODRM_RM_BASE_REGISTER			1							//������������ ������� �������
#define MODRM_RM_INDEX_REGISTER			2							//������������ ��������� �������
#define MODRM_RM_MULTIPLIER					4							//������������ ���������
#define MODRM_RM_OFFSET							8							//������������ ��������
#define MODRM_RM_MEMORY							16						//���� ����, ��� ������� RM ��������� ���������� � ������

struct MODRM																			//���������� � ������������������� ����� ModRM
{
	struct
	{
		BYTE bRORegister;															//������� �� ���� RO
	} RO;																						//���������� �� �������� RO
	struct
	{
		BYTE bFlags;																	//����� ���� RM
		union
		{
			BYTE bRegister;															//������� (��� ������������� ����������� ���������)
			BYTE bBaseRegister;													//������� �������
		};
		BYTE bIndexRegister;													//��������� �������
		BYTE bMultiplier;															//��������� (1, 2, 4, 8)
		DWORD dwOffset;																//��������
	} RM;																						//���������� �� �������� RM
};

#endif
