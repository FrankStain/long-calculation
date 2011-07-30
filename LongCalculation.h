//---------------------------------------------------------------------------

#ifndef LongCalculationH
#define LongCalculationH

#include <dstring.h>
#include <windows.h>

/**
 * Величина необходимой точности для операций с плавоющей точкой.
 * Используется во время деления вещественных чисел.
 *
 * Примечания:
 * 1- желательно указывать значение стемени двойки
 * 2- увеличение этого значения приведет к замедлению операции деления!
 */
#define LF_FRACDELTA 16


/**
 * Класс длинного целого числа, хранит произвольно задаваемое число разрядов
 * и позволяет производить ряд стандартных операций целочисленной арифметики
 */
typedef class CLongUnsignedInteger {
  private:

    /**
     * Хранилище для самого числа. Порядок записи - прмой. Т.е. строка "235" будет эквивалентна числу 235
     * Поле приватное, доступ к нему только через специальные механизмы. Это сделано для обеспечения целостности данных
     */
    AnsiString FValue;

  protected:

    /**
     * Функция возвращает текущее значение FValue. Используется для извлечения строки с числом из класса.
     * Функция сделана виртуальной на случай, если в дочернем классе текущее значение
     * прийдется возвращать другими способами.
     */
    virtual AnsiString GetValue();

    /**
     * Функциа для перезаписи текущего значения FValue новым значением.
     * Функция возвращает новоустановленное значение для обеспечения операции рекурсивного присваивания.
     * (пример рекурсивного присваивания: a.value = b.value = c.value ...)
     */
    virtual AnsiString SetValue(const AnsiString);

    /**
     * Функция нормализации числа. Приводит число в естественный вид, удаляет все нули слева.
     * Для обеспечения совместимости в дочерних классах функция является фиртуальной.
     */
    virtual void Normalize();

  public:
    CLongUnsignedInteger();
    CLongUnsignedInteger(CLongUnsignedInteger&);

    /**
     * Добавление одной цифры к числу.
     * Пристыковка цифры старшего разряда.
     */
    virtual void PushDigit(char);

    /**
     * Изъятие цифры старшего разряда с полным удалением из числа.
     */
    virtual void PopDigit(char&);

    /**
     * Добавление цифры младшего разряда.
     */
    virtual void AddDigit(char);

    /**
     * Изъятие цифры младшего разряда, с полным удалением из числа.
     */
    virtual void RemoveDigit(char&);

    /**
     * Пользовательский, отформатированый вывод числа.
     */
    virtual AnsiString PrintText();

    /**
     * перекрытые операторы для удобства работы с классом
     */
    CLongUnsignedInteger& operator =(CLongUnsignedInteger);
    CLongUnsignedInteger  operator +(CLongUnsignedInteger);
    CLongUnsignedInteger  operator -(CLongUnsignedInteger);
    CLongUnsignedInteger  operator *(CLongUnsignedInteger);
    CLongUnsignedInteger  operator /(CLongUnsignedInteger);
    // постфиксные формы (a++)
    CLongUnsignedInteger  operator ++(int);
    CLongUnsignedInteger  operator --(int);
    // префиксные формы  (++a)
    CLongUnsignedInteger  operator ++();
    CLongUnsignedInteger  operator --();
    // логические операторы
    bool operator ==(CLongUnsignedInteger&);
    bool operator >(CLongUnsignedInteger&);
    bool operator <(CLongUnsignedInteger&);
    __inline bool operator >=(CLongUnsignedInteger& Value){return ((*this > Value)||(*this == Value));};
    __inline bool operator <=(CLongUnsignedInteger& Value){return ((*this < Value)||(*this == Value));};
    // приведение к типу AnsiString, пользовательская форма представления
    __inline operator AnsiString(){return PrintText();};

    /**
     * Свойство значения числа. Возвращает служебное значение,
     * без форматирования, знаков и всего прочего.
     */
    __property AnsiString Value = {read=GetValue, write=SetValue};

} CLongUnsignedInteger;

