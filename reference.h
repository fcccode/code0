#ifndef QS_REFERENCE_H
#define QS_REFERENCE_H

/*
���� �������� ����������� ������ ������.
*/

#include <functional>
#include "cyber_memory.h"

class CReference
{
protected:
	//�������� �����
	CYBER_ADDRESS AddressFrom;
	//������ �����
	CYBER_ADDRESS AddressTo;

public:
	//������� ��������� � ������ �����������
	static class CompareForward
	{
	};
	//������� ��������� � �������� �����������
	static class CompareBackward
	{
	};
};

#endif
