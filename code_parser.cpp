#include "code_parser.h"
#include "instruction.h"
#include "interpretations.h"

#include "consts.h"
#include <list>

/*
����, ������� ��������� ������ ����������.
*/

CCodeParser::CCodeParser(CCyberMemory* pMemory,CAddressMap* pAddressMap,CInstructionSet* pSet16,CInstructionSet* pSet32)
{
	//������� ������ ��� ������� � �����������
	mpMemory=new CCyberMemoryAccessor(pMemory);

	//������������ ��������� �������
	mModes[MODE_16BIT].pInstructionSet=pSet16;
	pSet16->AddRef();
	mModes[MODE_16BIT].NormalAddressOperandSize=2;
	mModes[MODE_16BIT].RedefinedAddressOperandSize=4;

	mModes[MODE_32BIT].pInstructionSet=pSet32;
	pSet32->AddRef();
	mModes[MODE_32BIT].NormalAddressOperandSize=4;
	mModes[MODE_32BIT].RedefinedAddressOperandSize=2;

	//��������� ��������� �� ��������� �������� ����������
	mpAddressMap=pAddressMap;
	mpAddressMap->AddRef();
}

CCodeParser::~CCodeParser()
{
	//������� ������ ��� ������� � ������
	mpMemory->Release();
	//���������� ����� �������
	mpAddressMap->Release();
	//���������� ������ ����������
	mModes[MODE_16BIT].pInstructionSet->Release();
	mModes[MODE_32BIT].pInstructionSet->Release();
}

VOID CCodeParser::UpdateMode()
{
	//���� ������������ ������� ��������� ����������� ��������
	if(mPrefixFlags & PF_OS)
		//������� �������� �����
		mpCurrentMode=mModes+!mMode;
	else
		//����� �� ��������
		mpCurrentMode=mModes+mMode;
	//���� ������������ ������� ��������� ����������� ������
	if(mPrefixFlags & PF_AS)
		//������� ������� ������ ������
		mAddressOperandSize=mModes[mMode].RedefinedAddressOperandSize;
	else
		mAddressOperandSize=mModes[mMode].NormalAddressOperandSize;
}

