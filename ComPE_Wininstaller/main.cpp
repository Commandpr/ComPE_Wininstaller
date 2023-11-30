#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <CommCtrl.h>
#include <atlimage.h>
#include "resource.h"
#include "gdiplus.h"
#include "gdiplusgraphics.h"

#define ghobt 1000
#define xpbt 1001
#define wimbt 1002
#define diskbt 1003
#define bootbt 1004
#define logobt 1005
#define gholoadbt 1006
#define xploadbt 1007
#define wimloadbt 1008
using namespace Gdiplus;

int scrWidth = GetSystemMetrics(SM_CXSCREEN);
int scrHeight = GetSystemMetrics(SM_CYSCREEN);
int createx = scrWidth / 2 - 320;
int createy = scrHeight / 2 - 240;
HWND hWnd;
HWND btnlogo;
HFONT hFont;
HFONT hFont2;
HWND block;
HWND hTabCtrl;
HWND btndisk;
HWND btnreboot;
HWND btnxp;
HWND btnwim;
HWND btnghost;
HWND hpbar;
HWND win1, win2, win3, win4;
HWND edit, hWndComboBox;
LRESULT CALLBACK InWin1Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin2Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin3Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin4Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
using namespace std;
string TCHAR2STRING(TCHAR* str)
{
	string strstr;
	try
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, str, -1, chRtn, iLen, NULL, NULL);
		strstr = chRtn;
	}
	catch (exception e) {}
	return strstr;
}
string GetFirmware() {
	FIRMWARE_TYPE ft;
	GetFirmwareType(&ft);
	switch (ft) {
	case FirmwareTypeBios:
	{
		return "BIOS";

	}
	case FirmwareTypeUefi: {
		return "UEFI";
	}
	default:
	{
		return "UNKNOWN";
	}
	}
}

void SetWindowShow() {
	int sel = TabCtrl_GetCurSel(hTabCtrl);
	if (sel == 0) {
		ShowWindow(win1, SW_SHOW);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_HIDE);
	}
	else if (sel == 1) {
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_SHOW);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_HIDE);
	}
	else if (sel == 2) {
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_SHOW);
		ShowWindow(win4, SW_HIDE);
	}
	else if (sel == 3) {
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_SHOW);
	}
}


bool improvePv()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	//打开当前进程的权限令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		return false;

	//获得某一特定权限的权限标识LUID，保存在tkp中
	//查看里面的权限
	//第一个参数：NULL表示要查看的是当前本地系统
	//第二个参数：指定特权名称
	//第三个参数：用来接收返回的特权名称信息
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
		return false;

	//设置特权数组的个数
	tkp.PrivilegeCount = 1;
	//SE_PRIVILEGE_ENABLE_BY_DEFAULT    特权默认启动
	//SE_PRIVILEGE_ENABLED              特权启动
	//SE_PRIVILEGE_USER_FOR_ACCESS      用来访问一个对象或者服务
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;    //特权启动

	//调用AdjustTokenPrivileges来提升我们需要的系统权限
	//修改令牌的权限
	//第一个参数：调整特权的句柄
	//第二个参数：禁止所有权限表示
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, NULL, NULL, NULL))
		return false;

	return true;
}

//关机
bool powerOffProc()
{
	if (!improvePv() || !ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION))
		return false;
	return true;
}



