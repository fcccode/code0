#include "instruction_set.h"

// class CInstructionSet realization

CInstructionSet::CInstructionSet()
{
	//�������� ������
	mInstructions=(INSTRUCTION_INFORMATION*)VirtualAlloc(NULL,MAX_INSTRUCTIONS_COUNT*sizeof(INSTRUCTION_INFORMATION),MEM_RESERVE | MEM_COMMIT,PAGE_READWRITE);
	mInstructionBytes=(INSTRUCTION_BYTE*)VirtualAlloc(NULL,MAX_INSTRUCTION_BYTES_COUNT*sizeof(INSTRUCTION_BYTE),MEM_RESERVE | MEM_COMMIT,PAGE_READWRITE);
	//���������������� ����������
	mInstructionsCount=0;
	mInstructionBytesCount=0x100;
}

CInstructionSet::~CInstructionSet()
{
	//���������� ������
	VirtualFree(mInstructions,0,MEM_RELEASE);
	VirtualFree(mInstructionBytes,0,MEM_RELEASE);
}

VOID CInstructionSet::CorrectInstruction(INSTRUCTION_INFORMATION& II)
{
	//���� �� ���������
	for(DWORD i=0;i<II.bOperandsCount;++i)
	{
		//���� ������� ������� ����� Mod/RM
		if(II.Operands[i].bSource==OIS_MODRM_RO || II.Operands[i].bSource==OIS_MODRM_RM)
			//��������� ����
			II.bFlags|=IIF_MODRM;
		//���� ������� ������� ����������� ����
		if(II.Operands[i].bSize==OIS_REGISTER_BYTE)
		{
			//������� ���
			II.bRegisterByteOffset=II.bCodeSize-1;
			//��������� ����
			II.bFlags|=IIF_REGBYTE;
		}
	}
}

DWORD CInstructionSet::LoadInstruction_OneByte(INSTRUCTION_INFORMATION II,BYTE Code)
{
	//��������������� ����������
	CorrectInstruction(II);
	//�������� ���������� � �����
	mInstructions[mInstructionsCount]=II;
	
	//�������� ���� ����������
	INSTRUCTION_BYTE& IB=mInstructionBytes[Code];
	//���� ���� ���������� ��� ������������
	if(IB.bFlags)
		MessageBox(NULL,TEXT(""),TEXT(""),0);
	IB.bFlags=IBF_OK | IBF_END;
	IB.bMask=0;
	IB.dwInstructionID=mInstructionsCount;

	//������� ������������� ����������
	return mInstructionsCount++;
}

DWORD CInstructionSet::LoadInstruction_TwoBytes(INSTRUCTION_INFORMATION II,BYTE Code1,BYTE Code2,BYTE Mask)
{
	DWORD Group;
	//���� ������������ ��� ������ ������ ������ ��� ��������
	if(mInstructionBytes[Code1].bFlags)
		//������������ ���
		Group=mInstructionBytes[Code1].dwNextByteGroup;
	else
	{
		//�������� ������������
		Group=mInstructionBytesCount;
		mInstructionBytesCount+=0x100;
	}

	//�������� ������ ����
	INSTRUCTION_BYTE IB;
	IB.bFlags=IBF_OK;
	IB.bMask=Mask;
	IB.dwNextByteGroup=Group;
	//���� ���� ���������� ��� ������������
	if(mInstructionBytes[Code1].bFlags & IBF_END)
		MessageBox(NULL,TEXT(""),TEXT(""),0);
	mInstructionBytes[Code1]=IB;
	//�������� ������ ����
	IB.bFlags=IBF_OK | IBF_END;
	IB.bMask=0;
	IB.dwInstructionID=mInstructionsCount;
	//���� ���� ���������� ��� ������������
	if(mInstructionBytes[Group | Code2].bFlags)
		MessageBox(NULL,TEXT(""),TEXT(""),0);
	mInstructionBytes[Group | Code2]=IB;

	//��������������� ����������
	CorrectInstruction(II);
	//�������� ���������� � �����
	mInstructions[mInstructionsCount]=II;

	//������� ������������� ����������
	return mInstructionsCount++;
}

DWORD CInstructionSet::LoadInstruction_ThreeBytes(INSTRUCTION_INFORMATION II,BYTE Code1,BYTE Code2,BYTE Code3,BYTE Mask1,BYTE Mask2)
{
	DWORD Group1;
	//���� ������������ ��� ������ ������ ������ ��� ��������
	if(mInstructionBytes[Code1].bFlags)
		//������������ ���
		Group1=mInstructionBytes[Code1].dwNextByteGroup;
	else
	{
		//�������� ������������
		Group1=mInstructionBytesCount;
		mInstructionBytesCount+=0x100;
	}
	DWORD Group2;
	//���� ������������ ��� ������ ������� ������ ��� ��������
	if(mInstructionBytes[Group1 | Code2].bFlags)
		//������������ ���
		Group2=mInstructionBytes[Group1 | Code2].dwNextByteGroup;
	else
	{
		//�������� ������������
		Group2=mInstructionBytesCount;
		mInstructionBytesCount+=0x100;
	}

	//�������� ������ ����
	INSTRUCTION_BYTE IB;
	IB.bFlags=IBF_OK;
	IB.bMask=Mask1;
	IB.dwNextByteGroup=Group1;
	//���� ���� ���������� ��� ������������
	if(mInstructionBytes[Code1].bFlags & IBF_END)
		MessageBox(NULL,TEXT(""),TEXT(""),0);
	mInstructionBytes[Code1]=IB;
	//�������� ������ ����
	IB.bFlags=IBF_OK;
	IB.bMask=Mask2;
	IB.dwNextByteGroup=Group2;
	//���� ���� ���������� ��� ������������
	if(mInstructionBytes[Group1 | Code2].bFlags & IBF_END)
		MessageBox(NULL,TEXT(""),TEXT(""),0);
	mInstructionBytes[Group1 | Code2]=IB;
	//�������� ������ ����
	IB.bFlags=IBF_OK | IBF_END;
	IB.bMask=0;
	IB.dwInstructionID=mInstructionsCount;
	//���� ���� ���������� ��� ������������
	if(mInstructionBytes[Group2 | Code3].bFlags)
		MessageBox(NULL,TEXT(""),TEXT(""),0);
	mInstructionBytes[Group2 | Code3]=IB;

	//��������������� ����������
	CorrectInstruction(II);
	//�������� ���������� � �����
	mInstructions[mInstructionsCount]=II;

	//������� ������������� ����������
	return mInstructionsCount++;
}

