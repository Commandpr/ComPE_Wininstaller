#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <io.h>
#include <windowsx.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <CommCtrl.h>
#include <atlimage.h>
#include <regex>
#include <vector>
#include <comdef.h>
#include <filesystem>
#include <Shlobj.h>
#include "wimlib.h"
#include "resource.h"
#pragma comment(lib,"libwim.lib")

#define ghobt 1000
#define xpbt 1001
#define wimbt 1002
#define diskbt 1003
#define bootbt 1004
#define logobt 1005
#define gholoadbt 1006
#define ghodiskbt 1009
#define wimdiskbt1 2000
#define wimdiskbt2 2001
#define xploadbt 1007
#define txtloadbt 3001
#define xpdiskbt 3002
#define wimloadbt 1008
#define xmloadbt 1111
#define ghostartbt 1010
#define wimstartbt 1011
#define xpstartbt 1012
#define diskmode 5000
#define parmode 6000
using namespace Gdiplus;

int scrWidth = GetSystemMetrics(SM_CXSCREEN);
int scrHeight = GetSystemMetrics(SM_CYSCREEN);
int createx = scrWidth / 2 - 320;
int createy = scrHeight / 2 - 240;
bool ispar = true;
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
HWND btnghost, ghostartbtn,btwimstart, btxpstart;
HWND hpbar;
HWND win1, win2, win3,win4;
HWND edit, hWndComboBox, edit2, hWndComboBox2, hWndComboBox3, edit3, hWndComboBox4, edit4, edit5, hWndComboBox5;
HWND selectmodedisk, selectmodepar;
LRESULT CALLBACK InWin1Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin2Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin3Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
bool isFileExists_ifstream(string& name) {
	ifstream f(name.c_str());
	return f.good();
}
bool is_folder_path(string& path) {
	// Check for existence.
	if ((_access(path.c_str(), 0)) != -1) {
		return true; // 是文件夹
	}
	else {
		return false; // 不是文件夹或路径不存在或无法访问
	}
}
LPCWSTR STRING2LPCWSTR(string str)
{
	size_t size = str.length();
	int wLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t* buffer = new wchar_t[wLen + 1];
	memset(buffer, 0, (wLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, (LPWSTR)buffer, wLen);
	return buffer;
}
int GetPartitionNumber(const char* rootPath) {
	// 创建一个磁盘扩展分区信息结构体
	PARTITION_INFORMATION_EX partitionInfo;
	// 获取分区信息所需的字节数
	DWORD bytesReturned;
	// 打开分区的句柄
	HANDLE hPartition = CreateFileA(rootPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	// 判断句柄是否有效
	if (hPartition == INVALID_HANDLE_VALUE) {
		// 输出错误信息
		//throw "无法打开分区，程序运行异常";
		// 返回-1表示失败
		return -1;
	}
	// 调用DeviceIoControl函数，获取分区信息
	BOOL result = DeviceIoControl(hPartition, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0, &partitionInfo, sizeof(partitionInfo), &bytesReturned, NULL);
	// 关闭分区的句柄
	CloseHandle(hPartition);
	// 判断结果是否成功
	if (result) {
		// 返回分区序号
		return partitionInfo.PartitionNumber;
	}
	else {
		// 输出错误信息
		//throw "无法获取分区序号，程序运行异常";
		// 返回-1表示失败
		return -1;
	}
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
	switch (sel)
	{
	case 0: 
	{
		ShowWindow(win1, SW_SHOW);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_HIDE);
		break;
	}
	case 1:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_SHOW);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_HIDE);
		break;
	}
	case 2:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_SHOW);
		ShowWindow(win4, SW_HIDE);
		break;
	}
	case 3:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_SHOW);
		break;
	}
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



TCHAR* GetGhoFile(LPCWSTR ftr) {
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
	var->lpstrFilter = ftr;
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
			delete var;
			return szFilePath;
			//此处应该有对获得后的文件名和文件完整路径的各种操作
		}
	}
	else {
		delete var;
		return 0;
	}
	delete var;
	free(p);
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
	return sdn.DeviceNumber;
}

