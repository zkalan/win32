#include "stdafx.h"
#include "resource.h"
#include "math.h"
#include "string"

#define IDT_CLOCK 100

#define PI 3.141592653

const int IDM_HELP = 100;
const int IDM_TOPMOST = 101;

int SECOND[3] = {195, 4, 15};
int MINUTE[3] = {130, 10, 20};
int HOUR[3] = {100, 10, 20};

COLORREF color_minute = RGB(71,60,139);
COLORREF color_second = RGB(0,0,0);
COLORREF color_hour = RGB(25,25,112);

int SHADOW_X = 4;
int SHADOW_Y = -4;
COLORREF COLOR_SHADOW = RGB(205,201,201);

COLORREF important_hour = RGB(255,255,255);

static COLORREF back_color = RGB(255,255,255);

//Сʱ
static int s_nPreHour;
//����
static int s_nPreMinute;
//��
static int s_nPreSecond;
//���ڿͻ����Ĵ�С
static int s_cxClient;
static int s_cyClient;

//�Ƿ�λ�����
static BOOL s_bTopMost;

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

void Descartes(HDC hdc, int cx,int cy);

void DrawPoint(HDC hdc, int RADIUS, int pRADIUS, int pRADIUS_B, int POINT_NUMBER, int POINT_NUMBER_B);

void DrawHand(HDC hdc, int style[], int degree, COLORREF clrColor);

void DigitalClock(HDC hdc);

void ShowNumber(HDC hdc);

//void showShadow(HDC hdc, POINT ptt[], int num, int shadow_x,int shadow_y, COLORREF color_shadow);

