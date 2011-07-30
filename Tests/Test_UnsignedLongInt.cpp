//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <conio.h>
#include "../LongCalculation.h"

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[]){
  longfloat lFirstArg;
  longfloat lSecondArg;
  longfloat lOpResult;
  char cPoped;

  lFirstArg.Value = "1312";
  lFirstArg.Sign = true;
  lSecondArg.Value = "184";
  lSecondArg.Sign = false;

  lFirstArg.Fraction = "9385";
  lSecondArg.Fraction = "852";

  printf("\n\n\tlFirstArg : %s\n\n",(AnsiString)lFirstArg);

  lFirstArg.PushDigit('5');
  printf("\tlFirstArg : %s\n\n",(AnsiString)lFirstArg);
  lFirstArg.PushDigit('8');
  printf("\tlFirstArg : %s\n\n",(AnsiString)lFirstArg);
  lFirstArg.PushDigit('2');
  printf("\tlFirstArg : %s\n\n",(AnsiString)lFirstArg);
  lFirstArg.PopDigit(cPoped);
  printf("\tlFirstArg : %s\tPoped : %c\n\n",(AnsiString)lFirstArg,cPoped);
  lFirstArg.PopDigit(cPoped);
  printf("\tlFirstArg : %s\tPoped : %c\n\n",(AnsiString)lFirstArg,cPoped);
  lFirstArg.PopDigit(cPoped);
  printf("\tlFirstArg : %s\tPoped : %c\n\n\n",(AnsiString)lFirstArg,cPoped);

  lFirstArg.Value = "1312";
  lFirstArg.Sign = true;
  lSecondArg.Value = "184";
  lSecondArg.Sign = false;
  lOpResult = lFirstArg + lSecondArg;
  printf("  %s + %s == %s\t",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lSecondArg.Sign = true;
  lOpResult = lFirstArg + lSecondArg;
  printf(" %s + %s == %s\t\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lFirstArg.Sign = false;
  lOpResult = lFirstArg + lSecondArg;
  printf(" %s + %s == %s\t",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lSecondArg.Sign = false;
  lOpResult = lFirstArg + lSecondArg;
  printf(" %s + %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  
  lOpResult = lFirstArg - lSecondArg;
  printf("  %s - %s == %s\t",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lFirstArg.Sign = true;
  lOpResult = lFirstArg - lSecondArg;
  printf("  %s - %s == %s\t\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lSecondArg.Sign = true;
  lOpResult = lFirstArg - lSecondArg;
  printf("  %s - %s == %s\t",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lFirstArg.Sign = false;
  lOpResult = lFirstArg - lSecondArg;
  printf("  %s - %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  try{
    lOpResult = lSecondArg - lFirstArg;
    printf("\t%s - %s == %s\n\n",(AnsiString)lSecondArg,(AnsiString)lFirstArg,(AnsiString)lOpResult);
  }catch(bool bFlag){
    if(!bFlag) printf("\tUnsigned buffer overflow catched in : %s - %s\n\n",(AnsiString)lSecondArg,(AnsiString)lFirstArg);
  };

  
  lOpResult = lFirstArg * lSecondArg;
  printf("\t%s * %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  
  lOpResult = lFirstArg / lSecondArg;
  printf("\t%s / %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lFirstArg.Sign = false;

  lFirstArg.Value = "32";
  lSecondArg.Value = "2";
  lOpResult = lFirstArg / lSecondArg;
  printf("\t%s / %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lFirstArg.Value = "128";
  lSecondArg.Value = "4";
  lOpResult = lFirstArg / lSecondArg;
  printf("\t%s / %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  lFirstArg.Value = "15";
  lSecondArg.Value = "7";
  lOpResult = lFirstArg / lSecondArg;
  printf("\t%s / %s == %s\n\n",(AnsiString)lFirstArg,(AnsiString)lSecondArg,(AnsiString)lOpResult);

  //*/

  getch();
  return 0;
}
//---------------------------------------------------------------------------
