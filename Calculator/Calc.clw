; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCalcDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Calc.h"

ClassCount=3
Class1=CCalcApp
Class2=CCalcDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CALC_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_CALC_DIALOG (English (U.S.))
Resource6=IDR_MENU1

[CLS:CCalcApp]
Type=0
HeaderFile=Calc.h
ImplementationFile=Calc.cpp
Filter=N

[CLS:CCalcDlg]
Type=0
HeaderFile=CalcDlg.h
ImplementationFile=CalcDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_EXIT

[CLS:CAboutDlg]
Type=0
HeaderFile=CalcDlg.h
ImplementationFile=CalcDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_CALC_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CCalcDlg

[DLG:IDD_CALC_DIALOG (English (U.S.))]
Type=1
Class=CCalcDlg
ControlCount=20
Control1=IDC_EDIT_SHOW,edit,1350631562
Control2=IDB_NUM7,button,1342242816
Control3=IDB_NUM8,button,1342242816
Control4=IDB_NUM9,button,1342242816
Control5=IDB_SQUARE,button,1342242816
Control6=IDB_CLEAR,button,1342242816
Control7=IDB_NUM4,button,1342242816
Control8=IDB_NUM5,button,1342242816
Control9=IDB_NUM6,button,1342242816
Control10=IDB_ADD,button,1342242816
Control11=IDB_SUBTRACT,button,1342242816
Control12=IDB_NUM1,button,1342242816
Control13=IDB_NUM2,button,1342242816
Control14=IDB_NUM3,button,1342242816
Control15=IDB_MULTIPLY,button,1342242816
Control16=IDB_DIVIDE,button,1342242816
Control17=IDB_NUM0,button,1342242816
Control18=IDB_REMAINDER,button,1342242816
Control19=IDB_POINT,button,1342242816
Control20=IDB_EQUAL,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_EXIT
CommandCount=1

