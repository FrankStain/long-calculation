//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LongCalculation.h"
//---------------------------------------------------------------------------
//--- CLongUnsignedInteger --------------------------------------------------
CLongUnsignedInteger::CLongUnsignedInteger(): FValue("0") {
};

CLongUnsignedInteger::CLongUnsignedInteger(CLongUnsignedInteger& oValue) {
  SetValue(oValue.Value);
};

CLongUnsignedInteger& CLongUnsignedInteger::operator =(CLongUnsignedInteger oValue){
  SetValue(oValue.Value);
  Normalize();
  return *this;
};

/**
 * �������� �������� ���� ����� �����.
 *
 * ������:
 *   ��� �������� ���������� ��������� ����� �� ������� ����, � ����� ���������� ������� ��.
 *   ����� ���������� � ����� ��������, ����� ��� �������� ����� ������ ������� ��������� ����� ������ 9
 *   � ���� ������ � ��� ������ ���������� �������� ������� �� ������� ����� �� 10, � ���� �����
 *   ��������� �� 10 � ������� � ������ ����� ���������� �������.
 *
 * ����������:
 *   1- � ���������� ������� ������� ���� �������� ���������������, �� ������� 0 � ������� 9.
 *      � ���� ����� ����� ����� ������ �������� �������� ����������� �������� ����� �� �������
 *      � �������� ����. (������: '9' - '0' == 9; �.�. '9' == 0x39, � '0' == 0x30)
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator +(CLongUnsignedInteger oValue){
  // ��������� ��������.
  CLongUnsignedInteger oResult;
  // ������� ������� ������� ������ ��������
  int iLeftPos  = FValue.Length();
  // ������� ������� ������� ������� ��������
  int iRightPos = oValue.Value.Length();
  // ����� �������� � ����� ������������ � ����� ����
  int iOFlag    = 0;

  oResult.Value = "";
  // � �������� ����������� ����� ��������� ���������� ����������� ��������
  while(iLeftPos && iRightPos){
    int iLeft  = FValue[iLeftPos--] - '0';
    int iRight = oValue.Value[iRightPos--] - '0';

    // ��������� �������� �������� ������� � ���������� ��� � ���������
    iOFlag += iLeft + iRight;
    oResult.PushDigit('0' + (char)(iOFlag % 10));
    // ������������ �������� � ����������� �������������� ��������
    iOFlag /= 10;
  };

  // �������� ������ ���������� ���������� ������� �������, ���� ���� �� ��������� ������� �������.
  // ������� ������ ���� ��� �� ���������� ����� ����� ������������.
  if(iLeftPos){
    while(iLeftPos){
      iOFlag += (FValue[iLeftPos--] - '0');
      oResult.PushDigit('0' + (char)(iOFlag % 10));
      iOFlag /= 10;
    };
  }else if(iRightPos){
    while(iRightPos){
      iOFlag += (oValue.Value[iRightPos--] - '0');
      oResult.PushDigit('0' + (char)(iOFlag % 10));
      iOFlag /= 10;
    };
  };
  // ���������� ������� � ������, ���� �� ����
  if(iOFlag) oResult.PushDigit('0' + (char)iOFlag);

  // ��������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ��������� ������ ������ �� �������.
 *
 * ������:
 *   �������� ���������� ��������, �� ����������� ������ �����������.
 *   ���� ������ ������� ������ ������, ��������� ��������� ����� �������������, ��� ����������� � ������ ������.
 *
 * ����������:
 *   1- ���� ������ ������� ������ �������, ������������ ����������
 *   2- ��� �������� ������� �������� ������� �������� �������� �������������� �������
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator -(CLongUnsignedInteger oValue){
  // ��������� ��������
  CLongUnsignedInteger oResult;

  // ���� ������ ������� ������ ������, ���������� ���������� [catch(...)/catch(bool)]
  if(*this < oValue) throw false;

  // ������� �������� ������� ������ ��������
  int iLeftPos   = FValue.Length();
  // �������� �������� ������� ������� ��������
  int iRightPos  = oValue.Value.Length();
  // ����� �������� � ����� ������������ � ����� ����
  int iOFlag;
  // ���� ������������ ��������, ����� ��� �������� ������������� �� �������� �������
  bool bOverflow = false;

  oResult.Value = "";
  // � �������� ����������� ����� ��������� ���������� ����������� ���������
  while(iLeftPos && iRightPos){
    // ������� �������� �������� ������� ������ ��������,
    // ������������ ��� ��������� ����� ������������
    int iLeft  = (FValue[iLeftPos--] - '0') - (int)bOverflow;
    // ������� �������� �������� ������� ������� ��������
    int iRight = oValue.Value[iRightPos--] - '0';
    // ���������� ���� ������������
    bOverflow  = false;

    // ��������� �������� � ������� �������
    iOFlag = iLeft - iRight;
    // ���� ��������� ������ ����, ���������� ������������� �� �������� �������
    if(0 > iOFlag){
      // ������� ������������ � ������� �������
      bOverflow = true;
      // ������������ ��������� ������� ��������
      iOFlag += 10;
    };
    // ���������� �������� ������ � ������ ��������� ��������
    oResult.PushDigit('0' + (char)(iOFlag % 10));
  };

  // ������ ������� �������� ������ ������
  // �������� ������ ���������� �������, ��� ����������, ������� �� ������ ��������
  if(iLeftPos){
    while(0 < iLeftPos){
      iOFlag = (FValue[iLeftPos--] - '0') - (int)bOverflow;
      bOverflow = false;
      if(0 > iOFlag){
        bOverflow = true;
        iOFlag += 10;
      };
      oResult.PushDigit('0' + (char)(iOFlag % 10));
    };
  };

  // ��������� ����������������, ���� ��� ���� ���� ������� ������������,
  // ������ ������ ������� ��� ���� ������ � ���������� �������������� ����������
  if(bOverflow) throw false;

  // ��������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ������������ ���� ����� �����.
 *
 * ������:
 *   ��� ������������ ���� ����� ���������� ������ ������ ������� �������� �������� �� ����� �������,
 *   �������� ����� ����� � ����� �� �������� �������� ��������� ���������.
 *   ����� �������� �������� ������� ����� ��� ����, � ����������� ��������� ����������� ���������� ��������.
 *
 *   �� ���� ��, ������ ��������� ������������ ��������� ������������ � ����������� ��������.
 *   ��� ��������� �������� ���������� ����� ����. ������� �������� �������� ������������ ������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� �������������� �������.
 *   2- �������� ����� M*N ������������, ��� ������ ��������, ��� ������ �����������.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator *(CLongUnsignedInteger oValue){
  // ��������� ����������
  CLongUnsignedInteger oResult;
  // ����� ������������ ���������
  CLongUnsignedInteger oMulBuffer;
  // ������� �������� ������� ������ ��������
  int iLeftPos   = FValue.Length();
  // ������� �������� ������� ������� ��������
  int iRightPos  = oValue.Value.Length();
  // ����� ����������� �������� � ����� ������������ � ����� ����
  int iOFlag;

  oResult.Value = "0";
  // ������ ������ ������� �������� ���� �������� �� ����� �������
  while(iRightPos){
    // ������� �������� � ������� ������� ������� ��������
    int iRight = oValue.Value[iRightPos--] - '0';
    // ���������� �������� ������ ��������
    iOFlag     = 0;

    // ���� ������ �������, ����� �� ������ ������ �� ��� � ������ ������� � ���������� �������
    if(!iRight) continue;

    oMulBuffer.Value = "";
    // ��������� ����� ��������� ����������� ������ �����, �������� ������� ������� ������� ��������
    for(int iOffset = (oValue.Value.Length() - iRightPos - 1); 0 < iOffset; iOffset--) oMulBuffer.AddDigit('0');
    // ���������� ����������� ���������
    for(int iLeftTrack = iLeftPos; 0 < iLeftTrack; iLeftTrack--){
      int iLeft  = FValue[iLeftTrack] - '0';

      // ���������� ��������� ��� ������� �������� � ���������� ��������� � �����
      iOFlag += iLeft * iRight;
      oMulBuffer.PushDigit('0' + (char)(iOFlag % 10));
      // ������������ �������� � ����������� �������������� ��������
      iOFlag /= 10;
    };

    // ���� ���� ������������ ��������, ���������� ��� � �����
    if(iOFlag) oMulBuffer.PushDigit('0' + (char)(iOFlag % 10));

    // ���������� �������� ������ � ����������� ��������
    oResult = oResult + oMulBuffer;
  };

  // ��������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ������� ���� ����� �����.
 *
 * ������:
 *   ������� ����� ������������� ������� ����������� ����� ��������� ����� � ���������.
 *   � ����� �� �������� �������� ������� ���������� �� ��� ��, �� � ��������� ���������.
 *
 * ������ (���������� ��������� �������� ������� ����� ����):
 *   - ���� ����� ������� ������ �������, ������������� ������� � ���������� ��������� ��������.
 *   - ����������� ����� �������� ������� ��������, ���� ��� �� ������ ������ ������.
 *   - ��������� ����� �������� ������� �������� �� 1. ���������� ������� � �������� ��������.
 *   - �������� ������ ������� �� ������, ��������� ������ � ����� �������.
 *   - ���������� � ���������� �������� �������� �������� ��������.
 *   - ��������������� ����������� �������� ������� ��������.
 *   - ������������ � ������� ������ �������.
 *
 * ����������:
 *   1- X / 0 -- ������������ ����������.
 *   2- ���� ����� ������� ���������� ������ �������, �������� ������ �� ������������.
 *      ������������ ����������� ������� ������� ������������ ���������� ("0").
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator /(CLongUnsignedInteger oValue){
  // ��������� ��������
  CLongUnsignedInteger oResult;
  // ������� ��������
  CLongUnsignedInteger oMulBuffer;
  // ����� ������� �������� ��� ������� � ����������� �������
  CLongUnsignedInteger oDivBuffer;
  // ����� ������� ��� ��������� � ������� ����������
  CLongUnsignedInteger oLeft((CLongUnsignedInteger&)*this);

  oResult.Value = "0";
  // ���� ������������ ������� �� ����, �� ������������ ����������
  if(oResult == oValue) throw false;

  // ���������, ������ �� ����� ������� �������
  while(oLeft > oValue){
    // ����� ��� ������ ������ �������� �������.
    // ������������ - ���� ������� ������ � ������ ��������
    char cDigit;

    // ��������������� ����������� �������� ������� ��������, ������� �������� - � 1, ���� ������ �������
    oDivBuffer = (CLongUnsignedInteger&)oValue;
    oMulBuffer.Value = "1";
    cDigit = 0;
    // ���������� ����� ��������, ���������� ����� �������, ���� ������ ������� ������ ������
    while(oDivBuffer < oLeft){
      oDivBuffer.AddDigit('0');
      oMulBuffer.AddDigit('0');
      cDigit = 1;
    };
    // [��� ������������] ���������, ���� �� ������ � ������ ��������
    // ���� ������� ������ � ������ ������� ������ ������, ������������� �������
    if(!cDigit && (oDivBuffer > oLeft)) break;
    // ��������� ����� �������� ������� �������� � �������� �������� �� 1
    oDivBuffer.RemoveDigit(cDigit);
    oMulBuffer.RemoveDigit(cDigit);
    // ���������� ��������� ��������� � ������������ ����������� �������
    while(oLeft >= oDivBuffer){
      oLeft = oLeft - oDivBuffer;
      oResult = oResult + oMulBuffer;
    };
  };
  // [��� ������������] ���� ������ ������� ��� ��� ������ ������, ��������� ��������� �������� �� 1
  //if(oDivBuffer > *this) oResult--;

  // ��������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * ����������� ���������. [ a++ ]
 *
 * ������:
 *   ���������� ��������� ������� �������� �����, ���������� ���������
 *   � ����������� ������� ���������� ��������.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator ++(int iType){
  // ������ �������, ��� ����������
  CLongUnsignedInteger oIncr;
  // ��������� ��������
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // ���� �������� ������������ ����, ������ ������� ����������� ����� ���������.
  if(!iType){
    // ���������� ������� ��������
    oResult = *this;
    // ����������� ����������� �������� �� 1
    *this = *this + oIncr;
  }else{
    oResult = *this = *this + oIncr;
  };

  // ��������� ������������
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ����������� ���������. [ a-- ]
 *
 * ������:
 *   ���������� ��������� ������� �������� �����, ���������� ���������
 *   � ����������� ������� ���������� ��������.
 *
 * ����������:
 *   1- "0"-- ������� ����������. ������� �������� �� ������������.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator --(int iType){
  // ������ �������, ��� ����������
  CLongUnsignedInteger oIncr;
  // ��������� ��������
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // ���� �������� ������������ ����, ������ ������� ����������� ����� ���������.
  if(!iType){
    // ���������� ������� ��������
    oResult = *this;
    // ��������� ����������� �������� �� 1
    *this = *this - oIncr;
  }else{
    oResult = *this = *this - oIncr;
  };

  // ��������� ������������
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ���������� ���������. [ ++a ]
 *
 * ������:
 *   �������� ������ ���������� � ������������ � ��� ���������� ����.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator ++(){
  // ������ �������, ��� ����������
  CLongUnsignedInteger oIncr;
  // ��������� ��������
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // ��������� � ����������� ������������
  oResult = *this = *this + oIncr;

  // ���������� ������������
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ���������� ���������. [ --a ]
 *
 * ������:
 *   �������� ������ ���������� � ������������ � ��� ���������� ����.
 *
 * ����������:
 *   1- --"0" ������� ����������. ������� �������� �� ������������.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator --(){
  // ������ �������, ��� ����������
  CLongUnsignedInteger oIncr;
  // ��������� ��������
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // ��������� � ����������� ������������
  oResult = *this = *this - oIncr;

  // ���������� ������������
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ���������� ��������� ���������������.
 *
 * ������:
 *   ���������� ������ �������� ��� ����� ��� ��� ������, � ���� ������ ��������� ���� ����������� �����������.
 *
 */
