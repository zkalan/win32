#include "stdafx.h"
#include "Resource.h"
#include "string"
#include "stdlib.h"
#include "math.h"
#include "define.h"

//���尴ť���ı���
HWND hEditResultLine,
hButton0, hButton1, hButton2,
hButton3, hButton4, hButton5, hButton6,
hButton7, hButton8, hButton9,
hButtonAdd, hButtonSubtract, hButtonMultiply, hButtonDivide,
hButtonPoint,
hButtonSquare,
hButtonClear,
hButtonEqual,
hButtonRemainder;
//���ʵ��
HINSTANCE hInst;

char operateDir[] = "N";		//����������
int NumOrOpe = 0;		//����Ƿ������������
bool bPoint = false;		//����Ƿ������˵�
bool squareFlag = false;

int strLength;

char *stop;

char lpAddItem[50] = "";

char lpResult1[50] = "";
char lpResult2[50] = "";

double dAddItem = 0;

double dResult1 = 0, dResult2 = 0;

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
//����������
void operateCharacter();
void Square();
//�������ֺ�С����
void operateNumber(char *number);
void clearLpResult2();

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{

	WNDCLASSEX wndclass;

	//�����������ڵĲ������WNDCLASSEX�ṹ
	//�ṹ�Ĵ�С
	wndclass.cbSize = sizeof(wndclass);
	//ָ�������С�ı���ػ�
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	//���ڵĺ���ָ�� 
	wndclass.lpfnWndProc = MainWndProc;
	//û�ж�������ڴ� 
	wndclass.cbClsExtra = 0;
	//û�ж���Ĵ����ڴ�
	wndclass.cbWndExtra = 0;
	//ʵ�����
	wndclass.hInstance = hInstance;
	//ʹ��Ԥ����ͼ��
	wndclass.hIcon = ::LoadIcon(hInstance, (LPSTR)IDI_TYPER);
	//ʹ��Ԥ����Ĺ��
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	//ʹ�ð�ɫ������ˢ
	//(HBRUSH)::GetStockObject(WHITE_BRUSH)
	wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	//��ָ���˵� 
	wndclass.lpszMenuName = NULL;
	//�����������
	char szClassName[] = "MainWClass";
	wndclass.lpszClassName = szClassName;
	//û�����Сͼ��
	wndclass.hIconSm = NULL;

	wndclass.lpszMenuName = (LPCSTR)IDR_TYPER;


	//ע�����������
	::RegisterClassEx(&wndclass);
	//����������
	HWND hwnd = ::CreateWindowEx(
		0, //��չ��ʽ
		szClassName, //����
		"My First Windows!", //����
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, //���ڷ��
		CW_USEDEFAULT, //��ʼ��X����
		CW_USEDEFAULT, //��ʼ��Y����
		400, //���
		420, //�߶�
		NULL, //�����ھ��
		NULL, //�˵����
		hInstance, //����ʵ�����
		NULL); //�û�����

	if (hwnd == NULL)
	{
		::MessageBox(NULL, "����������ʧ��!", "error", MB_OK);
		return -1;
	}

	//��ʾ���ڣ�ˢ�´��ڿͻ���
	::ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	MSG msg;
	//����Ϣ������ȡ����Ϣ���������ں�������ֱ��GetMessage����FALSE��������Ϣѭ��
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		//ת��������Ϣ
		::TranslateMessage(&msg);
		//����Ϣ���͵���Ӧ�Ĵ��ں���
		::DispatchMessage(&msg);
	}

	//��GetMessage����FALSEʱ�������
	return msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) 
	{
	case WM_CREATE:
	{
		//���ô��ڱ���
		::SetWindowText(hwnd, "�ҵ�Win32������1.0");

		hEditResultLine = CreateWindow("Edit", NULL,
			WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_READONLY,
			20, 20, 350, 24,
			hwnd, (HMENU)IDE_RESULT,
			hInst,
			NULL);
		hButton7 = CreateWindow("BUTTON", "7",
			WS_CHILD | WS_VISIBLE,
			20, 80, 60, 60,
			hwnd, (HMENU)IDB_NUM7,
			hInst, NULL);
		hButton8 = CreateWindow("BUTTON", "8",
			WS_CHILD | WS_VISIBLE,
			90, 80, 60, 60,
			hwnd, (HMENU)IDB_NUM8,
			hInst, NULL);
		hButton9 = CreateWindow("BUTTON", "9",
			WS_CHILD | WS_VISIBLE,
			160, 80, 60, 60,
			hwnd, (HMENU)IDB_NUM9,
			hInst, NULL);
		hButtonSquare = CreateWindow("BUTTON", "Square",
			WS_CHILD | WS_VISIBLE,
			230, 80, 60, 60,
			hwnd, (HMENU)IDB_SQUARE,
			hInst, NULL);
		hButtonClear = CreateWindow("BUTTON", "Clear",
			WS_CHILD | WS_VISIBLE,
			300, 80, 60, 60,
			hwnd, (HMENU)IDB_CLEAR,
			hInst, NULL);
		hButton4 = CreateWindow("BUTTON", "4",
			WS_CHILD | WS_VISIBLE,
			20, 150, 60, 60,
			hwnd, (HMENU)IDB_NUM4,
			hInst, NULL);
		hButton5 = CreateWindow("BUTTON", "5",
			WS_CHILD | WS_VISIBLE,
			90, 150, 60, 60,
			hwnd, (HMENU)IDB_NUM5,
			hInst, NULL);
		hButton6 = CreateWindow("BUTTON", "6",
			WS_CHILD | WS_VISIBLE,
			160, 150, 60, 60,
			hwnd, (HMENU)IDB_NUM6,
			hInst, NULL);
		hButton1 = CreateWindow("BUTTON", "1",
			WS_CHILD | WS_VISIBLE,
			20, 220, 60, 60,
			hwnd, (HMENU)IDB_NUM1,
			hInst, NULL);
		hButton2 = CreateWindow("BUTTON", "2",
			WS_CHILD | WS_VISIBLE,
			90, 220, 60, 60,
			hwnd, (HMENU)IDB_NUM2,
			hInst, NULL);
		hButton3 = CreateWindow("BUTTON", "3",
			WS_CHILD | WS_VISIBLE,
			160, 220, 60, 60,
			hwnd, (HMENU)IDB_NUM3,
			hInst, NULL);
		hButton0 = CreateWindow("BUTTON", "0",
			WS_CHILD | WS_VISIBLE,
			20, 290, 130, 60,
			hwnd, (HMENU)IDB_NUM0,
			hInst, NULL);
		hButtonPoint = CreateWindow("BUTTON", ".",
			WS_CHILD | WS_VISIBLE,
			160, 290, 60, 60,
			hwnd, (HMENU)IDB_POINT,
			hInst, NULL);
		hButtonAdd = CreateWindow("BUTTON", "+",
			WS_CHILD | WS_VISIBLE,
			230, 150, 60, 60,
			hwnd, (HMENU)IDB_ADD,
			hInst, NULL);
		hButtonSubtract = CreateWindow("BUTTON", "-",
			WS_CHILD | WS_VISIBLE,
			300, 150, 60, 60,
			hwnd, (HMENU)IDB_SUBTRACT,
			hInst, NULL);
		hButtonMultiply = CreateWindow("BUTTON", "*",
			WS_CHILD | WS_VISIBLE,
			230, 220, 60, 60,
			hwnd, (HMENU)IDB_MULTIPLY,
			hInst, NULL);
		hButtonDivide = CreateWindow("BUTTON", "/",
			WS_CHILD | WS_VISIBLE,
			300, 220, 60, 60,
			hwnd, (HMENU)IDB_DIVIDE,
			hInst, NULL);
		hButtonEqual = CreateWindow("BUTTON", "=",
			WS_CHILD | WS_VISIBLE,
			230, 290, 60, 60,
			hwnd, (HMENU)IDB_EQUAL,
			hInst, NULL);
		hButtonRemainder = CreateWindow("BUTTON", "%",
			WS_CHILD | WS_VISIBLE,
			300, 290, 60, 60,
			hwnd, (HMENU)IDB_REMAINDER,
			hInst, NULL);

		SetWindowText(hEditResultLine, "0");
		//SetFocus(hwnd);
		break;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		//��Ӧ���ֺ�С����
		case IDB_NUM0:
		{
			operateNumber("0");
			break;
		}
		case IDB_NUM1:
		{
			operateNumber("1");
			break;
		}
		case IDB_NUM2:
		{
			operateNumber("2");
			break;
		}
		case IDB_NUM3:
		{
			operateNumber("3");
			break;
		}
		case IDB_NUM4:
		{
			operateNumber("4");
			break;
		}
		case IDB_NUM5:
		{
			operateNumber("5");
			break;
		}
		case IDB_NUM6:
		{
			operateNumber("6");
			break;
		}
		case IDB_NUM7:
		{
			operateNumber("7");
			break;
		}
		case IDB_NUM8:
		{
			operateNumber("8");
			break;
		}
		case IDB_NUM9:
		{
			operateNumber("9");
			break;
		}
		case IDB_POINT:
		{
			if (bPoint == true)
			{
				break;
			}
			operateNumber(".");
			NumOrOpe = 1;
			bPoint = true;
			break;
		}
		//�������Ӧ
		case IDB_ADD:
		{
			operateCharacter();
			strcpy(operateDir, "+");
			break;
		}
		case IDB_SUBTRACT:
		{
			operateCharacter();
			strcpy(operateDir, "-");
			break;
		}
		case IDB_MULTIPLY:
		{
			operateCharacter();
			strcpy(operateDir, "*");
			break;
		}
		case IDB_DIVIDE:
		{
			operateCharacter();
			strcpy(operateDir, "/");
			break;
		}
		case IDB_REMAINDER:
		{
			operateCharacter();
			strcpy(operateDir, "%");
			break;
		}
		case IDB_EQUAL:
		{
			operateCharacter();
			strcpy(operateDir, "N");
			break;
		}
		case IDB_SQUARE:
		{
			squareFlag = true;
			Square();
			break;
		}
		case IDB_CLEAR:
		{
			SetWindowText(hEditResultLine, "0");
			NumOrOpe = 0;
			bPoint = false;
			squareFlag = false;
			for (int i = 0 ; i < 50 ; i++)
			{
				lpResult1[i] = ' ';
				lpResult2[i] = ' ';
			}
			strcpy(operateDir, "N");
			break;
		}

		//�˵���Ӧ
		case ID_FILE_EXIT:
		{
			//��hwndָ���Ĵ��ڷ���һ��WM_CLOSE��Ϣ
			::SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		case ID_HELP_MESSAGE:
		{
			MessageBox(hwnd, "  �ſ� κ���\n zkalan@qq.com\n   201430346\n   14011405\n   2017-2-21", "����", MB_OK);
			return 0;
		}
		}
		SetFocus(hwnd);
		break;
	}
	//�Ӽ�������
	case WM_CHAR:
	{
		switch (char(wParam))
		{
			case '0':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM0, 0);
				break;
			case '1':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM1, 0);
				break;
			case '2':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM2, 0);
				break;
			case '3':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM3, 0);
				break;
			case '4':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM4, 0);
				break;
			case '5':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM5, 0);
				break;
			case '6':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM6, 0);
				break;
			case '7':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM7, 0);
				break;
			case '8':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM8, 0);
				break;
			case '9':
				::SendMessage(hwnd, WM_COMMAND, IDB_NUM9, 0);
				break;
			case '.':
				::SendMessage(hwnd, WM_COMMAND, IDB_POINT, 0);
				break;
			case '+':
				::SendMessage(hwnd, WM_COMMAND, IDB_ADD, 0);
				break;
			case '-':
				::SendMessage(hwnd, WM_COMMAND, IDB_SUBTRACT, 0);
				break;
			case '*':
				::SendMessage(hwnd, WM_COMMAND, IDB_MULTIPLY, 0);
				break;
			case '/':
				::SendMessage(hwnd, WM_COMMAND, IDB_DIVIDE, 0);
				break;
			case '=':
				::SendMessage(hwnd, WM_COMMAND, IDB_EQUAL, 0);
				break;
			case '%':
				::SendMessage(hwnd, WM_COMMAND, IDB_REMAINDER, 0);
				break;		
			case 13:
				::SendMessage(hwnd, WM_COMMAND, IDB_EQUAL, 0);
				break;
			case 8:
				::SendMessage(hwnd, WM_COMMAND, IDB_CLEAR, 0);
				break;
			break;
		}
		
		SetFocus(hwnd);
		break;
	}

	//���ٴ���
	case WM_DESTROY:
	{
		//����Ϣ����Ͷ��һ��WM_QUIT��Ϣ����ʹGetMessage��������0��������Ϣѭ��
		::PostQuitMessage(0);
		return 0;
	}
	}

	//�����������Ϣ����ϵͳ��Ĭ�ϴ���
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

