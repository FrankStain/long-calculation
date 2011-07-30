//������ ULINTClass
//�������� �������� ������� ULINT, LINT, LFLT
//---------------------------------------------------------------------------

#ifndef ULINTClassH
#define ULINTClassH

//  ����� ����� ����� ��� �����
class ULINT

{
  // ���������� ���� (� ��������� ������ ����� ������ ������ ������� � ������������ ������)
     protected:
       AnsiString fLNum;  //������� �����
       UINT fFigures;  //  ���������� ����
       UINT fBlocks;   // ���������� ����� ���������
  // �������� ������ (� ��������� ������ ����� ������ ����� ������� ���������, ������� �� �� ������� ��������)
     public:
       ULINT(AnsiString NumS);      //����������� (��������� ��� �������������, ����������� ������ ����� ��� ������ � �� ����� ��� ������������� ��������)
       ULINT* operator +(ULINT* B); //�������� (������������� �������� ��� ����, ����� ������������ �� ��� ���� ��������� �����  � ������� ������ ��������� �� �)
       ULINT* operator -(ULINT* B); //���������
       ULINT* operator *(ULINT* B); //���������
       ULINT* operator /(ULINT* B); //�������
       AnsiString GetNumString();   //����� ������
       void PutNumString(AnsiString SNum);  //���� ������
       ULINT* MinL(ULINT* A, ULINT* B);     //����������� �� ���� ���������
       ULINT* MaxL(ULINT* A, ULINT* B);     //������������ �� ���� ���������
};//ULINT

// ����� ������ ����� �� ������
class LINT : public ULINT  // ������������ �� ������ ULINT

{
   // ���������� ����
     protected:
       AnsiString fSign;  // ���� �����
   //
     public:
       void PutSign(AnsiString Sign);  // ���� ����� �����
       AnsiString GetSign();           // ����� ����� �����
       LINT(AnsiString NumS);          //�����������
       LINT* operator +(LINT* B);       //��������
       LINT* operator -(LINT* B);       //���������
       LINT* operator *(LINT* B);       //���������
       LINT* operator /(LINT* B);       //�������
       AnsiString GetNumString();        //����� �����
       void PutNumString(AnsiString SNum);   //���� �����
       AnsiString GetString();                //����� ����� ��� �����
       LINT* MinL(LINT* A, LINT* B);          //����������� �����
       LINT* MaxL(LINT* A, LINT* B);          //������������ �����
};  //LINT

//������ ����� �� ������
class LFLT : public LINT  // ������������ �� LINT

{
     // ���������� ����
     protected:
       AnsiString fInt;  // ����� ����� �����
       AnsiString fFrac;  // ������� ����� �����
    // �������� ������
     public:
       LFLT(AnsiString NumS);  // ����������
       LFLT* operator +(LFLT* B);  // ��������
       LFLT* operator -(LFLT* B);  //���������
       LFLT* operator *(LFLT* B);  //���������
       LFLT* operator /(LFLT* B);  // �������
       void PutNumString(AnsiString SNum);  // ���� �����
}; // LFLT


//---------------------------------------------------------------------------
#endif

������� ���������� ������, ����������� �������� ������� ������� 