TCHAR* GetGhoFile() {
	TCHAR szPath[MAX_PATH]{ 0 };//存储当前文件所在路径（不含文件名称和最后面的斜杠）
	TCHAR szTitle[MAX_PATH]{ 0 };//存储当前文件名
	TCHAR szFilePath[MAX_PATH]{ 0 };//存储当前文件路径
	TCHAR szFiles[80 * MAX_PATH]{ 0 };//可以存放至少80个文件的路径,即存储所有获得文件
	TCHAR* p;
	LPOPENFILENAME var = new OPENFILENAMEW;
	ZeroMemory(var, sizeof(OPENFILENAMEW));
	var->hwndOwner = hWnd;
	//指向所有者对话框窗口的句柄。这个成员可以是任意有效窗口句柄，或如果对话框没有所有者它可以为NULL。
	var->lStructSize = sizeof(OPENFILENAMEW);
	var->lpstrTitle = L"选择文件：";
	var->lpstrFilter = L"Ghost映像文件(*.gho)\0*.gho\0\0";
	//要选择的文件后缀，每种以一个逗号分隔,最后必须以两个NULL字符结束
	var->lpstrInitialDir = L"D:\\";//默认的初始文件路径
	var->lpstrFile = szFiles;//存放将要获取的所有文件的缓冲区
	var->nMaxFile = sizeof(szFiles);//缓冲区的字节数
	var->nFilterIndex = NULL;
	var->lpstrFileTitle = szTitle;
	//指向接收选择的文件的文件名和扩展名的缓冲(不带路径信息)。这个成员可以是NULL。
	var->Flags =
		OFN_PATHMUSTEXIST | //只允许输入存在路径
		OFN_FILEMUSTEXIST | //只允许输入存在文件
		OFN_EXPLORER | 		//使用Explorer风格的默认对话框的子窗口作为指定模板来建立一个对话框
		OFN_NOCHANGEDIR | 	//不变化当前路径，以防止影响文件操作
		OFN_ALLOWMULTISELECT;	//允许多选
	if (GetOpenFileName(var)) {
		lstrcpyn(szPath, szFiles, var->nFileOffset);
		//把第一个文件名前的复制到szPath,即:
		//如果只选了一个文件,就复制到最后一个'/'
		//如果选了多个文件,就复制到第一个NULL字符
		szPath[var->nFileOffset] = L'\0';//封好字符串,完成路径的获取
		lstrcpy(szFilePath, szPath);//将路径给予文件路径以准备获取完整文件路径
		lstrcat(szFilePath, TEXT("\\"));//附加上一个\号
		p = szFiles + var->nFileOffset;//把指针移到第一个文件
		while (*p) {//只要没有指向\0，也就是说还有文件名
			szFilePath[var->nFileOffset] = L'\0';//封好字符串,准备连接
			lstrcpy(szTitle, p);//获取该文件的文件名
			lstrcat(szFilePath, szTitle);//获得文件完整路径
			
			return szFilePath;
			//此处应该有对获得后的文件名和文件完整路径的各种操作
		}
	}
	else {
		return 0;
	}
	delete var;
}