//�������, ����������� ������ ���� ModR/M
/*
        ������ ���� ModRM:
          [ ][ ][ ][ ][ ][ ][ ][ ]
           7                    0
          [MOD ][  R/O  ][  R/M  ]
        ������ ���� SIB:
          [ ][ ][ ][ ][ ][ ][ ][ ]
           7                    0
          [ S  ][   I   ][   B   ]
*/
VOID CCodeParser::ParseModRM(MODRM* pModRM)
{
	//������� ���� ModR/M
	BYTE bModRM=mpMemory->CurrentByte();

	//�������� ���� R/M
	BYTE bRM=(bModRM & 0x7);
	//�������� ���� R/O
	BYTE bRO=((bModRM>>3) & 0x7);
	//�������� ���� MOD
	BYTE bMOD=((bModRM>>6) & 0x7);

	//�������� ����� RM
	pModRM->RM.bFlags=0;

	//������������ ������� R/O, ������� - ������ �������
	pModRM->RO.bRORegister=bRO;

	//����� ����� ���� ������ ������ �������� R/M

	//���� ������������ ����������� ���������
	if(bMOD==3)
	{
		//���� R/M ��������� �������; ������������ ������� R/M
		pModRM->RM.bRegister=bRM;
		//��!
		return;
	}

	//����� ������� R/M ��������� � ������; MOD ����� ����� �������� 0, 1, 2

	//������� ��� ��������
	pModRM->RM.bFlags=MODRM_RM_MEMORY;

	//���������� ������ �������� �������� � ������� ���� MOD
	BYTE bOffsetLength;
	//���� MOD=0, �� �������� ���
	if(bMOD==0) bOffsetLength=0;
	//����� ���� MOD=1, �� �������� - ��������
	else if(bMOD==1) bOffsetLength=1;
	//����� MOD=2, � �������� �������� 2 ��� 4 ����� � ����������� �� �������� ������
	else
		bOffsetLength=mAddressOperandSize;

	//����� �������� ������� �� ������ ����������
	if(mAddressOperandSize==4)		// 32-������ �����
	{
		//���� ���� R/M �� ����� 4, �� SIB �� ������������
		if(bRM!=4)
		{
			//���� R/M=5 � MOD=0, �� ������������ ������ 32-������ �������� (� ������� �� ��������� ������� MOD=0), � �� ������������ �������
			if((bRM==5) && (bMOD==0))
			{
				//��������� ������ �������� ��������
				bOffsetLength=4;
				//���������� ����� ��������-����������
				pModRM->RM.bFlags|=MODRM_RM_OFFSET;
			}
			else
			{
				//�������� ������� ������� ��� ���������
				pModRM->RM.bBaseRegister=REGISTER_ID(RC_DWORD,bRM);
				//���������� �����
				pModRM->RM.bFlags|=OMF_BASE_REGISTER | (bOffsetLength ? MODRM_RM_OFFSET : 0);
			}
			//������� ��������
			pModRM->RM.dwOffset=mpMemory->CurrentSignedData(bOffsetLength);
			//��!
			return;
		}

		//������ ������� ������ ������, ����� R/M=4, �.�. ������������ SIB

		//��������� SIB

		//������� ���� SIB
		BYTE bSIB=mpMemory->CurrentByte();

		//�������� ������� ������� �� ���� B
		pModRM->RM.bBaseRegister=bSIB & 0x7;
		//(������� ������� �� ������������, ���� B=5 � MOD=0
		if(!((pModRM->RM.bBaseRegister==5) && (bMOD==0)))
		{
			//�������� ���� ������������� �������� ��������
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER;
			//������������ ������������� ��������
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_DWORD,pModRM->RM.bBaseRegister);
		}
		//����� ������������� B=5 � MOD=0
		else
			//� ����� ������ �������� �������� ���, �� ���� 32-������ ��������
			bOffsetLength=4;

		//�������� ��������� ������� �� ���� I
		pModRM->RM.bIndexRegister=(bSIB >> 3) & 0x7;
		//(��������� ������� �� ������������, ���� I=4)
		if(pModRM->RM.bIndexRegister!=4)
		{
			//�������� ���� ������������� ���������� ��������
			pModRM->RM.bFlags|=MODRM_RM_INDEX_REGISTER;
			//������������ ������������� ��������
			pModRM->RM.bIndexRegister=REGISTER_ID(RC_DWORD,pModRM->RM.bIndexRegister);
		}

		//�������� ��������� �� ���� S
		pModRM->RM.bMultiplier=1<<(bSIB >> 6);
		//�������� ���� ������������� ���������
		if(pModRM->RM.bMultiplier>1) pModRM->RM.bFlags|=MODRM_RM_MULTIPLIER;

		//������� ��������
		pModRM->RM.dwOffset=mpMemory->CurrentSignedData(bOffsetLength);
		//������� ���� ������� �������� ��������-����������
		pModRM->RM.bFlags|=((bOffsetLength && pModRM->RM.dwOffset) ? MODRM_RM_OFFSET : 0);
	}
	else		// 16-������ �����
	{
		//���� MOD=00 � R/M=6, �� ������������ ������ ��������
		if((bMOD==0) && (bRM==6))
		{
			//���������� �����
			pModRM->RM.bFlags|=MODRM_RM_OFFSET;
			//��������� ������ �������� ��������
			bOffsetLength=2;
			//������� ��������
			pModRM->RM.dwOffset=mpMemory->CurrentSignedData(bOffsetLength);
			//��!
			return;
		}

		//������ �������� ������ ��������� ������ R/M
		switch(bRM)
		{
		case 0:					// [BX+SI]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER | MODRM_RM_INDEX_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,3);
			pModRM->RM.bIndexRegister=REGISTER_ID(RC_WORD,6);
			break;
		case 1:					// [BX+DI]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER | MODRM_RM_INDEX_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,3);
			pModRM->RM.bIndexRegister=REGISTER_ID(RC_WORD,7);
			break;
		case 2:					// [BP+SI]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER | MODRM_RM_INDEX_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,5);
			pModRM->RM.bIndexRegister=REGISTER_ID(RC_WORD,6);
			break;
		case 3:					// [BP+DI]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER | MODRM_RM_INDEX_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,5);
			pModRM->RM.bIndexRegister=REGISTER_ID(RC_WORD,7);
			break;
		case 4:					// [SI]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,6);
			break;
		case 5:					// [DI]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,7);
			break;
		case 6:					// [BP]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_BP;
			break;
		case 7:					// [BX]
			pModRM->RM.bFlags|=MODRM_RM_BASE_REGISTER;
			pModRM->RM.bBaseRegister=REGISTER_ID(RC_WORD,3);
			break;
		}

		//�������� ��������
		if(bOffsetLength)
		{
			//�������� ����
			pModRM->RM.bFlags|=MODRM_RM_OFFSET;
			//������� ��������
			pModRM->RM.dwOffset=mpMemory->CurrentSignedData(bOffsetLength);
		}
	}

	//��!
}

