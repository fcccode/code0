#include "danglewindow_code.h"
#include "application.h"
#include "dialog_gotoaddress.h"
#include "dialog_gotoxreference.h"
#include "dialog_editlabel.h"
#include "dialog_editcomment.h"
#include "dialog_list_imports.h"
#include "dialog_list_exports.h"
//#include "dialog_instruction_visualizer.h"
#include "xreference.h"
#include "resource.h"

// class CDangleWindowCode

CDangleWindowCode::CDangleWindowCode(CExploredImage* pExploredImage,CYBER_ADDRESS Address,LPCTSTR szFileName)
{
	//�������� � ��������� ��������� �� ����������� �������
	mpExploredImage=pExploredImage;
	mpExploredImage->AddRef();
	mpAddressMap=pExploredImage->GetAddressMap();
	mpMemory=pExploredImage->GetMemory();
	mpCodeParser=pExploredImage->GetCodeParser();
	//������� �������
	mpListing=new CListingDevice;

	//���������������� �������� ��������� � ������� ���������
	mHistoryCurrentIterator=mHistoryList.end();

	//���������������� �������� �����
	mEndAddress=Address;
	//������� � ��������� ������
	GoToAddress(Address);

	//������������ ��� ����
	//�������� ��� ��� ����
	TCHAR szFileTitle[MAX_PATH];
	GetFileTitle(szFileName,szFileTitle,MAX_PATH);
	//���� ��� ������� �������
	if(_tcslen(szFileTitle)>MAX_DANGLE_NAME_LENGTH)
	{
		//�������� ���
		CopyMemory(mszNameBuffer,szFileTitle,MAX_DANGLE_NAME_LENGTH*sizeof(TCHAR));
		mszNameBuffer[MAX_DANGLE_NAME_LENGTH]=0;
	}
	//����� ����� �������
	else
		_tcscpy(mszNameBuffer,szFileTitle);

	//���������� ��������� �� ���
	mszName=mszNameBuffer;
	//������� ���������� �� UI
	mUI.hMenu=LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_MENU_CODE));
	mUI.hAccelerators=LoadAccelerators(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_ACCELERATOR_CODE));
}

CDangleWindowCode::~CDangleWindowCode()
{
	//������� �������
	SafeDelete(mpListing);
	//���������� �������
	mpExploredImage->Release();
	mpAddressMap->Release();
	mpMemory->Release();
	mpCodeParser->Release();
}

VOID CDangleWindowCode::OffsetView(INT Offset)
{
	//���������� ��� �� ��������� ����������
	if(Offset>0)
		for(INT i=0;i<Offset;++i)
		{
			//�������� ���
			CAddressData* pAddressData=mpAddressMap->GetAddress(mAddress);
			if(pAddressData)
			{
				mAddress+=pAddressData->GetSize();
				pAddressData->Release();
			}
			else mAddress++;
		}
	else if(Offset<0)
		for(INT i=Offset+1;i<=0;++i)
		{
			//�������� ���
			CAddressData* pAddressData=mpAddressMap->GetAddress(mAddress);
			if(pAddressData)
			{
				mAddress-=pAddressData->GetPreviousSize();
				pAddressData->Release();
			}
			else mAddress--;
		}
}

VOID CDangleWindowCode::OffsetSelection(INT Offset)
{
	//���������� ��������� �� ��������� ����������
	if(Offset>0)
		for(INT i=0;i<Offset;++i)
		{
			//�������� ���������
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			if(pAddressData)
			{
				mSelectedAddress+=pAddressData->GetSize();
				pAddressData->Release();
			}
			else mSelectedAddress++;
		}
	else if(Offset<0)
		for(INT i=Offset+1;i<=0;++i)
		{
			//�������� ���������
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			if(pAddressData)
			{
				mSelectedAddress-=pAddressData->GetPreviousSize();
				pAddressData->Release();
			}
			else mSelectedAddress--;
		}
}

