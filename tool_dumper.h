#ifndef QS_TOOL_DUMPER_H
#define QS_TOOL_DUMPER_H

#ifdef CODE0_COMPILE_DUMPING_TOOL

/*
���� �������� ����������� ����������� - ������� ������ ���������.
*/

#include "dialog.h"
#include "usercontrol_list.h"

class CToolDumper														//����� - ���������� ������ ������ ������
{
protected:
	//����� ������� ������ ��������
	class CSelectProcessDialog;
protected:
	//������������� ��������������� ��������
	DWORD mProcessID;

public:
	//������� ����������
	VOID Start();
};

#endif

#endif
