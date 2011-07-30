//---------------------------------------------------------------------------

#ifndef MUnitH
#define MUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <vcl\Clipbrd.hpp>

#include "LongCalculation.h"
#include "AboutUnit.h"

//---------------------------------------------------------------------------
class TMForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N5;
  TMenuItem *N6;
  TMenuItem *N7;
  TMenuItem *N8;
  TStaticText *STValue;
  TGroupBox *GBNumbers;
  TButton *Button1;
  TButton *Button2;
  TButton *Button3;
  TButton *Button4;
  TButton *Button5;
  TButton *Button6;
  TButton *Button7;
  TButton *Button8;
  TButton *Button9;
  TButton *Button10;
  TButton *Button11;
  TButton *Button12;
  TGroupBox *GBOperators;
  TButton *Button13;
  TButton *Button14;
  TButton *Button17;
  TButton *Button18;
  TButton *Button20;
  TButton *Button21;
  TButton *Button24;
  TGroupBox *GroupBox1;
  TButton *Button15;
  TButton *Button16;
  TButton *Button19;
  TStaticText *STFIntFrac;
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall OnResetClicked(TObject *Sender);
  void __fastcall OnBackspaceClicked(TObject *Sender);
  void __fastcall OnClearClicked(TObject *Sender);
  void __fastcall OnAddDigit(TObject *Sender);
  void __fastcall OnAdd3Zero(TObject *Sender);
  void __fastcall OnChangeFracState(TObject *Sender);
  void __fastcall OnReturn(TObject *Sender);
  void __fastcall OnOperation(TObject *Sender);
  void __fastcall OnTurnSign(TObject *Sender);
  void __fastcall N7Click(TObject *Sender);
  void __fastcall N8Click(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall N6Click(TObject *Sender);
private:	// User declarations
  // Контроллер калькулятора, обеспечивает хранение чисел и все операции с ними
  CCalcController oCalculator;

public:		// User declarations
  __fastcall TMForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMForm *MForm;
//---------------------------------------------------------------------------
#endif