VOID CDangleWindowCode::GoToAddress(CYBER_ADDRESS Address)
{
	//��������� ������� �����
	KeepHistoryCurrent();
	//������� � ������� ��������� ��� ��������, ������� � mHistoryCurrentIterator
	mHistoryList.erase(mHistoryCurrentIterator,mHistoryList.end());

	//���� ����� ����� � ����
	if(Address>=mAddress && Address<mEndAddress)
		//������ �������� �����
		mSelectedAddress=Address;
	//�����
	else
	{
		//������� � ���������� ������
		mAddress=Address;
		mSelectedAddress=Address;
	}
	//�������� ��������� ����� � ������
	mHistoryList.push_back(Address);
	//������� ����� ������� ��������
	mHistoryCurrentIterator=mHistoryList.end();
	//�������� ����
	InvalidateRect(mhWindow,NULL,FALSE);
}

VOID CDangleWindowCode::GoHistoryBack()
{
	//�������� ���������� �������� �������
	std::list<CYBER_ADDRESS>::iterator i=mHistoryCurrentIterator;
	--i;
	//���� ���� �������� �� ������
	if(i!=mHistoryList.begin())
	{
		//��������� ������� �����
		KeepHistoryCurrent();
		//����������� ������� �������� �����
		--mHistoryCurrentIterator;
		//�������� ����� ����� � ������� ���
		mAddress=*(--i);
		mSelectedAddress=mAddress;
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
	}
}

VOID CDangleWindowCode::GoHistoryForward()
{
	//���� ������� �������� �� ���������
	if(mHistoryCurrentIterator!=mHistoryList.end())
	{
		//��������� ������� �����
		KeepHistoryCurrent();
		//�������� ����� ����� � ������� ���
		mAddress=*(mHistoryCurrentIterator++);
		mSelectedAddress=mAddress;
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
	}
}

VOID CDangleWindowCode::KeepHistoryCurrent()
{
	//�������� �������� ������� ������
	std::list<CYBER_ADDRESS>::iterator i=mHistoryCurrentIterator;
	if(i!=mHistoryList.begin()) *(--i)=mSelectedAddress;
	else
	{
		mHistoryList.push_back(mSelectedAddress);
		mHistoryCurrentIterator=mHistoryList.end();
	}
}

VOID CDangleWindowCode::Paint(HDC hdc)
{
	//�������� ������������� ����
	RECT Rect;
	GetClientRect(mhWindow,&Rect);

	//������� �������� ��� ��������
	LISTING_DEVICE_CONTEXT Context;
	Context.hdc=hdc;
	Context.Rect=Rect;
	mpListing->SetContext(Context);
	//������� ����� �������
	mpListing->SetAddressMap(mpAddressMap);

	//�������� ����
	SelectBrush(hdc,GetStockBrush(WHITE_BRUSH));
	PatBlt(hdc,Rect.left,Rect.top,Rect.right,Rect.bottom,PATCOPY);
	//�������� ������ ���� � �����
	SelectPen(hdc,gApplication.mhPenSelect);
	SelectBrush(hdc,gApplication.mhBrushSelect);
	//�������� �����
	SelectFont(hdc,gApplication.mhFontCode);

	//���������� ����������� �����
	{
		//���������� ����� ������������ ������
		mpListing->SetControlGrouping();
		CAddressData* pData=mpAddressMap->GetAddress(mSelectedAddress);
		if(pData)
		{
			//������� ������� �����
			mpListing->SetAddress(mSelectedAddress);
			//���������� ������ ������
			pData->Print(mpListing);
			//���������� ������
			pData->Release();
		}
		//������������� ������ (��� ����� �������� ����������� �����)
		mpListing->GroupLines();
	}

	//���� ������
	CYBER_ADDRESS Address=mAddress;
	for(;;)
	{
		//�������� ������ ������
		CAddressData* pData=mpAddressMap->GetAddress(Address);

		//���������� ���� ������������, ���� �����
		mpListing->SetSelecting(Address==mSelectedAddress);

		//������� ������� �����
		mpListing->SetAddress(Address);

		if(pData)
		{
			//���������� ������ ������
			pData->Print(mpListing);
			//�������� ����� �� ������ ������
			Address+=pData->GetSize();
			//���������� ������ ������
			pData->Release();
		}
		else
		{
			//����������, ��� ������������� ���
			mpListing->BeginLine();
			mpListing->Print(mpListing->GetColumnMargin(LISTING_COLUMN_NAME),TEXT("(no interpretation)"));
			mpListing->EndLine();
			//�������� ����� �� 1
			Address++;
		}

		//������������� ������������ ������
		mpListing->GroupLines();

		//���� ������ ����� �� ������� ��������������, ���������
		if(mpListing->IsOverflow()) break;
	}
	//��������� �������� �����
	mEndAddress=Address-2;
}

