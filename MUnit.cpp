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
  // на всякий случай, сбрасываем калькулятор
  oCalculator.Reset();
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------


void __fastcall TMForm::OnResetClicked(TObject *Sender){
  // сбрасиываем калькулятор
  oCalculator.Reset();
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnBackspaceClicked(TObject *Sender){
  // затираем один правый символ в области ввода
  oCalculator.RemoveDigit();
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnClearClicked(TObject *Sender){
  // очищаем область ввода
  oCalculator.ClearInput();
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnAddDigit(TObject *Sender){
  // добавляем одну цифру (0...9) в область ввода
  // В каждом из закрепленных объектов, в поле Tag хранится добавляемое значение
  oCalculator.InputDigit('0' + (char)((TWinControl*)Sender)->Tag);
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnAdd3Zero(TObject *Sender){
  // добавляем три нуля справа
  oCalculator.InputDigit('0');
  oCalculator.InputDigit('0');
  oCalculator.InputDigit('0');
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnChangeFracState(TObject *Sender){
  // меняем область ввода
  oCalculator.Turn2Frac();
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnReturn(TObject *Sender){
  // производим вычисления, если неыло ошибок, то показываем результат
  if(oCalculator.MakeReturn())
    STValue->Caption = oCalculator.GetResult();
  else
    // если есть ошибки, пкоазываем ошибку
    STValue->Caption = "Error...";
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnOperation(TObject *Sender){
  // задаем операцию для вычисления и выводим результат
  if(oCalculator.MakeOperation((ECalcOperation)((TWinControl*)Sender)->Tag))
    STValue->Caption = oCalculator.GetResult();
  else
    // показываем ошибку, если операция не удалась
    STValue->Caption = "Error...";
  // показываем текущую область ввода
  STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
}
//---------------------------------------------------------------------------

void __fastcall TMForm::OnTurnSign(TObject *Sender){
  // меняем знак числа
  oCalculator.TurnSign();
  // выводим текущее значение ввода
  STValue->Caption = oCalculator.Print();
}
//---------------------------------------------------------------------------


void __fastcall TMForm::N7Click(TObject *Sender){
  TClipboard* oClipBoard = Clipboard();
  // копируем текущее выведенное значение в буфер обмена
  oClipBoard->SetTextBuf(STValue->Caption.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMForm::N8Click(TObject *Sender){
  // принимаемое из буфера значение
  char cClip[256] = {0};
  // бегунок по принятой строке
  char *pDigit = cClip;
  TClipboard* oClipBoard = Clipboard();

  // прием значения из буфера
  oClipBoard->GetTextBuf(cClip, 255);
  // проходим бегунком по принятой трочке
  while(*pDigit){
    // если хоть один символ будет неподходящим, обрываем операцию вставки
    if(
      (oCalculator.InFrac && ('.' == *pDigit))||
      ((0 > (int)(*pDigit - '0'))&&(9 < (int)(*pDigit - '0')))||
      (!strchr(".0123456789",*pDigit))
    ) break;
    // вставляем просто через псевдонажатие клавиши
    FormKeyPress(this,*pDigit);
    // смещаем бегунок далее
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
      // вводим цифру
      oCalculator.InputDigit(Key);
      // выводим текущее значение ввода
      STValue->Caption = oCalculator.Print();
    break;
    case '.':
      // меняем область ввода
      oCalculator.Turn2Frac();
      // выводим текущее значение ввода
      STValue->Caption = oCalculator.Print();
    break;
  };
}
//---------------------------------------------------------------------------

void __fastcall TMForm::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift){
  switch(Key){
    case VK_BACK: // клавиша Backspace
      // удаляем цифру
      oCalculator.RemoveDigit();
      // выводим текущее значение ввода
      STValue->Caption = oCalculator.Print();
      // показываем текущую область ввода
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_ADD: // '+'
      if(oCalculator.MakeOperation(coAdd))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // показываем текущую область ввода
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_SUBTRACT: // '-'
      if(oCalculator.MakeOperation(coSub))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // показываем текущую область ввода
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_MULTIPLY: // '*'
      if(oCalculator.MakeOperation(coMul))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // показываем текущую область ввода
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_DIVIDE: // '/'
      if(oCalculator.MakeOperation(coDiv))
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // показываем текущую область ввода
      STFIntFrac->Caption = (oCalculator.InFrac)? "FRAC":"INT";
    break;
    case VK_RETURN:
      if(oCalculator.MakeReturn())
        STValue->Caption = oCalculator.GetResult();
      else
        STValue->Caption = "Error...";
      // показываем текущую область ввода
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