void operateCharacter()
{
	if (strcmp(operateDir, "N") == 0)
	{
		dResult1 = strtod(lpResult1, &stop);
	}
	else
	{
		switch (*operateDir)
		{
		case '+':
		{
			dResult1 = strtod(lpResult1, &stop);
			dResult2 = strtod(lpResult2, &stop);
			dResult1 = dResult1 + dResult2;
			clearLpResult2();
			break;
		}
		case '-':
		{
			dResult1 = strtod(lpResult1, &stop);
			dResult2 = strtod(lpResult2, &stop);
			dResult1 = dResult1 - dResult2;
			clearLpResult2();
			break;
		}
		case '*':
		{
			dResult1 = strtod(lpResult1, &stop);
			strtod(lpResult2, &stop);
			if ( stop == lpResult2)
			{
				break;
			}
			else
			{
				dResult2 = strtod(lpResult2, &stop);
			}
			dResult1 = dResult1 * dResult2;
			clearLpResult2();
			break;
		}
		case '/':
		{
			dResult1 = strtod(lpResult1, &stop);
			strtod(lpResult2, &stop);
			if ( stop == lpResult2)
			{
				break;
			}
			else
			{
				dResult2 = strtod(lpResult2, &stop);
			}
			if (dResult2 == 0)
			{
				MessageBox(NULL, "��������Ϊ0��", "����", MB_OK);
				MessageBeep(MB_OK);
				return;
			}
			else
			{
				dResult1 = dResult1 / dResult2;
				clearLpResult2();
				break;
			}
		}
		case '%':
		{
			dResult1 = strtod(lpResult1, &stop);
			if ((dResult1 - (int)dResult1) != 0)
			{
				MessageBox(NULL, "С������ȡ�࣡", "����", MB_OK);
				MessageBeep(MB_OK);
				return;
			}
			strtod(lpResult2, &stop);
			if ( stop == lpResult2)
			{
				break;
			}
			else
			{
				dResult2 = strtod(lpResult2, &stop);
				if ((dResult2 - (int)dResult2) != 0)
					{
					MessageBox(NULL, "С������ȡ�࣡", "����", MB_OK);
					MessageBeep(MB_OK);
					return;
				}
			}
			if (dResult2 == 0)
			{
				MessageBox(NULL, "ȡ��������Ϊ0��", "����", MB_OK);
				MessageBeep(MB_OK);
				clearLpResult2();
				return;	
			}
			else
			{
				int a = dResult1, b = dResult2;
				dResult1 = a % b;
				clearLpResult2();
				break;
			}
		}
		}
	}
	//��˫����ת��Ϊ�ַ���
	_gcvt(dResult1, 15, lpResult1);
	SetWindowText(hEditResultLine, lpResult1);
	NumOrOpe = 0;
	bPoint = false;
}