int GetDiskNum(char str[2]) {
	char deviceName[7] = "\\\\.\\";
	strcat_s(deviceName, str);
	STORAGE_DEVICE_NUMBER sdn;
	HANDLE hwnd = CreateFileA(deviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hwnd == INVALID_HANDLE_VALUE) {
		CloseHandle(hwnd);
		return -1;
	}
	if (DeviceIoControl(hwnd,IOCTL_STORAGE_GET_DEVICE_NUMBER,NULL,NULL,&sdn,16,0,0) == 0) {
		CloseHandle(hwnd);
		return -1;
	}
	CloseHandle(hwnd);
	//MessageBox(hWnd, (LPCWSTR)to_string(sdn.DeviceNumber).c_str(), NULL, NULL);
	return sdn.DeviceNumber;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	WNDCLASS wndcls; //创建一个窗体类
	wndcls.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls.hbrBackground = CreateSolidBrush(RGB(255,255,255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);//设置光标
	wndcls.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));//设置窗体左上角的图标
	wndcls.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls.lpfnWndProc = WinSunProc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls.lpszClassName = L"ComPEWindowClass";//设置窗体的类名
	wndcls.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls);//向操作系统注册窗体
	WNDCLASS wndcls2; //创建一个窗体类
	wndcls2.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls2.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls2.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls2.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls2.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls2.lpfnWndProc = InWin1Proc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls2.lpszClassName = L"INWIN1";//设置窗体的类名
	wndcls2.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls2.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls2);//向操作系统注册窗体
	WNDCLASS wndcls3; //创建一个窗体类
	wndcls2.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls2.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls2.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls2.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls2.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls2.lpfnWndProc = InWin2Proc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls2.lpszClassName = L"INWIN2";//设置窗体的类名
	wndcls2.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls2.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls2);//向操作系统注册窗体
	WNDCLASS wndcls4; //创建一个窗体类
	wndcls2.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls2.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls2.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls2.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls2.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls2.lpfnWndProc = InWin3Proc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls2.lpszClassName = L"INWIN3";//设置窗体的类名
	wndcls2.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls2.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls2);//向操作系统注册窗体
	WNDCLASS wndcls5; //创建一个窗体类
	wndcls2.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls2.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls2.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls2.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls2.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls2.lpfnWndProc = InWin4Proc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls2.lpszClassName = L"INWIN4";//设置窗体的类名
	wndcls2.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls2.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls2);//向操作系统注册窗体
	//产生一个窗体，并返回该窗体的句柄，第一个参数必须为要创建的窗体的类名，第二个参数为窗体标题名
	HWND hwnd = CreateWindow(L"ComPEWindowClass", L"ComPE Windows安装器",
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME, createx,
		createy,640, 480,
		NULL, NULL, hInstance, NULL);
	hWnd = hwnd;
	btnghost = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Ghost模式",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT ,  // Styles 
		22,         // x position 
		107,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)ghobt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	btnwim = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"WIM/ESD模式",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		165,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)wimbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	btnxp = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"旧版Windows模式",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		223,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)xpbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	btnreboot = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"重启计算机",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		339,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)bootbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	btndisk = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"磁盘分区",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON |BS_FLAT,  // Styles 
		22,         // x position 
		281,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)diskbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	HBITMAP logo = (HBITMAP)LoadImage(NULL,L".\\COMPE.bmp", IMAGE_BITMAP, 208, 60, LR_LOADFROMFILE);
	btnlogo = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL,      // Button text 
		WS_VISIBLE | WS_CHILD | BS_BITMAP,  // Styles 
		18,         // x position 
		13,         // y position 
		208,        // Button width
		60,        // Button height
		hwnd,     // Parent window
		(HMENU)logobt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	hpbar = CreateWindowEx(0,
		PROGRESS_CLASS, (LPTSTR)NULL,    // Button text 
		WS_VISIBLE | WS_CHILD | PBS_MARQUEE,  // Styles 
		0,         // x position 
		424,         // y position 
		624,        // Button width
		17,        // Button height
		hwnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	hFont = CreateFont(16,                                    //   字体的高度   
		0,                                          //   字体的宽度  
		0,                                          //  nEscapement 
		0,                                          //  nOrientation   
		FW_NORMAL,                                  //   nWeight   
		FALSE,                                      //   bItalic   
		FALSE,                                      //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("幼圆"));
	hFont2 = CreateFont(15,                                    //   字体的高度   
		0,                                          //   字体的宽度  
		0,                                          //  nEscapement 
		0,                                          //  nOrientation   
		FW_NORMAL,                                  //   nWeight   
		FALSE,                                      //   bItalic   
		FALSE,                                      //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("幼圆"));
	hTabCtrl = CreateWindowEx(0, TEXT("SysTabControl32"), NULL,WS_VISIBLE|WS_CHILD | TCS_TABS,
		233, 45, 372, 355, hWnd, (HMENU)10001, hInstance, 0);
	::SendMessage(btndisk, WM_SETFONT, (WPARAM)hFont, 1);
	::SendMessage(btnghost, WM_SETFONT, (WPARAM)hFont, 1);
	::SendMessage(btnwim, WM_SETFONT, (WPARAM)hFont, 1);
	::SendMessage(btnxp, WM_SETFONT, (WPARAM)hFont, 1);
	::SendMessage(btnreboot, WM_SETFONT, (WPARAM)hFont, 1);
	::SendMessage(btnlogo, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)logo);
	TCITEM item1, item2, item3, item4;
	// 设置第一个选项卡项的文本和图像索引
	item1.mask = TCIF_TEXT | TCIF_IMAGE;
	item1.pszText = L"item1";
	item1.iImage = 0;

	// 设置第二个选项卡项的文本和图像索引
	item2.mask = TCIF_TEXT | TCIF_IMAGE;
	item2.pszText = L"item2";
	item2.iImage = 1;

	item3.mask = TCIF_TEXT | TCIF_IMAGE;
	item3.pszText = L"item3";
	item3.iImage = 2;

	item4.mask = TCIF_TEXT | TCIF_IMAGE;
	item4.pszText = L"item4";
	item4.iImage = 3;

	// 将两个选项卡项插入到选项卡控件中
	TabCtrl_InsertItem(hTabCtrl, 0, &item1);
	TabCtrl_InsertItem(hTabCtrl, 1, &item2);
	TabCtrl_InsertItem(hTabCtrl, 2, &item3);
	TabCtrl_InsertItem(hTabCtrl, 3, &item4);
	::EnableWindow(btnwim, true);
	::EnableWindow(btnghost, false);
	::EnableWindow(btnxp, true);
	::EnableWindow(btndisk, true);

	RECT rc;
	TabCtrl_GetItemRect(hTabCtrl, 0, & rc);

	win1 = CreateWindow(L"INWIN1", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),   // g_hInst is the global instance handle
		NULL);
	win2 = CreateWindow(L"INWIN2", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	win3 = CreateWindow(L"INWIN3", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	win4 = CreateWindow(L"INWIN4", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);

	//测试控件显示
	/*
	HWND btntest1 = CreateWindow(L"BUTTON",L"anniu1", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		281,         // y position 
		187,        // Button width
		56,        // Button height
		win1,     // Parent window
		(HMENU)diskbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btntest2 = CreateWindow(L"BUTTON", L"anniu2", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		281,         // y position 
		187,        // Button width
		56,        // Button height
		win2,     // Parent window
		(HMENU)diskbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	*/
	HWND btfile1 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
	280,119, 64, 21, win1, (HMENU)gholoadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
	NULL);
	::SendMessage(btfile1, WM_SETFONT, (WPARAM)hFont2, 1);
	edit = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		100, 120, 180, 19,win1, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	::SendMessage(edit, WM_SETFONT, (WPARAM)hFont2, 1);
	hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		100, 150, 180,19, win1, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	GetDiskNum("E:");
	SetWindowShow();
	ShowWindow(hwnd, SW_SHOWNORMAL);//把窗体显示出来
	UpdateWindow(hwnd);//更新窗体
	UpdateWindow(win1);//更新窗体
	UpdateWindow(win2);//更新窗体
	UpdateWindow(win3);//更新窗体
	UpdateWindow(win4);//更新窗体

	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


LRESULT CALLBACK InWin1Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"                     诺顿克隆精灵（Norton Ghost）"; 
		TCHAR msg2[] = L"能够完整而快速地复制备份、还原整个硬盘或单一分区。";
		TCHAR msg3[] = L"              本页面可用于对该类文件应用到磁盘。";
		TCHAR msg4[] = L"注：因Ghost软件自身原因，GHO目录请不要带有非英文字母！";
		TCHAR ghodir[] = L"GHO文件位置：";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 34, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 49, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 64, msg3, _tcslen(msg3));
		TextOut(hdc, 21, 79, msg4, _tcslen(msg4));
		TextOut(hdc, 16, 120, ghodir, _tcslen(ghodir));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case gholoadbt:
		{
			Edit_SetText(edit, GetGhoFile());
			break;
		}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}