VOID CDangleWindowCode::OnKey(UINT Key)
{
	//����� �� ������� �������
	switch(Key)
	{
	case VK_DOWN:							//����
		//���� ������� Control ������
		if(GetKeyState(VK_CONTROL)<0)
		{
			//�������� ���
			OffsetView(1);
			//�������� ���������
			OffsetSelection(1);
		}
		else
		{
			//�������� ���������
			OffsetSelection(1);
			//���� ���������� ����� �� ����� �� ������
			if(mSelectedAddress>=mEndAddress)
				//�������� � ���
				OffsetView(1);
		}

		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case VK_UP:								//�����
		//���� ������� Control ������
		if(GetKeyState(VK_CONTROL)<0)
		{
			//�������� ���
			OffsetView(-1);
			//�������� ���������
			OffsetSelection(-1);
		}
		else
		{
			//�������� ���������
			OffsetSelection(-1);
			//���� ���������� ����� �� ����� �� ������
			if(mSelectedAddress<mAddress)
				//�������� � ���
				OffsetView(-1);
		}

		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case VK_NEXT:							// PAGE DOWN
		//�������� ���
		OffsetView(0x10);
		//�������� ���������
		OffsetSelection(0x10);

		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case VK_PRIOR:						// PAGE UP
		//�������� ���
		OffsetView(-0x10);
		//�������� ���������
		OffsetSelection(-0x10);

		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	}
}

