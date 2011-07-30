//---------------------------------------------------------------------------

#ifndef LongCalculationH
#define LongCalculationH

#include <dstring.h>
#include <windows.h>

/**
 * �������� ����������� �������� ��� �������� � ��������� ������.
 * ������������ �� ����� ������� ������������ �����.
 *
 * ����������:
 * 1- ���������� ��������� �������� ������� ������
 * 2- ���������� ����� �������� �������� � ���������� �������� �������!
 */
#define LF_FRACDELTA 16


/**
 * ����� �������� ������ �����, ������ ����������� ���������� ����� ��������
 * � ��������� ����������� ��� ����������� �������� ������������� ����������
 */
typedef class CLongUnsignedInteger {
  private:

    /**
     * ��������� ��� ������ �����. ������� ������ - �����. �.�. ������ "235" ����� ������������ ����� 235
     * ���� ���������, ������ � ���� ������ ����� ����������� ���������. ��� ������� ��� ����������� ����������� ������
     */
    AnsiString FValue;

  protected:

    /**
     * ������� ���������� ������� �������� FValue. ������������ ��� ���������� ������ � ������ �� ������.
     * ������� ������� ����������� �� ������, ���� � �������� ������ ������� ��������
     * ��������� ���������� ������� ���������.
     */
    virtual AnsiString GetValue();

    /**
     * ������� ��� ���������� �������� �������� FValue ����� ���������.
     * ������� ���������� ����������������� �������� ��� ����������� �������� ������������ ������������.
     * (������ ������������ ������������: a.value = b.value = c.value ...)
     */
    virtual AnsiString SetValue(const AnsiString);

    /**
     * ������� ������������ �����. �������� ����� � ������������ ���, ������� ��� ���� �����.
     * ��� ����������� ������������� � �������� ������� ������� �������� �����������.
     */
    virtual void Normalize();

  public:
    CLongUnsignedInteger();
    CLongUnsignedInteger(CLongUnsignedInteger&);

    /**
     * ���������� ����� ����� � �����.
     * ����������� ����� �������� �������.
     */
    virtual void PushDigit(char);

    /**
     * ������� ����� �������� ������� � ������ ��������� �� �����.
     */
    virtual void PopDigit(char&);

    /**
     * ���������� ����� �������� �������.
     */
    virtual void AddDigit(char);

    /**
     * ������� ����� �������� �������, � ������ ��������� �� �����.
     */
    virtual void RemoveDigit(char&);

    /**
     * ����������������, ���������������� ����� �����.
     */
    virtual AnsiString PrintText();

    /**
     * ���������� ��������� ��� �������� ������ � �������
     */
    CLongUnsignedInteger& operator =(CLongUnsignedInteger);
    CLongUnsignedInteger  operator +(CLongUnsignedInteger);
    CLongUnsignedInteger  operator -(CLongUnsignedInteger);
    CLongUnsignedInteger  operator *(CLongUnsignedInteger);
    CLongUnsignedInteger  operator /(CLongUnsignedInteger);
    // ����������� ����� (a++)
    CLongUnsignedInteger  operator ++(int);
    CLongUnsignedInteger  operator --(int);
    // ���������� �����  (++a)
    CLongUnsignedInteger  operator ++();
    CLongUnsignedInteger  operator --();
    // ���������� ���������
    bool operator ==(CLongUnsignedInteger&);
    bool operator >(CLongUnsignedInteger&);
    bool operator <(CLongUnsignedInteger&);
    __inline bool operator >=(CLongUnsignedInteger& Value){return ((*this > Value)||(*this == Value));};
    __inline bool operator <=(CLongUnsignedInteger& Value){return ((*this < Value)||(*this == Value));};
    // ���������� � ���� AnsiString, ���������������� ����� �������������
    __inline operator AnsiString(){return PrintText();};

    /**
     * �������� �������� �����. ���������� ��������� ��������,
     * ��� ��������������, ������ � ����� �������.
     */
    __property AnsiString Value = {read=GetValue, write=SetValue};

} CLongUnsignedInteger;

