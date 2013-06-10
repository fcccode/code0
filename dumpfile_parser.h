#ifndef QS_DUMPFILE_PARSER_H
#define QS_DUMPFILE_PARSER_H

/*
���� �������� ����������� ������ CDumpFileParser, ������������
�������� � ������ ����� ����� �������� � ������� Code0.
*/

#include "file_parser.h"
#include "fileformat_dump.h"

class CDumpFileParser : public CFileParser
{
protected:
	//��������� �� ������� ����������
	FILE_DUMP_HEADER* mpHeader;
	FILE_DUMP_REGION* mpRegions;
	FILE_DUMP_THREAD* mpThreads;
	FILE_DUMP_MODULE* mpModules;
	//������� �������� ����������
	UINT mRegionsCount;
	UINT mThreadsCount;
	UINT mModulesCount;

public:
	//���������, ����� �� ������ ���������� ���������� ������ ����
	virtual BOOL PrepareAnalize();
	//��������� ������
	virtual DWORD Analize();

	//�������� ��������� ����� ��� ���������
	virtual CYBER_ADDRESS GetBeginAddress();

	//�������� ���������� �� �����������
	virtual VOID About(FILE_PARSER_ABOUT* pAbout);
};

#endif