VOID CDangleWindowCode::OnCommand(UINT CommandID,UINT Code,HWND hControl)
{
	//����� �� ��������������
	switch(CommandID)
	{
		//** ���� "Code"
	case MID_CODE_CODE_SAVELISTING:
		//�������������� ������� � ���� (TEST)
		mpAddressMap->ExplortListingToFile();
		break;
		//** ���� "Information"
	case MID_CODE_INFORMATION_IMPORTSLIST:
		{
			CDialogListImports Dialog;

			//��������� ������ ������ �������
			CYBER_ADDRESS Address;
			if(Dialog.Show(mpExploredImage,&Address,mhWindow))
				//���� ������������ �������, ������������� � ������ �����
				GoToAddress(Address);
		}
		break;
	case MID_CODE_INFORMATION_EXPORTSLIST:
		{
			CDialogListExports Dialog;

			//��������� ������ ������ ��������
			CYBER_ADDRESS Address;
			if(Dialog.Show(mpExploredImage,&Address,mhWindow))
				//���� ������������ �������, ������������� � ������ �����
				GoToAddress(Address);
		}
		break;
/*	case MID_CODE_INFORMATION_INSTRUCTIONVISUALIZATION:
		{
			//���������, ��� ������������� � ���������� ������ - �������
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			if(pAddressData)
			{
				CInterpretation* pInterpretation=pAddressData->GetInterpretation();
				if(pInterpretation)
				{
					if(pInterpretation->GetInterpretationType()==ADEIT_CODE)
					{
						//������� ������
						CDialogInstructionVisualizer Dialog;
						Dialog.Show(mpMemory,mSelectedAddress,pAddressData->GetSize(),mhWindow);
					}
					pInterpretation->Release();
				}
				pAddressData->Release();
			}
		}
		break;*/
		//** ���� "Navigate"
	case MID_CODE_NAVIGATE_GOTOADDRESS:
		{
			//������� ������
			CDialogGoToAddress Dialog;
			CYBER_ADDRESS Address=mSelectedAddress;
			if(Dialog.Show(mhWindow,&Address))
				//������� �� ������
				GoToAddress(Address);
		}
		break;
	case MID_CODE_NAVIGATE_HISTORYBACK:
		//������� �� ������� ��������� �����
		GoHistoryBack();
		break;
	case MID_CODE_NAVIGATE_HISTORYFORWARD:
		//������� �� ������� ��������� �����
		GoHistoryForward();
		break;
	case MID_CODE_NAVIGATE_GOTOREFERENCE:
		{
			//�������� ������ ������
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			if(pAddressData)
			{
				//�������� �������������
				CInterpretation* pInterpretation=pAddressData->GetInterpretation();
				//���������� ������ ������
				pAddressData->Release();
				if(pInterpretation)
				{
					//�������� ������ ������
					CYBER_ADDRESS Address;
					if(pInterpretation->GetReference(&Address))
						//������� �� ������
						GoToAddress(Address);
					//���������� �������������
					pInterpretation->Release();
				}
			}
		}
		break;
	case MID_CODE_NAVIGATE_GOTOXREFERENCE:
		{
			//�������� ������ ������
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			if(pAddressData)
			{
				//��������� ���������� ������������ ������
				CAddressData::ADEL* pElements=pAddressData->GetElements();
				UINT ElementsCount=pElements->count(ADET_XREFERENCE);
				//���� ���� 1 ������������ ������
				if(ElementsCount==1)
					//������ ������� �� ���
					GoToAddress( ((CXReference*)((*pElements->find(ADET_XREFERENCE)).second))->GetAddressFrom() );
				//����� ���� ���� ������ ����� ������
				else if(ElementsCount>1)
				{
					//������� ������
					CDialogGoToXReference Dialog;
					CYBER_ADDRESS Address;
					if(Dialog.Show(mhWindow,pAddressData,mpAddressMap,&Address))
						//������� �� ������
						GoToAddress(Address);
				}
				//���������� ������ ������
				pAddressData->Release();
			}
		}
		break;
		//** ���� "Interpretate"
	case MID_CODE_INTERPRETATE_DATA_BYTE:
		//������� �������������
		mpExploredImage->SetAddressInterpretationData(mSelectedAddress,1);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_DATA_WORD:
		//������� �������������
		mpExploredImage->SetAddressInterpretationData(mSelectedAddress,2);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_DATA_DWORD:
		//������� �������������
		mpExploredImage->SetAddressInterpretationData(mSelectedAddress,4);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_CODE:
		//������� �������������
		mpExploredImage->SetAddressInterpretationCode(mSelectedAddress);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_UNICODESTRING:
		//������� �������������
		mpExploredImage->SetAddressInterpretationStringUnicode(mSelectedAddress);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_ASCIISTRING:
		//������� �������������
		mpExploredImage->SetAddressInterpretationStringASCII(mSelectedAddress);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_DELETE:
		mpExploredImage->DeleteAddressIntepretation(mSelectedAddress);
		//�������� ����
		InvalidateRect(mhWindow,NULL,FALSE);
		break;
	case MID_CODE_INTERPRETATE_EDITLABEL:
		{
			//�������� �����
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			CLabel* pExistLabel;
			LPCTSTR szLabel=NULL;
			if(pAddressData)
			{
				pExistLabel=pAddressData->GetLabel();
				pAddressData->Release();
				if(pExistLabel)
					szLabel=pExistLabel->GetName();
			}
			else
				pExistLabel=NULL;
			//������� ���� �������������� �����
			CDialogEditLabel Dialog;
			LPTSTR szResultLabel;
			if(Dialog.Show(szLabel,&szResultLabel,mhWindow))
			{
				//���� ����� ����
				if(szResultLabel)
				{
					//������� ����� �����
					CLabel* pNewLabel=new CLabel(szResultLabel);
					//���������� ������ ������
					delete [] szResultLabel;
					//������� ������ ������
					pAddressData=mpAddressMap->OpenAddress(mSelectedAddress);
					//������� �����
					pAddressData->SetLabel(pNewLabel);
					//���������� �����
					pNewLabel->Release();
					//���������� ������ ������
					pAddressData->Release();
				}
				//����� ����� ������� �����
				else
					//���� ����� ����
					if(pExistLabel)
					{
						//������� ����� ��� ������ ������
						pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
						pAddressData->SetLabel(NULL);
						pAddressData->Release();
					}
				//�������� ����
				InvalidateRect(mhWindow,NULL,FALSE);
			}
			//���������� �����
			SafeRelease(pExistLabel);
		}
		break;
	case MID_CODE_INTERPRETATE_EDITCOMMENT:
		{
			//�������� �����������
			CAddressData* pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
			CComment* pExistComment;
			LPCTSTR szComment=NULL;
			if(pAddressData)
			{
				pExistComment=pAddressData->GetComment();
				pAddressData->Release();
				if(pExistComment)
					szComment=pExistComment->GetText();
			}
			else
				pExistComment=NULL;
			//������� ���� �������������� �����������
			CDialogEditComment Dialog;
			LPTSTR szResultComment;
			if(Dialog.Show(szComment,&szResultComment,mhWindow))
			{
				//���� ����������� ����
				if(szResultComment)
				{
					//������� ����� �����������
					CComment* pNewComment=new CComment(szResultComment);
					//���������� ������ ������
					delete [] szResultComment;
					//������� ������ ������
					pAddressData=mpAddressMap->OpenAddress(mSelectedAddress);
					//������� �����������
					pAddressData->SetComment(pNewComment);
					//���������� �����������
					pNewComment->Release();
					//���������� ������ ������
					pAddressData->Release();
				}
				//����� ����� ������� �����������
				else
					//���� ����������� ���
					if(pExistComment)
					{
						//������� ����������� ��� ������ ������
						pAddressData=mpAddressMap->GetAddress(mSelectedAddress);
						pAddressData->SetComment(NULL);
						pAddressData->Release();
					}
				//�������� ����
				InvalidateRect(mhWindow,NULL,FALSE);
			}
			//���������� �����������
			SafeRelease(pExistComment);
		}
		break;
	case MID_CODE_INTERPRETATE_CREATESUBROUTINE:
		{
			//������� ��������� ��� ������� ����
			CAddressData* pAddressData=mpAddressMap->OpenAddress(mSelectedAddress);
			CInterpretation* pInterpretation=pAddressData->GetInterpretation();
			if(pInterpretation)
			{
				if(pInterpretation->GetInterpretationType()==ADEIT_CODE)
				{
					pAddressData->CreateSubroutine();
					//�������� ����
					InvalidateRect(mhWindow,NULL,FALSE);
				}
				pInterpretation->Release();
			}
			pAddressData->Release();
		}
		break;
	}
}

VOID CDangleWindowCode::OnMouseWheel(INT Delta,UINT Keys)
{
	//������� �������� ��������� � ��������� � � ���������� �����������
	Delta*=(-3);
	//���������� ���� �� ��������� ����������
	OffsetView(Delta);
	OffsetSelection(Delta);

	//�������� ����
	InvalidateRect(mhWindow,NULL,FALSE);
}

LRESULT CDangleWindowCode::DangleProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_KEYDOWN:
		return OnKey(wParam),0;
	case WM_COMMAND:
		return OnCommand(LOWORD(wParam),HIWORD(wParam),(HWND)lParam),0;
	case WM_MOUSEWHEEL:
		return OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA,GET_KEYSTATE_WPARAM(wParam)),0;
	default:
		return DefWindowProc(mhWindow,Msg,wParam,lParam);
	}
}