void showShadow(HDC hdc, int style[], int degree, COLORREF clrColor, int shadow_x,int shadow_y, COLORREF color_shadow);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szClassName[] = "MainWClass";

	WNDCLASSEX wndclass;

	//�����������ڵĲ������WNDCLASSEX�ṹ
	//�ṹ�Ĵ�С
	wndclass.cbSize = sizeof(wndclass); 
	//ָ�������С�ı���ػ�
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
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
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	//��ָ���˵� 
	wndclass.lpszMenuName = NULL; 
	//�����������
	wndclass.lpszClassName = szClassName; 
	//û�����Сͼ��
	wndclass.hIconSm = NULL; 

	//ע�����������
	::RegisterClassEx(&wndclass);
	//����������
	HWND hwnd = ::CreateWindowEx(
		0, //��չ��ʽ
		szClassName, //����
		"zkalan Win32 clock 1.0", //����
		WS_POPUP|WS_SYSMENU|WS_SIZEBOX, //���ڷ��
		CW_USEDEFAULT, //��ʼ��X����
		CW_USEDEFAULT, //��ʼ��Y����
		500, //���
		500, //�߶�
		NULL, //�����ھ��
		NULL, //�˵����
		hInstance, //����ʵ�����
		NULL); //�û�����
 	
	if(hwnd == NULL)
	{
		::MessageBox(NULL, "create windows fail!", "error", MB_OK);
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
	switch(message) {

	case WM_CREATE:
		{
			//����Զ���˵�
			HMENU hSysMenu;
			hSysMenu = GetSystemMenu(hwnd, FALSE);
			AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
			AppendMenu(hSysMenu, MF_STRING, IDM_TOPMOST, "�ö�");
			AppendMenu(hSysMenu, MF_STRING, IDM_HELP, "����");
			

			//���ʱ��
			SYSTEMTIME time;
			GetLocalTime(&time);
			s_nPreHour = time.wHour%12;
			s_nPreMinute = time.wMinute;
			s_nPreSecond = time.wSecond;

			//������ʱ��
			SetTimer(hwnd, IDT_CLOCK, 1000, NULL);
			return 0;
		}



	case WM_PAINT: //���ڿͻ�����Ҫ�ػ�
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			RECT rt;
			GetClientRect(hwnd, &rt);

			//���õѿ�������ϵ
			Descartes(hdc, rt.right, rt.bottom);

			//�Կͻ���λ�߽续һ����Բ
			HPEN hPen = CreatePen(PS_SOLID, 9, RGB(0,0,0));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, -220, -220, 220, 220);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);

			//���Ʊ���
			DrawPoint(hdc, 200, 5, 2, 12, 48);

			ShowNumber(hdc);

			showShadow(hdc, HOUR, s_nPreHour*30 + s_nPreMinute/2, color_hour, SHADOW_X, SHADOW_Y, COLOR_SHADOW);
			showShadow(hdc, MINUTE, s_nPreMinute*6, color_hour, SHADOW_X, SHADOW_Y, COLOR_SHADOW);

			DigitalClock(hdc);

			showShadow(hdc, SECOND, s_nPreSecond*6, color_second, SHADOW_X, SHADOW_Y, COLOR_SHADOW);

			DrawHand(hdc, HOUR, s_nPreHour*30 + s_nPreMinute/2, color_hour);
			DrawHand(hdc, MINUTE, s_nPreMinute*6, color_minute);

			DigitalClock(hdc);

			DrawHand(hdc, SECOND, s_nPreSecond*6, color_second);

			EndPaint(hwnd, &ps);
			break;
		}

	case WM_SIZE:
		{
			s_cxClient = LOWORD(lParam);
			s_cyClient = HIWORD(lParam);
			return 0;
		}
	case WM_TIMER:
		{
			//������ڴ�����С��״̬��nothing to do
			if (IsIconic(hwnd))
			{
				return 0;
			}
			//���ϵͳʱ��
			SYSTEMTIME time;
			GetLocalTime(&time);
			//��������ϵ
			HDC hdc = GetDC(hwnd);
			Descartes(hdc, s_cxClient, s_cyClient);
			//����ָ��
			COLORREF crfColor = RGB(255,255,255);
			back_color = RGB(255,255,255);
			//���Ӹı�
			if (time.wMinute != s_nPreMinute)
			{
				showShadow(hdc, HOUR, s_nPreHour*30 + s_nPreMinute/2, color_hour, SHADOW_X, SHADOW_Y, crfColor);
				DrawHand(hdc, HOUR, s_nPreHour*30 + s_nPreMinute/2, crfColor);
				showShadow(hdc, MINUTE, s_nPreMinute*6, color_hour, SHADOW_X, SHADOW_Y, crfColor);
				DrawHand(hdc, MINUTE, s_nPreMinute*6, crfColor);

				s_nPreHour = time.wHour;
				s_nPreMinute = time.wMinute;
			}
			//��ı�
			if (time.wSecond != s_nPreSecond)
			{
				showShadow(hdc, SECOND, s_nPreSecond*6, color_second, SHADOW_X, SHADOW_Y, crfColor);
				DrawHand(hdc, SECOND, s_nPreSecond*6, crfColor);

				ShowNumber(hdc);

				showShadow(hdc, HOUR, time.wHour*30 + time.wMinute/2, color_hour, SHADOW_X, SHADOW_Y, COLOR_SHADOW);
				showShadow(hdc, MINUTE, time.wMinute*6, color_hour, SHADOW_X, SHADOW_Y, COLOR_SHADOW);
				DigitalClock(hdc);
				showShadow(hdc, SECOND, time.wSecond*6, color_second, SHADOW_X, SHADOW_Y, COLOR_SHADOW);

				DrawHand(hdc, HOUR, time.wHour*30 + time.wMinute/2, color_hour);

				DigitalClock(hdc);

				DrawHand(hdc, MINUTE, time.wMinute*6, color_minute);

				DrawHand(hdc, SECOND, time.wSecond*6, color_second);

				s_nPreSecond = time.wSecond;
			}

			return 0;
		}

	case WM_NCHITTEST:
		{
			UINT nHitTest;
			nHitTest = ::DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
			if (nHitTest == HTCLIENT &&
				::GetAsyncKeyState(MK_LBUTTON) < 0) // ������������£�GetAsyncKeyState�����ķ���ֵС��0
				nHitTest = HTCAPTION;
			return nHitTest;
		}

	case WM_CONTEXTMENU:
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		{
			//���ϵͳ�˵��ľ��
			HMENU hSysMenu = ::GetSystemMenu(hwnd, FALSE);

			//����ϵͳ�˵�
			int nID = TrackPopupMenu(hSysMenu, TPM_LEFTALIGN|TPM_RETURNCMD,
				pt.x, pt.y, 0, hwnd, NULL);
			if (nID > 0)
			{
				SendMessage(hwnd, WM_SYSCOMMAND, nID, 0);
			}
			return 0;
		}
	case WM_SYSCOMMAND:
		{
			int nID = wParam;
			{
				if (nID == IDM_HELP)
				{
					::MessageBox(hwnd, "�ſ�\nzkalan@qq.com\n2014303446\n14011405\n2017-2-24", "����", 0);
				}
				else if (nID == IDM_TOPMOST)
				{
					HMENU hSysMenu = ::GetSystemMenu(hwnd, FALSE);
					if (s_bTopMost)
					{
						//����ID��ΪIDM_TOPMOST�Ĳ˵�Ϊδѡ��״̬
						::CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_UNCHECKED);
						//�������ᵽ���д��ڵ����
						::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
							SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE);
						s_bTopMost = FALSE;
					}
					else
					{
						CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_CHECKED);
						SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
							SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE);
						s_bTopMost = TRUE;
					}
				}
				return ::DefWindowProc(hwnd, WM_SYSCOMMAND, nID, 0);
			}
		}
	case WM_DESTROY: //�������ٴ���
		//����Ϣ����Ͷ��һ��WM_QUIT��Ϣ����ʹGetMessage��������0��������Ϣѭ��
		::PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		{
			::KillTimer(hwnd, IDT_CLOCK);
			DestroyWindow(hwnd);
			return 0;
		}
	}

	//�����������Ϣ����ϵͳ��Ĭ�ϴ���
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