//���������� ��������� ���� ����������
DWORD CCodeParser::ParseInstructionByte(DWORD Group,BYTE Mask)
{
	//������� ������� ���� � �������� �� ���� �����
	BYTE Byte=mpMemory->CurrentByte() & Mask;

	//�������� ���������� � ����� ����������
	INSTRUCTION_BYTE IB=mpCurrentMode->pInstructionSet->mInstructionBytes[Group+Byte];

	//���� ���� ���� ��������
	if(IB.bFlags & IBF_OK)
		//���� ���� ���� - �������
		if(IB.bFlags & IBF_PREFIX)
		{
			//���������� ������c������� ���� ��������
			mPrefixFlags|=IB.dwPrefixFlag;
			//�������� �������������� ����������, ����� ���������� ���������� ���������� �����
			mInstructionAddress++;
			//���������� ������ ������
			return ParseInstructionByte(Group,Mask);
		}
		//����� ���� ���� ���� - ��������
		else if(IB.bFlags & IBF_END)
			//������ ��������, ������� ������������� ����������
			return IB.dwInstructionID;
		//����� ���� ���� �� ���������
		else
			//���������� ���������� ������
			return ParseInstructionByte(IB.dwNextByteGroup,IB.bMask);
	//����� ���� ���� �� ��������
	else
		//������� ������
		return INSTRUCTION_ID_ERROR;
}

