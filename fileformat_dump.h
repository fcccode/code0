#ifndef QS_FILEFORMAT_DUMP_H
#define QS_FILEFORMAT_DUMP_H

/*
���� �������� ����������� ��������, ������������ �
������ ����� ������� Code0.
*/

/*
������ ����� �����:

FILE_DUMP_HEADER Header;																		//��������� ����� �����
FILE_DUMP_REGION Regions[Header.RegionsCount];							//������ ��������
FILE_DUMP_THREAD Threads[Header.ThreadsCount];							//������ �������
FILE_DUMP_MODULE Modules[Header.ModulesCount];							//������ �������
BYTE Data[?];																								//��������� ������; ������������ �������� � ���������� �� ���������� ������ �����
*/

//**** ��������� ����� �����
//��������� ����� �����
#define FILE_DUMP_MAGIC0									'edoC'
#define FILE_DUMP_MAGIC1									'pmd0'
//��������� ���������
struct FILE_DUMP_HEADER
{
	DWORD Magics[2];												//��������� ����� �����

	DWORD FileSize;													//������ �����
	DWORD RegionsCount;											//���������� ��������
	DWORD ThreadsCount;											//���������� �������
	DWORD ModulesCount;											//���������� �������
};

//**** ��������� �������
struct FILE_DUMP_REGION
{
	DWORD Flags;														//�����
	DWORD BaseAddress;											//����� ������ �������
	DWORD Size;															//������ �������
	DWORD State;														//��������� �������
	DWORD Protect;													//�������� ������ ������
	DWORD DataOffset;												//�������� �� ������ ����� �� ������ �������
};

//**** ��������� ������
struct FILE_DUMP_THREAD
{
	DWORD Flags;														//�����
	DWORD ThreadID;													//������������� ������
	DWORD UsageCount;												//���������� ������ �� �����
	LONG PriorityBase;											//������� ������� ����������
	LONG PriorityDelta;											//��������� ������ ����������
};

//**** ��������� ������
struct FILE_DUMP_MODULE
{
	DWORD Flags;														//�����
	DWORD BaseAddress;											//������� ����� ������
	DWORD BaseSize;													//������ ������
	DWORD UsageGlobalCount;									//���������� ������ �� ������ � �������
	DWORD UsageProcessCount;								//���������� ������ �� ������ � ��������
	TCHAR ModuleName[256];									//��� ������ (256 = MAX_MODULE_NAME32+1)
	TCHAR ModulePath[MAX_PATH];							//�������� �� ������ ����� �� ������� ����� ����� ������
};

#endif