LRESULT CALLBACK InWin2Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"                WIM是Windows基于文件的映像格式。";
		TCHAR msg2[] = L"WIM 映像格式并非基于扇区的映像格式，它是基于文件的。";
		TCHAR msg3[] = L"              本页面可用于对该类文件应用到磁盘。";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 34, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 49, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 64, msg3, _tcslen(msg3));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case gholoadbt:
		{
			GetGhoFile();
			break;
		}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}
LRESULT CALLBACK InWin3Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"             对于早期Windows操作系统（XP以前）";
		TCHAR msg2[] = L"        光盘映像内由一个I386文件夹存放安装信息";
		TCHAR msg3[] = L"              本页面可用于对该类文件应用到磁盘。";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 34, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 49, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 64, msg3, _tcslen(msg3));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case gholoadbt:
		{
			GetGhoFile();
			break;
		}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}
LRESULT CALLBACK InWin4Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"                     诺顿克隆精灵（Norton Ghost）";
		TCHAR msg2[] = L"能够完整而快速地复制备份、还原整个硬盘或单一分区。";
		TCHAR msg3[] = L"              本页面可用于对该类备份文件应用到磁盘。";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 34, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 49, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 64, msg3, _tcslen(msg3));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case gholoadbt:
		{
			GetGhoFile();
			break;
		}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}

LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	TCHAR msg[] = L"当前启动类型：";
	TCHAR msg2[8];
	swprintf(msg2, 8, L"%S", GetFirmware().c_str());
	TCHAR msg3[] = L"纯净安全且简洁的Windows PE维护系统";
	TCHAR msg4[] = L"Copyright © 2023 ComPE-纯净且简洁的Windows PE系统";
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_NOTIFY:
	{
		NMHDR* nmhdr = (NMHDR*)lParam;
		if (nmhdr->hwndFrom == hTabCtrl) {
			switch (nmhdr->code) {
			case TCN_SELCHANGE:
				SetWindowShow();
				break;
			}
		}
		break;
	}
	case WM_PAINT: // 绘制消息
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 510, 405, msg, _tcslen(msg));
		TextOut(hdc, 590, 405, msg2, _tcslen(msg2));
		TextOut(hdc, 12, 83, msg3, _tcslen(msg3));
		TextOut(hdc, 254, 16, msg4, _tcslen(msg4));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case diskbt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 3, 0);
			::EnableWindow(btnwim, true);
			::EnableWindow(btnghost, true);
			::EnableWindow(btnxp, true);
			::EnableWindow(btndisk, false);
			SetWindowShow();
			break;
		}
		case xpbt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 2, 0);
			::EnableWindow(btnwim, true);
			::EnableWindow(btnghost, true);
			::EnableWindow(btnxp, false);
			::EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		case wimbt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 1, 0);
			::EnableWindow(btnwim, false);
			::EnableWindow(btnghost, true);
			::EnableWindow(btnxp, true);
			::EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		case ghobt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 0, 0);
			::EnableWindow(btnwim, true);
			::EnableWindow(btnghost, false);
			::EnableWindow(btnxp, true);
			::EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		case bootbt:
		{
			int ask = MessageBox(hwnd, L"确定要重启计算机，离开ComPE系统吗？若有操作正在进行，请确保其执行完毕后重启，否则可能造成设备损坏。\n重启请按“是”，否则请按“否”。", L"提示：", MB_YESNO | MB_ICONQUESTION);
			if (ask == IDYES) {
				//MessageBox(hwnd, L"这里是关机的代码\nExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION);\n测试原因暂时不启用，用于判断代码块是否被执行。", NULL, NULL);
				powerOffProc();
			}
			break;
		}
		case gholoadbt: 
		{
			GetGhoFile();
			break;
		}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);//销毁窗口并发送WM_DESTROY消息，但是程序没有退出
		break;
	case WM_DESTROY:
		PostQuitMessage(0);//发出WM_QUIT消息，结束消息循环
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}