//���õѿ�������ϵ
void Descartes(HDC hdc, int cx, int cy)
{
			//�趨�ͻ������߼���С
			SetMapMode(hdc, MM_ISOTROPIC);
			SetWindowExtEx(hdc, 500, 500, NULL);
			SetViewportExtEx(hdc, cx, -cy, NULL);
			SetViewportOrgEx(hdc, cx/2, cy/2, NULL);
			return;
}

void DrawPoint(HDC hdc, int RADIUS, int pRADIUS, int pRADIUS_B, int POINT_NUMBER, int POINT_NUMBER_B)
{
			int i = 0;
			int ALL_POINT = POINT_NUMBER_B + POINT_NUMBER;
			double INCLUDE_ANGLE = 0;
			int px = 0, py = RADIUS;

			HBRUSH hBrush0 = CreateSolidBrush(important_hour);
			HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush0);
			for (i = 0; i < POINT_NUMBER; i++)
			{
				INCLUDE_ANGLE = (2 * PI / POINT_NUMBER) * i;
				px = (int)RADIUS * sin(INCLUDE_ANGLE);
				py = (int)RADIUS * cos(INCLUDE_ANGLE);
				//����
				Ellipse(hdc, px - pRADIUS, py + pRADIUS, px + pRADIUS, py - pRADIUS);
			}
			SelectObject(hdc,holdBrush);
			DeleteObject(hBrush0);

			HBRUSH hBrush1 = CreateSolidBrush(RGB(0,0,0));
			holdBrush = (HBRUSH)SelectObject(hdc, hBrush1);
			for (i = 0;i < ALL_POINT; i++)
			{
				if (i % 5 != 0)
				{
					INCLUDE_ANGLE = (2 * PI / ALL_POINT) * i;
					px = (int)RADIUS * sin(INCLUDE_ANGLE);
					py = (int)RADIUS * cos(INCLUDE_ANGLE);
					//����
					Ellipse(hdc, px - pRADIUS_B, py + pRADIUS_B, px + pRADIUS_B, py - pRADIUS_B);
				}
			}
			SelectObject(hdc,holdBrush);
			DeleteObject(hBrush1);
			return;
}

void DrawHand(HDC hdc, int style[], int degree, COLORREF clrColor)
{
	double RADIUS_lo = degree * 2 * PI / 360;
	double RADIUS_mi = (degree + 90) * 2 * PI / 360;

	POINT pt[4];
	pt[0].x = (int)(style[0] * sin(RADIUS_lo));
	pt[0].y = (int)(style[0] * cos(RADIUS_lo));

	pt[1].x = (int)(style[1] * sin(RADIUS_mi));
	pt[1].y = (int)(style[1] * cos(RADIUS_mi));

	pt[2].x = -(int)(style[2] * sin(RADIUS_lo));
	pt[2].y = -(int)(style[2] * cos(RADIUS_lo));

	pt[3].x = -(int)(style[1] * sin(RADIUS_mi));
	pt[3].y = -(int)(style[1] * cos(RADIUS_mi));

	HBRUSH hBrush = CreateSolidBrush(clrColor);
	HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_NULL, 0, back_color);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

