#ifndef QS_STRINGS_H
#define QS_STRINGS_H

/*
���� �������� ��������� ��������������� ������� ��� ������ �� ��������.

��������� I � ����� �������� ������� ��������, ��� ������� ��������� ���������
��� ����� �������� ��������.
*/

#include "windows.h"

//��������, ���������� �� ������-������ � �������� ������
BOOL IsAgreeFilter(LPCTSTR szString,LPCTSTR szFilter);
BOOL IsAgreeFilterI(LPCTSTR szString,LPCTSTR szFilter);

#endif