CInstruction* CCodeParser::ParseInstruction(CYBER_ADDRESS Address)
{
	//��������� ����� ������� ����������
	mInstructionAddress=Address;
	//���������� ������� �����
	mpMemory->SetPointer(Address);

	//�������� ����� ���������
	mPrefixFlags=0;

	//��������� ������ ������ ���������� � �������� ����� ����������
	DWORD InstructionID=ParseInstructionByte(0,0xFF);

	//����� ������ Address ������������ mInstructionAddress, ��� ��� ��� ��������
	//������ ���� � ������ ������ ��-�� ������� ��������� (�������������� �
	//ParseInstructionByte).

	//���� ���������� ��������
	if(InstructionID==INSTRUCTION_ID_ERROR)
		//������� ������
		return NULL;

	//�������� ������� �����
	UpdateMode();

	//���������� ���������� �� ����������
	INSTRUCTION_INFORMATION InstructionInformation=mpCurrentMode->pInstructionSet->mInstructions[InstructionID];

	//���� ���������� ��������� ����������� ����
	BYTE ByteRegister;
	{
		CCyberMemory* pMemory;
		try
		{
			if(InstructionInformation.bFlags & IIF_REGBYTE)
			{
				//������� ��� � ������� ����� ��������
				pMemory=mpMemory->Memory();
				ByteRegister=(pMemory->Byte(mInstructionAddress+InstructionInformation.bRegisterByteOffset) & 0x7);
				pMemory->Release();
			}
		}
		catch(CCyberMemoryPageFaultException)
		{
			//���������� ������
			pMemory->Release();
			//�������� ���������� ������
			throw;
		}
	}

	//���� ������������ ������� ��������������� ��������
	BYTE Segment;
	BOOL IsSegment=FALSE;
	if(mPrefixFlags & PREFIXES_SEGMENT)
	{
		//������� ���� ������� ��������
		IsSegment=TRUE;
		//�������� ���
		BYTE j=0;
		for(BYTE i=1;i<=PREFIXES_SEGMENT;i<<=1,j++)
			if(mPrefixFlags & i)
			{
				Segment=j;
				break;
			}
	}

	//���������� ������� ��������� ������ �� ��������
	mpMemory->SetPointer(mInstructionAddress+InstructionInformation.bCodeSize);

	//���� ���������� ��������� ���� ModRM
	MODRM ModRM;
	//����� try ���, ��� ��� ������� �� ����������
	if(InstructionInformation.bFlags & IIF_MODRM)
		//�������� � ���������� ���
		ParseModRM(&ModRM);

	//������� ����������
	CInstruction* pInstruction=new CInstruction(mpCurrentMode->pInstructionSet,InstructionID,mPrefixFlags);

	//������ ��������� ��� ����������� �������������
	std::list<COperand*> OperandsList;

	//���� �� ���������
	for(BYTE i=0;i<InstructionInformation.bOperandsCount;++i)
	{
		//���������� ���������� �� ��������
		OPERAND_INFORMATION OperandInformation=InstructionInformation.Operands[i];

		//���� ��������� ���������� ������ ������
		try
		{
			//����� �� ��������� ��������
			COperand* pOperand;
			switch(OperandInformation.bSource)
			{
			case OIS_DATA:												//���������
				//������� ������� - ���������
				pOperand=new COperandData(mpMemory->CurrentData(OperandInformation.bSize));
				break;
			case OIS_SIGNED_DATA:									//��������� �� ������
				//������� ������� - ���������
				pOperand=new COperandData(mpMemory->CurrentSignedData(OperandInformation.bSize));
				break;
			case OIS_REGISTER_BYTE:								//������� �� ������������ �����
				//������� ������� - �������
				pOperand=new COperandRegister(ByteRegister);
				break;
			case OIS_MODRM_RO:										//������� �� ���� RO ����� ModR/M
				//������� ������� - �������
				pOperand=new COperandRegister(ModRM.RO.bRORegister);
				break;
			case OIS_MODRM_RM:										//������� ��� ���������� �� ���� RM ����� ModR/M
				//���� ������� - ����������
				if(ModRM.RM.bFlags & MODRM_RM_MEMORY)
					//������� ������� - ����������
					pOperand=new COperandMemory(ModRM);
				//����� ������� - �������
				else
					//������� ������� - �������
					pOperand=new COperandRegister(ModRM.RM.bRegister);
				break;
			case OIS_MEMORY_IM:										//���������� �� ������������ ������
				//������� ������� - ���������� (�������� ������ ������������ ������, ���� �����)
				//ERROR FIXED
//				pOperand=new COperandMemory(mpMemory->CurrentData(OperandInformation.bSize>1 ? mAddressOperandSize : 1));
				pOperand=new COperandMemory(mpMemory->CurrentData(mAddressOperandSize));
				break;
			case OIS_CONST_REGISTER:							//���������� �������
				//������� ������� - �������
				pOperand=new COperandRegister(OperandInformation.bConstRegister);
				break;
			case OIS_CONST_DATA:									//���������� ���������
				//������� ������� - ���������
				pOperand=new COperandData(OperandInformation.bConstData);
				break;
			}
			//�������� ������� � ������ ���������� � � ��������� ������
			pInstruction->AddOperand(pOperand);
			OperandsList.push_back(pOperand);
		}
		catch(CCyberMemoryPageFaultException)
		{
			//�� ������� ������� �����-���� ����������

			//������� ��� ������������ ��������
			for(std::list<COperand*>::iterator i=OperandsList.begin();i!=OperandsList.end();i++)
				(*i)->Release();
			OperandsList.clear();
			//������� ����������
			pInstruction->Release();

			//�������� ���������� ������
			throw;
		}
	}

	//��������������� ��������
	//������������� ����������� ����� ��������� ���� ���������, ����� �������� ����� ����������
	DWORD j=0;
	for(std::list<COperand*>::iterator i=OperandsList.begin();i!=OperandsList.end();i++)
	{
		//�������� ��������� �� �������
		COperand* pOperand=*i;
		//���������� ���������� �� ��������
		OPERAND_INFORMATION OperandInformation=InstructionInformation.Operands[j++];
		//����� �� ���� ��������
		switch(pOperand->GetType())
		{
		case OT_DATA:
			{
				//���� ����, ��� ����� ��������� ������
				BOOL bNeedLink=FALSE;
				//���� ������ ��������������� ��� ���������� ���������
				if(OperandInformation.bDataReference & OIR_ABSOLUTE_POINTER)
				{
					bNeedLink=TRUE;
					//� 16-������ ������ 4-� ������� ����� - ��� far-������ (�������:��������)
					if(mMode==MODE_16BIT && OperandInformation.bSize==4)
						//����� ������� �� ������������� far-������
						((COperandData*)pOperand)->mReferenceFlags&=~OIR_ABSOLUTE_POINTER;
//						((COperandData*)pOperand)->mData=(((COperandData*)pOperand)->mData & 0xFFFF) + ((((COperandData*)pOperand)->mData & 0xFFFF0000)>>12);
				}
				//����� ���� ������ ��������������� ��� ������������� ���������
				else if(OperandInformation.bDataReference & OIR_RELATIVE_POINTER)
				{
					bNeedLink=TRUE;
					//��������������� ���������
					//� 16-������ ������ �������� �������� ������ ����� �������� (�� ������ ��������)
					if(mMode==MODE_16BIT && OperandInformation.bSize==2)
					{
						WORD offset=(WORD)((COperandData*)pOperand)->mData + (WORD)(mpMemory->GetPointer() & 0xFFFF);
						((COperandData*)pOperand)->mData=(mpMemory->GetPointer() & 0xFFFF0000) | offset;
					}
					else
						((COperandData*)pOperand)->mData+=mpMemory->GetPointer();
				}
				//���� ���������� ������� ������
				if(bNeedLink)
				{
					//��������� ��� ������ ������
					((COperandData*)pOperand)->mReferenceFlags=OperandInformation.bDataReference;
					//������� ������������ ������
					AddXReference(Address,((COperandData*)pOperand)->mData,OperandInformation.bDataReference);
				}
			}
			break;
		case OT_REGISTER:
			//��������������� ������������� �������� � ������� ��������� ��������
			((COperandRegister*)pOperand)->mRegister=REGISTER_ID(OperandInformation.bRegisterCategory,((COperandRegister*)pOperand)->mRegister);
			break;
		case OT_MEMORY:
			//��������� ������ ��������
			((COperandMemory*)pOperand)->mSize=OperandInformation.bSize;
			//���� ���� ������� ��������������� ��������
			if(IsSegment)
				//������� �������
				((COperandMemory*)pOperand)->SetSegment(Segment);
			//���� ���������� ������� ������ ���������
			if(((COperandMemory*)pOperand)->mFlags==OMF_OFFSET)
				//������� ������������ ������
				AddXReference(Address,((COperandMemory*)pOperand)->mOffset,OperandInformation.bAddressReference,OperandInformation.bSize);
			break;
		}

		//���������� �������
		pOperand->Release();
	}

	//�������� ������ ���������� �� ��������
	OperandsList.clear();

	//��!
	return pInstruction;
};

