//ћодуль ULINTClass
//—одержит описание классов ULINT, LINT, LFLT
//---------------------------------------------------------------------------

#ifndef ULINTClassH
#define ULINTClassH

//  класс целое число без знака
class ULINT

{
  // защищенные пол€ (к элементам класса имеют доступ методы данного и производного класса)
     protected:
       AnsiString fLNum;  //длинное число
       UINT fFigures;  //  количество цифр
       UINT fBlocks;   // количество групп разбиени€
  // открытые методы (к элементам класса имеют доступ любые функции программы, имеющие ту же область действи€)
     public:
       ULINT(AnsiString NumS);      //конструктор (необходим дл€ инициализации, конструктор всегда имеет им€ класса и не имеет тип возвращаемого значени€)
       ULINT* operator +(ULINT* B); //сложение (перегружаема€ операци€ дл€ того, чтобы использовать ее дл€ трех различный типов  в скобках указан указатель на ¬)
       ULINT* operator -(ULINT* B); //вычитание
       ULINT* operator *(ULINT* B); //умножение
       ULINT* operator /(ULINT* B); //деление
       AnsiString GetNumString();   //вывод строки
       void PutNumString(AnsiString SNum);  //ввод строки
       ULINT* MinL(ULINT* A, ULINT* B);     //минимальный из двух элементов
       ULINT* MaxL(ULINT* A, ULINT* B);     //максимальный из двух элементов
};//ULINT

// класс целого числа со знаком
class LINT : public ULINT  // наследование от класса ULINT

{
   // защищенные пол€
     protected:
       AnsiString fSign;  // знак числа
   //
     public:
       void PutSign(AnsiString Sign);  // ввод знака числа
       AnsiString GetSign();           // вывод знака числа
       LINT(AnsiString NumS);          //конструктор
       LINT* operator +(LINT* B);       //сложение
       LINT* operator -(LINT* B);       //вычитание
       LINT* operator *(LINT* B);       //умножение
       LINT* operator /(LINT* B);       //деление
       AnsiString GetNumString();        //вывод числа
       void PutNumString(AnsiString SNum);   //ввод числа
       AnsiString GetString();                //вывод числа без знака
       LINT* MinL(LINT* A, LINT* B);          //минимальное число
       LINT* MaxL(LINT* A, LINT* B);          //максимальное число
};  //LINT

//дробно число со знаком
class LFLT : public LINT  // наследование от LINT

{
     // защищенные пол€
     protected:
       AnsiString fInt;  // цела€ часть числа
       AnsiString fFrac;  // дробна€ часть числа
    // открытые методы
     public:
       LFLT(AnsiString NumS);  // контруктор
       LFLT* operator +(LFLT* B);  // сложение
       LFLT* operator -(LFLT* B);  //вычитание
       LFLT* operator *(LFLT* B);  //умножение
       LFLT* operator /(LFLT* B);  // деление
       void PutNumString(AnsiString SNum);  // ввод числа
}; // LFLT


//---------------------------------------------------------------------------
#endif

Ћистинг фрагментов модул€, содержащего описание методов классов 

//   операци€ сложени€
ULINT* ULINT::operator+(ULINT* B)
 {
  AnsiString SBlockA,SBlockB,SSubSumBlock,Sum;
  UINT NBlockA, NBlockB, NSumBlock; //создание объектов класса UINT
  UINT R,k,k1,i; //создание объектов класса UINT
  int j;

  k= 9;
  R= 0;
  // обработка каждого блока числа
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
  }//конец for
  ULINT *C= new ULINT(Sum);
  return C;
 }// ULINT::operator+

//  операци€ вычитани€
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
 }// операци€ вычитани€

// операци€ умножени€
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

// операци€ делени€
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