/**
 * ����� �������� ������ ����� �� ������, ��������� ��� �������� �������� ������ �����, �������� ���� (+/-) �����.
 * ��������� ������������� ���������� ��������� � ������ ���������� �����.
 *
 * ����������:
 * 1- +0 >= -0
 */
typedef class CLongSignedInteger: public CLongUnsignedInteger {
  protected:
    /**
     * ���� �������, true - �������� ��� ����� ��������� � ������������� ���������.
     */
    bool FSignFlag;

  public:
    CLongSignedInteger();
    CLongSignedInteger(CLongSignedInteger&);

    /**
     * ���������� ������ ����������������� ������������� �����.
     * ������ ����������� ���� �����.
     */
    virtual AnsiString PrintText();

    /**
     * ���������� ��������� ��� �������� ������ � �������.
     */
    CLongSignedInteger& operator =(CLongSignedInteger);
    CLongSignedInteger  operator +(CLongSignedInteger);
    CLongSignedInteger  operator -(CLongSignedInteger);
    CLongSignedInteger  operator *(CLongSignedInteger);
    CLongSignedInteger  operator /(CLongSignedInteger);
    // ����������� ����� (a++)
    CLongSignedInteger  operator ++(int);
    CLongSignedInteger  operator --(int);
    // ���������� �����  (++a)
    CLongSignedInteger  operator ++();
    CLongSignedInteger  operator --();
    // ���������� ���������
    bool operator ==(CLongSignedInteger&);
    bool operator >(CLongSignedInteger&);
    bool operator <(CLongSignedInteger&);

    __inline bool operator >=(CLongSignedInteger& Value){return ((*this > Value)||(*this == Value));};
    __inline bool operator <=(CLongSignedInteger& Value){return ((*this < Value)||(*this == Value));};

    // �������� ����� �����
    __property bool Sign = {read=FSignFlag, write=FSignFlag};

} CLongSignedInteger;


/**
 * ����� �������� ������������� �����.
 * ��������� ������� �������������� � �������� ����� �����.
 * ����� �������������� ���� - ������������ ����� � ���� ���������� �����.
 */
typedef class CLongFloat : public CLongSignedInteger {
  private:
    /**
     * ��������� ������������ ����� �����.
     * ������ ������, �.�. ������ "931" ����� ������������ ����� 0.931
     */
    AnsiString FFrac;

    /**
     * ����� ������������ �������������� ����� �� ���������� �����.
     * ���������� �����, ������������ � ��������� ������������.
     */
    CLongFloat MulIntFrac(bool, AnsiString&, bool, AnsiString&);

    /**
     * ����� ������������ ���� ���������� ������.
     * ���������� �����, ������������ � ��������� ������������.
     */
    CLongFloat MulFracFrac(bool, AnsiString&, bool, AnsiString&);

  protected:
    /**
     * ����� ������������ ����� �����.
     */
    AnsiString GetFrac();

    /**
     * ���� � ��������� ������������ ����� �����.
     * ���������� ����������������� �������� ��� ����������� ������������ ������������.
     */
    AnsiString SetFrac(const AnsiString);

    /**
     * ���������� ������ ������������ �����.
     * �������� ����� ������ ������������ ������������ �����.
     */
    virtual void Normalize();

    /**
     * ������������� ������������ ����� �����
     */
    virtual void NormalizeFrac();

  public:
    CLongFloat();
    CLongFloat(CLongFloat&);

    /**
     * ���������� ������ ����������������� ������������� �����.
     * �������� ����� ������� �����.
     */
    virtual AnsiString PrintText();

    /**
     * ���������� ����� ����� � ������� ����� �����
     * ���������� ����� ������ ������� ������� �����
     */
    virtual void PushFrac(char);

    /**
     * ������� ����� ������ ������� ������� �����
     */
    virtual void PopFrac(char&);

    /**
     * ���������� ������ ������ ������� ������� �����
     */
    virtual void AddFrac(char);

    /**
     * ������� ������ ������ ������� ������� �����
     */
    virtual void RemoveFrac(char&);

    /**
     * ���������� ��������� ��� �������� ������ � �������.
     */
    CLongFloat& operator =(CLongFloat);
    CLongFloat  operator +(CLongFloat);
    CLongFloat  operator -(CLongFloat);
    CLongFloat  operator *(CLongFloat);
    CLongFloat  operator /(CLongFloat);
    // ����������� ����� (a++), ���������� ������ ����� �����
    CLongFloat  operator ++(int);
    CLongFloat  operator --(int);
    // ���������� �����  (++a), ���������� ������ ����� �����
    CLongFloat  operator ++();
    CLongFloat  operator --();
    // ���������� ���������
    bool operator ==(CLongFloat&);
    bool operator >(CLongFloat&);
    bool operator <(CLongFloat&);

    __inline bool operator >=(CLongFloat& Value){return ((*this > Value)||(*this == Value));};
    __inline bool operator <=(CLongFloat& Value){return ((*this < Value)||(*this == Value));};

    /**
     * �������� ������������ ����� �����, ��������� ������������� ������������ �����.
     */
    __property AnsiString Fraction = {read=GetFrac, write=SetFrac};

} CLongFloat;

