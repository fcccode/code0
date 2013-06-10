#include "comment.h"

// class CComment

CComment::CComment(LPCTSTR szText)
{
	//��������� �����
	mszText=new TCHAR[_tcslen(szText)+1];
	_tcscpy(mszText,szText);
}

CComment::~CComment()
{
	//������� �����
	delete [] mszText;
}

LPCTSTR CComment::GetText()
{
	return mszText;
}

DWORD CComment::GetElementType()
{
	return ADET_COMMENT;
}

VOID CComment::Print(CListing* pListing)
{
	//��������� �� ������� ����� ������
	LPTSTR szText=mszText;
	//���� ������ ����� �����������
	while(*szText)
	{
		//������ ������
		pListing->BeginLine();

		//����� ������ �������� ������
		LPTSTR szEnd;
		for(szEnd=szText;*szEnd;++szEnd)
			if(*szEnd==TEXT('\r') || *szEnd==TEXT('\n'))
				break;
		//��������� ������ ������
		TCHAR OldChar=*szEnd;
		//������� ����� ������
		*szEnd=0;
		//������� ������
		pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_COMMENTS),TEXT("; "));
		pListing->Print(szText);
		//������� ������ ������
		*szEnd=OldChar;
		//��������� ������
		pListing->EndLine();

		//���� ��� �� ����� ������
		if(OldChar)
			//��������� ��������� ������ - ����� ��� ������ � \r
			if(szEnd[0]==TEXT('\r') && szEnd[1]==TEXT('\n'))
				//������� � ��������� ������
				szText=szEnd+2;
			else
				szText=szEnd+1;
		//����� ���������
		else
			break;
	}
}
