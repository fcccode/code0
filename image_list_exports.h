#ifndef QS_IMAGE_LIST_EXPORTS_H
#define QS_IMAGE_LIST_EXPORTS_H

#include "explored_image.h"
#include <list>

//����� ������ ��������
class CImageListExports : public CObject
{
public:
	//����� �������������� �������
	class CExportFunction : public CObject
	{
	protected:
		//��� �������
		LPTSTR mszName;
		//������� �������
		UINT mOrdinal;
		//���� ����������� ������� (�.�. �� �������)
		BOOL mIsLocal;
		union
		{
			//����� ���� �������
			CYBER_ADDRESS mAddress;
			//��� �������-��������
			LPTSTR mszForwardName;
		};

	public:
		//������������ � ����������
		CExportFunction(LPCTSTR szName,UINT Ordinal,CYBER_ADDRESS Address);
		CExportFunction(LPCTSTR szName,UINT Ordinal,LPCTSTR szForwardName);
		virtual ~CExportFunction();

		//�������� ���
		LPCTSTR GetName();
		//�������� �������
		UINT GetOrdinal();
		//�������� ���� �����������
		BOOL IsLocal();
		//�������� ����� �������
		CYBER_ADDRESS GetAddress();
		//�������� ��� �������-��������
		LPCTSTR GetForwardName();
	};

protected:
	//������ �������������� �������
	std::list<CExportFunction*> mFunctionsList;

public:
	//����������� � ����������
	CImageListExports();
	virtual ~CImageListExports();

	//�������� ������ �������
	std::list<CExportFunction*>* GetList();
};

#endif
