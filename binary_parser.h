#ifndef QS_BINARY_PARSER_H
#define QS_BINARY_PARSER_H

/*
���� �������� ����������� ����������� �������� ������.
*/

#include "file_parser.h"

class CBinaryParser : public CFileParser
{
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
