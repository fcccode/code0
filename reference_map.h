#ifndef QS_REFERENCE_MAP_H
#define QS_REFERENCE_MAP_H

/*
���� �������� ����������� ������ ����� ������.
*/

#include "reference.h"
#include <set>

class CReferenceMap : public CObject
{
public:
	//���� ���������� ��� ��������� �����
	typedef std::set<CReference*,CReference::CompareForward>::iterator REFERENCE_FORWARD_ITERATOR;
	typedef std::set<CReference*,CReference::CompareBackward>::iterator REFERENCE_BACKWARD_ITERATOR;

protected:
	//������ � �������� ������ ������
	std::set<CReference*,CReference::CompareForward> mSetForward;
	std::set<CReference*,CReference::CompareBackward> mSetBackward;

public:
	//�������� ������ ������, ��������� �� ������
	std::pair<REFERENCE_FORWARD_ITERATOR,CReferenceMap::REFERENCE_FORWARD_ITERATOR> QueryFromAddress(CYBER_ADDRESS Address);
	//�������� ������ ������, �������� � �����
	std::pair<REFERENCE_BACKWARD_ITERATOR,CReferenceMap::REFERENCE_BACKWARD_ITERATOR> QueryToAddress(CYBER_ADDRESS Address);
};

#endif