bool CLongUnsignedInteger::operator ==(CLongUnsignedInteger& oValue){
  return 0 == strcmp(FValue.c_str(), oValue.Value.c_str());
};

/**
 * ���������� ��������� �������������.
 *
 * ������:
 *   ��� ������� ����� ������ ������������� � ���������� ������������ �������� �������� ���������.
 *   ��� ���� ��������� �������� ������������ ������ ������ �� ���� ���������,
 *   � ����� - �������� �������� ����� ������������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� ��������������� ��������� ��������.
 *
 */
bool CLongUnsignedInteger::operator >(CLongUnsignedInteger& oValue){
  // ������� �������� ������� ������ ��������
  int iLeftPos   = FValue.Length();
  // ������� �������� ������� ������� ��������
  int iRightPos  = oValue.Value.Length();
  // ���� ������������, true - �������� ������� ������������
  bool bOverflow = false;

  // ���������� ����� ������ ������ �� ������ �����.
  // ������ �������� �������� � �������� ��������� ���������
  if(iLeftPos > iRightPos) return true;
  if((iLeftPos < iRightPos)||(*this == oValue)) return false;
  // ���������� ����������� ��������� � ���������� ��������� ������������ ��������.
  while(iLeftPos && iRightPos){
    int iResult = (int)(FValue[iLeftPos--] - '0') - (int)(oValue.Value[iRightPos--] - '0') - (int)bOverflow;
    bOverflow = 0 > iResult;
  };

  // ���� ������������ ����, ������ ������ ������� ������ ������, �������� ������ ������� true.
  // ����� ���������� ������� false.
  return !bOverflow;
};

