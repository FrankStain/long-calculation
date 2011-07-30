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
 * Операция сложения двух целых чисел.
 *
 * Теория:
 *   Для сложения необходимо выравнять числа по правому краю, а затем поразрядно сложить их.
 *   Может получиться и такая ситуация, когда при сложении чисел одного разряда получится число больше 9
 *   В этом случае в сам разряд необходимо записать остаток от деления числа на 10, а само число
 *   разделить на 10 и сложить с суммоу чисел следующего разряда.
 *
 * Примечания:
 *   1- в символьной таблице символы цифр хранятся последовательно, от символа 0 к символу 9.
 *      В виду этого будет очень удобно получать числовые эквиваленты символов через их разницу
 *      с символом нуля. (пример: '9' - '0' == 9; т.к. '9' == 0x39, а '0' == 0x30)
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator +(CLongUnsignedInteger oValue){
  // результат операции.
  CLongUnsignedInteger oResult;
  // позиция первого разряда левого операнда
  int iLeftPos  = FValue.Length();
  // позиция первого разряда правого операнда
  int iRightPos = oValue.Value.Length();
  // буфер операции и буфер переполнения в одном лице
  int iOFlag    = 0;

  oResult.Value = "";
  // в пределах определения обоих операндов производим поразрядное сложение
  while(iLeftPos && iRightPos){
    int iLeft  = FValue[iLeftPos--] - '0';
    int iRight = oValue.Value[iRightPos--] - '0';

    // Вычисляем значение текущего разряда и записываем его в результат
    iOFlag += iLeft + iRight;
    oResult.PushDigit('0' + (char)(iOFlag % 10));
    // Корректируем значение с сохранением переполненниых разрядов
    iOFlag /= 10;
  };

  // остается только переписать неучтенные старшие разряды, если один из операндов длиннее второго.
  // разряды должны быть так же переписаны через буфер переполнения.
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
  // записываем остаток в буфере, если он есть
  if(iOFlag) oResult.PushDigit('0' + (char)iOFlag);

  // выполняем нормализацию
  oResult.Normalize();
  return oResult;
};