/**
 * ����������� ��� ������������, ���������� �������� �����������
 *
 */
typedef enum ECalcOperation {
  coNop = 0 /* ������ �������� */, coAdd /* �������� */, coSub  /* ��������� */,
  coMul     /* ��������� */,       coDiv /* ������� */,  coTurn /* ��������� (1/x) */,
  coRet     /* ���������� */
} ECalcOperation;

/**
 * ����� ����������� ������������.
 *
 * ��������� ��� �������� ������ � �������, ����, ���������� � ����� ����������.
 * ��� �������� ������ � ���� ��� ��������, ��������� ����� � ��������������� ��������.
 *
 */
typedef class CCalcController {
  private:

    /**
     * ����� ������� � ���������.
     * � ���� �� ������� ��������� ��������� ��������, ��� ������� ��� ����,
     * ���� ����� ���������� �������� ����� ���� ���������� ���� � ���������� ����������.
     *
     */
    CLongFloat     FLeft;

    /**
     * ������ ������� � ���������.
     * ���� �� ������� ������ ������� �������� �����.
     *
     */
    CLongFloat     FRight;

    /**
     * ������� ��������� ��������
     *
     */
    ECalcOperation FOperation;

    /**
     * ��������� ������� �����
     *   false - ������� ����� - ����� �����
     *   true  - ������� ����� - ������� �����
     *
     */
    bool           FFracInput;

  public:
    CCalcController();

    /**
     * ���������� ����� ����� � ������� �������� �����
     */
    bool InputDigit(char);

    /**
     * �������� ����� ����� �� ������� ������� �����
     */
    bool RemoveDigit();

    /**
     * ����� ��������� �����
     */
    bool ClearInput();

    /**
     * ������ ����� ��������� ������������
     */
    bool Reset();

    /**
     * �������� ������� �����, INT -> FRAC � FRAC -> INT
     */
    bool Turn2Frac();

    /**
     * �������� ���� ��������� �����
     */
    bool TurnSign();

    /**
     * ������������� ��������������� ��������
     */
    bool MakeOperation(ECalcOperation);

    /**
     * ���������� ��������������� ��������
     */
    bool MakeReturn();

    /**
     * ���������� ������� ��������� ����������
     */
    AnsiString GetResult();

    /**
     * ���������� ������� ��������� ������� �����
     */
    AnsiString Print();

    /**
     * �������� ����� ��������� �����
     */
    __property bool InFrac = {read = FFracInput};

} CCalcController;

/**
 * ��� ���������� ������������� ������� �������� ���������� �����.
 */
typedef CLongUnsignedInteger longuint;
typedef CLongSignedInteger   longint;
typedef CLongFloat           longfloat;

//---------------------------------------------------------------------------
#endif
