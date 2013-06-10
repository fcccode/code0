#ifndef QS_IMAGE_LIST_IMPORTS_H
#define QS_IMAGE_LIST_IMPORTS_H

#include "explored_image.h"
#include <list>

//����� ������ �������
class CImageListImports : public CObject
{
public:
	//����� ������������� �������
	class CImportFunction : public CObject
	{
	protected:
		//��� �������
		LPTSTR mszName;
		//����� �������� � IAT, ���������������� �������
		CYBER_ADDRESS mAddressInIAT;

	public:
		//����������� � ����������
		CImportFunction(LPCTSTR szName,CYBER_ADDRESS AddressInIAT);
		virtual ~CImportFunction();

		//�������� ���
		LPCTSTR GetName();
		//�������� ����� � IAT
		CYBER_ADDRESS GetAddressInIAT();
	};

	//����� ������������� ����������
	class CImportLibrary : public CObject
	{
	protected:
		//������ ������������� �������
		std::list<CImportFunction*> mFunctionsList;
		//��� ����������
		LPTSTR mszName;
		//����� ������ IAT
		CYBER_ADDRESS mAddressIAT;

	public:
		//����������� � ����������
		CImportLibrary(LPCTSTR szName,CYBER_ADDRESS AddressIAT);
		~CImportLibrary();

		//�������� ���
		LPCTSTR GetName();
		//�������� ����� IAT
		CYBER_ADDRESS GetAddressIAT();
		//�������� ������ �������
		std::list<CImportFunction*>* GetList();
	};

protected:
	//������ ������������� ���������
	std::list<CImportLibrary*> mLibrariesList;

public:
	//����������� � ����������
	CImageListImports();
	virtual ~CImageListImports();

	//�������� ������ ���������
	std::list<CImportLibrary*>* GetList();
};

#endif
