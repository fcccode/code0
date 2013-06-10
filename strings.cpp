#include "strings.h"

//��������, ���������� �� ������-������ � �������� ������
BOOL IsAgreeFilter(LPCTSTR szString,LPCTSTR szFilter)
{
	//�������� ����� ������-�������
	INT FilterLength=_tcslen(szFilter);
	//�������� ����� ������ ��� ����� ������-�������
	INT StringLength=_tcslen(szString)-FilterLength+1;

	//���� ��������
	INT i,j;
	for(i=0;i<StringLength;++i)
	{
		for(j=0;j<FilterLength;++j)
			if(szString[i+j]!=szFilter[j])
				break;
		if(j>=FilterLength)
			return TRUE;
	}
	return FALSE;
}
BOOL IsAgreeFilterI(LPCTSTR szString,LPCTSTR szFilter)
{
	//�������� ����� ������-�������
	INT FilterLength=_tcslen(szFilter);
	//�������� ����� ������ ��� ����� ������-�������
	INT StringLength=_tcslen(szString)-FilterLength+1;

	//���� ��������
	INT i,j;
	for(i=0;i<StringLength;++i)
	{
		for(j=0;j<FilterLength;++j)
			if(CharUpper((LPTSTR)szString[i+j])!=CharUpper((LPTSTR)szFilter[j]))
				break;
		if(j>=FilterLength)
			return TRUE;
	}
	return FALSE;
}