/**
 * Операция вычитания одного целого из другого.
 *
 * Теория:
 *   Операция аналогична сложению, за исключением одного ограничения.
 *   Если правый операнд больше левого, результат вычитания будет отрицательным, что недопустимо в данном классе.
 *
 * Примечания:
 *   1- Если правый операнд больше лоевого, генерируется исключение
 *   2- Для изучения деталей механики следует смотреть описания вышеизложенных методов
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator -(CLongUnsignedInteger oValue){
  // результат операции
  CLongUnsignedInteger oResult;

  // если правый операнд больше левого, генерируем исключение [catch(...)/catch(bool)]
  if(*this < oValue) throw false;

  // позиция младшего разряда левого операнда
  int iLeftPos   = FValue.Length();
  // подзиция младшего разряда правого операнда
  int iRightPos  = oValue.Value.Length();
  // буфер операции и буфер переполнения в одном лице
  int iOFlag;
  // флаг переполнения операции, нужен для операции заимствования из старшего разряда
  bool bOverflow = false;

  oResult.Value = "";
  // в пределах определения обоих операндов производим поразрядное вычитание
  while(iLeftPos && iRightPos){
    // достаем значение текущего разряда левого операнда,
    // корректируем его значением флага переполнения
    int iLeft  = (FValue[iLeftPos--] - '0') - (int)bOverflow;
    // достаем значение текущего разряда правого операнда
    int iRight = oValue.Value[iRightPos--] - '0';
    // сбрасываем флаг переполнения
    bOverflow  = false;

    // вычисляем значение в текущем разряде
    iOFlag = iLeft - iRight;
    // если результат меньше нуля, необходимо заимствование из старшего разряда
    if(0 > iOFlag){
      // имеется переполнение в старшем разряде
      bOverflow = true;
      // корректируем результат текущей операции
      iOFlag += 10;
    };
    // записываем значение буфера в разряд резултата операции
    oResult.PushDigit('0' + (char)(iOFlag % 10));
  };

  // правый операнд заведомо меньше левого
  // Остается только переписать старшие, еще неучтенные, разряды из левого операнда
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

  // небольшая предосторожность, если все таки было вывлено переполнение,
  // значит правый операнд все таки больше и необходимо сгененрировать исключение
  if(bOverflow) throw false;

  // выполняем нормализацию
  oResult.Normalize();
  return oResult;
};

/**
 * Операция перемножения двух целых чисел.
 *
 * Теория:
 *   Для перемножения двух чисел необходимо каждый разряд правого операнда умножить на левый операнд,
 *   получить набор чисел и через их сложение получить результат умножения.
 *   Благо операция сложения длинных чисел уже есть, а поразрядное умножение механически аналогично сложению.
 *
 *   На деле же, каждый результат поразрядного умножения складывается с результатом операции.
 *   Сам результат операции изначально равен нулю. Никакой проблемы хранения динамических данных.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенных методов.
 *   2- Операция имеет M*N трудоемкость, чем больше разрядов, тем дольше перемножать.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator *(CLongUnsignedInteger oValue){
  // результат вычислений
  CLongUnsignedInteger oResult;
  // буфер поразрядного умножения
  CLongUnsignedInteger oMulBuffer;
  // позиция младшего разряда левого операнда
  int iLeftPos   = FValue.Length();
  // позиция младшего разряда правого операнда
  int iRightPos  = oValue.Value.Length();
  // буфер поразрядной операции и буфер переполнения в одном лице
  int iOFlag;

  oResult.Value = "0";
  // каждый разряд правого операнда надо умножить на левый операнд
  while(iRightPos){
    // Достаем значение в текущем разряде правого операнда
    int iRight = oValue.Value[iRightPos--] - '0';
    // Сбрасываем значение буфера операции
    iOFlag     = 0;

    // если разряд нулевой, можно не делать рутину за зря и просто перейти к следующему разряду
    if(!iRight) continue;

    oMulBuffer.Value = "";
    // дополняем буфер умножения необходимым числом нулей, согласно позиции разряда правого операнда
    for(int iOffset = (oValue.Value.Length() - iRightPos - 1); 0 < iOffset; iOffset--) oMulBuffer.AddDigit('0');
    // производим поразрядное умножение
    for(int iLeftTrack = iLeftPos; 0 < iLeftTrack; iLeftTrack--){
      int iLeft  = FValue[iLeftTrack] - '0';

      // производим умножение для текущих разрядов и записываем результат в буфер
      iOFlag += iLeft * iRight;
      oMulBuffer.PushDigit('0' + (char)(iOFlag % 10));
      // Корректируем значение с сохранением переполненниых разрядов
      iOFlag /= 10;
    };

    // если есть переполнение разрядов, записываем его в буфер
    if(iOFlag) oMulBuffer.PushDigit('0' + (char)(iOFlag % 10));

    // производим сложение буфера с результатом операции
    oResult = oResult + oMulBuffer;
  };

  // выполняем нормализацию
  oResult.Normalize();
  return oResult;
};

/**
 * Операция деления двух целых чисел.
 *
 * Теория:
 *   Удобнее всего целочисленное деление реализовать через поэтапный сдвиг и вычитание.
 *   К томму же машинный механизм деления базируется на том же, но в побитовом выражении.
 *
 * Способ (изначально результат операции деления равен нулю):
 *   - Если левый операнд меньше правого, останавливаем деление и возвращаем результат операции.
 *   - Увеличиваем число разрядов правого операнда, пока тот не станет больше левого.
 *   - Уменьшаем число разрядов правого операнда на 1. Запоминаем разряды в счетчике разрядов.
 *   - Вычитаем правый операнд из левого, результат кладем в левый операнд.
 *   - Прибавляем к результату операции значение счетчика разрядов.
 *   - Восстанавливаем изначальное значение правого операнда.
 *   - Возвращаемся к первому пункту способа.
 *
 * Примечания:
 *   1- X / 0 -- генерируется исключение.
 *   2- Если левый операнд изначально меньше правого, операция делния не производится.
 *      Производится немедленный возврат заранее определнного результата ("0").
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator /(CLongUnsignedInteger oValue){
  // Результат операции
  CLongUnsignedInteger oResult;
  // Счетчик разрядов
  CLongUnsignedInteger oMulBuffer;
  // Буфер правого операнда для оперций с поразрядным сдвигом
  CLongUnsignedInteger oDivBuffer;
  // Левый операнд для изменений в течении вычислений
  CLongUnsignedInteger oLeft((CLongUnsignedInteger&)*this);

  oResult.Value = "0";
  // Если производится деление на ноль, то генерируется исключение
  if(oResult == oValue) throw false;

  // Проыеряем, больше ли левый операнд правого
  while(oLeft > oValue){
    // буфер для иъятия одного младшего разряда.
    // Одновременно - флаг наличия сдвига в правом операнде
    char cDigit;

    // Восстанавливаем изначальное значение правого операнда, счетчик разрядов - в 1, флаг сдвига сброшен
    oDivBuffer = (CLongUnsignedInteger&)oValue;
    oMulBuffer.Value = "1";
    cDigit = 0;
    // производим сдвиг разрядов, запоминаем число сдвигов, пока правый операнд меньше левого
    while(oDivBuffer < oLeft){
      oDivBuffer.AddDigit('0');
      oMulBuffer.AddDigit('0');
      cDigit = 1;
    };
    // [для безопасности] проверяем, были ли сдвиги в правом операнде
    // Если сдвигов небыло и правый операнд больше левого, останавливаем деление
    if(!cDigit && (oDivBuffer > oLeft)) break;
    // уменьшаем число разрядов правого операнда и счетчика разрядов на 1
    oDivBuffer.RemoveDigit(cDigit);
    oMulBuffer.RemoveDigit(cDigit);
    // производим вычитание операндов и суммирование результатов деления
    while(oLeft >= oDivBuffer){
      oLeft = oLeft - oDivBuffer;
      oResult = oResult + oMulBuffer;
    };
  };
  // [для безопасности] если правый операнд все еще больше левого, уменьшаем результат операции на 1
  //if(oDivBuffer > *this) oResult--;

  // Выполняем нормализацию
  oResult.Normalize();
  return oResult;
};

/**
 * Постфиксный инкремент. [ a++ ]
 *
 * Теория:
 *   Необходимо запомнить текущее значение числа, произвести инкремент
 *   и результатом вернуть запмненное значение.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator ++(int iType){
  // правый операнд, для инкремента
  CLongUnsignedInteger oIncr;
  // результат операции
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // если параметр эквивалентен нулю, значит вызвана постфиксная форма оператора.
  if(!iType){
    // запоминаем текущее значение
    oResult = *this;
    // увеличиваем собственное значение на 1
    *this = *this + oIncr;
  }else{
    oResult = *this = *this + oIncr;
  };

  // выполняем нормализацию
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Постфиксный декремент. [ a-- ]
 *
 * Теория:
 *   Необходимо запомнить текущее значение числа, произвести декремент
 *   и результатом вернуть запмненное значение.
 *
 * Примечания:
 *   1- "0"-- вызовет исключение. Никаких проверок не производится.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator --(int iType){
  // правый операнд, для декремента
  CLongUnsignedInteger oIncr;
  // результат операции
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // если параметр эквивалентен нулю, значит вызвана постфиксная форма оператора.
  if(!iType){
    // запоминаем текущее значение
    oResult = *this;
    // уменьшаем собственное значение на 1
    *this = *this - oIncr;
  }else{
    oResult = *this = *this - oIncr;
  };

  // выполняем нормализацию
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Префиксный инкремент. [ ++a ]
 *
 * Теория:
 *   Значение сперва изменяется и возвращается в уже измененном виде.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator ++(){
  // правый операнд, для инкремента
  CLongUnsignedInteger oIncr;
  // результат операции
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // инкремент и рекурсивное присваивание
  oResult = *this = *this + oIncr;

  // выполнение нормализации
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Префиксный декремент. [ --a ]
 *
 * Теория:
 *   Значение сперва изменяется и возвращается в уже измененном виде.
 *
 * Примечания:
 *   1- --"0" вызовет исключение. Никаких проверок не производится.
 *
 */
