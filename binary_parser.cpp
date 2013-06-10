#include "binary_parser.h"
#include "interpretations.h"
#include "application.h"

// class CBinaryParser

BOOL CBinaryParser::PrepareAnalize()
{
	//������ ����� �� ����������, ��� ��� �������� ���� - ��� � � ������ �������� ����
	//��� ��� ������ �������, ��� ���� ����������� ��������

	//������� ����������� ������ ������ ��� ������ �����
	mFileBufferSize=mFileSize;

	//��
	return TRUE;
}

DWORD CBinaryParser::Analize()
{
	//����������� ���� � �����
	CopyMemory(mpFileBuffer,mpFileMapping,mFileSize);
	//������������� ����� � �����������, ������� � �������� ������
	mpMemory->Map(0,mpFileBuffer,ALIGN_UP(mFileSize,MEMORY_PAGE_SIZE));

	//������� ���������� ����
	mpCodeParser=new CCodeParser(mpMemory,mpAddressMap,&gApplication.mInstructionSet16,&gApplication.mInstructionSet32);
	//���������� 32-������ �����
	mpCodeParser->SetMode(MODE_32BIT);

	//������� ����� � ������ � ����� �����
	mpAddressMap->SetLabel(0,TEXT("FileBegin"));
	mpAddressMap->SetLabel(mFileSize,TEXT("FileEnd"));
	//����������� ���� � �������� ���� (������ �� 16 ����)
	for(CYBER_ADDRESS i=0;i<mFileSize;i+=16)
	{
		//�������� ������ ������
		UINT Size=mFileSize-i;
		if(Size>16) Size=16;
		//������� ������ ������
		CAddressData* pAddressData=mpAddressMap->OpenAddress(i);
		//������� �������������
		CInterpretation* pInterpretation=new CInterpretationData(mpMemory,i,Size);
		//������� �������������
		pAddressData->SetInterpretation(pInterpretation);
		mpAddressMap->SetAddressSize(i,Size);
		//������� ������������� � ������ ������
		pInterpretation->Release();
		pAddressData->Release();
	}

	//��!
	return FILE_PARSER_ERROR_SUCCESS;
}

CYBER_ADDRESS CBinaryParser::GetBeginAddress()
{
	//������� ������� ��������� �����
	return 0;
}

VOID CBinaryParser::About(FILE_PARSER_ABOUT* pAbout)
{
	//������� ��������
	_tcscpy(pAbout->szName,TEXT("Binary Files Parser"));
	_tcscpy(pAbout->szDescription,TEXT("This parser no perform automatic analizing and no parse whatever structures. Use this parser with non-standard executable files, or files of unknown formats."));
	pAbout->Version=MAKELONG(0,1);
}