VOID CInstructionSet::LoadPrefix(BYTE Code,DWORD Flag)
{
	//�������� ���� ����������
	INSTRUCTION_BYTE IB;
	IB.bFlags=IBF_OK | IBF_PREFIX;
	IB.bMask=0;
	IB.dwPrefixFlag=Flag;
	mInstructionBytes[Code]=IB;
}

VOID CInstructionSet::SetInstructionFlag(DWORD Flag)
{
	mInstructions[mInstructionsCount-1].bFlags|=Flag;
}

#include <stdio.h>
#include "consts.h"

VOID CInstructionSet::PrintHTML(LPCTSTR szFile,DWORD GroupByte)
{
	//������� ����
	FILE* f=_tfopen(szFile,TEXT("w"));

	//HTML-���������
	fprintf(f,"<HTML><HEAD><TITLE>������� �������</TITLE></HEAD><BODY><H1>������� �������</H1>");
	if(GroupByte)
		fprintf(f,"<H2>������ ���� %02X</H2>",GroupByte);
	fprintf(f,"<TABLE style=\"FONT-SIZE: 8pt; FONT-FAMILY: Courier New\" width=\"200%\" border=\"1\" bordercolordark=\"0\" bordercolorlight=\"0\">");

	DWORD Group;
	if(GroupByte)
		Group=mInstructionBytes[GroupByte].dwNextByteGroup;
	else
		Group=0;

	// ������ ������
	fprintf(f,"<TR valign=\"middle\" align=\"center\"><TD></TD>");
	for(DWORD x=0;x<0x10;++x)
		fprintf(f,"<TD>%X</TD>",x);
	fprintf(f,"</TR>");

	//������
	for(DWORD y=0;y<0x10;++y)
	{
		fprintf(f,"<TR valign=\"middle\" align=\"center\"><TD>%X</TD>",y);
		//�������
		for(DWORD x=0;x<0x10;++x)
		{
			fprintf(f,"<TD>");

			//���� ����������
			INSTRUCTION_BYTE IB=mInstructionBytes[Group+y*0x10+x];
			if(IB.bFlags & (IBF_OK | IBF_END))
			{
				//���������� �� ����������
				INSTRUCTION_INFORMATION II=mInstructions[IB.dwInstructionID];
				//������� ���
				fprintf(f,"<B>");
				for(LPCTSTR s=II.szName;*s;s++)
					fprintf(f,"%c",*((LPCSTR)s));
				fprintf(f,"</B><BR>");

				//���� �� ���������
				for(DWORD i=0;i<II.bOperandsCount;++i)
				{
					//�������
					OPERAND_INFORMATION OI=II.Operands[i];

					if(i)
						fprintf(f,", ");

					//����� �� ���� ��������
					switch(OI.bSource)
					{
					case OIS_DATA:
					case OIS_SIGNED_DATA:
						switch(OI.bSize)
						{
						case 1:
							fprintf(f,"ib");
							break;
						case 2:
							fprintf(f,"iw");
							break;
						case 4:
							fprintf(f,"id");
							break;
						}
						break;
					case OIS_REGISTER_BYTE:
						break;
					case OIS_MODRM_RO:
						switch(OI.bRegisterCategory)
						{
						case RC_SEGMENT:
							fprintf(f,"sr");
							break;
						case RC_BYTE:
							fprintf(f,"r8");
							break;
						case RC_WORD:
							fprintf(f,"r16");
							break;
						case RC_DWORD:
							fprintf(f,"r32");
							break;
						}
						break;
					case OIS_MODRM_RM:
						switch(OI.bRegisterCategory)
						{
						case RC_SEGMENT:
							fprintf(f,"sr");
							break;
						case RC_BYTE:
							fprintf(f,"r8");
							break;
						case RC_WORD:
							fprintf(f,"r16");
							break;
						case RC_DWORD:
							fprintf(f,"r32");
							break;
						}
						fprintf(f,"/m%d",OI.bSize*8);
						break;
					case OIS_MEMORY_IM:
						fprintf(f,"[im]");
						break;
					case OIS_CONST_REGISTER:
						{
							for(LPCTSTR s=gcszRegisterNames[REGISTER_ID(OI.bRegisterCategory,OI.bConstRegister)];*s;s++)
								fprintf(f,"%c",*((LPCSTR)s));
						}
						break;
					}
				}
			}
			else
				fprintf(f,"-");
			fprintf(f,"</TD>");

		}
		//����� ������
		fprintf(f,"</TR>");
	}

	//����� �����
	fprintf(f,"</TABLE></BODY></HTML>");

	//������� ����
	fclose(f);
}