void AddDiskList(HWND cb) {
	SendMessage(cb, CB_RESETCONTENT, 0, 0);
	wchar_t drives[26 * 4 + 1];

	// 调用GetLogicalDriveStringsW函数，获取所有的磁盘盘符，并存放到字符数组中
	DWORD len = GetLogicalDriveStringsW(sizeof(drives), (LPWSTR)drives);
	// 遍历字符数组中的每个磁盘盘符
	for (DWORD i = 0; i < len; i += 4)
	{
		// 调用GetDriveTypeW函数，获取当前磁盘盘符的类型
		UINT type = GetDriveTypeW((LPWSTR)drives + i);
		if (type == DRIVE_FIXED || type == DRIVE_REMOVABLE)
		{
			ComboBox_AddString(cb, drives + i);
		}
	}
	SendMessage(cb, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

void GetDrivelist() {
	SendMessage(hWndComboBox, CB_RESETCONTENT, 0, 0);
	for (int a = 0; a <= 24; a++) {
		char deviceName[24] = "\\\\.\\PhysicalDrive";
		char str[4] = {0};
		_itoa(a, str, 10);
		strcat_s(deviceName, str);
		VOLUME_DISK_EXTENTS sdn;
		HANDLE hwnd = CreateFileA(deviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
		if (hwnd == INVALID_HANDLE_VALUE) {
			CloseHandle(hwnd);
			continue;
		}
		DWORD dwBytesReturned;
		DISK_GEOMETRY dg;
		BOOL bResult = DeviceIoControl(hwnd, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &dg, sizeof(dg), &dwBytesReturned, NULL);
		if (!bResult) {
			CloseHandle(hwnd);
			continue;
		}
		// 计算磁盘的大小
		long long disk_size = dg.Cylinders.QuadPart * dg.TracksPerCylinder * dg.SectorsPerTrack * dg.BytesPerSector;
		char strd[1024] = { 0 };
		_itoa(disk_size/1024/1024/1024, strd, 10);
		// 关闭物理驱动器
		CloseHandle(hwnd);
		CString str2 = CString(str);
		USES_CONVERSION;
		LPCWSTR wszClassName1 = new WCHAR[str2.GetLength() + 1];
		wcscpy((LPTSTR)wszClassName1, T2W((LPTSTR)str2.GetBuffer(NULL)));
		str2.ReleaseBuffer();
		CString str3 = CString(strd);
		LPCWSTR wszClassName2 = new WCHAR[str3.GetLength() + 1];
		wcscpy((LPTSTR)wszClassName2, T2W((LPTSTR)str3.GetBuffer(NULL)));
		str3.ReleaseBuffer();
		SendMessage((hWndComboBox), 0x0143, 0L, (LPARAM)(LPCTSTR)("磁盘" + str2 + "     大小："+str3+" GB"));
		SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}
}

void GetWimSysInfo(TCHAR* wimstr) {
	SendMessage(hWndComboBox4, CB_RESETCONTENT, 0, 0);
	WIMStruct *WIMFile;
	int result;
	result = wimlib_open_wim(wimstr,0,&WIMFile);
	if (result != 0) {
		ComboBox_AddString(hWndComboBox4, L"错误：无法打开WIM/ESD文件！");
		SendMessage(hWndComboBox4, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		return;
	}
	void *xml_data;
	size_t xml_size = 0;
	result = wimlib_get_xml_data(WIMFile, &xml_data, &xml_size);
	if (result != 0) {
		ComboBox_AddString(hWndComboBox4, L"未知");
		SendMessage(hWndComboBox4, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		wimlib_free(WIMFile);
		return;
	}
	wimlib_free(WIMFile);
	_bstr_t a((wchar_t*)xml_data);
	const char* xml_file = a;
	regex r("<DISPLAYNAME>(.*?)</DISPLAYNAME>");
	string xml = xml_file;
	sregex_iterator it(xml.begin(), xml.end(), r);
	sregex_iterator end;
	while (it != end) {
		ComboBox_AddString(hWndComboBox4, STRING2LPCWSTR(it->str(1)));
		it++;
	}
	SendMessage(hWndComboBox4, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
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
	wndcls3.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls3.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls3.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls3.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls3.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls3.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls3.lpfnWndProc = InWin2Proc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls3.lpszClassName = L"INWIN2";//设置窗体的类名
	wndcls3.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls3.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls3);//向操作系统注册窗体
	WNDCLASS wndcls4; //创建一个窗体类
	wndcls4.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls4.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls4.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls4.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls4.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls4.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls4.lpfnWndProc = InWin3Proc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
	wndcls4.lpszClassName = L"INWIN3";//设置窗体的类名
	wndcls4.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls4.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls4);//向操作系统注册窗体
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
		L"程序说明",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON |BS_FLAT,  // Styles 
		22,         // x position 
		281,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)diskbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	HBITMAP logo = (HBITMAP)LoadImage(NULL, L".\\COMPE.bmp", IMAGE_BITMAP, 208, 60, LR_LOADFROMFILE);
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
		PROGRESS_CLASS, (LPTSTR)NULL,
		WS_VISIBLE | WS_CHILD | PBS_MARQUEE,  // Styles 
		0,         // x position 
		424,         // y position 
		624,        // Button width
		17,        // Button height
		hwnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	hFont = CreateFont(20,                                    //   字体的高度   
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
		_T("微软雅黑"));
	hFont2 = CreateFont(17,                                    //   字体的高度   
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
		_T("微软雅黑"));
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
	win4 = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 372, 355, hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	SendMessage(win4, WM_SETFONT, (WPARAM)hFont2, 1);
	string program_info = "感谢您使用ComPE Windows安装程序！\n\
目前Windows的安装方式主要有Ghost系统安装，Windows Vista+的WIM/ESD安装和NT5.x类的I386三种安装方法，程序提供了三种方式以应对不同的操作系统的安装执行。\n\
为保证您的使用体验，使用时需注意以下几点：\n\
1.Ghost备份还原的时候由于赛门铁克Ghost官方限制，程序路径请不要携带非英文字符，否则可能导致因无法识别目录导致安装失败。\n\
2.WIM/ESD安装期间，程序可能陷入一段时间的无响应，属于正常现象，不要试图关闭程序或重启计算机，否则可能导致设备损坏。\n\
3.WIM/ESD安装（Legacy BIOS启动）和NT5.x安装方式要求，目标分区必须是一个活动主分区，否则将会因为没有引导而启动失败，可通过DiskGenius或傲梅分区助手查看分区状态是否为活动主分区。\n\
4.WIM/ESD安装（UEFI BIOS启动）模式中，为了正常的引导，建议您提前将ESP分区分配一个盘符，以便于安装程序安装引导。\n\
5.NT5.x系统不支持UEFI启动和GPT硬盘格式，请确定自己的启动方式是Legacy启动并且硬盘格式是MBR，可通过Diskgenius或傲梅分区助手检查格式并更改成MBR格式磁盘。\n\
6.程序执行操作期间，不要对计算机进行任何操作，否则都有可能对您的设备造成损坏！\n\
感谢您的使用，祝您使用愉快！";
	SetWindowText(win4, STRING2LPCWSTR(program_info));
	HWND btfile1 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
	280,119, 64, 22, win1, (HMENU)gholoadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
	NULL);
	HWND btre1 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 150, 64, 22, win1, (HMENU)ghodiskbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(btfile1, WM_SETFONT, (WPARAM)hFont2, 1);
	::SendMessage(btre1, WM_SETFONT, (WPARAM)hFont2, 1);
	edit = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		100, 119, 180, 21,win1, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	::SendMessage(edit, WM_SETFONT, (WPARAM)hFont2, 1);
	hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		100, 150, 180,18, win1, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(hWndComboBox, WM_SETFONT, (WPARAM)hFont2, 1);
	ghostartbtn = CreateWindow(L"BUTTON", L"执行Ghost还原", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win1, (HMENU)ghostartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(ghostartbtn, WM_SETFONT, (WPARAM)hFont2, 1);
	selectmodepar =  CreateWindowEx(WS_EX_WINDOWEDGE,
		L"BUTTON",
		L"分区模式",
		WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,  // <---- WS_GROUP group the following radio buttons 1st,2nd button
		90, 179,
		100, 20,
		win1, //<----- Use main window handle
		(HMENU)parmode,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),NULL );
	selectmodedisk = CreateWindow(
		L"BUTTON",
		L"磁盘模式",
		WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,  // Styles
		190, 179,
		100, 20,
		win1,
		(HMENU)diskmode,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	::SendMessage(selectmodedisk, WM_SETFONT, (WPARAM)hFont2, 1);
	::SendMessage(selectmodepar, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(selectmodepar, BM_SETCHECK, 1, 0);
	SendMessage(selectmodedisk, BM_SETCHECK, 0, 0);
	HWND btfile2 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 94, 64, 22, win2, (HMENU)wimloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btre2 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 125, 64, 22, win2, (HMENU)wimdiskbt1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	
	::SendMessage(btfile2, WM_SETFONT, (WPARAM)hFont2, 1);
	::SendMessage(btre2, WM_SETFONT, (WPARAM)hFont2, 1);
	edit2 = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		100, 94, 180, 21, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hWndComboBox2 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		100, 125, 180, 18, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	if (GetFirmware() == "UEFI") {
		hWndComboBox3 = CreateWindow(WC_COMBOBOX, TEXT(""),
			CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			100, 185, 180, 18, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);
		::SendMessage(hWndComboBox3, WM_SETFONT, (WPARAM)hFont2, 1);
		HWND btre3 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
			280, 186, 64, 22, win2, (HMENU)wimdiskbt2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);
		::SendMessage(btre3, WM_SETFONT, (WPARAM)hFont2, 1);
	}
	hWndComboBox4 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		100, 155, 180, 18, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(edit2, WM_SETFONT, (WPARAM)hFont2, 1);
	::SendMessage(hWndComboBox2, WM_SETFONT, (WPARAM)hFont2, 1);
	::SendMessage(hWndComboBox4, WM_SETFONT, (WPARAM)hFont2, 1);
	btwimstart = CreateWindow(L"BUTTON", L"应用WIM/ESD映像", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win2, (HMENU)wimstartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btfile3 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 250, 64, 22, win2, (HMENU)xmloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(btwimstart, WM_SETFONT, (WPARAM)hFont2, 1);
	::SendMessage(btfile3, WM_SETFONT, (WPARAM)hFont2, 1);
	edit3 = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		100, 250, 180, 21, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	::SendMessage(edit3, WM_SETFONT, (WPARAM)hFont2, 1);
	btxpstart = CreateWindow(L"BUTTON", L"安装旧版Windows", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win3, (HMENU)xpstartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(btxpstart, WM_SETFONT, (WPARAM)hFont2, 1);
	edit4 = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		100, 99, 180, 21, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	::SendMessage(edit4, WM_SETFONT, (WPARAM)hFont2, 1);
	edit5 = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		100, 169, 180, 21, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	::SendMessage(edit5, WM_SETFONT, (WPARAM)hFont2, 1);
	hWndComboBox5 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		100, 134, 180, 18, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(hWndComboBox5, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btre3 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 134, 64, 22, win3, (HMENU)xpdiskbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(btre3, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btdir = CreateWindow(L"BUTTON", L"选择文件夹", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 99, 64, 22, win3, (HMENU)xploadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(btdir, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btfile4 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 169, 64, 22, win3, (HMENU)txtloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	::SendMessage(btfile4, WM_SETFONT, (WPARAM)hFont2, 1);
	AddDiskList(hWndComboBox);
	AddDiskList(hWndComboBox2);
	AddDiskList(hWndComboBox3);
	AddDiskList(hWndComboBox5);
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
		TCHAR msg3[] = L"           本页面可用于对该类文件应用到磁盘分区。";
		TCHAR msg4[] = L"注：因Ghost软件自身原因，GHO目录请不要带有非英文字母！";
		TCHAR ghodir[] = L"GHO文件位置：";
		TCHAR ghodisk[] = L"   安装目标：";
		TCHAR msg5[] = L"      建议您在备份好数据的情况下清空分区文件再继续。";
		TCHAR msg6[] = L"               安装期间请勿操作设备，以免产生损坏。";
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
		TextOut(hdc, 31, 152, ghodisk, _tcslen(ghodisk));
		TextOut(hdc, 32, 215, msg5, _tcslen(msg5));
		TextOut(hdc, 32, 230, msg6, _tcslen(msg6));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case gholoadbt:
		{
			Edit_SetText(edit, GetGhoFile(L"Ghost映像文件(*.gho)\0*.gho\0\0"));
			break;
		}
		case ghodiskbt:
		{
			if (ispar) {
				AddDiskList(hWndComboBox);
			}
			else {
				GetDrivelist();
			}
			break;
		}
		case parmode: {
			AddDiskList(hWndComboBox);
			ispar = true;
			SendMessage(selectmodepar, BM_SETCHECK, 1, 0);
			SendMessage(selectmodedisk, BM_SETCHECK, 0, 0);
			break;
		}
		case diskmode: {
			GetDrivelist();
			ispar = false;
			SendMessage(selectmodepar, BM_SETCHECK, 0, 0);
			SendMessage(selectmodedisk, BM_SETCHECK, 1, 0);
			break;
		}
		case ghostartbt: {
			TCHAR dirs[1024] = { 0 };
			Edit_GetText(edit, dirs, 1024);
			string dirstr = TCHAR2STRING(dirs);
			if (!isFileExists_ifstream(dirstr)) {
				MessageBox(hwnd, L"请选择正确的GHO文件！", L"错误：", MB_ICONERROR);
			}
			else {
				if (MessageBox(hwnd, L"确认应用Ghost映像吗？执行操作期间请勿操作电脑。", L"警告：", MB_YESNO | MB_ICONWARNING) == IDYES) {
					::EnableWindow(ghostartbtn, false);
					if (ispar) {
						TCHAR disk[3] = { 0 };
						char roots[7] = "\\\\.\\";
						ComboBox_GetText(hWndComboBox, disk, 3);
						string target = TCHAR2STRING(disk);
						const char* tg = strcat(roots, target.c_str());
						int pn = GetPartitionNumber(tg);
						string ghostexec = ".\\Ghost\\ghost64.exe -clone,mode=pload,src=" + dirstr + ":1,dst=" + to_string(GetDiskNum(strcpy((char*)malloc(target.length() + 1), target.c_str()))+1) + ":" + to_string(pn) + " -sure";
						system(ghostexec.c_str());
					}
					else {
						TCHAR root[1024] = { 0 };
						ComboBox_GetText(hWndComboBox,root,1024);
						string diskroot = TCHAR2STRING(root);
						char target = diskroot.back();
						string ghostexec = ".\\Ghost\\ghost64.exe -clone,mode=load,src=" + TCHAR2STRING(dirs)+",dst="+target+" -sure";
						system(ghostexec.c_str());
					}
					::EnableWindow(ghostartbtn, true);
				}
			}
		}
		break;
		}
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
		TCHAR msg2[] = L"WIM映像格式并非基于扇区的映像格式，它是基于文件的。";
		TCHAR msg3[] = L"   ESD文件是WIM格式的升级版，使用更高级的压缩算法。";
		TCHAR msg4[] = L"          本页面可用于安装对应操作系统以及还原备份。";
		TCHAR wimdir[] = L"WIM/ESD位置：";
		TCHAR wimdisk[] = L"   安装目标：";
		TCHAR wimbisk[] = L"EFI引导分区：";
		TCHAR wimos[] = L"   系统选择：";
		TCHAR xmldir[] = L"  应答文件：";
		TCHAR msg5[] = L"如果您是UEFI（请保证目标分区所在磁盘分区格式是GPT）";
		TCHAR msg6[] = L"建议您临时将ESP分区分配盘符后，将其设置成您的引导分区";
		TCHAR msg7[] = L"          （留空或路径不正确则不进行无人值守模式）";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 20, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 35, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 50, msg3, _tcslen(msg3));
		TextOut(hdc, 32, 65, msg4, _tcslen(msg4));
		TextOut(hdc, 12, 95, wimdir, _tcslen(wimdir));
		TextOut(hdc, 31, 128, wimdisk, _tcslen(wimdisk));
		if (GetFirmware() == "UEFI") {
			TextOut(hdc, 27, 190, wimbisk, _tcslen(wimbisk));
		}
		TextOut(hdc, 31, 160, wimos, _tcslen(wimos));
		TextOut(hdc, 21, 250, xmldir, _tcslen(xmldir));
		TextOut(hdc, 32, 215, msg5, _tcslen(msg5));
		TextOut(hdc, 32, 230, msg6, _tcslen(msg6));
		TextOut(hdc, 32, 270, msg7, _tcslen(msg7));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case wimloadbt:
		{
			TCHAR* file = GetGhoFile(L"Windows映像文件(*.wim/*.esd)\0*.wim;*.esd\0\0");
			Edit_SetText(edit2, file);
			GetWimSysInfo(file);
			break;
		}
		case wimdiskbt1:
		{
			AddDiskList(hWndComboBox2);
			break;
		}
		case wimdiskbt2:
		{
			AddDiskList(hWndComboBox3);
			break;
		}
		case xmloadbt:
		{
			Edit_SetText(edit3, GetGhoFile(L"应答文件(*.xml)\0*.xml\0\0"));
			break;
		}
		case wimstartbt:
		{
			TCHAR dirs[1024] = { 0 };
			Edit_GetText(edit2, dirs, 1024);
			string dirstr = TCHAR2STRING(dirs);
			if (!isFileExists_ifstream(dirstr))
			{
				MessageBox(hwnd, L"请选择正确的WIM文件！", L"错误：", MB_ICONERROR);
			}
			else 
			{
				if (MessageBox(hwnd, L"确认应用WIM吗？执行操作期间请勿操作电脑。", L"警告：", MB_YESNO | MB_ICONWARNING) == IDYES)
				{
					SendMessage(hpbar, PBM_SETRANGE, 0, 4);
					::EnableWindow(btwimstart, false);
					WIMStruct* WIM;
					int result;
					result = wimlib_open_wim(dirs, 0, &WIM);
					if (result != 0) {
						MessageBox(hWnd, L"未能成功打开WIM/ESD文件，请检查文件是否是正确的映像文件！", 0, MB_ICONERROR);
						::EnableWindow(btwimstart, true);
						break;
					}
					SendMessage(hpbar, PBM_DELTAPOS, 1, 0);
					//
					int cs = ComboBox_GetCurSel(hWndComboBox4) + 1;
					TCHAR tar[1024] = { 0 };
					ComboBox_GetText(hWndComboBox2, tar, 1024);
					result = wimlib_extract_image(WIM, cs, tar, NULL);
					if (result != 0) {
						MessageBox(hWnd, STRING2LPCWSTR("应用WIM/ESD映像的时候发生错误，错误代码：" + to_string(result)), 0, MB_ICONERROR);
						::EnableWindow(btwimstart, true);
						wimlib_free(WIM);
						break;
					}
					SendMessage(hpbar, PBM_DELTAPOS, 1, 0);
					//
					wimlib_free(WIM);
					TCHAR uad[1024] = { 0 };
					Edit_GetText(edit3, uad, 1024);
					string uadfile = TCHAR2STRING(uad);
					if (isFileExists_ifstream(uadfile)) {
						CopyFile(uad, STRING2LPCWSTR(TCHAR2STRING(tar) + "Windows\\Panther\\unattend.xml"),false);
					}
					SendMessage(hpbar, PBM_DELTAPOS, 1, 0);
					//
					if (GetFirmware() == "UEFI") {
						TCHAR boot[1024] = { 0 };
						ComboBox_GetText(hWndComboBox3, boot, 1024);
						string cmd = "bcdboot " + TCHAR2STRING(tar) + "Windows /s "+TCHAR2STRING(boot).at(0)+": /f UEFI";
						system(cmd.c_str());
					}
					else {
						string cmd = ".\\bootsect.exe /nt60 " + to_string(TCHAR2STRING(tar).at(0)) + ": /mbr";
						system(cmd.c_str());
					}
					SendMessage(hpbar, PBM_DELTAPOS, 1, 0);
					//
					MessageBox(hWnd, L"应用完成！重启计算机后将进行进一步安装Windows操作！", L"成功：", MB_ICONERROR);
					::EnableWindow(btwimstart, true);
					SendMessage(hpbar, PBM_SETPOS, 0, 0);
					break;
				}
			}
		}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}

LRESULT CALLBACK InWin3Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //该子窗口消息不会被处理，测试阶段
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"         对于早期Windows操作系统（XP及更早版本）";
		TCHAR msg2[] = L"        光盘映像内由一个I386文件夹存放安装信息";
		TCHAR msg3[] = L"   本页面可用于安装NT5.x的操作系统（暂不支持x64）。";
		TCHAR i86dir[] = L"   I386目录：";
		TCHAR i86disk[] = L"   安装分区：";
		TCHAR txtdir[] = L"   应答文件：";
		TCHAR msg4[] = L"          （留空或路径不正确则不进行无人值守模式）";
		TCHAR msg5[] = L"    注：所有早期Windows操作系统均不支持UEFI启动";
		TCHAR msg6[] = L"              I386文件夹目录格式示例：“D:\\I386”";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 34, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 49, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 64, msg3, _tcslen(msg3));
		TextOut(hdc, 32, 99, i86dir, _tcslen(i86dir));
		TextOut(hdc, 32, 134, i86disk, _tcslen(i86disk));
		TextOut(hdc, 32, 169, txtdir, _tcslen(txtdir));
		TextOut(hdc, 32, 194, msg4, _tcslen(msg4));
		TextOut(hdc, 32, 219, msg5, _tcslen(msg5));
		TextOut(hdc, 32, 234, msg6, _tcslen(msg6));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case txtloadbt:
		{
			Edit_SetText(edit5, GetGhoFile(L"应答文件(*.txt/*.sif)\0*.txt;*.sif\0\0"));
			break;
		}
		case xpdiskbt:
		{
			AddDiskList(hWndComboBox5);
			break;
		}
		case xploadbt:
		{
			TCHAR szBuffer[MAX_PATH] = { 0 };
			BROWSEINFO bi;
			ZeroMemory(&bi, sizeof(BROWSEINFO));
			bi.hwndOwner = NULL;
			bi.pszDisplayName = szBuffer;
			bi.lpszTitle = _T("从下面选I386文件夹目录:");
			bi.ulFlags = BIF_RETURNFSANCESTORS;
			LPITEMIDLIST idl = SHBrowseForFolder(&bi);
			if (NULL == idl)
			{
				break;
			}
			SHGetPathFromIDList(idl, szBuffer);
			Edit_SetText(edit4, szBuffer);
		}
		case xpstartbt:
		{
			TCHAR dirs[1024] = { 0 };
			Edit_GetText(edit4, dirs, 1024);
			string dirstr = TCHAR2STRING(dirs);
			if (!is_folder_path(dirstr))
			{
				MessageBox(hwnd, L"请输入正确的文件夹路径！", L"错误：", MB_ICONERROR);
			}
			else {
				if (MessageBox(hwnd, L"确认安装NT5.x的操作系统吗？期间请勿进行任何操作。", L"提示：", MB_ICONQUESTION | MB_OKCANCEL) == IDOK) 
				{
					::EnableWindow(btxpstart, false);
					TCHAR tar[1024] = { 0 };
					ComboBox_GetText(hWndComboBox5, tar, 1024);
					string tarstr = TCHAR2STRING(tar);
					TCHAR i86fdr[1024] = { 0 };
					string fileslist[] = { "1394BUS.SY_","ABP480N5.SY_","ACPI.SY_","ACPIEC.SY_","ADPU160M.SY_","AHA154X.SY_","AIC78U2.SY_","AIC78XX.SY_","ALIIDE.SY_","AMSINT.SY_","ASC.SY_","ASC3350P.SY_","ASC3550.SY_","ATAPI.SY_","BIOSINFO.INF","BOOTFONT.BIN","BOOTVID.DL_","CBIDF2K.SY_","CD20XRNT.SY_","CDFS.SY_","CDROM.SY_","CLASSPNP.SY_","CMDIDE.SY_","CPQARRAY.SY_","C_936.NL_","DAC2W2K.SY_","DAC960NT.SY_","DISK.SY_","DISK1","DMBOOT.SY_","DMIO.SY_","DMLOAD.SY_","DPTI2O.SY_","DRVMAIN.SDB","FASTFAT.SY_","FDC.SY_","FLPYDISK.SY_","FTDISK.SY_","HAL.DL_","HALAACPI.DL_","HALACPI.DL_","HALAPIC.DL_","HIDCLASS.SY_","HIDPARSE.SY_","HIDUSB.SY_","HPN.SY_","I2OMGMT.SY_","I2OMP.SY_","I8042PRT.SY_","INI910U.SY_","INTELIDE.SY_","ISAPNP.SY_","KBDCLASS.SY_","KBDHID.SY_","KBDUS.DLL","KD1394.DL_","KDCOM.DL_","KSECDD.SYS","LBRTFDC.SY_","L_INTL.NL_","migrate.inf","MOUNTMGR.SY_","MRAID35X.SY_","NTDETECT.COM","NTFS.SYS","NTKRNLMP.EX_","NTLDR","OHCI1394.SY_","OPRGHDLR.SY_","PARTMGR.SY_","PCI.SY_","PCIIDE.SY_","PCIIDEX.SY_","PCMCIA.SY_","PERC2.SY_","PERC2HIB.SY_","QL1080.SY_","QL10WNT.SY_","QL12160.SY_","QL1240.SY_","QL1280.SY_","RAMDISK.SY_","SBP2PORT.SY_","SCSIPORT.SY_","SERENUM.SY_","SERIAL.SY_","SETUPDD.SY_","SETUPLDR.BIN","SETUPREG.HIV","SFLOPPY.SY_","SPARROW.SY_","SPCMDCON.SYS","SPDDLANG.SY_","SYMC810.SY_","SYMC8XX.SY_","SYM_HI.SY_","SYM_U3.SY_","TFFSPORT.SY_","TOSIDE.SY_","TXTSETUP.SIF","ULTRA.SY_","USBCCGP.SY_","USBD.SY_","USBEHCI.SY_","USBHUB.SY_","USBOHCI.SY_","USBPORT.SY_","USBSTOR.SY_","USBUHCI.SY_","VGA.SY_","VGAOEM.FO_","VIAIDE.SY_","VIDEOPRT.SY_","WMILIB.SY_" };
					SendMessage(hpbar, PBM_SETRANGE, 0, sizeof(fileslist));
					namespace fs = std::filesystem;
					for (int a = 0; a <= sizeof(fileslist); a++)
					{
						for (const auto& entry : fs::directory_iterator(dirstr))
						{
							if (entry.path().u8string() == dirstr + "\\" + fileslist[a]) {
								CopyFile(STRING2LPCWSTR(entry.path().u8string()), STRING2LPCWSTR(tarstr + "$WIN_NT$.~BT\\" + fileslist[a]), false);
								SendMessage(hpbar, PBM_DELTAPOS, 1, 0);
								break;
							}
							MessageBox(hwnd, L"发生复制错误：安装文件夹不完整，请检查资源完整性，然后重新尝试。", NULL, MB_ICONERROR);
							::EnableWindow(btxpstart, true);
							SendMessage(hpbar, PBM_SETPOS, 0, 0);
							return 0;
						}
					}
					TCHAR txtfile[1024] = { 0 };
					Edit_GetText(edit5, txtfile, 1024);
					if (isFileExists_ifstream(TCHAR2STRING(txtfile))) {
						CopyFile(txtfile, STRING2LPCWSTR(tarstr + "$WIN_NT$.~BT\\WINNT.SIF"),false);
					}
					string cmd = ".\\bootsect.exe /nt60 " + to_string(TCHAR2STRING(tar).at(0)) + ": /mbr";
					system(cmd.c_str());
					MessageBox(hwnd, L"安装成功！重启后将进行进一步安装。", L"提示：", MB_ICONINFORMATION);
					::EnableWindow(btxpstart, true);
					SendMessage(hpbar, PBM_SETPOS, 0, 0);
				}
			}
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
				powerOffProc();//不采用shutdown.exe，而是用API重启计算机，防止PE内无shutdown.exe导致无法重启
			}
			break;
		}
		case logobt:
			try {
				system("about");
			}
			catch(exception){

			}
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"确定要关闭程序吗？执行操作期间关闭可能导致设备损坏，确保所有操作结束后，选择“是”关闭程序。", L"提示：", MB_ICONQUESTION | MB_YESNO) == IDYES) {
			DestroyWindow(hwnd);//销毁窗口并发送WM_DESTROY消息，但是程序没有退出
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);//发出WM_QUIT消息，结束消息循环
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}