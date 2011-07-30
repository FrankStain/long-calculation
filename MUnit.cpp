//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMForm *MForm;
//---------------------------------------------------------------------------
__fastcall TMForm::TMForm(TComponent* Owner): TForm(Owner) {

}
//---------------------------------------------------------------------------
void __fastcall TMForm::FormCreate(TObject *Sender){
  // �� ������ ������, ���������� �����������
  oCalculator.Reset();
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------


void __fastcall TMForm::OnResetClicked(TObject *Sender){
  // ����������� �����������
  oCalculator.Reset();
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnBackspaceClicked(TObject *Sender){
  // �������� ���� ������ ������ � ������� �����
  oCalculator.RemoveDigit();
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnClearClicked(TObject *Sender){
  // ������� ������� �����
  oCalculator.ClearInput();
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnAddDigit(TObject *Sender){
  // ��������� ���� ����� (0...9) � ������� �����
  // � ������ �� ������������ ��������, � ���� Tag �������� ����������� ��������
  oCalculator.InputDigit('0' + (char)((TWinControl*)Sender)->Tag);
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnAdd3Zero(TObject *Sender){
  // ��������� ��� ���� ������
  oCalculator.InputDigit('0');
  oCalculator.InputDigit('0');
  oCalculator.InputDigit('0');
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnChangeFracState(TObject *Sender){
  // ������ ������� �����
  oCalculator.Turn2Frac();
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnReturn(TObject *Sender){
  // ���������� ����������, ���� ����� ������, �� ���������� ���������
  if(oCalculator.MakeReturn())
    STValue->Caption = oCalculator.GetResult();
  else
    // ���� ���� ������, ���������� ������
    STValue->Caption = "Error...";
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnOperation(TObject *Sender){
  // ������ �������� ��� ���������� � ������� ���������
  if(oCalculator.MakeOperation((ECalcOperation)((TWinControl*)Sender)->Tag))
    STValue->Caption = oCalculator.GetResult();
  else
    // ���������� ������, ���� �������� �� �������
    STValue->Caption = "Error...";
  // ���������� ������� ������� �����
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnTurnSign(TObject *Sender){
  // ������ ���� �����
  oCalculator.TurnSign();
  // ������� ������� �������� �����
  STValue->Caption = oCalculator.Print();
}
//---------------------------------------------------------------------------


void __fastcall TMForm::N7Click(TObject *Sender){
  TClipboard* oClipBoard = Clipboard();
  // �������� ������� ���������� �������� � ����� ������
  oClipBoard->SetTextBuf(STValue->Caption.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMForm::N8Click(TObject *Sender){
  // ����������� �� ������ ��������
  char cClip[256] = {0};
  // ������� �� �������� ������
  char *pDigit = cClip;
  TClipboard* oClipBoard = Clipboard();

  // ����� �������� �� ������
  oClipBoard->GetTextBuf(cClip, 255);
  // �������� �������� �� �������� ������
  while(*pDigit){
    // ���� ���� ���� ������ ����� ������������, �������� �������� �������
    if(
      (oCalculator.InFrac && ('.' == *pDigit))||
      ((0 > (int)(*pDigit - '0'))&&(9 < (int)(*pDigit - '0')))||
      (!strchr(".0123456789",*pDigit))
    ) break;
    // ��������� ������ ����� ������������� �������
    FormKeyPress(this,*pDigit);
    // ������� ������� �����
    pDigit++;
  };
}
//---------------------------------------------------------------------------

void __fastcall TMForm::FormKeyPress(TObject *Sender, char &Key){
  switch(Key){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      // ������ �����
      oCalculator.InputDigit(Key);
      // ������� ������� �������� �����
      STValue->Caption = oCalculator.Print();
    break;
    case '.':
      // ������ ������� �����
      oCalculator.Turn2Frac();
      // ������� ������� �������� �����
      STValue->Caption = oCalculator.Print();
    break;
  };
}
//---------------------------------------------------------------------------

void __fastcall TMForm::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift){
  switch(Key){
    case VK_BACK: // ������� Backspace
      // ������� �����
      oCalculator.RemoveDigit();
      // ������� ������� �������� �����
      STValue->Caption = oCalculator.Print();
      // ���������� ������� ������� �����
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_ADD: // '+'
      if(oCalculator.MakeOperation(coAdd))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // ���������� ������� ������� �����
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_SUBTRACT: // '-'
      if(oCalculator.MakeOperation(coSub))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // ���������� ������� ������� �����
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_MULTIPLY: // '*'
      if(oCalculator.MakeOperation(coMul))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // ���������� ������� ������� �����
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_DIVIDE: // '/'
      if(oCalculator.MakeOperation(coDiv))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // ���������� ������� ������� �����
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_RETURN:
      if(oCalculator.MakeReturn())
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // ���������� ������� ������� �����
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
      Key = 0;
    break;
  };
}
//---------------------------------------------------------------------------


void __fastcall TMForm::N6Click(TObject *Sender){
  AboutForm->ShowModal();
}
//---------------------------------------------------------------------------

