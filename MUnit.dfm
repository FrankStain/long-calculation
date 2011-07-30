object MForm: TMForm
  Left = 214
  Top = 315
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Калькулятор длинных чисел'
  ClientHeight = 235
  ClientWidth = 425
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object STValue: TStaticText
    Left = 5
    Top = 5
    Width = 415
    Height = 16
    Alignment = taRightJustify
    AutoSize = False
    BorderStyle = sbsSunken
    Caption = 'STValue'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    ShowAccelChar = False
    TabOrder = 0
  end
  object GBNumbers: TGroupBox
    Left = 5
    Top = 80
    Width = 216
    Height = 152
    TabOrder = 1
    object Button1: TButton
      Tag = 7
      Left = 5
      Top = 10
      Width = 65
      Height = 30
      Caption = '7'
      TabOrder = 0
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button2: TButton
      Tag = 8
      Left = 75
      Top = 10
      Width = 65
      Height = 30
      Caption = '8'
      TabOrder = 1
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button3: TButton
      Tag = 9
      Left = 145
      Top = 10
      Width = 65
      Height = 30
      Caption = '9'
      TabOrder = 2
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button4: TButton
      Tag = 6
      Left = 145
      Top = 45
      Width = 65
      Height = 30
      Caption = '6'
      TabOrder = 3
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button5: TButton
      Tag = 5
      Left = 75
      Top = 45
      Width = 65
      Height = 30
      Caption = '5'
      TabOrder = 4
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button6: TButton
      Tag = 4
      Left = 5
      Top = 45
      Width = 65
      Height = 30
      Caption = '4'
      TabOrder = 5
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button7: TButton
      Tag = 3
      Left = 145
      Top = 80
      Width = 65
      Height = 30
      Caption = '3'
      TabOrder = 6
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button8: TButton
      Tag = 2
      Left = 75
      Top = 80
      Width = 65
      Height = 30
      Caption = '2'
      TabOrder = 7
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button9: TButton
      Tag = 1
      Left = 5
      Top = 80
      Width = 65
      Height = 30
      Caption = '1'
      TabOrder = 8
      TabStop = False
      OnClick = OnAddDigit
    end
    object Button10: TButton
      Left = 145
      Top = 115
      Width = 65
      Height = 30
      Caption = ','
      TabOrder = 9
      TabStop = False
      OnClick = OnChangeFracState
    end
    object Button11: TButton
      Left = 75
      Top = 115
      Width = 65
      Height = 30
      Caption = '000'
      TabOrder = 10
      TabStop = False
      OnClick = OnAdd3Zero
    end
    object Button12: TButton
      Left = 5
      Top = 115
      Width = 65
      Height = 30
      Caption = '0'
      TabOrder = 11
      TabStop = False
      OnClick = OnAddDigit
    end
  end
  object GBOperators: TGroupBox
    Left = 225
    Top = 80
    Width = 193
    Height = 152
    TabOrder = 2
    object Button13: TButton
      Tag = 1
      Left = 10
      Top = 10
      Width = 85
      Height = 30
      Caption = '+'
      TabOrder = 1
      TabStop = False
      OnClick = OnOperation
    end
    object Button14: TButton
      Tag = 2
      Left = 100
      Top = 10
      Width = 85
      Height = 30
      Caption = '-'
      TabOrder = 2
      TabStop = False
      OnClick = OnOperation
    end
    object Button17: TButton
      Tag = 4
      Left = 100
      Top = 45
      Width = 85
      Height = 30
      Caption = '/'
      TabOrder = 3
      TabStop = False
      OnClick = OnOperation
    end
    object Button18: TButton
      Tag = 3
      Left = 10
      Top = 45
      Width = 85
      Height = 30
      Caption = '*'
      TabOrder = 4
      TabStop = False
      OnClick = OnOperation
    end
    object Button20: TButton
      Tag = 5
      Left = 100
      Top = 80
      Width = 85
      Height = 30
      Caption = '1/x'
      TabOrder = 5
      TabStop = False
      OnClick = OnOperation
    end
    object Button21: TButton
      Left = 10
      Top = 80
      Width = 85
      Height = 30
      Caption = '±'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      TabStop = False
      OnClick = OnTurnSign
    end
    object Button24: TButton
      Left = 10
      Top = 115
      Width = 175
      Height = 30
      Caption = '='
      TabOrder = 0
      TabStop = False
      OnClick = OnReturn
    end
  end
  object GroupBox1: TGroupBox
    Left = 80
    Top = 30
    Width = 338
    Height = 46
    TabOrder = 3
    object Button15: TButton
      Left = 60
      Top = 10
      Width = 90
      Height = 30
      Caption = 'Reset'
      TabOrder = 0
      TabStop = False
      OnClick = OnResetClicked
    end
    object Button16: TButton
      Left = 155
      Top = 10
      Width = 85
      Height = 30
      Caption = 'Backspace'
      TabOrder = 1
      TabStop = False
      OnClick = OnBackspaceClicked
    end
    object Button19: TButton
      Left = 245
      Top = 10
      Width = 85
      Height = 30
      Caption = 'Clear Input'
      TabOrder = 2
      TabStop = False
      OnClick = OnClearClicked
    end
    object STFIntFrac: TStaticText
      Left = 5
      Top = 10
      Width = 50
      Height = 30
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'INT'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      ShowAccelChar = False
      TabOrder = 3
    end
  end
  object MainMenu: TMainMenu
    Left = 5
    Top = 30
    object N1: TMenuItem
      Caption = 'Правка'
      object N7: TMenuItem
        Caption = 'Копировать'
        ShortCut = 16451
        OnClick = N7Click
      end
      object N8: TMenuItem
        Caption = 'Вставить'
        ShortCut = 16470
        OnClick = N8Click
      end
    end
    object N2: TMenuItem
      Caption = 'Вид'
      Enabled = False
    end
    object N3: TMenuItem
      Caption = 'Справка'
      object N4: TMenuItem
        Caption = 'Выхов справки'
        ShortCut = 16496
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object N6: TMenuItem
        Caption = 'О программе'
        OnClick = N6Click
      end
    end
  end
end