/**
 * Класс длинного целого числа со знаком, наследует все свойства длинного целого числа, содержит знак (+/-) числа.
 * Алгоритмы целочисленной арифметики арсширены с учетом знаковости чисел.
 *
 * Примечания:
 * 1- +0 >= -0
 */
typedef class CLongSignedInteger: public CLongUnsignedInteger {
  protected:
    /**
     * Флаг символа, true - означает что число находится в отрицательной плоскости.
     */
    bool FSignFlag;

  public:
    CLongSignedInteger();
    CLongSignedInteger(CLongSignedInteger&);

    /**
     * Обновление метода пользовательского представления числа.
     * Теперь учитывается знак числа.
     */
    virtual AnsiString PrintText();

    /**
     * Перекрытые операторы для удобства работы с классом.
     */
    CLongSignedInteger& operator =(CLongSignedInteger);
    CLongSignedInteger  operator +(CLongSignedInteger);
    CLongSignedInteger  operator -(CLongSignedInteger);
    CLongSignedInteger  operator *(CLongSignedInteger);
    CLongSignedInteger  operator /(CLongSignedInteger);
    // постфиксные формы (a++)
    CLongSignedInteger  operator ++(int);
    CLongSignedInteger  operator --(int);
    // префиксные формы  (++a)
    CLongSignedInteger  operator ++();
    CLongSignedInteger  operator --();
    // логические операторы
    bool operator ==(CLongSignedInteger&);
    bool operator >(CLongSignedInteger&);
    bool operator <(CLongSignedInteger&);

    __inline bool operator >=(CLongSignedInteger& Value){return ((*this > Value)||(*this == Value));};
    __inline bool operator <=(CLongSignedInteger& Value){return ((*this < Value)||(*this == Value));};

    // Свойство знака числа
    __property bool Sign = {read=FSignFlag, write=FSignFlag};

} CLongSignedInteger;


/**
 * Класс длинного вещественного числа.
 * Наследует базовые характеристики и свойства целых чисел.
 * Имеет дополнительное поле - вещественную часть в виде десятичной дроби.
 */
typedef class CLongFloat : public CLongSignedInteger {
  private:
    /**
     * Хранилище вещественной части числа.
     * Запись прямая, т.е. строка "931" будет эквивалентна числу 0.931
     */
    AnsiString FFrac;

    /**
     * Метод перемножения целочисленного числа на десятичную дробь.
     * Внутренний метод, используется в операторе перемножения.
     */
    CLongFloat MulIntFrac(bool, AnsiString&, bool, AnsiString&);

    /**
     * Метод перемножения двух десятичных дробей.
     * Внутренний метод, используется в операторе перемножения.
     */
    CLongFloat MulFracFrac(bool, AnsiString&, bool, AnsiString&);

  protected:
    /**
     * Вывод вещественной части числа.
     */
    AnsiString GetFrac();

    /**
     * Ввод и изменение вещественной части числа.
     * Возвращает новоустановленное значение для обеспечения рекурсивного присваивания.
     */
    AnsiString SetFrac(const AnsiString);

    /**
     * Обновление метода нормализации числа.
     * Добавлен вызов метода нормализации вещественной части.
     */
    virtual void Normalize();

    /**
     * Нормальизация вещественной части числа
     */
    virtual void NormalizeFrac();

  public:
    CLongFloat();
    CLongFloat(CLongFloat&);

    /**
     * Обновление метода пользовательского представления числа.
     * Добавлен вывод дробной части.
     */
    virtual AnsiString PrintText();

    /**
     * Добавление одной цифры к дробной части числа
     * Добавление слева одного разряда дробной части
     */
    virtual void PushFrac(char);

    /**
     * Изъятие слева одного разряда дробной части
     */
    virtual void PopFrac(char&);

    /**
     * Добавление справа одного разряда дробной части
     */
    virtual void AddFrac(char);

    /**
     * Изъятие справа одного разряда дробной части
     */
    virtual void RemoveFrac(char&);

    /**
     * Перекрытые операторы для удобства работы с классом.
     */
    CLongFloat& operator =(CLongFloat);
    CLongFloat  operator +(CLongFloat);
    CLongFloat  operator -(CLongFloat);
    CLongFloat  operator *(CLongFloat);
    CLongFloat  operator /(CLongFloat);
    // постфиксные формы (a++), изменяется только целая часть
    CLongFloat  operator ++(int);
    CLongFloat  operator --(int);
    // префиксные формы  (++a), изменяется только целая часть
    CLongFloat  operator ++();
    CLongFloat  operator --();
    // логические операторы
    bool operator ==(CLongFloat&);
    bool operator >(CLongFloat&);
    bool operator <(CLongFloat&);

    __inline bool operator >=(CLongFloat& Value){return ((*this > Value)||(*this == Value));};
    __inline bool operator <=(CLongFloat& Value){return ((*this < Value)||(*this == Value));};

    /**
     * Свойство вещественной части числа, служебное предстваление вещественной части.
     */
    __property AnsiString Fraction = {read=GetFrac, write=SetFrac};

} CLongFloat;