CLongUnsignedInteger CLongUnsignedInteger::operator --(){
  // правый операнд, для декремента
  CLongUnsignedInteger oIncr;
  // результат операции
  CLongUnsignedInteger oResult;

  oIncr.Value = "1";
  // декремент и рекурсивное присваивание
  oResult = *this = *this - oIncr;

  // выполнение нормализации
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Логическое сравнение эквивалентности.
 *
 * Теория:
 *   Достаточно просто сравнить два числа как две строки, в этом случае вскроется даже поразрядное неравенство.
 *
 */
bool CLongUnsignedInteger::operator ==(CLongUnsignedInteger& oValue){
  return 0 == strcmp(FValue.c_str(), oValue.Value.c_str());
};

/**
 * Логическое сравнение превосходства.
 *
 * Теория:
 *   Для решения задач логики превосходства и недостатка используется алгоритм разности операндов.
 *   При этом результат операции определяется сперва исходя из длин операндов,
 *   а потом - согласно значению флага переполнения.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенного оператора разности.
 *
 */
bool CLongUnsignedInteger::operator >(CLongUnsignedInteger& oValue){
  // позиция младшего разряда левого операнда
  int iLeftPos   = FValue.Length();
  // позиция младшего разряда правого операнда
  int iRightPos  = oValue.Value.Length();
  // флаг переполнения, true - означает наличие переполнения
  bool bOverflow = false;

  // сравнивать можно только равные по длинам числа.
  // другие варианты приводят к заведомо известным значениям
  if(iLeftPos > iRightPos) return true;
  if((iLeftPos < iRightPos)||(*this == oValue)) return false;
  // производим поразрядное вычитание и определние состояния переполнения разрядов.
  while(iLeftPos && iRightPos){
    int iResult = (int)(FValue[iLeftPos--] - '0') - (int)(oValue.Value[iRightPos--] - '0') - (int)bOverflow;
    bOverflow = 0 > iResult;
  };

  // Если переполнение есть, значит правый операнд меньше левого, операция должна вернуть true.
  // Иначе необходимо вернуть false.
  return !bOverflow;
};

/**
 * Логическое сравнение недостатка.
 *
 * Теория:
 *   Для решения задач логики превосходства и недостатка используется алгоритм разности операндов.
 *   При этом результат операции определяется сперва исходя из длин операндов,
 *   а потом - согласно значению флага переполнения.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенного оператора разности.
 *
 */
bool CLongUnsignedInteger::operator <(CLongUnsignedInteger& oValue){
  // позиция младшего разряда левого операнда
  int iLeftPos   = FValue.Length();
  // позиция младшего разряда правого операнда
  int iRightPos  = oValue.Value.Length();
  // флаг переполнения, true - означает наличие переполнения
  bool bOverflow = false;

  // сравнивать можно только равные по длинам числа.
  // другие варианты приводят к заведомо известным значениям
  if(iLeftPos < iRightPos) return true;
  if((iLeftPos > iRightPos)||(*this == oValue)) return false;
  // производим поразрядное вычитание и определние состояния переполнения разрядов.
  while(iLeftPos && iRightPos){
    int iResult = (int)(FValue[iLeftPos--] - '0') - (int)(oValue.Value[iRightPos--] - '0') - (int)bOverflow;
    bOverflow = 0 > iResult;
  };

  // Если переполнение есть, значит левый операнд меньше правого, операция должна вернуть true.
  // Иначе необходимо вернуть false.
  return bOverflow;
};

/**
 * Нормализация числа.
 *
 * Теория:
 *   Из числа просто надо удалить все нули слева. Не важно, каким образом они там могли оказаться.
 *
 */
void CLongUnsignedInteger::Normalize(){
  while((1 < FValue.Length())&&('0' == FValue[1])) FValue.Delete(1,1);
};

/**
 * Выдача служебного значения чивла.
 *
 */
AnsiString CLongUnsignedInteger::GetValue(){
  return FValue;
};

/**
 * Установка нового значения числа.
 *
 */
AnsiString CLongUnsignedInteger::SetValue(const AnsiString sValue){
  FValue = sValue;
  Normalize();
  return FValue;
};

/**
 * Добавление одного разряда слева
 *
 * Теория:
 *   Первый символ строки означает позицию самого старшего разряда числа.
 *   Необходимо просто сместить строку вправо и вставить на новый разряд в первый символ.
 *   Это делается стандартными средствами С++
 *
 */
void CLongUnsignedInteger::PushDigit(char cValue){
  AnsiString sOpValue(cValue);
  FValue.Insert(sOpValue,1);
};

/**
 * Изъятие одного разряда слева
 *
 * Теория:
 *   Из строки изымается первый символ и производится сдвиг строки в влево.
 *   Это делается стандартными средствами С++
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
 * Добавление одного разряда справа
 *
 * Теория:
 *   Последний символ строки - самый младший разряд, добавляется к хвосту строки.
 *   Это делается стандартными средствами С++
 *
 */
void CLongUnsignedInteger::AddDigit(char cValue){
  AnsiString sOpValue(cValue);
  if((FValue == "0") && (cValue != '0')) FValue = "";
  FValue.Insert(sOpValue,FValue.Length()+1);
};

/**
 * Изъятие одного разряда справа
 *
 * Теория:
 *   Из строки изымается самый последний символ, строка обрезается на 1 символ.
 *   Это делается стандартными средствами С++
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
 * Пользовательский вывод значения числа.
 *
 * Теория:
 *   Целое бесзнаковое число не обладает дополнительными свойствами,
 *   пользовательское и служебыное значения ни чем не отличаются.
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
 * Конструктор копирования.
 *
 * Теория:
 *   Конструктор копирования производного класса должен выполнять те же действия,
 *   что и конструктор копирования базового класса, для этого необходимо создать
 *   обратную наследственную связь. Нобходмио увязать конструкторы копирования между собой.
 *
 *   ...er& oValue): CLongUnsignedInteger(oValue),...
 *
 *   Данная увязка позволяет однозначно задать конструктор для инициализации облсти базового
 *   класса внутри области производного класса.
 *
 */
CLongSignedInteger::CLongSignedInteger(CLongSignedInteger& oValue): CLongUnsignedInteger(oValue), FSignFlag(oValue.Sign) {

};

/**
 * Обновленный метод пользовательского представления числа.
 *
 * Теория:
 *   Данный класс хранит число со знаком, положительное число отображается без знака
 *   Отрицательное число отображается со знаком "минус".
 *   Принято, что если FSignFlag установлено в значение true, значит число отрицательно.
 *   Следовательно знак минуса необходимо подставлять только когда FSignFlag == true.
 *
 * Примечания:
 *   1- Выполняется стандартными средствами С++
 */
AnsiString CLongSignedInteger::PrintText(){
  AnsiString sResult;

  sResult.printf("%s%s",((FSignFlag)? "-":""),GetValue());

  return sResult;
};

/**
 * Оператор присваивания.
 *
 * Теория:
 *   Параметр - это правый операнд, this - это левый операнд.
 *   Для обеспечения рекурсивного присваивания, оператор должен
 *   возвращать ссылку на значение, в частности - возвращать *this.
 *
 *   CLongUnsignedInteger::operator =((CLongUnsignedInteger&)oValue);
 *
 *   Для обеспечения совместимости с родительским классом производится вызов
 *   соответствующего оператора из пространства имен родительского класса.
 *
 */
CLongSignedInteger& CLongSignedInteger::operator =(CLongSignedInteger oValue){
  CLongUnsignedInteger::operator =((CLongUnsignedInteger&)oValue);
  FSignFlag = oValue.Sign;
  Normalize();
  return *this;
};

/**
 * Сложение двух знаковых чисел.
 *
 * Теория:
 *   Теперь числа обладают знаком и операция сложения усложнилось. Может возникнуть ситуация,
 *   кгда на деле прийдется вовсе не складывать, а вычитать.
 *
 * Алгоритм (сравнения по модулю, знак числа учитывается отдельно):
 *   - если оба положительны, выполняется сложение двух простых бесзнаковых, знак результата положителен
 *   - если оба отрицательны, выполняется сложение двух простых бесзнаковых, знак результата отрицателен
 *   - если отрицательно только одно из чисел, вычитание простых большего числа из меньшего,
 *     знак результата как у большего.
 *
 * Примечания:
 *   1- Вычисления оператора основываются на уже реализованных вычислениях бесзнаковых чисел
 *   2- Для изучения деталей механики следует смотреть описания вышеизложенного оператора разности.
 */
CLongSignedInteger CLongSignedInteger::operator +(CLongSignedInteger oValue){
  // результат операции
  CLongSignedInteger oResult;
  // левый и правый операнды, представление простого целого
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // если только один из операндов отрицателен, применяем сложную логику
  if((!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign)){
    // проверка терминальная, далее одно из чисел будет обязательно отрицательным
    // если левый операнд отрицателен
    if(FSignFlag){
      // если слева еще и большее число, знак результата будет отрицательным
      oResult.Sign = oLeft > oRight;
      // вычитается только меньшее из большего, которое число больше,
      // определяется по знаку результата
      if(!oResult.Sign) (CLongUnsignedInteger&)oResult = oRight - oLeft;
      else (CLongUnsignedInteger&)oResult = oLeft - oRight;
    }else{
      // левое положительно, значит правое точно отрицательно
      // и если правое число будет больше, результат будет отрицательным.
      oResult.Sign = oLeft < oRight;
      // вычитается только меньшее из большего, которое число больше,
      // определяется по знаку результата
      if(!oResult.Sign) (CLongUnsignedInteger&)oResult = oLeft - oRight;
      else (CLongUnsignedInteger&)oResult = oRight - oLeft;
    };
  }else{
    // делаем простое сложение и устанавливаем знак обоих операндов
    (CLongUnsignedInteger&)oResult = oLeft + oRight;
    oResult.Sign = FSignFlag || oValue.Sign;
  };

  // непременное выполнение нормализации
  oResult.Normalize();
  return oResult;
};

/**
 * Вычитание двух знаковых чисел.
 *
 * Теория:
 *   В случае со знаковыми числами, операция вычитания так же становится нелинейной. Появляется
 *   случай, когда вычитание становится сложением.
 *
 * Примечания:
 *   1- Вычисления оператора основываются на уже реализованных вычислениях бесзнаковых чисел.
 *   2- Для изучения деталей механики следует смотреть описания вышеизложенного оператора разности.
 *   3- Логически ликвидирована генерация исключений переполнения.
 *
 */
CLongSignedInteger CLongSignedInteger::operator -(CLongSignedInteger oValue){
  // результат операции
  CLongSignedInteger oResult;
  // левый и правый операнды, представление простого целого
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  if(oValue.Sign){
    // если правый операнд отрицателен,
    // получается ситуация А - -Б, которая преобразуется в А + Б
    CLongSignedInteger oModule(oValue);
    oModule.Sign = false;
    oResult = *this + oModule;
  }else{
    // справа положительное, надо проверить знак левого
    if(FSignFlag){
      // если слева отрицательное, значит производится сложение в отрицательной плоскости
      (CLongUnsignedInteger&)oResult = oLeft + oRight;
      oResult.Sign = true;
    }else{
      // если правое число будет больше, результат операции будет отрицательным.
      oResult.Sign = oLeft < oRight;
      // вычитается только меньшее из большего, которое число больше,
      // определяется по знаку результата
      if(!oResult.Sign) (CLongUnsignedInteger&)oResult = oLeft - oRight;
      else (CLongUnsignedInteger&)oResult = oRight - oLeft;
    };
  };

  // выполнение нормализации
  oResult.Normalize();
  return oResult;
};

/**
 * Операция умножения знаковых чисел.
 *
 * Теория:
 *   Механически операция ни чем не отличается от перемножения бесзнаковых чисел.
 *   Основное отличие перемножения чисел со знаком состоит в том, что знак результата
 *   становится отрицательным только тогда, когда отрицателен лиш один операнд.
 *
 * Примечания:
 *   1- Вычисления оператора основываются на уже реализованных вычислениях бесзнаковых чисел.
 *   2- Знак результата определяется по логике XOR, сами числа перемножаются как бесзнаковые.
 *
 *   Терминальная логика операции XOR:   XOR | 0 | 1 |
 *                                       ----+---+---+
 *                                         0 | 0 | 1 |
 *                                       ----+---+---+
 *                                         1 | 1 | 0 |
 *
 */
CLongSignedInteger CLongSignedInteger::operator *(CLongSignedInteger oValue){
  // результат операции
  CLongSignedInteger oResult;
  // левый и правый операнды, представление простого целого
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // выполняется перемножение простых
  (CLongUnsignedInteger&)oResult = oLeft * oRight;
  // определяется знак результата операции
  oResult.Sign = (!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign);

  // непременное выполнение нормализации, избыточное быть может
  oResult.Normalize();
  return oResult;
};

/**
 * Операция деления знаковых чисел.
 *
 * Теория:
 *   Аналогично механизму перемножения знаковых, деление знаковых так же ни чем
 *   не отличается от деления бесзнаковых.
 *
 * Примечания:
 *   1- Вычисления оператора основываются на уже реализованных вычислениях бесзнаковых чисел.
 *   2- Знак результата определяется по логике XOR, сами числа перемножаются как бесзнаковые.
 *   3- Для изучения деталей механики следует смотреть описания вышеизложенных операторов.
 *
 */
CLongSignedInteger CLongSignedInteger::operator /(CLongSignedInteger oValue){
  // результат оперции
  CLongSignedInteger oResult;
  // левый и правый операнды, представление простого целого
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // выполняется деление простых
  (CLongUnsignedInteger&)oResult = oLeft / oRight;
  // определяется знак результата операции
  oResult.Sign = (!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign);

  // непременное выполнение нормализации, избыточное быть может
  oResult.Normalize();
  return oResult;
};

/**
 * Постфиксный инкремент.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенных операторов.
 *
 */
CLongSignedInteger CLongSignedInteger::operator ++(int iType){
  // результат оперции
  CLongSignedInteger oResult;
  // буфер инкремента, единичка для сложения
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // постфиксный оператор вызывается с параметром, равным нулю
  switch(iType){
    case 0:
      // запоминаем текущее значение
      oResult = *this;
      // производим инкремент
      *this = *this + oIncr;
    break;
  };

  // выполнение нормализации обоих чисел, избыточное быть может
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Постфиксный декремент.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенных операторов.
 *
 */
CLongSignedInteger CLongSignedInteger::operator --(int iType){
  // результат оперции
  CLongSignedInteger oResult;
  // буфер декремента, единичка для вычитания
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // постфиксный оператор вызывается с параметром, равным нулю
  switch(iType){
    case 0:
      // запоминаем текущее значение
      oResult = *this;
      // производим декремент
      *this = *this - oIncr;
    break;
  };

  // эту строчку еще стоит описывать? - нормализацияяя... :)
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Префиксный инкремент.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенных операторов.
 *
 */
CLongSignedInteger CLongSignedInteger::operator ++(){
  // результат оперции
  CLongSignedInteger oResult;
  // буфер инкремента, единичка для сложения
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // рекурсивное присваивание, выполнение инкремента
  oResult = *this = *this + oIncr;

  // нормализация обоих чисел
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Префиксный декремент.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания вышеизложенных операторов.
 *
 */
CLongSignedInteger CLongSignedInteger::operator --(){
  // результат оперции
  CLongSignedInteger oResult;
  // буфер декремента, единичка для вычитания
  CLongSignedInteger oIncr;

  oIncr.Value = "1";
  // рекурсивное присваивание, выполнение декремента
  oResult = *this = *this - oIncr;

  // нормализация обоих чисел
  Normalize();
  oResult.Normalize();
  return oResult;
};

/**
 * Оператор логической эквивалентности.
 *
 * Примечания:
 *   1- Для изучения деталей механики следует смотреть описания оператора родитльского класса.
 *
 */
bool CLongSignedInteger::operator ==(CLongSignedInteger& oValue){
  return ((CLongUnsignedInteger&)*this == (CLongUnsignedInteger&)oValue)&&(FSignFlag == oValue.Sign);
};

/**
 * Оператор сравнения превосходства левого.
 *
 * Теория:
 *   Необходимо просто учесть знаки чисел.
 *   Учтя знаки чисел, сравнение можно реализовать через уже определенные операторы
 *   сравнения базового класса. Просто надо учесть специфику знака чисел.
 *
 *       7 > 4, но -7 < -4.
 *
 *   Просто снимаем оба минуса с чисел и меняем знак.
 */
bool CLongSignedInteger::operator >(CLongSignedInteger& oValue){

  // слева отрицательное, справа положительное, результат однозначно определен
  if(FSignFlag && !oValue.Sign) return false;
  // слева положительное, справа отрицательное, результат однозначен
  if(!FSignFlag && oValue.Sign) return true;
  // далее уже точно известно, что оба числа или положительны, или отрицательны

  // опеределяем левый и правый операнды для сравнения по модулю
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // если числа отрицательны, большим числом будет меньшее по модулю
  // если числа положительны, то сравнение производится как и обычно
  return (FSignFlag)? (oRight > oLeft):(oLeft > oRight);
};

/**
 * Оператор сравнения превосходства правого.
 *
 * Примечания:
 *   1- Операторы превосходства реализуются абсолютно идентично.
 *   2- Для изучения деталей механики следует смотреть описания оператора родитльского класса.
 *
 */
bool CLongSignedInteger::operator <(CLongSignedInteger& oValue){

  // слева положительно, справа отрицательное, результат однозначно определн
  if(!FSignFlag && oValue.Sign) return false;
  // слева отрицательное, справа положительное, результат так же однозначен
  if(FSignFlag && !oValue.Sign) return true;

  // опеределяем левый и правый операнды для сравнения по модулю
  CLongUnsignedInteger &oLeft  = (CLongUnsignedInteger&)*this;
  CLongUnsignedInteger &oRight = (CLongUnsignedInteger&)oValue;

  // если числа отрицательны, меньшим числом будет большее по модулю
  // если числа положительны, то сравнение производится как и обычно
  return (FSignFlag)? (oRight < oLeft):(oLeft < oRight);
};

//--- CLongFloat ------------------------------------------------------------
CLongFloat::CLongFloat(): FFrac("") {

};

/**
 * Конструктор копирования.
 *
 * Теория:
 *   Конструктор наследует логику конструктора копирования знаковых чисел.
 *
 */
CLongFloat::CLongFloat(CLongFloat& oValue): CLongSignedInteger((CLongSignedInteger&)oValue) {
  SetFrac(oValue.FFrac);
};

/**
 * Оператор присваивания для вещественных чисел.
 *
 * Теория:
 *   Область родительского класса (целая часть и знак) определяется через вызов оператора этого класса.
 *   Определяется вещественная часть числа.
 *   В любом случае необходимо сделать нормализацию.
 *
 */
CLongFloat& CLongFloat::operator =(CLongFloat oValue){
  // вызываем оператор родительского класса
  CLongSignedInteger::operator =((CLongSignedInteger)oValue);
  // устанавливаем значение дробной части
  SetFrac(oValue.Fraction);

  // выполняем нормализацию
  Normalize();
  return *this;
};

/**
 * Оператор сложения двух вещественных.
 *
 * Теория:
 *   Для сложения чисел используется предварительное выравнивание дробных частей.
 *   На бумаге это выглядело бы так: (складываем 36.005 и 3.06)
 *     Чтоб выравнять дробные части, домножаем левую и правую части на 1000,
 *     получаем 36005 + 3060.
 *     Вычисляем 36005 + 3060 == 39065; делим 39065 на 1000 и получаем
 *     окончательный результат -- 39.065
 *
 * Примечания:
 *   1- Операция полностью базируется на CLognSignedInteger::operator +()
 *
 */
CLongFloat CLongFloat::operator +(CLongFloat oValue){
  // результат операции
  CLongFloat oResult;
  // Дубликаты дробных частей операндов, нужны только для выравнивания
  CLongFloat oLeftFrac;
  CLongFloat oRightFrac;
  // левый и правый операнды
  CLongSignedInteger oLeft = (CLongSignedInteger&)*this;
  CLongSignedInteger oRight = (CLongSignedInteger&)oValue;
  // порядок выравнивания дробных разрядов
  int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

  // Переписываем дроби в целые части чисел
  oLeftFrac.Fraction = FFrac;
  oRightFrac.Fraction = oValue.FFrac;

  // выравниваем числа по порядку дробных частей.
  for(int iTrack = 0; iTrack < iMaxFracLength; iTrack++){
    char cDigit;
    // вынимаем старший разряд из дроби
    oLeftFrac.PopFrac(cDigit);
    // если разряд вынулся, кладем его в младший разряд целого
    // если не, кладем туда ноль
    oLeft.AddDigit((cDigit)? cDigit:'0');
    // синхронно и аналогично поступаем и с правым операндом
    oRightFrac.PopFrac(cDigit);
    oRight.AddDigit((cDigit)? cDigit:'0');
  };

  // операнды выравнены, теперь их достаточно только сложить
  // операнды складываются как простые знаковые числа
  (CLongSignedInteger&)oResult = oLeft + oRight;
  // а после сложения достаточно только вернуть дробные разряды на место
  while(iMaxFracLength--){
    char cDigit;
    // достаем младший разряд из целой
    oResult.RemoveDigit(cDigit);
    // и кладем его в старший разряд дробной части
    // если разряд не достался, кладем в дробь нолик
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // выполнение нормализации
  oResult.Normalize();
  return oResult;
};

/**
 * Оператор разницы двух вещественных.
 *
 * Теория:
 *   Сложение и вычитание делаются полностью одинаково.
 *
 * Примечания:
 *   1- Операция полностью базируется на CLognSignedInteger::operator -()
 *
 */
CLongFloat CLongFloat::operator -(CLongFloat oValue){
  // результат операции
  CLongFloat oResult;
  // Дубликаты дробных частей операндов, нужны только для выравнивания
  CLongFloat oLeftFrac;
  CLongFloat oRightFrac;
  // левый и правый операнды
  CLongSignedInteger oLeft = (CLongSignedInteger&)*this;
  CLongSignedInteger oRight = (CLongSignedInteger&)oValue;
  // порядок выравнивания дробных разрядов
  int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

  // Переписываем дроби в целые части чисел
  oLeftFrac.Fraction = FFrac;
  oRightFrac.Fraction = oValue.FFrac;

  // выравниваем числа по порядку дробных частей.
  for(int iTrack = 0; iTrack < iMaxFracLength; iTrack++){
    char cDigit;
    // вынимаем старший разряд из дроби
    oLeftFrac.PopFrac(cDigit);
    // если разряд вынулся, кладем его в младший разряд целого
    // если не, кладем туда ноль
    oLeft.AddDigit((cDigit)? cDigit:'0');
    // синхронно и аналогично поступаем и с правым операндом
    oRightFrac.PopFrac(cDigit);
    oRight.AddDigit((cDigit)? cDigit:'0');
  };

  // операнды выравнены, теперь их достаточно только сложить
  // операнды складываются как простые знаковые числа
  (CLongSignedInteger&)oResult = oLeft - oRight;
  // а после сложения достаточно только вернуть дробные разряды на место
  while(iMaxFracLength--){
    char cDigit;
    // достаем младший разряд из целой
    oResult.RemoveDigit(cDigit);
    // и кладем его в старший разряд дробной части
    // если разряд не достался, кладем в дробь нолик
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // выполнение нормализации
  oResult.Normalize();
  return oResult;
};

/**
 * Оператор умножения дробных
 *
 * Теория:
 *   Умножение двух дробных чисел можно сделать через умножение целых,
 *   выравнив добные части по разрядам. Но это решение будет требовать предела точночти.
 *   Еще умножение можно представить в виде операции сложения:
 *
 *     aa.bb * xx.yy == (aa*xx) + (bb*yy) + (aa*yy) + (xx*bb)
 *
 *   Таким образом, можно выделить 3 подоперации умножения:
 *   - умножение двух целых        (уже реализовано в родительском классе)
 *   - умножение двух дробных      (MulFracFrac)
 *   - умножение целого на дробное (MulIntFrac)
 *
 *   Эти подоперации реализованы в виде приватных методов класса.
 *
 */
CLongFloat CLongFloat::operator *(CLongFloat oValue){
  // результат операции
  CLongFloat oResult;

  // заносим в результат значение умножения целых частей
  (CLongSignedInteger&)oResult = (CLongSignedInteger&)*this * (CLongSignedInteger&)oValue;
  // добавляем результат умножения целого слева на дробное справа
  oResult = oResult + MulIntFrac(FSignFlag, GetValue(), oValue.Sign, oValue.Fraction);
  // добавляем результат умножения целого справа на дробное слева
  oResult = oResult + MulIntFrac(oValue.Sign, oValue.Value, FSignFlag, FFrac);
  // добавляем результат перемножения двух дробных
  oResult = oResult + MulFracFrac(FSignFlag, FFrac, oValue.Sign, oValue.Fraction);

  // выполняем нормализацию результата
  oResult.Normalize();
  return oResult;
};

/**
 * Операция деления дробных.
 *
 * Теория:
 *   Операция деления является бесконечно рекурсивной. С каждым добавленным
 *   знаком после запятой повышается точность результата. Но при этом и время
 *   вычисления нелинейно увеличивается. Чтоб добиться оптимального времени и
 *   точности вычисления, необходимо использовать так называемый "предел точности".
 *
 *   D-точности - это максимальное число дробных разрядов, в пределах которых будет
 *   производиться вычисление деления. Число определено как макрос "LF_FRACDELTA".
 *
 * Способ:
 *   - постепенно умножаем оба операнда на 10, пока дробные части не обнулятся у обоих операндов
 *   - запоминаем количество умножений операндов в счетчик умножений
 *   - умножаем левый операнд на 10, пока он не станет больше правого
 *   - запоминаем количество этих умножений и суммируем со счетчиком умножений
 *   - умножаем левый операнд на 10 LF_FRACDELTA раз, таким образом будет учтен предел точности
 *   - увеличиваем счетчик умножений на LF_FRACDELTA
 *   - определяем большее число разрядов дробной части из левого и правого операндов
 *   - вычитаем из счетчика это число разрядов (компенсация деления дробных частей)
 *   - производим деление двух целых знаковых
 *   - постепенно уменьшая счетчик умножений, делим полученный результат на 10
 *   - возвращаем значение операции
 *
 */
CLongFloat CLongFloat::operator /(CLongFloat oValue){
  // результат операции
  CLongFloat oLeft;
  // левый и правый операнды для операции деления
  CLongFloat oRight;
  CLongFloat oResult;
  // счетчик умножений
  int iFracLength = 0;

  // определяем левый и правый операнды
  oLeft = *this;
  oRight = oValue;

  // выравниваем дробные в целые части операндов, постепенно умножая операнды на 10
  // это делается через простой переброс цыфры из старшего разряда дробной в младший разряд целой части
  while(((oLeft.Fraction != "0") || (oRight.Fraction != "0"))&&(oLeft.Fraction.Length() || oRight.Fraction.Length())){
    char cDigit;
    // достаем старший разряд левого дробного
    oLeft.PopFrac(cDigit);
    // кладем его, если есть, в младший разряд левого целого, если нет, то кладем 0
    oLeft.AddDigit((cDigit)? cDigit:'0');
    // достаем старший разряд правого дробного
    oRight.PopFrac(cDigit);
    // кладем его, если есть, в младший разряд правого целого, если нет, то кладем 0
    oRight.AddDigit((cDigit)? cDigit:'0');
    // так запоминается чило умножений
    iFracLength++;
  };

  // делаем так, чтоб левое стало больше правого
  // заёмные разряды так же складываются в счетчик
  while((CLongUnsignedInteger&)oLeft < (CLongUnsignedInteger&)oRight){
    oLeft.AddDigit('0');
    iFracLength++;
  };

  // добавляем к левому операнду LF_FRACDELTA нулей, так будет учтена дельта точности
  for(int iTrack = 0; iTrack < LF_FRACDELTA; iTrack++) oLeft.AddDigit('0');
  // корректируем счетчик умножений с учетом максимальной длины дробной части операндов и дельты точности
  iFracLength += (LF_FRACDELTA - ((FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length()));

  // производим стандартное деление двух огромных целых чисел
  (CLongUnsignedInteger&)oResult = (CLongUnsignedInteger&)oLeft / (CLongUnsignedInteger&)oRight;

  // восстанавливаем положение дробной части результата
  // для этого просто изымаем столько младших разрядов целой части,
  // сколько насчитал счетчик умножений, и ставим их в старшие разряды
  // дробной части.
  while(iFracLength--){
    char cDigit;
    oResult.RemoveDigit(cDigit);
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // выполняем непременную нормализацию результата
  oResult.Normalize();
  // определяем знак результата, хотя скорее всего эта операция избыточна.
  oResult.Sign = (!FSignFlag && oValue.Sign)||(FSignFlag && !oValue.Sign);
  return oResult;
};

/**
 * Перемножение целого на десятичную дробь. ( aa * 0.bb )
 *
 * Теория:
 *   Дробную часть всегда можно представить как целое, разделенное на порядок числа 10,
 *   и это очень хорошо! Фактически, для перемножения целого на дробную часть
 *   дробь можно представить в виде такого же целого, перемножить их как два целых,
 *   а потом просто сделать поэтапное делние результата на 10, до тех пор, пока порядок
 *   дробной части результата не восстановится на уровне порядка дробного операнда.
 *
 * Примечания:
 *   1- дробное число передается без точки и нулевой целой части, но с нулями слева (если есть).
 *
 */
CLongFloat CLongFloat::MulIntFrac(bool bLSign, AnsiString& sLInt, bool bRSign, AnsiString& sRFrac){
  // левый и правый операнды для перемножения
  CLongUnsignedInteger oLeft;
  CLongUnsignedInteger oRight;
  // результат операции
  CLongFloat oResult;

  // устанавливаем значения операндов
  oLeft.Value = sLInt;
  oRight.Value = (sRFrac.Length())? sRFrac:AnsiString("0");

  // производим обычное целочисленное умножение
  (CLongUnsignedInteger&)oResult = oLeft * oRight;
  // корректируем длину результата, если разрядов меньше необходимого
  while(sRFrac.Length() > oResult.Value.Length()) oResult.PushDigit('0');
  oResult.Fraction = "";
  // переписываем разряды дробной части на их места
  // в дробной части должно быть столько разрядов, сколько их в дробном операнде
  while(sRFrac.Length() > oResult.Fraction.Length()){
    char cDigit;
    oResult.RemoveDigit(cDigit);
    oResult.PushFrac((cDigit)? cDigit:'0');
  };

  // определяем знак результата операции
  oResult.Sign = (!bLSign && bRSign)||(bLSign && !bRSign);
  return oResult;
};

/**
 * Перемножение двух дробных частей (0.aa * 0.bb)
 *
 * Теория:
 *   Дробные можно перемножить как два целых, а результат операции просто выравнять
 *   по числу разрядов суммы дробных разрядов обоих операндов.
 *   Это полностью эквивалентно поэтапному перемножению сперва числителей дробей, потом знаменателей.
 *
 * Примечания:
 *   1- дробное число передается без точки и нулевой целой части, но с нулями слева (если есть).
 *
 */
CLongFloat CLongFloat::MulFracFrac(bool bLSign, AnsiString& sLFrac, bool bRSign, AnsiString& sRFrac){
  // левый и праый операнды для перемножения
  CLongUnsignedInteger oLeft;
  CLongUnsignedInteger oRight;
  // чило дробных разрядов для выравнивания (сумма длин разрядов обоих операндов)
  int iFractLength = sLFrac.Length() + sRFrac.Length();
  // результат операции
  CLongFloat oResult;

  // устанавливаем значения операндов
  oLeft.Value = (sLFrac.Length())? sLFrac:AnsiString("0");
  oRight.Value = (sRFrac.Length())? sRFrac:AnsiString("0");

  // производим операцию умножения (фактически это перемножение числителей)
  (CLongUnsignedInteger&)oResult = oLeft * oRight;
  // добавляем слева к результату столько нулей, сколько нехватает
  // (эта операция эквивалентна перемножению знаменателей)
  while(iFractLength > oResult.Value.Length()) oResult.PushDigit('0');
  // переносим все разряды в область дроби
  oResult.Fraction = oResult.Value;
  // обнуляем целую часть (вычисления ранее велись в целой части)
  oResult.Value = "0";

  // устанавливаем знак операции
  oResult.Sign = (!bLSign && bRSign)||(bLSign && !bRSign);
  return oResult;
};

/**
 * Оператор постфиксного инкремента
 *
 * Примечания:
 *  1- операция выполняется только над целой частью.
 *
 */
CLongFloat CLongFloat::operator ++(int iType){
  CLongFloat oResult = *this;
  ((CLongSignedInteger&)*this).operator ++(iType);
  return oResult;
};

/**
 * Оператор постфиксного декремента
 *
 * Примечания:
 *  1- операция выполняется только над целой частью.
 *
 */
CLongFloat CLongFloat::operator --(int iType){
  CLongFloat oResult = *this;
  ((CLongSignedInteger&)*this).operator --(iType);
  return oResult;
};

/**
 * Оператор инфиксного инкремента
 *
 * Примечания:
 *  1- операция выполняется только над целой частью.
 *
 */
CLongFloat CLongFloat::operator ++(){
  ((CLongSignedInteger&)*this).operator ++();
  return *this;
};

/**
 * Оператор инфиксного декремента
 * 
 * Примечания:
 *  1- операция выполняется только над целой частью.
 *
 */
CLongFloat CLongFloat::operator --(){
  ((CLongSignedInteger&)*this).operator --();
  return *this;
};

/**
 * Оператор сравнения эквивалентности
 *
 * Примечания:
 *  1- Реализуется стандартными методами.
 *
 */
bool CLongFloat::operator ==(CLongFloat& oValue){
  return CLongSignedInteger::operator ==((CLongSignedInteger&)oValue) && (0 == strcmp(FFrac.c_str(), oValue.Fraction.c_str()));
};

/**
 * Оператор сравнения превосходства левого
 *
 */
bool CLongFloat::operator >(CLongFloat& oValue){
  // флаг превосходства дробной части
  bool bOverflow = false;

  // левый операнд сравнения, может меняться
  CLongSignedInteger oLeft(*this);

  if((FFrac.Length() && oValue.Fraction.Length()) && ((FFrac != "0") && (oValue.Fraction != "0"))){
    // левый и правый операнды для сравнения дробных частей
    CLongSignedInteger oLeftFrac;
    CLongSignedInteger oRightFrac;
    // порядок выравнивания разрядов
    int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

    // сперва операция над дробными частями, дроби заносим в операнды вместе со знаками
    oLeftFrac.Value = FFrac;
    oRightFrac.Value = oValue.FFrac;
    oLeftFrac.Sign = FSignFlag;
    oRightFrac.Sign = oValue.Sign;
    // производим выравнивание обоих операндов по разрядам
    // если было 0.87 и 0.022, будет 870 и 22
    if((iMaxFracLength > FFrac.Length()) && (oLeftFrac.Value != "0"))
      while(iMaxFracLength > oLeftFrac.Value.Length()) oLeftFrac.AddDigit('0');
    if((iMaxFracLength > oValue.Fraction.Length()) && (oRightFrac.Value != "0"))
      while(iMaxFracLength > oRightFrac.Value.Length()) oRightFrac.AddDigit('0');

    // производим сравнение операндов
    bOverflow = oLeftFrac > oRightFrac;
  };

  // если левая дробь больше правой, увеличиваем целую часть левого
  if(bOverflow) oLeft++;

  // производим сравнение целых частей
  return oLeft > (CLongSignedInteger&)oValue;
};

/**
 * Оператор сравнения превосходства правого
 *
 *
 */
bool CLongFloat::operator <(CLongFloat& oValue){
  // флаг превосходства дробной части
  bool bOverflow = false;

  // левый операнд сравнения, может меняться
  CLongSignedInteger oLeft(*this);

  if((FFrac.Length() && oValue.Fraction.Length()) && ((FFrac != "0") && (oValue.Fraction != "0"))){
    // левый и правый операнды для сравнения дробных частей
    CLongSignedInteger oLeftFrac;
    CLongSignedInteger oRightFrac;
    // порядок выравнивания разрядов
    int iMaxFracLength = (FFrac.Length() > oValue.Fraction.Length())? FFrac.Length():oValue.Fraction.Length();

    // сперва операция над дробными частями, дроби заносим в операнды
    oLeftFrac.Value = FFrac;
    oRightFrac.Value = oValue.FFrac;
    oLeftFrac.Sign = FSignFlag;
    oRightFrac.Sign = oValue.Sign;
    // производим выравнивание обоих операндов по разрядам
    // если было 0.87 и 0.022, будет 870 и 22
    if((iMaxFracLength > FFrac.Length()) && (oLeftFrac.Value != "0"))
      while(iMaxFracLength > oLeftFrac.Value.Length()) oLeftFrac.AddDigit('0');
    if((iMaxFracLength > oValue.Fraction.Length()) && (oRightFrac.Value != "0"))
      while(iMaxFracLength > oRightFrac.Value.Length()) oRightFrac.AddDigit('0');

    // производим сравнение операндов
    bOverflow = oLeftFrac < oRightFrac;
  };

  // если левая дробь меньше правой, уменьшаем левый операнд
  if(bOverflow) oLeft--;

  // производим сравнение целых частей
  return oLeft < (CLongSignedInteger&)oValue;
};

/**
 * Обновленный метод нормализации числа
 *
 */
void CLongFloat::Normalize(){
  CLongSignedInteger::Normalize();
  NormalizeFrac();
};

/**
 * Нормализация дробной части числа
 *
 */
void CLongFloat::NormalizeFrac(){
  while((FFrac.Length())&&('0' == FFrac[FFrac.Length()])) FFrac.Delete(FFrac.Length(),1);
};

/**
 * Возвращает служебное значение дробной части
 *
 */
AnsiString CLongFloat::GetFrac(){
  return FFrac;
};

/**
 * Устанавливает значеине дробной части
 *
 */
AnsiString CLongFloat::SetFrac(const AnsiString sValue){
  FFrac = sValue;
  NormalizeFrac();
  return FFrac;
};

/**
 * Возвращает пользовательское значение дробного числа
 *
 */
AnsiString CLongFloat::PrintText(){
  AnsiString sResult;

  //Normalize();
  // если дробной части нет, то и выводить ее просто незачем
  if(FFrac.Length())
    sResult.printf("%s%s.%s",((FSignFlag)? "-":""),GetValue(),GetFrac());
  else
    sResult.printf("%s%s",((FSignFlag)? "-":""),GetValue());

  return sResult;
};

/**
 * Добавляет в дробную часть один разряд слева (старший)
 *
 */
void CLongFloat::PushFrac(char cValue){
  AnsiString sOpValue(cValue);
  FFrac.Insert(sOpValue,1);
};

/**
 * Изымает один разряд слева из дробной части
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
 * Добавляет в дробную часть один разряд справа (младший)
 *
 */
void CLongFloat::AddFrac(char cValue){
  AnsiString sOpValue(cValue);
  FFrac.Insert(sOpValue,FFrac.Length()+1);
};
  
/**
 * Изымает один разряд справа из дробной части
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
 * Ввод одной цифры
 *
 */
bool CCalcController::InputDigit(char cDigit){
  try{
    // проверяе, а цифра ли это вобще?
    if((0 > (int)(cDigit - '0'))||(9 < (int)(cDigit - '0'))) throw false;
    // записываем цифру туда, где сейчас остановлен ввод (целая или дробная части)
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
 * Удаление одной цифры
 *
 */
bool CCalcController::RemoveDigit(){
  try{
    char cDigit;

    // просто удаляем самую правую цифру из области ввода - самый младший разряд
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
 * Очищаем текущее состояние ввода
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
 * Полный сброс калькулятора
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
 * Переход состояния ввода с целой части на дробную
 *
 */
bool CCalcController::Turn2Frac(){
  FFracInput = !FFracInput;
  return FFracInput;
};

/**
 * Вычисление операции над числами
 *
 * Теория:
 *   Результат операции будет записан в левый операнд, таким образом
 *   после вычисления одной операции можно будет ввести новую, продолжить
 *   ввод нового числа и сделать новые вычисления с текущим результатом.
 *
 */
bool CCalcController::MakeReturn(){
  try{
    // производим допустимую операцию
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
      // недопустимые операции приводят к исключению
      default:
        throw false;
    };
    // очищаем текущий ввод
    ClearInput();
    // ставим состояние "Вычислено"
    FOperation = coRet;
  }catch(...){
    return false;
  };
  return true;
};

/**
 * Определение операции
 *
 * Теория:
 *   Операции в калькулятор можно вводить последовательно:
 *   число - операция - число - опреация - число - операция - ...
 *   То есть, в момент ввода операции надо проверить, запланирована ли уже
 *   какая либо еще опреация. И если есть запланированная операция, надо
 *   сперва сделать вычисление и только потом установить новую операцию в план.
 *
 */
bool CCalcController::MakeOperation(ECalcOperation eOpCode){
  // флаг удачной операции
  bool bOpResult = false;

  // если есть операция, ее необходимо вычислить
  if(FOperation){
    bOpResult = MakeReturn();
  }else try{
    // если операций нет, надо просто перекинуть число в левую часть
    FLeft = FRight;
    // и очистить область ввода
    ClearInput();
    bOpResult = true;
  }catch(...){
    // на случай ошибок вычислений
    return false;
  };

  // операция обращения должна вычисляться тут же
  if(coTurn == eOpCode){
    try{
      CLongFloat oBase;
      oBase.Value = "1";
      FLeft = oBase / FLeft;
    }catch(...){
      return false;
    };
  // иначе надо просто установить планируемую операцию
  }else FOperation = eOpCode;

  return bOpResult;
};

/**
 * Смена знака числа в текущем вводе
 *
 */
bool CCalcController::TurnSign(){
  FRight.Sign = !FRight.Sign;
  return true;
};

/**
 * Возврат результата операций
 *
 */
AnsiString CCalcController::GetResult(){
  return FLeft;
};

/**
 * Возврат текущего значения ввода
 *
 */
AnsiString CCalcController::Print(){
  return FRight;
};

//---------------------------------------------------------------------------

#pragma package(smart_init)