VOID CCodeParser::AddXReference(CYBER_ADDRESS AddressFrom,CYBER_ADDRESS AddressTo,BYTE ReferenceFlags,BYTE Size)
{
	//�������� ������������ ������ � ���������
	mpAddressMap->AddXReference(AddressFrom,AddressTo,ReferenceFlags);

	//�������� ������ ������
	CAddressData* pAddressData=mpAddressMap->OpenAddress(AddressTo);

	//������������ ��� �����
	TCHAR szLabel[0x10];

	//���� ������ ��������� ��� ����������
	if(ReferenceFlags & OIR_REFERENCE_EXECUTE)
	{
		//���� ������ ��������� ��� ����� ���������
		if(ReferenceFlags & OIR_REFERENCE_EXECUTE_CALL)
		{
			//�������� ����� � ������� ������������������
			mParsingQueue.insert(std::make_pair(AddressTo,AddressTo));
			_stprintf(szLabel,TEXT("sub_%X"),AddressTo);
			//����� ����, ������� �������
			pAddressData->CreateSubroutine();
		}
		else
		{
			//�������� ����� � ������� ������������������
			mParsingQueue.insert(std::make_pair(AddressTo,mSubroutineAddress));
			_stprintf(szLabel,TEXT("loc_%X"),AddressTo);
		}
	}
	//����� ������ ��������� �� ������
	else
	{
		//������� �����
		_stprintf(szLabel,TEXT("data_%X"),AddressTo);
		//���������� �������������-������ ��� ������, ���� � ���
		CInterpretation* pInterpretation=pAddressData->GetInterpretation();
		if(pInterpretation)
			pInterpretation->Release();
		else
		{
			CCyberMemory* pCyberMemory=mpMemory->Memory();
			pInterpretation=new CInterpretationData(pCyberMemory,AddressTo,Size);
			pCyberMemory->Release();
			pAddressData->SetInterpretation(pInterpretation);
			mpAddressMap->SetAddressSize(AddressTo,Size);
			pInterpretation->Release();
		}
	}

	//���������� ������ ������
	pAddressData->Release();

	//������� �����
	mpAddressMap->SetLabel(AddressTo,szLabel);
}