//   �������� ��������
ULINT* ULINT::operator+(ULINT* B)
 {
  AnsiString SBlockA,SBlockB,SSubSumBlock,Sum;
  UINT NBlockA, NBlockB, NSumBlock; //�������� �������� ������ UINT
  UINT R,k,k1,i; //�������� �������� ������ UINT
  int j;

  k= 9;
  R= 0;
  // ��������� ������� ����� �����
  for (i= 1;i<= (fBlocks > B->fBlocks ? fBlocks: B->fBlocks);i++)
  {
   k1= MaxL(this,B)->fBlocks;
   j= fFigures-k+1;
   if (!(j <= 0))
     SBlockA= fLNum.SubString(fFigures-k+1,9);
   else
     SBlockA= fLNum.SubString(1,fFigures-k+9);
   j= B->fFigures-k+1;
   if (!(j <= 0))
     SBlockB= B->fLNum.SubString(B->fFigures-k+1,9);
   else
     SBlockB= B->fLNum.SubString(1,B->fFigures-k+9);
   k+= 9;
   NBlockA= SBlockA.ToIntDef(0);
   NBlockB= SBlockB.ToIntDef(0);
   NSumBlock= NBlockA + NBlockB + R;
   SSubSumBlock= IntToStr(NSumBlock);
   if (SSubSumBlock.Length() > 9)
     {
      R= 1;
      SSubSumBlock.Delete(1,1);
      if (i == k1)
        i--;
     }
   else R= 0;
   if ((SSubSumBlock.Length() < 9) && (i < k1))
          for (k1= SSubSumBlock.Length(); k1 < 9;k1++)
           SSubSumBlock= "0" + SSubSumBlock;
   Sum= SSubSumBlock + Sum;
  }//����� for
  ULINT *C= new ULINT(Sum);
  return C;
 }// ULINT::operator+

//  �������� ���������
ULINT* ULINT::operator-(ULINT* B)
 {
  AnsiString SBlockA,SBlockB,SSubSumBlock,Sum,Sign;
  UINT NBlockA, NBlockB;
  int NSumBlock;
  UINT R,k,k1,i;
  int j;

  Sign= "";
  if (MinL(this,B) == this)
    {
     Sign= "-";
    }
  k= 9;
  R= 0;
  for (i= 1;i<= (fBlocks > B->fBlocks ? fBlocks: B->fBlocks);i++)
  {
   j= MaxL(this,B)->fFigures-k+1;
   if (j > 0)
     SBlockA= MaxL(this,B)->fLNum.SubString(MaxL(this,B)->fFigures-k+1,9);
   else
     SBlockA= MaxL(this,B)->fLNum.SubString(1,MaxL(this,B)->fFigures-k+9);
   j= MinL(this,B)->fFigures-k+1;
   if (j > 0)
     SBlockB= MinL(this,B)->fLNum.SubString(MinL(this,B)->fFigures-k+1,9);
   else
     SBlockB= MinL(this,B)->fLNum.SubString(1,MinL(this,B)->fFigures-k+9);
   k+= 9;
   NBlockA= SBlockA.ToIntDef(0);
   NBlockB= SBlockB.ToIntDef(0);
   NSumBlock= NBlockA - NBlockB - R;
   k1= fBlocks > B->fBlocks ? fBlocks: B->fBlocks;
   if ((NSumBlock < 0) && (i != k1))
     {
      NSumBlock+= 1000000000;
      R= 1;
     }
   else R=0;
   if ((i != k1) && (NSumBlock == 0))
     SSubSumBlock="000000000";
   else
     {if ((NSumBlock == 0) && (i>1))
       SSubSumBlock= "";
     else if (NSumBlock == 0)
            SSubSumBlock= "0";
     else
       SSubSumBlock= IntToStr(NSumBlock);
     if (i != k1)
       {
        SSubSumBlock= IntToStr(NSumBlock);
        if (SSubSumBlock.Length() < 9)
          for (k1= SSubSumBlock.Length(); k1 < 9;k1++)
           SSubSumBlock= "0" + SSubSumBlock;
       }}
   Sum= SSubSumBlock+Sum;
  }
  if (Sum.AnsiPos("0")==1)
    Sum= "0";
  ULINT *C= new ULINT(Sign+Sum);
  return C;
 }// �������� ���������

// �������� ���������
 ULINT* ULINT :: operator*(ULINT* B)
 {
  ULINT* MultSum= new ULINT("0");
  ULINT* MiniMultSum= new ULINT("0");
  AnsiString S,Ch;
  UINT i,j;
  S="";
  for (i= MinL(this,B)->fFigures; i >= 1; i-- )
  {
   Ch=MinL(this,B)->fLNum[i];
   for(j= 1; j<= Ch.ToIntDef(0);j++)
   {
    MiniMultSum= MiniMultSum->operator +(MaxL(this,B));
   }
   MiniMultSum->PutNumString(MiniMultSum->GetNumString()+S);
   MultSum= MultSum->operator +(MiniMultSum);
   MiniMultSum->PutNumString("");
   S= S+"0";
  }
  return MultSum;
 }  // ULINT :: operator*

