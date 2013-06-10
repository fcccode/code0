#ifndef QS_LISTING_H
#define QS_LISTING_H

/*
���� �������� ����������� ������ ������, ������������ ������� �� ������.
����� CListing ������������ ������ ��� ������ ��������������� ���������.

������ ������ ���������:
- �������� ������� �� �����. ����������� � ������������� ������
�������� BeginLine() � EndLine() ��������������.
- ������ ������ ����� �����, ������� ����������� ������� SetAddress().
- ������ ����������� �������� Print(), ������ ����� ��������� ����� � ���������������
������� ������.
- ���� ����� �� ������� � ������ �������, �� ����������.
- �������� ��� ��������� ������� ���������� ���������. ����� ���������� � ������
� ���������� ����� (TRANSPARENT) ������, ������� ��� ����� ������.
���� ������������ ���� ��������� �����, �� ��� ������ ������ �������� ������� �������������
������� ������. ��� ����, ����� ������������� ���������� ������,
���������� GroupSelectLines(), � �������� ������������ �������������, � ������� ������
� ������� �����.
*/

#include "object.h"

//������� ������ ��� ������������ ������ ��������
#define LISTING_COLUMN_ADDRESS					0
#define LISTING_COLUMN_LABEL						1
#define LISTING_COLUMN_NAME							2
#define LISTING_COLUMN_OPERANDS					3
#define LISTING_COLUMN_COMMENTS					4
//���������� �������
#define LISTING_COLUMNS_COUNT						5
//������������ ����� ������
#define LISTING_LINE_LENGTH							0x80

class CAddressMap;

class CListing : public CObject
{
protected:
	static const UINT mColumnMargins[LISTING_COLUMNS_COUNT];	//������ ���������� � ��������
	TCHAR mszLineText[LISTING_LINE_LENGTH];				//����� ������
	UINT mLineTextLength;													//����� ��� ������������� ������ ������
	UINT mLineCurrentPosition;										//������� ��������� �������
	DWORD mAddress;																//������� �����

	CAddressMap* mpAddressMap;										//��������� �� ����� �������

public:
	CListing();
	virtual ~CListing();

	VOID SetAddressMap(CAddressMap* pAddressMap);	//������� ����� �������

	VOID SetAddress(DWORD Address);								//������� ������� �����
	VOID BeginLine();															//������ ������
	virtual VOID EndLine() PURE;									//��������� ������

	//������� ������
	UINT Print(UINT Position,LPCTSTR szText);			//�������� �����, ������� � �������� ������� (������������ ������� ����� �����)
	UINT Print(LPCTSTR szText);										//�������� �����, ������� � ������� ������� (������������ ������� ����� �����)
	VOID SetPosition(UINT Position);							//���������� ������� ��������� �������
	UINT GetColumnMargin(DWORD ColumnIndex);			//�������� ������ ����� ������ �������
	virtual UINT PrintOffset(UINT Address);				//�������� ��������� ������������� ������
};

#endif