/**
 * ���������� ��������� ����������.
 *
 * ������:
 *   ��� ������� ����� ������ ������������� � ���������� ������������ �������� �������� ���������.
 *   ��� ���� ��������� �������� ������������ ������ ������ �� ���� ���������,
 *   � ����� - �������� �������� ����� ������������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� ��������������� ��������� ��������.
 *
 */
bool CLongUnsignedInteger::operator <(CLongUnsignedInteger& oValue){
  // ������� �������� ������� ������ ��������
  int iLeftPos   = FValue.Length();
  // ������� �������� ������� ������� ��������
  int iRightPos  = oValue.Value.Length();
  // ���� ������������, true - �������� ������� ������������
  bool bOverflow = false;

  // ���������� ����� ������ ������ �� ������ �����.
  // ������ �������� �������� � �������� ��������� ���������
  if(iLeftPos < iRightPos) return true;
  if((iLeftPos > iRightPos)||(*this == oValue)) return false;
  // ���������� ����������� ��������� � ���������� ��������� ������������ ��������.
  while(iLeftPos && iRightPos){
    int iResult = (int)(FValue[iLeftPos--] - '0') - (int)(oValue.Value[iRightPos--] - '0') - (int)bOverflow;
    bOverflow = 0 > iResult;
  };

  // ���� ������������ ����, ������ ����� ������� ������ �������, �������� ������ ������� true.
  // ����� ���������� ������� false.
  return bOverflow;
};

/**
 * ������������ �����.
 *
 * ������:
 *   �� ����� ������ ���� ������� ��� ���� �����. �� �����, ����� ������� ��� ��� ����� ���������.
 *
 */
void CLongUnsignedInteger::Normalize(){
  while((1 < FValue.Length())&&('0' == FValue[1])) FValue.Delete(1,1);
};

/**
 * ������ ���������� �������� �����.
 *
 */
AnsiString CLongUnsignedInteger::GetValue(){
  return FValue;
};

/**
 * ��������� ������ �������� �����.
 *
 */
AnsiString CLongUnsignedInteger::SetValue(const AnsiString sValue){
  FValue = sValue;
  Normalize();
  return FValue;
};

/**
 * ���������� ������ ������� �����
 *
 * ������:
 *   ������ ������ ������ �������� ������� ������ �������� ������� �����.
 *   ���������� ������ �������� ������ ������ � �������� �� ����� ������ � ������ ������.
 *   ��� �������� ������������ ���������� �++
 *
 */
void CLongUnsignedInteger::PushDigit(char cValue){
  AnsiString sOpValue(cValue);
  FValue.Insert(sOpValue,1);
};

/**
 * ������� ������ ������� �����
 *
 * ������:
 *   �� ������ ��������� ������ ������ � ������������ ����� ������ � �����.
 *   ��� �������� ������������ ���������� �++
 *
 */
void CLongUnsignedInteger::PopDigit(char& cValue){
  if(FValue == "0"){
    cValue = 0;
    return;
  };

  cValue = FValue[1];
  FValue.Delete(1,1);
  if(!FValue.Length()) FValue = "0";
};

/**
 * ���������� ������ ������� ������
 *
 * ������:
 *   ��������� ������ ������ - ����� ������� ������, ����������� � ������ ������.
 *   ��� �������� ������������ ���������� �++
 *
 */
void CLongUnsignedInteger::AddDigit(char cValue){
  AnsiString sOpValue(cValue);
  if((FValue == "0") && (cValue != '0')) FValue = "";
  FValue.Insert(sOpValue,FValue.Length()+1);
};

/**
 * ������� ������ ������� ������
 *
 * ������:
 *   �� ������ ��������� ����� ��������� ������, ������ ���������� �� 1 ������.
 *   ��� �������� ������������ ���������� �++
 *
 */
void CLongUnsignedInteger::RemoveDigit(char& cValue){
  if(FValue == "0"){
    cValue = 0;
    return;
  };

  cValue = FValue[FValue.Length()];
  FValue.Delete(FValue.Length(),1);
  if(!FValue.Length()) FValue = "0";
};

/**
 * ���������������� ����� �������� �����.
 *
 * ������:
 *   ����� ����������� ����� �� �������� ��������������� ����������,
 *   ���������������� � ���������� �������� �� ��� �� ����������.
 *
 */
AnsiString CLongUnsignedInteger::PrintText(){
  Normalize();
  return FValue;
};

//---------------------------------------------------------------------------
//---- CLongSignedInteger ---------------------------------------------------
CLongSignedInteger::CLongSignedInteger(): FSignFlag(false) {

};

/**
 * ����������� �����������.
 *
 * ������:
 *   ����������� ����������� ������������ ������ ������ ��������� �� �� ��������,
 *   ��� � ����������� ����������� �������� ������, ��� ����� ���������� �������
 *   �������� �������������� �����. ��������� ������� ������������ ����������� ����� �����.
 *
 *   ...er& oValue): CLongUnsignedInteger(oValue),...
 *
 *   ������ ������ ��������� ���������� ������ ����������� ��� ������������� ������ ��������
 *   ������ ������ ������� ������������ ������.
 *
 */
CLongSignedInteger::CLongSignedInteger(CLongSignedInteger& oValue): CLongUnsignedInteger(oValue), FSignFlag(oValue.Sign) {

};

/**
 * ����������� ����� ����������������� ������������� �����.
 *
 * ������:
 *   ������ ����� ������ ����� �� ������, ������������� ����� ������������ ��� �����
 *   ������������� ����� ������������ �� ������ "�����".
 *   �������, ��� ���� FSignFlag ����������� � �������� true, ������ ����� ������������.
 *   ������������� ���� ������ ���������� ����������� ������ ����� FSignFlag == true.
 *
 * ����������:
 *   1- ����������� ������������ ���������� �++
 */
AnsiString CLongSignedInteger::PrintText(){
  AnsiString sResult;

  sResult.printf("%s%s",((FSignFlag)? "-":""),GetValue());

  return sResult;
};

/**
 * �������� ������������.
 *
 * ������:
 *   �������� - ��� ������ �������, this - ��� ����� �������.
 *   ��� ����������� ������������ ������������, �������� ������
 *   ���������� ������ �� ��������, � ��������� - ���������� *this.
 *
 *   CLongUnsignedInteger::operator =((CLongUnsignedInteger&)oValue);
 *
 *   ��� ����������� ������������� � ������������ ������� ������������ �����
 *   ���������������� ��������� �� ������������ ���� ������������� ������.
 *
 */
CLongSignedInteger& CLongSignedInteger::operator =(CLongSignedInteger oValue){
  CLongUnsignedInteger::operator =((CLongUnsignedInteger&)oValue);
  FSignFlag = oValue.Sign;
  Normalize();
  return *this;
};