// �������� �������
 LFLT* LFLT::operator /(LFLT* B)
 {
  LINT* A1=new LINT(0);
  LINT* B1=new LINT(0);
  LINT* C1=new LINT(0);
  LFLT* C=new LFLT("0,0");
  AnsiString S,S1,FracS,IntS,SubS;
  UINT i;
  int NCh,k,PosCh;

  if (fInt != 0)
   S= this->fInt+this->fFrac;
  else
   S= this->fFrac;
  if (B->fInt != 0)
   S1= B->fInt+B->fFrac;
  else
   S1= B->fFrac;
  if (this->fFrac.Length() > B->fFrac.Length())
    for (i=1; i<=fFrac.Length() - B->fFrac.Length();i++)
      S1=S1+"0";
  else
    for (i=1; i<=B->fFrac.Length()-fFrac.Length();i++)
      S=S+"0";
  FracS="";
  if (S.Length() < S1.Length())
    {
     IntS= "0";
     k= S1.Length() - S.Length();
     for (i=1; i<= k;i++)
      {if (i != 1)
         FracS=FracS+"0";
       S=S+"0";
      }
     A1->PutNumString(S);
     B1->PutNumString(S1);
     while ((FracS.Length() <= 6) && (A1->GetNumString()!= B1->GetNumString()+"0"))
     {
      NCh= 0;
      while (A1->GetSign() == "+")
        {
         A1= A1->operator -(B1);
         NCh++;
        }
      if (A1->GetNumString()!= "-0")
        NCh--;
      A1=A1->operator +(B1);
      A1->PutNumString(A1->GetNumString()+"0");
      FracS= FracS+IntToStr(NCh);
     }
    }
    else
    {
     SubS= S.SubString(1,S1.Length());
     i=0;
     PosCh= S1.Length();
     A1->PutNumString(SubS);
     B1->PutNumString(S1);
     IntS="";
     while (i<= S.Length()-S1.Length())// && (A1->GetNumString()!= "-0"))
      {
       NCh= 0;
       while (A1->GetSign() == "+")
        {
         A1= A1->operator -(B1);
         NCh++;
        }
       if (A1->GetNumString()!= "-0")
        {NCh--;
        A1=A1->operator +(B1);}
       PosCh++;
       A1->PutNumString(A1->GetNumString()+S.SubString(PosCh,1));
       A1->PutSign("+");
       IntS=IntS+IntToStr(NCh);
       i++;
      }
      i=0;
      while ((FracS.Length() <= 6) && (A1->GetNumString()!= B1->GetNumString()+"0"))
     {
      NCh= 0;
      while (A1->GetSign() == "+")
        {
         A1= A1->operator -(B1);
         NCh++;
        }
      if (A1->GetNumString()!= "-0")
        NCh--;
      A1=A1->operator +(B1);
      A1->PutNumString(A1->GetNumString()+"0");
      A1->PutSign("+");
      if (i != 0)
        FracS= FracS+IntToStr(NCh);
      i++;
     }
    }
  if (IntS != "0")
   {
    while (IntS[1] == '0')
      {
       IntS= IntS.Delete(1,1);
      }
   }

  if (FracS != "0")
   {
    while ((FracS[FracS.Length()] == '0') && (FracS.Length() != 1 ))
      {
       FracS= FracS.Delete(FracS.Length()-1,1);
      }
   }

  if (FracS.Length() == 0)
    FracS= "0";
  if (fSign != B->fSign)
    C->PutNumString("-"+IntS+","+FracS);
  else
    C->PutNumString(IntS+","+FracS);
  return C;
 } // LFLT::operator /