// 	//��Ӱ
// 	if (clrColor == RGB(255,255,255))
// 	{
// 		showShadow(hdc, pt, 4, SHADOW_X ,SHADOW_Y , clrColor);
// 	}
// 	else
// 	{
// 		showShadow(hdc, pt, 4, SHADOW_X ,SHADOW_Y , COLOR_SHADOW);
// 	}

	Polygon(hdc, pt, 4);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);

	SelectObject(hdc, holdBrush);
	DeleteObject(hBrush);

	return;
}

// void showShadow(HDC hdc, POINT ptt[], int num, int shadow_x,int shadow_y, COLORREF color_shadow)
// {
// 	int i = 0;
// 	for (i = 0; i< num; i++)
// 	{
// 		ptt[i].x += shadow_x;
// 		ptt[i].y += shadow_y;
// 	}
// 
// 	HBRUSH hBrush = CreateSolidBrush(color_shadow);
// 	HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush);
// 
// 	HPEN hPen = CreatePen(PS_NULL, 0, back_color);
// 	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
// 
// 	Polygon(hdc, ptt, num);
// 
// 	SelectObject(hdc, hOldPen);
// 	DeleteObject(hPen);
// 
// 	SelectObject(hdc, holdBrush);
// 	DeleteObject(hBrush);
// 
// 	for (i = 0; i< num; i++)
// 	{
// 		ptt[i].x -= shadow_x;
// 		ptt[i].y -= shadow_y;
// 	}
// 
// 	return;
// }

void showShadow(HDC hdc, int style[], int degree, COLORREF clrColor, int shadow_x,int shadow_y, COLORREF color_shadow)
{
	double RADIUS_lo = degree * 2 * PI / 360;
	double RADIUS_mi = (degree + 90) * 2 * PI / 360;

	POINT pt[4];
	pt[0].x = (int)(style[0] * sin(RADIUS_lo));
	pt[0].y = (int)(style[0] * cos(RADIUS_lo));

	pt[1].x = (int)(style[1] * sin(RADIUS_mi));
	pt[1].y = (int)(style[1] * cos(RADIUS_mi));

	pt[2].x = -(int)(style[2] * sin(RADIUS_lo));
	pt[2].y = -(int)(style[2] * cos(RADIUS_lo));

	pt[3].x = -(int)(style[1] * sin(RADIUS_mi));
	pt[3].y = -(int)(style[1] * cos(RADIUS_mi));

	int i = 0;
	for (i = 0; i< 4; i++)
	{
		pt[i].x += shadow_x;
		pt[i].y += shadow_y;
	}

	HBRUSH hBrush = CreateSolidBrush(color_shadow);
	HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HPEN hPen = CreatePen(PS_NULL, 0, back_color);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	Polygon(hdc, pt, 4);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);

	SelectObject(hdc, holdBrush);
	DeleteObject(hBrush);

	for (i = 0; i< 4; i++)
	{
		pt[i].x -= shadow_x;
		pt[i].y -= shadow_y;
	}

	return;
}

//��ʾ����ʱ��
void DigitalClock(HDC hdc)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	Descartes(hdc, s_cxClient, s_cyClient);

	HFONT font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight = 30;
	lf.lfWidth = 30;
	font = CreateFontIndirect(&lf);
	SelectObject(hdc, font);
	
	char szText[50];
	wsprintf(szText, "%02d:%02d:%02d",time.wHour, time.wMinute, time.wSecond);

	SetTextColor(hdc, RGB(54,54,54));

	TextOut(hdc, -126, 130, szText, strlen(szText)); 

}

//��ʾ����12��3��6��9
void ShowNumber(HDC hdc)
{
	char twelve[3] = {'1','2'};
	char three[2] = {'3'};
	char six[2] = {'6'};
	char nine[2] = {'9'};
	char zkalan[7] = {'Z','K','A','L','A','N'};

	HFONT font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 20;
	lf.lfWidth = 20;
	font = CreateFontIndirect(&lf);
	SelectObject(hdc, font);

	SetTextColor(hdc, RGB(0, 0, 0));

	TextOut(hdc, -28, 190, twelve, strlen(twelve));
	TextOut(hdc, 168, 9, three, strlen(three));
	TextOut(hdc, -10, -175, six, strlen(six));
	TextOut(hdc, -188, 9, nine, strlen(nine));
	
	lf.lfHeight = 10;
	lf.lfWidth = 10;
	font = CreateFontIndirect(&lf);
	SelectObject(hdc, font);

	TextOut(hdc, -40, -100, zkalan, strlen(zkalan));

	DeleteObject(font);

}