/**
 * Специальный тип перечисления, допустимые операции контроллера
 *
 */
typedef enum ECalcOperation {
  coNop = 0 /* Пустая операция */, coAdd /* Сложение */, coSub  /* Вычитание */,
  coMul     /* Умножение */,       coDiv /* Деление */,  coTurn /* Обращение (1/x) */,
  coRet     /* Вычисление */
} ECalcOperation;

/**
 * Класс контроллера калькулятора.
 *
 * Реализует всю механику работы с числами, ввод, вычисления и вывод результата.
 * Для операций хранит в себе два операнда, состояние ввода и запланированную операцию.
 *
 */
typedef class CCalcController {
  private:

    /**
     * Левый операнд в операциях.
     * В этот же операнд заносится результат операций, это сделано для того,
     * чтоб после вычисления операции можно было продолжить ввод и дальнейшие вычисления.
     *
     */
    CLongFloat     FLeft;

    /**
     * Правый операнд в операциях.
     * Этот же операнд служит текущей областью ввода.
     *
     */
    CLongFloat     FRight;

    /**
     * Текущее состояние операции
     *
     */
    ECalcOperation FOperation;

    /**
     * Состояние области ввода
     *   false - область ввода - целая часть
     *   true  - область ввода - дробная часть
     *
     */
    bool           FFracInput;

  public:
    CCalcController();

    /**
     * добавление одной цифры в область текущего ввода
     */
    bool InputDigit(char);

    /**
     * Удаление одной цифры из текущей области ввода
     */
    bool RemoveDigit();

    /**
     * Сброс вводимого числа
     */
    bool ClearInput();

    /**
     * Полный сброс состояния калькулятора
     */
    bool Reset();

    /**
     * Изменяет область ввода, INT -> FRAC и FRAC -> INT
     */
    bool Turn2Frac();

    /**
     * Изменяет знак вводимого числа
     */
    bool TurnSign();

    /**
     * Устанавливает запланированную операцию
     */
    bool MakeOperation(ECalcOperation);

    /**
     * Производит запланированную операцию
     */
    bool MakeReturn();

    /**
     * Возвращает текущее состояние результата
     */
    AnsiString GetResult();

    /**
     * Возвращает текущее состояние области ввода
     */
    AnsiString Print();

    /**
     * Свойство флага состояния ввода
     */
    __property bool InFrac = {read = FFracInput};

} CCalcController;

/**
 * Для облегчения использования созданы короткие псевдонимы типов.
 */
typedef CLongUnsignedInteger longuint;
typedef CLongSignedInteger   longint;
typedef CLongFloat           longfloat;

//---------------------------------------------------------------------------
#endif
