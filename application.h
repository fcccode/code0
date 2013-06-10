#ifndef QS_APPLICATION_H
#define QS_APPLICATION_H

/*
���� �������� ����������� ������ CApplication, �������
��������� �����������, ������ ��� ����� �������������.
CApplication ��������� ��������� ������������� � ����������
����� ��������.

����� CApplication ����������� ������ ���� ���, � � ����������
������, ������� � �� ����� ��������� ������� ������� ���
���������� ����������.
*/

#include "instruction_set.h"
#include "file_parser.h"
#include "explored_image.h"
#include <list>

class CApplication
{
public:				//������������� �������

	//������ ����������
	CInstructionSet mInstructionSet16;
	CInstructionSet mInstructionSet32;

	//����� ������� ����
	LPCTSTR mszMainWindowClass;
	LPCTSTR mszDangleGroupWindowClass;
	LPCTSTR mszDangleWindowClass;

	//������� � ��������� GDI
	static const COLORREF mBrushSelectColor;
	static const COLORREF mBrushSlackSelectColor;
	HBRUSH mhBrushSelect;
	HBRUSH mhBrushSlackSelect;
	HPEN mhPenSelect;
	HFONT mhFontCode;
	HFONT mhFontCodeBold;
	//������ ������ �� ������� mhFontCode
	INT mFontCodeHeight;

	//������� ����
	HWND mhMainWindow;

	//��������� ����� ��� ��������� ��� ��������
	HDC mhdcBackBuffer;
	HBITMAP mbmpBackBuffer;

public:				//������� ���������

	//������ ����������� ������
	std::list<CExploredImage*> mExploredImagesList;

protected:
	//��������� ���������� (���������� - � load_instructions.cpp)
	VOID LoadInstructions();

public:
	VOID Initialize();
	VOID Uninitialize();

	//�������� ������ ������������ ������ (�������� ������������ ����������� ���������� �������� � ������� Release)
	VOID GetFileParsers(std::list<CFileParser*>* pList);

	//�������� ����������� ���� � ������
	VOID AddExploredImage(CExploredImage* pImage);
	//�������� ������ ����������� ������
	std::list<CExploredImage*>* GetExploredImagesList();
};

//���������� ����������� ������� Application
extern CApplication gApplication;

#endif
