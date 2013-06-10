#include "main_window.h"
#include "application.h"

/*
��� ��������� ������, � �������� ���������� ����������.
*/

//** ������� ��������� �������� ����

//��������� �������
INT WINAPI wWinMain(HINSTANCE,HINSTANCE,LPTSTR,INT)
{
#ifdef _DEBUG
	//��������� ��� ���������� ��������� ����� ���������� �� ������� ������
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//���������������� ����������
	gApplication.Initialize();

	//������� ������� ����
	CMainWindow MainWindow;

	//��������� ���������!
	MainWindow.Show();

	//��������� ������� ��������
	gApplication.Uninitialize();

	return 0;
}
