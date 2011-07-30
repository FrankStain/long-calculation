//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("LongCalc.res");
USEFORM("MUnit.cpp", MForm);
USEUNIT("LongCalculation.cpp");
USEFORM("AboutUnit.cpp", AboutForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TMForm), &MForm);
     Application->CreateForm(__classid(TAboutForm), &AboutForm);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