/**
 * �������� ���� �������� �����.
 *
 * ������:
 *   ������ ����� �������� ������ � �������� �������� �����������. ����� ���������� ��������,
 *   ���� �� ���� ��������� ����� �� ����������, � ��������.
 *
 * �������� (��������� �� ������, ���� ����� ����������� ��������):
 *   - ���� ��� ������������, ����������� �������� ���� ������� �����������, ���� ���������� �����������
 *   - ���� ��� ������������, ����������� �������� ���� ������� �����������, ���� ���������� �����������
 *   - ���� ������������ ������ ���� �� �����, ��������� ������� �������� ����� �� ��������,
 *     ���� ���������� ��� � ��������.
 *
 * ����������:
 *   1- ���������� ��������� ������������ �� ��� ������������� ����������� ����������� �����
 *   2- ��� �������� ������� �������� ������� �������� �������� ��������������� ��������� ��������.
 */
CLongSignedInteger CLongSignedInteger::operator +(CLongSignedInteger oValue){
  // ��������� ��������
  CLongSignedInteger oResult;
  // ����� � ������ ��������, ������������� �������� ������
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // ���� ������ ���� �� ��������� �����������, ��������� ������� ������
  if((!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign)){
    // �������� ������������, ����� ���� �� ����� ����� ����������� �������������
    // ���� ����� ������� �����������
    if(FSignFlag){
      // ���� ����� ��� � ������� �����, ���� ���������� ����� �������������
      oResult.Sign = oLeft > oRight;
      // ���������� ������ ������� �� ��������, ������� ����� ������,
      // ������������ �� ����� ����������
      if(!oResult.Sign) (CLongUnsignedInteger&)oResult = oRight - oLeft;
      else (CLongUnsignedInteger&)oResult = oLeft - oRight;
    }else{
      // ����� ������������, ������ ������ ����� ������������
      // � ���� ������ ����� ����� ������, ��������� ����� �������������.
      oResult.Sign = oLeft < oRight;
      // ���������� ������ ������� �� ��������, ������� ����� ������,
      // ������������ �� ����� ����������
      if(!oResult.Sign) (CLongUnsignedInteger&)oResult = oLeft - oRight;
      else (CLongUnsignedInteger&)oResult = oRight - oLeft;
    };
  }else{
    // ������ ������� �������� � ������������� ���� ����� ���������
    (CLongUnsignedInteger&)oResult = oLeft + oRight;
    oResult.Sign = FSignFlag || oValue.Sign;
  };

  // ����������� ���������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * ��������� ���� �������� �����.
 *
 * ������:
 *   � ������ �� ��������� �������, �������� ��������� ��� �� ���������� ����������. ����������
 *   ������, ����� ��������� ���������� ���������.
 *
 * ����������:
 *   1- ���������� ��������� ������������ �� ��� ������������� ����������� ����������� �����.
 *   2- ��� �������� ������� �������� ������� �������� �������� ��������������� ��������� ��������.
 *   3- ��������� ������������� ��������� ���������� ������������.
 *
 */
CLongSignedInteger CLongSignedInteger::operator -(CLongSignedInteger oValue){
  // ��������� ��������
  CLongSignedInteger oResult;
  // ����� � ������ ��������, ������������� �������� ������
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  if(oValue.Sign){
    // ���� ������ ������� �����������,
    // ���������� �������� � - -�, ������� ������������� � � + �
    CLongSignedInteger oModule(oValue);
    oModule.Sign = false;
    oResult = *this + oModule;
  }else{
    // ������ �������������, ���� ��������� ���� ������
    if(FSignFlag){
      // ���� ����� �������������, ������ ������������ �������� � ������������� ���������
      (CLongUnsignedInteger&)oResult = oLeft + oRight;
      oResult.Sign = true;
    }else{
      // ���� ������ ����� ����� ������, ��������� �������� ����� �������������.
      oResult.Sign = oLeft < oRight;
      // ���������� ������ ������� �� ��������, ������� ����� ������,
      // ������������ �� ����� ����������
      if(!oResult.Sign) (CLongUnsignedInteger&)oResult = oLeft - oRight;
      else (CLongUnsignedInteger&)oResult = oRight - oLeft;
    };
  };

  // ���������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ��������� �������� �����.
 *
 * ������:
 *   ����������� �������� �� ��� �� ���������� �� ������������ ����������� �����.
 *   �������� ������� ������������ ����� �� ������ ������� � ���, ��� ���� ����������
 *   ���������� ������������� ������ �����, ����� ����������� ��� ���� �������.
 *
 * ����������:
 *   1- ���������� ��������� ������������ �� ��� ������������� ����������� ����������� �����.
 *   2- ���� ���������� ������������ �� ������ XOR, ���� ����� ������������� ��� �����������.
 *
 *   ������������ ������ �������� XOR:   XOR | 0 | 1 |
 *                                       ----+---+---+
 *                                         0 | 0 | 1 |
 *                                       ----+---+---+
 *                                         1 | 1 | 0 |
 *
 */
CLongSignedInteger CLongSignedInteger::operator *(CLongSignedInteger oValue){
  // ��������� ��������
  CLongSignedInteger oResult;
  // ����� � ������ ��������, ������������� �������� ������
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // ����������� ������������ �������
  (CLongUnsignedInteger&)oResult = oLeft * oRight;
  // ������������ ���� ���������� ��������
  oResult.Sign = (!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign);

  // ����������� ���������� ������������, ���������� ���� �����
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ������� �������� �����.
 *
 * ������:
 *   ���������� ��������� ������������ ��������, ������� �������� ��� �� �� ���
 *   �� ���������� �� ������� �����������.
 *
 * ����������:
 *   1- ���������� ��������� ������������ �� ��� ������������� ����������� ����������� �����.
 *   2- ���� ���������� ������������ �� ������ XOR, ���� ����� ������������� ��� �����������.
 *   3- ��� �������� ������� �������� ������� �������� �������� �������������� ����������.
 *
 */
CLongSignedInteger CLongSignedInteger::operator /(CLongSignedInteger oValue){
  // ��������� �������
  CLongSignedInteger oResult;
  // ����� � ������ ��������, ������������� �������� ������
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // ����������� ������� �������
  (CLongUnsignedInteger&)oResult = oLeft / oRight;
  // ������������ ���� ���������� ��������
  oResult.Sign = (!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign);

  // ����������� ���������� ������������, ���������� ���� �����
  oResult.Normalize();
  return oResult;
};

/**
 * ����������� ���������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� �������������� ����������.
 *
 */
CLongSignedInteger CLongSignedInteger::operator ++(int iType){
  // ��������� �������
  CLongSignedInteger oResult;
  // ����� ����������, �������� ��� ��������
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // ����������� �������� ���������� � ����������, ������ ����
  switch(iType){
    case 0:
      // ���������� ������� ��������
      oResult = *this;
      // ���������� ���������
      *this = *this + oIncr;
    break;
  };

  // ���������� ������������ ����� �����, ���������� ���� �����
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ����������� ���������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� �������������� ����������.
 *
 */
CLongSignedInteger CLongSignedInteger::operator --(int iType){
  // ��������� �������
  CLongSignedInteger oResult;
  // ����� ����������, �������� ��� ���������
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // ����������� �������� ���������� � ����������, ������ ����
  switch(iType){
    case 0:
      // ���������� ������� ��������
      oResult = *this;
      // ���������� ���������
      *this = *this - oIncr;
    break;
  };

  // ��� ������� ��� ����� ���������? - ��������������... :)
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ���������� ���������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� �������������� ����������.
 *
 */
CLongSignedInteger CLongSignedInteger::operator ++(){
  // ��������� �������
  CLongSignedInteger oResult;
  // ����� ����������, �������� ��� ��������
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // ����������� ������������, ���������� ����������
  oResult = *this = *this + oIncr;

  // ������������ ����� �����
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * ���������� ���������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� �������������� ����������.
 *
 */
CLongSignedInteger CLongSignedInteger::operator --(){
  // ��������� �������
  CLongSignedInteger oResult;
  // ����� ����������, �������� ��� ���������
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // ����������� ������������, ���������� ����������
  oResult = *this = *this - oIncr;

  // ������������ ����� �����
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ���������� ���������������.
 *
 * ����������:
 *   1- ��� �������� ������� �������� ������� �������� �������� ��������� ������������ ������.
 *
 */
bool CLongSignedInteger::operator ==(CLongSignedInteger& oValue){
  return ((CLongUnsignedInteger&)*this == (CLongUnsignedInteger&)oValue)&&(FSignFlag == oValue.Sign);
};

/**
 * �������� ��������� ������������� ������.
 *
 * ������:
 *   ���������� ������ ������ ����� �����.
 *   ���� ����� �����, ��������� ����� ����������� ����� ��� ������������ ���������
 *   ��������� �������� ������. ������ ���� ������ ��������� ����� �����.
 *
 *       7 > 4, �� -7 < -4.
 *
 *   ������ ������� ��� ������ � ����� � ������ ����.
 */
bool CLongSignedInteger::operator >(CLongSignedInteger& oValue){

  // ����� �������������, ������ �������������, ��������� ���������� ���������
  if(FSignFlag && !oValue.Sign) return false;
  // ����� �������������, ������ �������������, ��������� ����������
  if(!FSignFlag && oValue.Sign) return true;
  // ����� ��� ����� ��������, ��� ��� ����� ��� ������������, ��� ������������

  // ����������� ����� � ������ �������� ��� ��������� �� ������
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // ���� ����� ������������, ������� ������ ����� ������� �� ������
  // ���� ����� ������������, �� ��������� ������������ ��� � ������
  return (FSignFlag)? (oRight > oLeft):(oLeft > oRight);
};

/**
 * �������� ��������� ������������� �������.
 *
 * ����������:
 *   1- ��������� ������������� ����������� ��������� ���������.
 *   2- ��� �������� ������� �������� ������� �������� �������� ��������� ������������ ������.
 *
 */
bool CLongSignedInteger::operator <(CLongSignedInteger& oValue){

  // ����� ������������, ������ �������������, ��������� ���������� ��������
  if(!FSignFlag && oValue.Sign) return false;
  // ����� �������������, ������ �������������, ��������� ��� �� ����������
  if(FSignFlag && !oValue.Sign) return true;

  // ����������� ����� � ������ �������� ��� ��������� �� ������
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // ���� ����� ������������, ������� ������ ����� ������� �� ������
  // ���� ����� ������������, �� ��������� ������������ ��� � ������
  return (FSignFlag)? (oRight < oLeft):(oLeft < oRight);
};

//--- CLongFloat ------------------------------------------------------------
CLongFloat::CLongFloat(): FFrac("") {

};

/**
 * ����������� �����������.
 *
 * ������:
 *   ����������� ��������� ������ ������������ ����������� �������� �����.
 *
 */
CLongFloat::CLongFloat(CLongFloat& oValue): CLongSignedInteger((CLongSignedInteger&)oValue) {
  SetFrac(oValue.FFrac);
};

/**
 * �������� ������������ ��� ������������ �����.
 *
 * ������:
 *   ������� ������������� ������ (����� ����� � ����) ������������ ����� ����� ��������� ����� ������.
 *   ������������ ������������ ����� �����.
 *   � ����� ������ ���������� ������� ������������.
 *
 */
CLongFloat& CLongFloat::operator =(CLongFloat oValue){
  // �������� �������� ������������� ������
  CLongSignedInteger::operator =((CLongSignedInteger)oValue);
  // ������������� �������� ������� �����
  SetFrac(oValue.Fraction);

  // ��������� ������������
  Normalize();
  return *this;
};

/**
 * �������� �������� ���� ������������.
 *
 * ������:
 *   ��� �������� ����� ������������ ��������������� ������������ ������� ������.
 *   �� ������ ��� ��������� �� ���: (���������� 36.005 � 3.06)
 *     ���� ��������� ������� �����, ��������� ����� � ������ ����� �� 1000,
 *     �������� 36005 + 3060.
 *     ��������� 36005 + 3060 == 39065; ����� 39065 �� 1000 � ��������
 *     ������������� ��������� -- 39.065
 *
 * ����������:
 *   1- �������� ��������� ���������� �� CLognSignedInteger::operator +()
 *
 */
CLongFloat CLongFloat::operator +(CLongFloat oValue){
  // ��������� ��������
  CLongFloat oResult;
  // ��������� ������� ������ ���������, ����� ������ ��� ������������
  CLongFloat oLeftFrac;
  CLongFloat oRightFrac;
  // ����� � ������ ��������
  CLongSignedInteger oLeft = (CLongSignedInteger&)*this;
  CLongSignedInteger oRight = (CLongSignedInteger&)oValue;
  // ������� ������������ ������� ��������
  int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

  // ������������ ����� � ����� ����� �����
  oLeftFrac.Fraction = FFrac;
  oRightFrac.Fraction = oValue.FFrac;

  // ����������� ����� �� ������� ������� ������.
  for(int iTrack = 0; iTrack < iMaxFracLength; iTrack++){
    char cDigit;
    // �������� ������� ������ �� �����
    oLeftFrac.PopFrac(cDigit);
    // ���� ������ �������, ������ ��� � ������� ������ ������
    // ���� ��, ������ ���� ����
    oLeft.AddDigit((cDigit)? cDigit:'0');
    // ��������� � ���������� ��������� � � ������ ���������
    oRightFrac.PopFrac(cDigit);
    oRight.AddDigit((cDigit)? cDigit:'0');
  };

  // �������� ���������, ������ �� ���������� ������ �������
  // �������� ������������ ��� ������� �������� �����
  (CLongSignedInteger&)oResult = oLeft + oRight;
  // � ����� �������� ���������� ������ ������� ������� ������� �� �����
  while(iMaxFracLength--){
    char cDigit;
    // ������� ������� ������ �� �����
    oResult.RemoveDigit(cDigit);
    // � ������ ��� � ������� ������ ������� �����
    // ���� ������ �� ��������, ������ � ����� �����
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // ���������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ������� ���� ������������.
 *
 * ������:
 *   �������� � ��������� �������� ��������� ���������.
 *
 * ����������:
 *   1- �������� ��������� ���������� �� CLognSignedInteger::operator -()
 *
 */
CLongFloat CLongFloat::operator -(CLongFloat oValue){
  // ��������� ��������
  CLongFloat oResult;
  // ��������� ������� ������ ���������, ����� ������ ��� ������������
  CLongFloat oLeftFrac;
  CLongFloat oRightFrac;
  // ����� � ������ ��������
  CLongSignedInteger oLeft = (CLongSignedInteger&)*this;
  CLongSignedInteger oRight = (CLongSignedInteger&)oValue;
  // ������� ������������ ������� ��������
  int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

  // ������������ ����� � ����� ����� �����
  oLeftFrac.Fraction = FFrac;
  oRightFrac.Fraction = oValue.FFrac;

  // ����������� ����� �� ������� ������� ������.
  for(int iTrack = 0; iTrack < iMaxFracLength; iTrack++){
    char cDigit;
    // �������� ������� ������ �� �����
    oLeftFrac.PopFrac(cDigit);
    // ���� ������ �������, ������ ��� � ������� ������ ������
    // ���� ��, ������ ���� ����
    oLeft.AddDigit((cDigit)? cDigit:'0');
    // ��������� � ���������� ��������� � � ������ ���������
    oRightFrac.PopFrac(cDigit);
    oRight.AddDigit((cDigit)? cDigit:'0');
  };

  // �������� ���������, ������ �� ���������� ������ �������
  // �������� ������������ ��� ������� �������� �����
  (CLongSignedInteger&)oResult = oLeft - oRight;
  // � ����� �������� ���������� ������ ������� ������� ������� �� �����
  while(iMaxFracLength--){
    char cDigit;
    // ������� ������� ������ �� �����
    oResult.RemoveDigit(cDigit);
    // � ������ ��� � ������� ������ ������� �����
    // ���� ������ �� ��������, ������ � ����� �����
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // ���������� ������������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ��������� �������
 *
 * ������:
 *   ��������� ���� ������� ����� ����� ������� ����� ��������� �����,
 *   �������� ������ ����� �� ��������. �� ��� ������� ����� ��������� ������� ��������.
 *   ��� ��������� ����� ����������� � ���� �������� ��������:
 *
 *     aa.bb * xx.yy == (aa*xx) + (bb*yy) + (aa*yy) + (xx*bb)
 *
 *   ����� �������, ����� �������� 3 ����������� ���������:
 *   - ��������� ���� �����        (��� ����������� � ������������ ������)
 *   - ��������� ���� �������      (MulFracFrac)
 *   - ��������� ������ �� ������� (MulIntFrac)
 *
 *   ��� ����������� ����������� � ���� ��������� ������� ������.
 *
 */
CLongFloat CLongFloat::operator *(CLongFloat oValue){
  // ��������� ��������
  CLongFloat oResult;

  // ������� � ��������� �������� ��������� ����� ������
  (CLongSignedInteger&)oResult = (CLongSignedInteger&)*this * (CLongSignedInteger&)oValue;
  // ��������� ��������� ��������� ������ ����� �� ������� ������
  oResult = oResult + MulIntFrac(FSignFlag, GetValue(), oValue.Sign, oValue.Fraction);
  // ��������� ��������� ��������� ������ ������ �� ������� �����
  oResult = oResult + MulIntFrac(oValue.Sign, oValue.Value, FSignFlag, FFrac);
  // ��������� ��������� ������������ ���� �������
  oResult = oResult + MulFracFrac(FSignFlag, FFrac, oValue.Sign, oValue.Fraction);

  // ��������� ������������ ����������
  oResult.Normalize();
  return oResult;
};

/**
 * �������� ������� �������.
 *
 * ������:
 *   �������� ������� �������� ���������� �����������. � ������ �����������
 *   ������ ����� ������� ���������� �������� ����������. �� ��� ���� � �����
 *   ���������� ��������� �������������. ���� �������� ������������ ������� �
 *   �������� ����������, ���������� ������������ ��� ���������� "������ ��������".
 *
 *   D-�������� - ��� ������������ ����� ������� ��������, � �������� ������� �����
 *   ������������� ���������� �������. ����� ���������� ��� ������ "LF_FRACDELTA".
 *
 * ������:
 *   - ���������� �������� ��� �������� �� 10, ���� ������� ����� �� ��������� � ����� ���������
 *   - ���������� ���������� ��������� ��������� � ������� ���������
 *   - �������� ����� ������� �� 10, ���� �� �� ������ ������ �������
 *   - ���������� ���������� ���� ��������� � ��������� �� ��������� ���������
 *   - �������� ����� ������� �� 10 LF_FRACDELTA ���, ����� ������� ����� ����� ������ ��������
 *   - ����������� ������� ��������� �� LF_FRACDELTA
 *   - ���������� ������� ����� �������� ������� ����� �� ������ � ������� ���������
 *   - �������� �� �������� ��� ����� �������� (����������� ������� ������� ������)
 *   - ���������� ������� ���� ����� ��������
 *   - ���������� �������� ������� ���������, ����� ���������� ��������� �� 10
 *   - ���������� �������� ��������
 *
 */
CLongFloat CLongFloat::operator /(CLongFloat oValue){
  // ��������� ��������
  CLongFloat oLeft;
  // ����� � ������ �������� ��� �������� �������
  CLongFloat oRight;
  CLongFloat oResult;
  // ������� ���������
  int iFracLength = 0;

  // ���������� ����� � ������ ��������
  oLeft = *this;
  oRight = oValue;

  // ����������� ������� � ����� ����� ���������, ���������� ������� �������� �� 10
  // ��� �������� ����� ������� �������� ����� �� �������� ������� ������� � ������� ������ ����� �����
  while(((oLeft.Fraction != "0") || (oRight.Fraction != "0"))&&(oLeft.Fraction.Length() || oRight.Fraction.Length())){
    char cDigit;
    // ������� ������� ������ ������ ��������
    oLeft.PopFrac(cDigit);
    // ������ ���, ���� ����, � ������� ������ ������ ������, ���� ���, �� ������ 0
    oLeft.AddDigit((cDigit)? cDigit:'0');
    // ������� ������� ������ ������� ��������
    oRight.PopFrac(cDigit);
    // ������ ���, ���� ����, � ������� ������ ������� ������, ���� ���, �� ������ 0
    oRight.AddDigit((cDigit)? cDigit:'0');
    // ��� ������������ ���� ���������
    iFracLength++;
  };

  // ������ ���, ���� ����� ����� ������ �������
  // ������ ������� ��� �� ������������ � �������
  while((CLongUnsignedInteger&)oLeft < (CLongUnsignedInteger&)oRight){
    oLeft.AddDigit('0');
    iFracLength++;
  };

  // ��������� � ������ �������� LF_FRACDELTA �����, ��� ����� ������ ������ ��������
  for(int iTrack = 0; iTrack < LF_FRACDELTA; iTrack++) oLeft.AddDigit('0');
  // ������������ ������� ��������� � ������ ������������ ����� ������� ����� ��������� � ������ ��������
  iFracLength += (LF_FRACDELTA - ((FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length()));

  // ���������� ����������� ������� ���� �������� ����� �����
  (CLongUnsignedInteger&)oResult = (CLongUnsignedInteger&)oLeft / (CLongUnsignedInteger&)oRight;

  // ��������������� ��������� ������� ����� ����������
  // ��� ����� ������ ������� ������� ������� �������� ����� �����,
  // ������� �������� ������� ���������, � ������ �� � ������� �������
  // ������� �����.
  while(iFracLength--){
    char cDigit;
    oResult.RemoveDigit(cDigit);
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // ��������� ����������� ������������ ����������
  oResult.Normalize();
  // ���������� ���� ����������, ���� ������ ����� ��� �������� ���������.
  oResult.Sign = (!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign);
  return oResult;
};

/**
 * ������������ ������ �� ���������� �����. ( aa * 0.bb )
 *
 * ������:
 *   ������� ����� ������ ����� ����������� ��� �����, ����������� �� ������� ����� 10,
 *   � ��� ����� ������! ����������, ��� ������������ ������ �� ������� �����
 *   ����� ����� ����������� � ���� ������ �� ������, ����������� �� ��� ��� �����,
 *   � ����� ������ ������� ��������� ������ ���������� �� 10, �� ��� ���, ���� �������
 *   ������� ����� ���������� �� ������������� �� ������ ������� �������� ��������.
 *
 * ����������:
 *   1- ������� ����� ���������� ��� ����� � ������� ����� �����, �� � ������ ����� (���� ����).
 *
 */
CLongFloat CLongFloat::MulIntFrac(bool bLSign, AnsiString& sLInt, bool bRSign, AnsiString& sRFrac){
  // ����� � ������ �������� ��� ������������
  CLongUnsignedInteger oLeft;
  CLongUnsignedInteger oRight;
  // ��������� ��������
  CLongFloat oResult;

  // ������������� �������� ���������
  oLeft.Value = sLInt;
  oRight.Value = (sRFrac.Length())? sRFrac:AnsiString("0");

  // ���������� ������� ������������� ���������
  (CLongUnsignedInteger&)oResult = oLeft * oRight;
  // ������������ ����� ����������, ���� �������� ������ ������������
  while(sRFrac.Length() > oResult.Value.Length()) oResult.PushDigit('0');
  oResult.Fraction = "";
  // ������������ ������� ������� ����� �� �� �����
  // � ������� ����� ������ ���� ������� ��������, ������� �� � ������� ��������
  while(sRFrac.Length() > oResult.Fraction.Length()){
    char cDigit;
    oResult.RemoveDigit(cDigit);
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // ���������� ���� ���������� ��������
  oResult.Sign = (!bLSign && bRSign)||(bLSign && !bRSign);
  return oResult;
};

/**
 * ������������ ���� ������� ������ (0.aa * 0.bb)
 *
 * ������:
 *   ������� ����� ����������� ��� ��� �����, � ��������� �������� ������ ���������
 *   �� ����� �������� ����� ������� �������� ����� ���������.
 *   ��� ��������� ������������ ���������� ������������ ������ ���������� ������, ����� ������������.
 *
 * ����������:
 *   1- ������� ����� ���������� ��� ����� � ������� ����� �����, �� � ������ ����� (���� ����).
 *
 */
CLongFloat CLongFloat::MulFracFrac(bool bLSign, AnsiString& sLFrac, bool bRSign, AnsiString& sRFrac){
  // ����� � ����� �������� ��� ������������
  CLongUnsignedInteger oLeft;
  CLongUnsignedInteger oRight;
  // ���� ������� �������� ��� ������������ (����� ���� �������� ����� ���������)
  int iFractLength = sLFrac.Length() + sRFrac.Length();
  // ��������� ��������
  CLongFloat oResult;

  // ������������� �������� ���������
  oLeft.Value = (sLFrac.Length())? sLFrac:AnsiString("0");
  oRight.Value = (sRFrac.Length())? sRFrac:AnsiString("0");

  // ���������� �������� ��������� (���������� ��� ������������ ����������)
  (CLongUnsignedInteger&)oResult = oLeft * oRight;
  // ��������� ����� � ���������� ������� �����, ������� ���������
  // (��� �������� ������������ ������������ ������������)
  while(iFractLength > oResult.Value.Length()) oResult.PushDigit('0');
  // ��������� ��� ������� � ������� �����
  oResult.Fraction = oResult.Value;
  // �������� ����� ����� (���������� ����� ������ � ����� �����)
  oResult.Value = "0";

  // ������������� ���� ��������
  oResult.Sign = (!bLSign && bRSign)||(bLSign && !bRSign);
  return oResult;
};

/**
 * �������� ������������ ����������
 *
 * ����������:
 *  1- �������� ����������� ������ ��� ����� ������.
 *
 */
CLongFloat CLongFloat::operator ++(int iType){
  CLongFloat oResult = *this;
  ((CLongSignedInteger&)*this).operator ++(iType);
  return oResult;
};

/**
 * �������� ������������ ����������
 *
 * ����������:
 *  1- �������� ����������� ������ ��� ����� ������.
 *
 */
CLongFloat CLongFloat::operator --(int iType){
  CLongFloat oResult = *this;
  ((CLongSignedInteger&)*this).operator --(iType);
  return oResult;
};

/**
 * �������� ���������� ����������
 *
 * ����������:
 *  1- �������� ����������� ������ ��� ����� ������.
 *
 */
CLongFloat CLongFloat::operator ++(){
  ((CLongSignedInteger&)*this).operator ++();
  return *this;
};

/**
 * �������� ���������� ����������
 * 
 * ����������:
 *  1- �������� ����������� ������ ��� ����� ������.
 *
 */
CLongFloat CLongFloat::operator --(){
  ((CLongSignedInteger&)*this).operator --();
  return *this;
};

/**
 * �������� ��������� ���������������
 *
 * ����������:
 *  1- ����������� ������������ ��������.
 *
 */
bool CLongFloat::operator ==(CLongFloat& oValue){
  return CLongSignedInteger::operator ==((CLongSignedInteger&)oValue) && (0 == strcmp(FFrac.c_str(), oValue.Fraction.c_str()));
};

/**
 * �������� ��������� ������������� ������
 *
 */
bool CLongFloat::operator >(CLongFloat& oValue){
  // ���� ������������� ������� �����
  bool bOverflow = false;

  // ����� ������� ���������, ����� ��������
  CLongSignedInteger oLeft(*this);

  if((FFrac.Length() && oValue.Fraction.Length()) && ((FFrac != "0") && (oValue.Fraction != "0"))){
    // ����� � ������ �������� ��� ��������� ������� ������
    CLongSignedInteger oLeftFrac;
    CLongSignedInteger oRightFrac;
    // ������� ������������ ��������
    int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

    // ������ �������� ��� �������� �������, ����� ������� � �������� ������ �� �������
    oLeftFrac.Value = FFrac;
    oRightFrac.Value = oValue.FFrac;
    oLeftFrac.Sign = FSignFlag;
    oRightFrac.Sign = oValue.Sign;
    // ���������� ������������ ����� ��������� �� ��������
    // ���� ���� 0.87 � 0.022, ����� 870 � 22
    if((iMaxFracLength > FFrac.Length()) && (oLeftFrac.Value != "0"))
      while(iMaxFracLength > oLeftFrac.Value.Length()) oLeftFrac.AddDigit('0');
    if((iMaxFracLength > oValue.Fraction.Length()) && (oRightFrac.Value != "0"))
      while(iMaxFracLength > oRightFrac.Value.Length()) oRightFrac.AddDigit('0');

    // ���������� ��������� ���������
    bOverflow = oLeftFrac > oRightFrac;
  };

  // ���� ����� ����� ������ ������, ����������� ����� ����� ������
  if(bOverflow) oLeft++;

  // ���������� ��������� ����� ������
  return oLeft > (CLongSignedInteger&)oValue;
};

/**
 * �������� ��������� ������������� �������
 *
 *
 */
bool CLongFloat::operator <(CLongFloat& oValue){
  // ���� ������������� ������� �����
  bool bOverflow = false;

  // ����� ������� ���������, ����� ��������
  CLongSignedInteger oLeft(*this);

  if((FFrac.Length() && oValue.Fraction.Length()) && ((FFrac != "0") && (oValue.Fraction != "0"))){
    // ����� � ������ �������� ��� ��������� ������� ������
    CLongSignedInteger oLeftFrac;
    CLongSignedInteger oRightFrac;
    // ������� ������������ ��������
    int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

    // ������ �������� ��� �������� �������, ����� ������� � ��������
    oLeftFrac.Value = FFrac;
    oRightFrac.Value = oValue.FFrac;
    oLeftFrac.Sign = FSignFlag;
    oRightFrac.Sign = oValue.Sign;
    // ���������� ������������ ����� ��������� �� ��������
    // ���� ���� 0.87 � 0.022, ����� 870 � 22
    if((iMaxFracLength > FFrac.Length()) && (oLeftFrac.Value != "0"))
      while(iMaxFracLength > oLeftFrac.Value.Length()) oLeftFrac.AddDigit('0');
    if((iMaxFracLength > oValue.Fraction.Length()) && (oRightFrac.Value != "0"))
      while(iMaxFracLength > oRightFrac.Value.Length()) oRightFrac.AddDigit('0');

    // ���������� ��������� ���������
    bOverflow = oLeftFrac < oRightFrac;
  };

  // ���� ����� ����� ������ ������, ��������� ����� �������
  if(bOverflow) oLeft--;

  // ���������� ��������� ����� ������
  return oLeft < (CLongSignedInteger&)oValue;
};

/**
 * ����������� ����� ������������ �����
 *
 */
void CLongFloat::Normalize(){
  CLongSignedInteger::Normalize();
  NormalizeFrac();
};

/**
 * ������������ ������� ����� �����
 *
 */
void CLongFloat::NormalizeFrac(){
  while((FFrac.Length())&&('0' == FFrac[FFrac.Length()])) FFrac.Delete(FFrac.Length(),1);
};

/**
 * ���������� ��������� �������� ������� �����
 *
 */
AnsiString CLongFloat::GetFrac(){
  return FFrac;
};

/**
 * ������������� �������� ������� �����
 *
 */
AnsiString CLongFloat::SetFrac(const AnsiString sValue){
  FFrac = sValue;
  NormalizeFrac();
  return FFrac;
};

/**
 * ���������� ���������������� �������� �������� �����
 *
 */
AnsiString CLongFloat::PrintText(){
  AnsiString sResult;

  //Normalize();
  // ���� ������� ����� ���, �� � �������� �� ������ �������
  if(FFrac.Length())
    sResult.printf("%s%s.%s",((FSignFlag)? "-":""),GetValue(),GetFrac());
  else
    sResult.printf("%s%s",((FSignFlag)? "-":""),GetValue());

  return sResult;
};

/**
 * ��������� � ������� ����� ���� ������ ����� (�������)
 *
 */
void CLongFloat::PushFrac(char cValue){
  AnsiString sOpValue(cValue);
  FFrac.Insert(sOpValue,1);
};

/**
 * ������� ���� ������ ����� �� ������� �����
 *
 */
void CLongFloat::PopFrac(char& cValue){
  if(!FFrac.Length()){
    cValue = 0;
    return;
  };

  cValue = FFrac[1];
  FFrac.Delete(1,1);
};
  
/**
 * ��������� � ������� ����� ���� ������ ������ (�������)
 *
 */
void CLongFloat::AddFrac(char cValue){
  AnsiString sOpValue(cValue);
  FFrac.Insert(sOpValue,FFrac.Length()+1);
};
  
/**
 * ������� ���� ������ ������ �� ������� �����
 *
 */
void CLongFloat::RemoveFrac(char& cValue){
  if(!FFrac.Length()){
    cValue = 0;
    return;
  };
  
  cValue = FFrac[FFrac.Length()];
  FFrac.Delete(FFrac.Length(),1);
};

//---------------------------------------------------------------------------
//--- CCalcController -------------------------------------------------------
CCalcController::CCalcController(): FFracInput(false), FOperation(coNop) {

};
   
/**
 * ���� ����� �����
 *
 */
bool CCalcController::InputDigit(char cDigit){
  try{
    // ��������, � ����� �� ��� �����?
    if((0 > (int)(cDigit - '0'))||(9 < (int)(cDigit - '0'))) throw false;
    // ���������� ����� ����, ��� ������ ���������� ���� (����� ��� ������� �����)
    if(FFracInput){
      FRight.AddFrac(cDigit);
    }else{
      FRight.AddDigit(cDigit);
    };
  }catch(...){
    return false;
  };
  return true;
};
  
/**
 * �������� ����� �����
 *
 */
bool CCalcController::RemoveDigit(){
  try{
    char cDigit;

    // ������ ������� ����� ������ ����� �� ������� ����� - ����� ������� ������
    if(FFracInput){
      FRight.RemoveFrac(cDigit);
    }else{
      FRight.RemoveDigit(cDigit);
    };
  }catch(...){
    return false;
  };
  return true;
};
  
/**
 * ������� ������� ��������� �����
 *
 */
bool CCalcController::ClearInput(){
  try{
    FRight.Value = "0";
    FRight.Fraction = "";
    FRight.Sign = false;
    FFracInput = false;
  }catch(...){
    return false;
  };
  return true;
};
 
/**
 * ������ ����� ������������
 *
 */
bool CCalcController::Reset(){
  try{
    ClearInput();
    FLeft.Value = "0";
    FLeft.Fraction = "";
    FLeft.Sign = false;
    FOperation = coNop;
  }catch(...){
    return false;
  };
  return true;
};
 
/**
 * ������� ��������� ����� � ����� ����� �� �������
 *
 */
bool CCalcController::Turn2Frac(){
  FFracInput = !FFracInput;
  return FFracInput;
};

/**
 * ���������� �������� ��� �������
 *
 * ������:
 *   ��������� �������� ����� ������� � ����� �������, ����� �������
 *   ����� ���������� ����� �������� ����� ����� ������ �����, ����������
 *   ���� ������ ����� � ������� ����� ���������� � ������� �����������.
 *
 */
bool CCalcController::MakeReturn(){
  try{
    // ���������� ���������� ��������
    switch(FOperation){
      case coAdd:
        FLeft = FLeft + FRight;
      break;
      case coSub:
        FLeft = FLeft - FRight;
      break;
      case coMul:
        FLeft = FLeft * FRight;
      break;
      case coDiv:
        FLeft = FLeft / FRight;
      break;
      case coNop:
      case coRet:

      return true;
      // ������������ �������� �������� � ����������
      default:
        throw false;
    };
    // ������� ������� ����
    ClearInput();
    // ������ ��������� "���������"
    FOperation = coRet;
  }catch(...){
    return false;
  };
  return true;
};

/**
 * ����������� ��������
 *
 * ������:
 *   �������� � ����������� ����� ������� ���������������:
 *   ����� - �������� - ����� - �������� - ����� - �������� - ...
 *   �� ����, � ������ ����� �������� ���� ���������, ������������� �� ���
 *   ����� ���� ��� ��������. � ���� ���� ��������������� ��������, ����
 *   ������ ������� ���������� � ������ ����� ���������� ����� �������� � ����.
 *
 */
bool CCalcController::MakeOperation(ECalcOperation eOpCode){
  // ���� ������� ��������
  bool bOpResult = false;

  // ���� ���� ��������, �� ���������� ���������
  if(FOperation){
    bOpResult = MakeReturn();
  }else try{
    // ���� �������� ���, ���� ������ ���������� ����� � ����� �����
    FLeft = FRight;
    // � �������� ������� �����
    ClearInput();
    bOpResult = true;
  }catch(...){
    // �� ������ ������ ����������
    return false;
  };

  // �������� ��������� ������ ����������� ��� ��
  if(coTurn == eOpCode){
    try{
      CLongFloat oBase;
      oBase.Value = "1";
      FLeft = oBase / FLeft;
    }catch(...){
      return false;
    };
  // ����� ���� ������ ���������� ����������� ��������
  }else FOperation = eOpCode;

  return bOpResult;
};

/**
 * ����� ����� ����� � ������� �����
 *
 */
bool CCalcController::TurnSign(){
  FRight.Sign = !FRight.Sign;
  return true;
};

/**
 * ������� ���������� ��������
 *
 */
AnsiString CCalcController::GetResult(){
  return FLeft;
};

/**
 * ������� �������� �������� �����
 *
 */
AnsiString CCalcController::Print(){
  return FRight;
};

//---------------------------------------------------------------------------

#pragma package(smart_init)
