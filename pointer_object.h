#ifndef QS_POINTER_OBJECT
#define QS_POINTER_OBJECT

/*
���� �������� ����������� ���������������� ������-���������, ��������������� ��� �������� ���������� �� ��������� �������.
*/

#include "object.h"

class CPointerObject
{
protected:
	CObject* mpObject;

public:
	CPointerObject(CObject* pObject);
	~CPointerObject();
	T& operator*()
	{
		return *mpObject;
	}
};

#endif