VOID CCodeParser::SetMode(DWORD Mode)
{
	//���������� ������� �����
	mMode=Mode;
	mpCurrentMode=mModes+mMode;
}

VOID CCodeParser::ParseEntryPoint(CYBER_ADDRESS EntryPoint)
{
	//�������� ����� � ������� ������������������
	mParsingQueue.insert(std::make_pair(EntryPoint,EntryPoint));

	//����������������� ������� �����
	std::set<std::pair<CYBER_ADDRESS,CYBER_ADDRESS> >::iterator i;
	while((i=mParsingQueue.begin())!=mParsingQueue.end())
	{
		//�������� ����� ������
		CYBER_ADDRESS Address=(*i).first;
		mSubroutineAddress=(*i).second;
		//������� ������� ����
		mParsingQueue.erase(i);

		//���� ������������������
		for(;;)
		{
			//������� ������ ������
			CAddressData* pAddressData=mpAddressMap->OpenAddress(Address);
			//�������� ��� ������������ �������������, ���� ��� ����
			CInterpretation* pExistInterpretation=pAddressData->GetInterpretation();
			if(pExistInterpretation)
			{
				//���� ��� ������������� - ���, �� ���������
				if(pExistInterpretation->GetInterpretationType()==ADEIT_CODE)
				{
					pExistInterpretation->Release();
					pAddressData->Release();
					break;
				}
				pExistInterpretation->Release();
			}

			CInterpretationCode* pInterpretation;
			CInstruction* pInstruction;
			//����������������� ����������
			try
			{
				pInstruction=ParseInstruction(Address);
			}
			catch(CCyberMemoryPageFaultException)
			{
				//�� ����� �� ���������� ������
				//���������� ������ ������
				pAddressData->Release();
				//��������� ������������������ �����
				break;
			}
			//������� �������������
			pInterpretation=new CInterpretationCode(pInstruction);

			//������� ������������� ��� ������ ������
			pAddressData->SetInterpretation(pInterpretation);

			//������� ����� ������ ���������
			pAddressData->SetBlockAddress(mSubroutineAddress);

			//������ ������
			CYBER_ADDRESS Size;
			//������� ������ ������
			if(pInstruction)
				Size=mpMemory->GetPointer()-Address;
			else
				Size=1;
			mpAddressMap->SetAddressSize(Address,Size);

			//���� ���������� ����������
			if(pInstruction)
			{
				//�������� ������������� ����������
				DWORD InstructionID=pInstruction->mInstructionID;
				//���������� ����������
				pInstruction->Release();
				//���� ���������� �������� ���������� ����� ����
				if(mpCurrentMode->pInstructionSet->mInstructions[InstructionID].bFlags & IIF_ENDPROC)
				{
					//���������� ���� ����� ���������
					pInterpretation->SetFlags(ICF_SUBROUTINE_END);
					//���������� ������������� � ������ ������
					pInterpretation->Release();
					pAddressData->Release();
					//��������� ������������������
					break;
				}
				//����� ���� ���������� �������� ������������
				else if(mpCurrentMode->pInstructionSet->mInstructions[InstructionID].bFlags & IIF_SPACE)
					//���������� ���� �����������
					pInterpretation->SetFlags(ICF_SPACE);
			}

			//���������� ������������� � ������ ������
			pInterpretation->Release();
			pAddressData->Release();

			//������������� � ��������� ����������
			Address+=Size;
		}
	}
}
