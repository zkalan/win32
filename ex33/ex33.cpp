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

//小时
static int s_nPreHour;
//分钟
static int s_nPreMinute;
//秒
static int s_nPreSecond;
//窗口客户区的大小
static int s_cxClient;
static int s_cyClient;

//是否位于最顶层
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

	//用描述主窗口的参数填充WNDCLASSEX结构
	//结构的大小
	wndclass.cbSize = sizeof(wndclass); 
	//指定如果大小改变就重画
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	//窗口的函数指针 
	wndclass.lpfnWndProc = MainWndProc;
	//没有额外的类内存 
	wndclass.cbClsExtra = 0;
	//没有额外的窗口内存
	wndclass.cbWndExtra = 0; 
	//实例句柄
	wndclass.hInstance = hInstance; 
	//使用预定义图标
	wndclass.hIcon = ::LoadIcon(hInstance, (LPSTR)IDI_TYPER); 
	//使用预定义的光标
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW); 
	//使用白色背景画刷
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	//不指定菜单 
	wndclass.lpszMenuName = NULL; 
	//窗口类的名称
	wndclass.lpszClassName = szClassName; 
	//没有类的小图标
	wndclass.hIconSm = NULL; 

	//注册这个窗口类
	::RegisterClassEx(&wndclass);
	//创建主窗口
	HWND hwnd = ::CreateWindowEx(
		0, //扩展样式
		szClassName, //类名
		"zkalan Win32 clock 1.0", //标题
		WS_POPUP|WS_SYSMENU|WS_SIZEBOX, //窗口风格
		CW_USEDEFAULT, //初始化X坐标
		CW_USEDEFAULT, //初始化Y坐标
		500, //宽度
		500, //高度
		NULL, //父窗口句柄
		NULL, //菜单句柄
		hInstance, //程序实例句柄
		NULL); //用户数据
 	
	if(hwnd == NULL)
	{
		::MessageBox(NULL, "create windows fail!", "error", MB_OK);
		return -1;
	}

	//显示窗口，刷新窗口客户区
	::ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	MSG msg;
	//从消息队列中取出消息，交给窗口函数处理，直到GetMessage返回FALSE，结束消息循环
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		//转化键盘消息
		::TranslateMessage(&msg);
		//将消息发送到相应的窗口函数
		::DispatchMessage(&msg);
	}

	//当GetMessage返回FALSE时程序结束
	return msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {

	case WM_CREATE:
		{
			//添加自定义菜单
			HMENU hSysMenu;
			hSysMenu = GetSystemMenu(hwnd, FALSE);
			AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
			AppendMenu(hSysMenu, MF_STRING, IDM_TOPMOST, "置顶");
			AppendMenu(hSysMenu, MF_STRING, IDM_HELP, "帮助");
			

			//获得时间
			SYSTEMTIME time;
			GetLocalTime(&time);
			s_nPreHour = time.wHour%12;
			s_nPreMinute = time.wMinute;
			s_nPreSecond = time.wSecond;

			//创建定时器
			SetTimer(hwnd, IDT_CLOCK, 1000, NULL);
			return 0;
		}



	case WM_PAINT: //窗口客户区需要重画
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			RECT rt;
			GetClientRect(hwnd, &rt);

			//设置笛卡尔坐标系
			Descartes(hdc, rt.right, rt.bottom);

			//以客户区位边界画一个椭圆
			HPEN hPen = CreatePen(PS_SOLID, 9, RGB(0,0,0));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, -220, -220, 220, 220);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);

			//绘制表盘
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
			//如果窗口处于最小化状态，nothing to do
			if (IsIconic(hwnd))
			{
				return 0;
			}
			//获得系统时间
			SYSTEMTIME time;
			GetLocalTime(&time);
			//建立坐标系
			HDC hdc = GetDC(hwnd);
			Descartes(hdc, s_cxClient, s_cyClient);
			//擦除指针
			COLORREF crfColor = RGB(255,255,255);
			back_color = RGB(255,255,255);
			//分钟改变
			if (time.wMinute != s_nPreMinute)
			{
				showShadow(hdc, HOUR, s_nPreHour*30 + s_nPreMinute/2, color_hour, SHADOW_X, SHADOW_Y, crfColor);
				DrawHand(hdc, HOUR, s_nPreHour*30 + s_nPreMinute/2, crfColor);
				showShadow(hdc, MINUTE, s_nPreMinute*6, color_hour, SHADOW_X, SHADOW_Y, crfColor);
				DrawHand(hdc, MINUTE, s_nPreMinute*6, crfColor);

				s_nPreHour = time.wHour;
				s_nPreMinute = time.wMinute;
			}
			//秒改变
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
				::GetAsyncKeyState(MK_LBUTTON) < 0) // 如果鼠标左键按下，GetAsyncKeyState函数的返回值小于0
				nHitTest = HTCAPTION;
			return nHitTest;
		}

	case WM_CONTEXTMENU:
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		{
			//获得系统菜单的句柄
			HMENU hSysMenu = ::GetSystemMenu(hwnd, FALSE);

			//弹出系统菜单
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
					::MessageBox(hwnd, "张凯\nzkalan@qq.com\n2014303446\n14011405\n2017-2-24", "帮助", 0);
				}
				else if (nID == IDM_TOPMOST)
				{
					HMENU hSysMenu = ::GetSystemMenu(hwnd, FALSE);
					if (s_bTopMost)
					{
						//设置ID号为IDM_TOPMOST的菜单为未选中状态
						::CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_UNCHECKED);
						//将窗口提到所有窗口的最顶层
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
	case WM_DESTROY: //正在销毁窗口
		//向消息队列投递一个WM_QUIT消息，促使GetMessage函数返回0，结束消息循环
		::PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		{
			::KillTimer(hwnd, IDT_CLOCK);
			DestroyWindow(hwnd);
			return 0;
		}
	}

	//将不处理的消息交给系统做默认处理
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

//设置笛卡尔坐标系
void Descartes(HDC hdc, int cx, int cy)
{
			//设定客户区的逻辑大小
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
				//画点
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
					//画点
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

// 	//阴影
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

//显示数字时钟
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

//显示数字12、3、6、9
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