void operateNumber(char *number)
{
	if (NumOrOpe == 0)
	{
		SetWindowText(hEditResultLine, "");
	}
	strLength = GetWindowTextLength(hEditResultLine) + 1;
	if (strLength > 50)
	{
		return;
	}
	GetWindowText(hEditResultLine, lpAddItem, strLength);
	strcat(lpAddItem, number);
	if (strcmp(operateDir, "N") == 0 || squareFlag == true)
	{
		strcpy(lpResult1, lpAddItem);
		dResult1 = strtod(lpResult1, &stop);
		if (*number == '0' && bPoint == false && dResult1 == 0)
		{
			SetWindowText(hEditResultLine, "0");
		}
		else
		{
			SetWindowText(hEditResultLine, lpResult1);
		}
	}
	else
	{
		strcpy(lpResult2, lpAddItem);
		dResult2 = strtod(lpResult2, &stop);
		if (*number == '0' && bPoint == false && dResult2 == 0)
		{
			SetWindowText(hEditResultLine, "0");
		}
		else
		{
			SetWindowText(hEditResultLine, lpResult2);
		}
	}
	NumOrOpe = 1;  //���������ּ�
}

//��������������������ֿ���
void Square()
{
	dResult1 = strtod(lpResult1, &stop);
	if (dResult1 < 0)
	{
		MessageBox(NULL, "����û��ʵƽ������", "����", MB_OK);
		NumOrOpe = 0;
		bPoint = false;
		squareFlag = false;
		clearLpResult2();
		return;
	}
	dResult1 = sqrt(dResult1);
	_gcvt(dResult1, 15, lpResult1);
	SetWindowText(hEditResultLine, lpResult1);
	NumOrOpe = 0;
	bPoint = false;
	squareFlag = false;
	clearLpResult2();
}

void clearLpResult2()
{
	for (int i = 0 ; i < 50 ; i++)
	{
		lpResult2[i] = ' ';
	}
}