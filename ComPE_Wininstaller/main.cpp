﻿// Copyright (C) 2024  Commandpr
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Author: Commandpr
// Email: commandpr@foxmail.com
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <Windows.h>
#include <io.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <CommCtrl.h>
#include <atlimage.h>
#include <thread>
#include <regex>
#include <vector>
#include <spdlog/fmt/ostr.h>
#include <comdef.h>
#include <fcntl.h>
#include <Shlobj.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "wimlib.h"
#include "dismapi.h"
#include "json/json.h"
#include "tinyxml.h"
#include "resource.h"
#include "SecureBootAndTPMCheck.h"
#include "getdiskslist.h"
#pragma comment(lib,"libwim.lib")
#pragma comment(lib,"dismapi.lib")

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
#define xpisoloadbt 1008
#define txtloadbt 3001
#define xpdiskbt 3002
#define wimloadbt 1008
#define xmloadbt 1111
#define ghostartbt 1010
#define wimstartbt 1011
#define savestartbt 1013
#define xpstartbt 1012
#define DRIVELOADBT 9009
#define RUNIMGDOWNLOAD 9010
#define SWMBT 9011
#define saveloadbt 1113
#define savediskbt 2002
#define diskmode 5000
#define parmode 6000
#define uxml 7000
#define cxml 8000
#define xmlloadbt 1112
#define btyes 9001
#define btno 9000
#define fmt1 101
#define fmt2 102
#define fmt3 103

int scrWidth = GetSystemMetrics(SM_CXSCREEN);
int scrHeight = GetSystemMetrics(SM_CYSCREEN);
int createx = scrWidth / 2 - 320;
int createy = scrHeight / 2 - 240;
bool ispar = true;
HWND hWnd;
HWND btnlogo;
HFONT hFont;
HFONT hFont2,hFont3;
HWND block;
HWND hTabCtrl;
HWND btndisk;
HWND barbg, barfw, protxt,protxt2,protxt3;
HWND btnreboot;
HWND btnxp;
HWND btnwim;
HWND btnghost, ghostartbtn, btwimstart, btxpstart, btSWM;
HWND win1, win2, win3, win4, win5;
HWND edit, hWndComboBox, edit2, hWndComboBox2, hWndComboBox3, edit3, hWndComboBox4, edit4, edit5, hWndComboBox5,editDrive,StcCPUMode;
HWND hsavediskcb,hsavepathedit,hcompcbox;
HWND selectmodedisk, selectmodepar;
LRESULT CALLBACK InWin1Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin2Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin3Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InWin4Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TWNSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ISOSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SWMSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
using namespace std;
vector<wstring> EnableSWMs;
string MountedDisk = "";
bool isInstalled = true;
vector<string> MountEFI;
const char* WIMLIB_ERROR_STRING_OF_CHINESE = R"({
"0": "成功",
"1": "已锁定",
"2": "解压错误",
"6": "FUSE错误",
"8": "没有匹配项",
"10": "图像计数错误",
"11": "图像名称冲突",
"12": "权限不足",
"13": "完整性错误",
"14": "无效的捕获配置",
"15": "无效的块大小",
"16": "无效的压缩类型",
"17": "无效的头部",
"18": "无效的图像",
"19": "无效的完整性表",
"20": "无效的查找表条目",
"21": "无效的元数据资源",
"23": "无效的覆盖",
"24": "无效的参数",
"25": "无效的部分编号",
"26": "无效的可管道WIM",
"27": "无效的重解析数据",
"28": "无效的资源哈希",
"30": "无效的UTF-16字符串",
"31": "无效的UTF-8字符串",
"32": "是目录",
"33": "是拆分的WIM",
"35": "链接错误",
"36": "未找到元数据",
"37": "创建目录错误",
"38": "消息队列错误",
"39": "内存不足",
"40": "不是目录",
"41": "目录不为空",
"42": "不是常规文件",
"43": "不是WIM文件",
"44": "不可管道",
"45": "没有文件名",
"46": "NTFS-3G错误",
"47": "打开错误",
"48": "打开目录错误",
"49": "路径不存在",
"50": "读取错误",
"51": "读取链接错误",
"52": "重命名错误",
"54": "重解析点修复失败",
"55": "未找到资源",
"56": "资源顺序错误",
"57": "设置属性错误",
"58": "设置重解析数据错误",
"59": "设置安全性错误",
"60": "设置短名称错误",
"61": "设置时间戳错误",
"62": "拆分无效",
"63": "状态错误",
"65": "文件意外结束",
"66": "Unicode字符串不可表示",
"67": "未知版本",
"68": "不支持",
"69": "不支持的文件",
"71": "WIM是只读的",
"72": "写入错误",
"73": "XML错误",
"74": "WIM已加密",
"75": "WIMBOOT错误",
"76": "进度被中止",
"77": "未知的进度状态",
"78": "创建节点错误",
"79": "挂载的图像正忙",
"80": "不是挂载点",
"81": "不允许卸载",
"82": "FVE锁定卷",
"83": "无法读取捕获配置",
"84": "WIM不完整",
"85": "无法压缩",
"86": "图像有多个引用",
"87": "重复的导出图像",
"88": "检测到并发修改",
"89": "快照失败",
"90": "无效的扩展属性",
"91": "设置扩展属性错误"
})";

auto mylogger = spdlog::rotating_logger_mt("CWINST_LOGGER", ".\\log\\cwinst.log", 1048576 * 10, 3);
void initLogger() {
	// 创建一个循环文件日志接收器，文件大小限制为10MB
	mylogger->info("设置日志格式。");
	// 设置日志格式
	mylogger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] [%t] %v");
	spdlog::flush_every(std::chrono::seconds(1));
	// 写入一条信息级别的日志
	mylogger->info("初始化日志系统完成！");

}
DWORD RunMyExec(const char* cmd) {
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// 初始化 STARTUPINFO 结构体
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW; // 不显示窗口

	// 初始化 PROCESS_INFORMATION 结构体
	ZeroMemory(&pi, sizeof(pi));

	// 执行程序
	if (!CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, FALSE,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		return 1; // 返回错误代码
	}

	// 等待程序执行完毕
	WaitForSingleObject(pi.hProcess, INFINITE);

	// 获取程序退出代码
	DWORD exitCode;
	GetExitCodeProcess(pi.hProcess, &exitCode);

	// 关闭进程和线程句柄
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// 返回程序退出代码
	return exitCode;
}
string ws2s(const wstring& ws) {
	try {
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		return result;
	}
	catch (exception) {
		return "";
	}
}
wstring s2ws(const string& s) {
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

void InstalledImDisk() {
	int result = RunMyExec("imdisk --version");
	if (result != 0) {
		isInstalled = false;
		mylogger->info("环境没有imdisk，临时安装。");
		RunMyExec(".\\ImdiskPack\\install.cmd");
		return;
	}
	isInstalled = true;
}

void UninstalledImDisk() {
	if (!isInstalled) {
		mylogger->info("环境没有imdisk，卸载临时安装的imdisk。");
		RunMyExec(".\\ImdiskPack\\uninstall_imdisk.cmd");
		return;
	}
}
BOOL GetCPUOfWinXP(wstring filePath, wstring* SysVer) {
	std::wifstream file(filePath + L"\\I386\\TXTSETUP.SIF");

	if (!file.is_open()) {
		file.open(filePath + L"\\AMD64\\TXTSETUP.SIF");
		if (!file.is_open()) {
			mylogger->error("试图获取{:s}的镜像系统版本失败，文件无法打开。", ws2s(filePath));
			MessageBox(hWnd, L"无法打开安装程序文件！请检查选择的文件夹完整性！", NULL, MB_ICONERROR);
			return -1;
		}
	}

	std::wstring line;
	bool inStringsSection = false;
	std::wstring productName;

	while (getline(file, line)) {
		// 检查是否进入[Strings]节
		if (line.find(L"[Strings]") != std::string::npos) {
			inStringsSection = true;
			continue;
		}

		// 检查是否离开[Strings]节
		if (inStringsSection && line.find(L"]") != std::string::npos) {
			break;
		}

		// 如果在[Strings]节内，检查ProductName
		if (inStringsSection) {
			size_t keyStart = line.find(L"productname = ");
			if (keyStart != std::string::npos) {
				// 找到ProductName，提取其值
				size_t valueStart = keyStart + std::wstring(L"productname = ").length();
				size_t valueEnd = line.find_first_of(L"\n", valueStart);
				productName = line.substr(valueStart, valueEnd - valueStart);
				break;
			}
		}
	}

	file.close();

	if (!productName.empty()) {
		*SysVer = productName;
		return productName.find(L"x64") != std::wstring::npos;
	}
	else {
		mylogger->error("试图获取{:s}的镜像系统版本失败，文件格式错误。", ws2s(filePath));
		MessageBox(hWnd, L"不是有效的Windows安装源！请检查选择的文件夹完整性！", NULL, MB_ICONERROR);
		return -1;
	}

	return 0;
}
const wchar_t* GetWimErrorString(int ErrCode) {
	Json::Value value;
	Json::Reader reader;
	reader.parse(WIMLIB_ERROR_STRING_OF_CHINESE, value);
	string s = value[to_string(ErrCode)].asString();
	wstring ws = s2ws(s);
	const wchar_t* w = ws.c_str();
	return w;
}


bool isFileExists_ifstream(string& name) {
	ifstream f(name.c_str());
	return f.good();
}
bool endsWithEsd(const TCHAR* str) {
	const TCHAR* esdLower = _T(".esd");
	const TCHAR* esdUpper = _T(".ESD");
	size_t len = _tcslen(str);
	size_t esdLen = _tcslen(esdLower);

	if (len < esdLen) {
		return false;
	}

	const TCHAR* end = str + len - esdLen;

	return (_tcsicmp(end, esdLower) == 0 || _tcsicmp(end, esdUpper) == 0);
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
	FIRMWARE_TYPE ft = FirmwareTypeUnknown;
	GetFirmwareType(&ft);
	switch (ft) {
	case FirmwareTypeBios:{
		return "BIOS";
	}
	case FirmwareTypeUefi: {
		return "UEFI";
	}
	default:{
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
		ShowWindow(win5, SW_HIDE);
		break;
	}
	case 1:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_SHOW);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_HIDE);
		ShowWindow(win5, SW_HIDE);
		break;
	}
	case 2:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_SHOW);
		ShowWindow(win4, SW_HIDE);
		ShowWindow(win5, SW_HIDE);
		break;
	}
	case 3:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_SHOW);
		ShowWindow(win5, SW_HIDE);
		break;
	}
	case 4:
	{
		ShowWindow(win1, SW_HIDE);
		ShowWindow(win2, SW_HIDE);
		ShowWindow(win3, SW_HIDE);
		ShowWindow(win4, SW_HIDE);
		ShowWindow(win5, SW_SHOW);
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
atomic<bool> isloading(false);
//关机
bool powerOffProc()
{
	if (!improvePv() || !ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION))
	{

		return false;
	}
	return true;
}



TCHAR* GetGhoFile(LPCWSTR ftr,HWND hwnd) {
	mylogger->info("尝试打开文件，格式为{:s}", ws2s(ftr));
	TCHAR szPath[MAX_PATH]{ 0 };//存储当前文件所在路径（不含文件名称和最后面的斜杠）
	TCHAR szTitle[MAX_PATH]{ 0 };//存储当前文件名
	TCHAR szFilePath[MAX_PATH]{ 0 };//存储当前文件路径
	TCHAR szFiles[80 * MAX_PATH]{ 0 };//可以存放至少80个文件的路径,即存储所有获得文件
	TCHAR* p;
	LPOPENFILENAME var = new OPENFILENAMEW;
	ZeroMemory(var, sizeof(OPENFILENAMEW));
	var->hwndOwner = hwnd;
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
			mylogger->info("选择的文件是{:s}", ws2s(szFilePath));
			return szFilePath;
			//此处应该有对获得后的文件名和文件完整路径的各种操作
		}
	}
	else {
		delete var;
		return L"";
	}
	delete var;
	free(p);
	return L"";
}

int GetDiskNum(const char str[2]) {
	char deviceName[7] = "\\\\.\\";
	strcat_s(deviceName, str);
	STORAGE_DEVICE_NUMBER sdn;
	HANDLE hwnd = CreateFileA(deviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hwnd == INVALID_HANDLE_VALUE) {
		CloseHandle(hwnd);
		return -1;
	}
	if (DeviceIoControl(hwnd, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, NULL, &sdn, 16, 0, 0) == 0) {
		CloseHandle(hwnd);
		return -1;
	}
	CloseHandle(hwnd);
	return sdn.DeviceNumber;
}
void DeleteMountedEFI() {
	for (string efipar : MountEFI) {
		mylogger->info("删除临时盘符{:s}", efipar);
		DeleteVolumeMountPointA((efipar + "\\").c_str());
	}
}
void SetAllVolumeMountPoint() {
	mylogger->info("尝试挂载临时盘符");
	if(RunMyExec("pecmd show") == 0){
		return;
	}
	DeleteMountedEFI();
	char szVolumePathName[MAX_PATH] = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char volumeName[MAX_PATH] = { 0 };
	string VolumePathName;
	string VolumeName;
	// 开始枚举卷
	hFind = FindFirstVolumeA(volumeName, ARRAYSIZE(volumeName));
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}
	do {
		GetVolumePathNamesForVolumeNameA(volumeName, szVolumePathName, MAX_PATH, nullptr);
		VolumePathName = szVolumePathName;
		VolumeName = volumeName;
		if (VolumePathName == "") {
			char chPath = 'A';
			string MDisk = "";
			for (int i = 0; i < 26; ++i)
			{
				string strPath(1, chPath);
				strPath += ":";
				if (_access(strPath.c_str(), 0) != 0)
				{
					MDisk = strPath;
				}
				++chPath;
			}
			if (MDisk == "") {
				FindVolumeClose(hFind);
				return;
			}
			MountEFI.push_back(MDisk);
			SetVolumeMountPointA((MDisk + "\\").c_str(), VolumeName.c_str());
		}

	} while (FindNextVolumeA(hFind, volumeName, ARRAYSIZE(volumeName)));

	// 关闭枚举句柄
	FindVolumeClose(hFind);
	return;
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
		if (type != DRIVE_CDROM)
		{
			ComboBox_AddString(cb, drives + i);
		}
	}
	SendMessage(cb, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

string convertSecondsToTime(long long seconds) {
	long long hours = seconds / 3600;
	long long minutes = (seconds % 3600) / 60;
	seconds = seconds % 60;

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << hours << ":"
		<< std::setw(2) << std::setfill('0') << minutes << ":"
		<< std::setw(2) << std::setfill('0') << seconds;

	return ss.str();
}
atomic<long long>nowtime = 0;
void CountSeconds() {
	while (isloading) {
		Sleep(1000);
		nowtime += 1;
	}

}
string loading = "正在创建文件结构...";
wimlib_progress_status ApplyWimImage(wimlib_progress_msg msg_type, wimlib_progress_info* info, void* progctx)
{
		switch (msg_type) {
		case WIMLIB_PROGRESS_MSG_EXTRACT_IMAGE_BEGIN:
			MoveWindow(barfw, 0, 427, 0, 14, TRUE);
			break;
		case WIMLIB_PROGRESS_MSG_EXTRACT_FILE_STRUCTURE: {
			if (info->extract.current_file_count == 0 || info->extract.current_file_count == info->extract.end_file_count) {
				nowtime = 0;
			}
			float f = (float)info->extract.current_file_count / (float)info->extract.end_file_count;
			int f2 = round(f * 100);
			SetWindowText(protxt, s2ws("正在创建文件...[" + to_string(f2) + "%]").c_str());
			mylogger->info("正在创建文件...[" + to_string(f2) + "%]");
			float tarloca = (float)info->extract.current_file_count / (float)info->extract.end_file_count * 640;
			long long end_time = ((float)info->extract.end_file_count - (float)info->extract.current_file_count) / ((float)info->extract.current_file_count / nowtime);
			SetWindowTextA(protxt3, ("当前操作已用时间：" + convertSecondsToTime(nowtime)+"   剩余时间："+convertSecondsToTime(end_time)).c_str());
			MoveWindow(barfw, 0, 427, tarloca, 14, TRUE);
			break;
		}
		case WIMLIB_PROGRESS_MSG_EXTRACT_METADATA:
		{
			if (info->extract.current_file_count == 0 || info->extract.current_file_count == info->extract.end_file_count) {
				nowtime = 0;
			}
			float f = (float)info->extract.current_file_count / (float)info->extract.end_file_count;
			int f2 = round(f * 100);
			SetWindowText(protxt, s2ws("正在写入元数据...[" + to_string(f2) + "%]").c_str());
			mylogger->info("正在写入元数据...[" + to_string(f2) + "%]");
			float tarloca = (float)info->extract.current_file_count / (float)info->extract.end_file_count * 640;
			long long end_time = ((float)info->extract.end_file_count - (float)info->extract.current_file_count) / ((float)info->extract.current_file_count / nowtime);
			SetWindowTextA(protxt3, ("当前操作已用时间：" + convertSecondsToTime(nowtime) + "   剩余时间：" + convertSecondsToTime(end_time)).c_str());
			MoveWindow(barfw, 0, 427, tarloca, 14, TRUE);
			break;
		}
		case WIMLIB_PROGRESS_MSG_EXTRACT_STREAMS:
		{
			if (info->extract.completed_bytes <= 100) {
				nowtime = 0;
			}
			float f = (float)info->extract.completed_bytes / (float)info->extract.total_bytes;
			int f2 = round(f * 100);
			SetWindowText(protxt, s2ws("正在安装...[" + to_string(f2) + "%]").c_str());
			mylogger->info("正在安装...[" + to_string(f2) + "%]");
			float tarloca = (float)info->extract.completed_bytes / (float)info->extract.total_bytes * 640;
			long long end_time = ((float)info->extract.total_bytes - (float)info->extract.completed_bytes) / ((float)info->extract.completed_bytes / nowtime);
			SetWindowTextA(protxt3, ("当前操作已用时间：" + convertSecondsToTime(nowtime) + "   剩余时间：" + convertSecondsToTime(end_time)).c_str());
			MoveWindow(barfw, 0, 427, tarloca, 14, TRUE);
			break;
		}
		case WIMLIB_PROGRESS_MSG_EXTRACT_IMAGE_END:
			SetWindowText(protxt, NULL);
			SetWindowTextA(protxt3, NULL);
			MoveWindow(barfw, 0, 425, 0, 15, TRUE);
			break;
		case WIMLIB_PROGRESS_MSG_SCAN_BEGIN: {
			nowtime = 0;
			if (loading == "正在扫描文件...") {
				loading = "正在扫描文件.";
			}
			else if (loading == "正在扫描文件.") {
				loading = "正在扫描文件..";
			}
			else if (loading == "正在扫描文件..") {
				loading = "正在扫描文件...";
			}
			else {
				loading = "正在扫描文件.";
			}
			SetWindowText(protxt, s2ws(loading).c_str());
			mylogger->info(loading);
			SetWindowTextA(protxt3, NULL);
			break;
		}
		case WIMLIB_PROGRESS_MSG_WRITE_STREAMS: {
			if (info->write_streams.completed_bytes <= 10) {
				nowtime = 0;
			}
			float f = (float)info->write_streams.completed_bytes / (float)info->write_streams.total_bytes;
			int f2 = round(f * 100);
			SetWindowText(protxt, s2ws("正在备份...[" + to_string(f2) + "%]").c_str());
			mylogger->info("正在备份...[" + to_string(f2) + "%]");
			float tarloca = (float)info->write_streams.completed_bytes / (float)info->write_streams.total_bytes * 640;
			MoveWindow(barfw, 0, 427, tarloca, 14, TRUE);
			long long end_time = ((float)info->write_streams.total_bytes - (float)info->write_streams.completed_bytes) / ((float)info->extract.completed_bytes / nowtime);

			SetWindowTextA(protxt3, ("当前操作已用时间：" + convertSecondsToTime(nowtime) + "   剩余时间：" + convertSecondsToTime(end_time)).c_str());
			break;
		}
		case WIMLIB_PROGRESS_MSG_WRITE_METADATA_BEGIN: {
			nowtime = 0;
			loading = "正在写入元数据...";
			long long end_time = ((float)info->extract.total_bytes - (float)info->extract.completed_bytes) / ((float)info->extract.completed_bytes / nowtime);
			SetWindowTextA(protxt3, NULL);
			SetWindowText(protxt, s2ws(loading).c_str());
			mylogger->info(loading);
			break;
		}
		case WIMLIB_PROGRESS_MSG_WRITE_METADATA_END:
			SetWindowText(protxt, NULL);
			SetWindowTextA(protxt3, NULL);
			MoveWindow(barfw, 0, 427, 0, 14, TRUE);
			break;
		default:
		{
			SetWindowText(protxt, NULL);
			SetWindowTextA(protxt3, NULL);
			break;
		}
		}
	return WIMLIB_PROGRESS_STATUS_CONTINUE;
}
wstring unicode_to_wstring(unsigned int code) {
	wstring result;
	if (code <= 0xFFFF) {
		// 用一个wchar_t表示
		result += static_cast<wchar_t>(code);
	}
	else {
		// 用两个wchar_t表示，参考UTF-16编码规则
		code -= 0x10000;
		result += static_cast<wchar_t>((code >> 10) + 0xD800);
		result += static_cast<wchar_t>((code & 0x3FF) + 0xDC00);
	}
	return result;
}


void loading_anim() {
	while (true) {
		for (unsigned int i = 0xE100; i <= 0xE176; i++) {
			Sleep(40);
			if (!isloading) {
				SetWindowText(protxt2, NULL);
				EnableWindow(btnlogo, TRUE);
				EnableWindow(hTabCtrl, TRUE);
				return;
			}
			EnableWindow(btnlogo, FALSE);
			EnableWindow(hTabCtrl, FALSE);
			wstring ws = unicode_to_wstring(i);
			SetWindowText(protxt2, ws.c_str());
		}
	}
}
void GetWimSysInfo(const TCHAR* wimstr) {
	SendMessage(hWndComboBox4, CB_RESETCONTENT, 0, 0);
	WIMStruct* WIMFile;
	int result;
	TCHAR wimpath[MAX_PATH];
	_tcscpy_s(wimpath, sizeof(wimpath) / sizeof(TCHAR), wimstr);
	result = wimlib_open_wim(wimpath, 0, &WIMFile);
	if (result != 0) {
		wstring err = GetWimErrorString(result);
		mylogger->error("打开WIM发生错误，原因是{:s}", ws2s(err));
		MessageBox(hWnd, (L"无法打开WIM！原因：" + err).c_str(), NULL, MB_ICONERROR);
		SendMessage(hWndComboBox4, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		return;
	}
	void* xml_data;
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
	const char* xml_file = a;;
	regex r("<DISPLAYNAME>(.*?)</DISPLAYNAME>");
	string xml = xml_file;
	sregex_iterator it(xml.begin(), xml.end(), r);
	sregex_iterator end;
	bool havedn = false;
	while (it != end) {
		wstring temp = s2ws(it->str(1));
		ComboBox_AddString(hWndComboBox4, temp.c_str());
		havedn = true;
		it++;
	}
	if (havedn == false) {
		regex r2("<NAME>(.*?)</NAME>");
		string xml2 = xml_file;
		sregex_iterator it2(xml2.begin(), xml2.end(), r2);
		sregex_iterator end2;
		while (it2 != end2) {
			wstring temp = s2ws(it2->str(1));
			ComboBox_AddString(hWndComboBox4, temp.c_str());
			it2++;
		}
	}
	SendMessage(hWndComboBox4, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}
std::string toUpperCase(const std::string& str) {
	std::string upperCaseStr;
	for (char c : str) {
		// 使用std::toupper函数将字符转换为大写
		upperCaseStr += std::toupper(c);
	}
	return upperCaseStr;
}
vector<string> findFilesWithExtensions(const string& directoryPath, const vector<string>& extensions) {
	vector<string> foundFiles;
	for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath+"\\")) {
		if (filesystem::is_regular_file(entry)) {
			for (const auto& ext : extensions) {
				if (toUpperCase(entry.path().extension().string()) == ext) {
					foundFiles.push_back(entry.path().string());
					break;
				}
			}
		}
	}
	return foundFiles;
}


string isopath;
int imgtype;
vector<string> imglist;
void mountwimiso() {
	EnableWindow(win1, false);
	EnableWindow(win2, false);
	EnableWindow(win3, false);
	EnableWindow(btndisk,false);
	EnableWindow(btnghost, false);
	EnableWindow(btnxp, false);
	EnableWindow(btnreboot, false);
	EnableWindow(btnwim, false);

	isloading = true;
	thread t(loading_anim);
	t.detach();
	mylogger->info("开始挂载盘符。");
	SetWindowText(protxt, s2ws("取消上次挂载...").c_str());
	string unmountcmd = "imdisk -D -m "+MountedDisk;
	RunMyExec(unmountcmd.c_str());
	SetWindowText(protxt, L"准备挂载...");
	char chPath = 'A';
	string MDisk = "";
	for (int i = 0; i < 26; ++i)
	{
		string strPath(1, chPath);
		strPath += ":";
		if (_access(strPath.c_str(), 0) != 0)
		{
			MDisk = strPath;
		}
		++chPath;
	}
	if (MDisk == "") {
		isloading = false;
		EnableWindow(win1, true);
		EnableWindow(win2, true);
		EnableWindow(win3, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnwim, true);

		EnableWindow(btnxp, true);
		EnableWindow(btnreboot, true);
		SetWindowText(protxt, NULL);
		mylogger->error("尝试挂载ISO的时候发现没有可以容纳的盘符可供选择。");
		MessageBox(hWnd, L"没有可用盘符，无法挂载ISO以读取镜像信息！", NULL, MB_ICONERROR);
		return;
	}
	SetWindowText(protxt, L"挂载ISO镜像...");
	string mountcmd = "imdisk -a -m "+MDisk+" -f \"" + isopath + "\"";
	int status = RunMyExec(mountcmd.c_str());
	if (status != 0) {
		isloading = false;
		EnableWindow(win1, true);
		EnableWindow(win2, true);
		EnableWindow(win3, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnwim, true);

		EnableWindow(btnreboot, true);
		SetWindowText(protxt, NULL);
		mylogger->error("尝试挂载ISO的时候发现ISO似乎不对。");
		MessageBox(hWnd, L"无法挂载ISO以读取镜像信息，请检查程序完整性或确定镜像是否正确！", NULL, MB_ICONERROR);
		return;
	}
	MountedDisk = MDisk;
	SetWindowText(protxt, L"寻找映像资源...");
	vector<string> extensions;
	switch (imgtype) {
	case 0: {
		extensions = {".WIM",".ESD",".SWM" };
		break;
	}
	case 1: {
		extensions = { ".GHO" };
		break;
	}
	case 2: {
		Edit_SetText(edit4, s2ws(MDisk).c_str());
		wstring WinVer;
		BOOL cpuresult = GetCPUOfWinXP(s2ws(MDisk), &WinVer);
		if (cpuresult == -1) {
			SetWindowText(StcCPUMode, L"不是有效的Windows源文件！");
		}
		else if (cpuresult) {
			SetWindowText(StcCPUMode, WinVer.c_str());
		}
		else {
			SetWindowText(StcCPUMode, WinVer.c_str());
		}
		isloading = false;
		EnableWindow(win1, true);
		EnableWindow(win2, true);
		EnableWindow(win3, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnwim, true);
		EnableWindow(btnreboot, true);
		SetWindowText(protxt, NULL);
		return;
	}
	}
	vector<string> foundFiles;
	try {
		foundFiles = findFilesWithExtensions(MDisk, extensions);
		if (foundFiles.empty()) {
			isloading = false;
			EnableWindow(win1, true);
			EnableWindow(win2, true);
			EnableWindow(win3, true);
			EnableWindow(btndisk, true);
			EnableWindow(btnghost, true);
			EnableWindow(btnxp, true);
			EnableWindow(btnreboot, true);
			EnableWindow(btnwim, true);
			SetWindowText(protxt, NULL);
			mylogger->error("ISO里没有任何的WIM/ESD/GHO/映像文件夹文件。");
			MessageBox(hWnd, L"ISO不包含映像，请检查选择的ISO是否正确", NULL, MB_ICONERROR);
			string unmountcmd = "imdisk -D -m " + MountedDisk;
			RunMyExec(unmountcmd.c_str());
			return;
		}
	}
	catch (exception e) {
		isloading = false;
		EnableWindow(win1, true);
		EnableWindow(win2, true);
		EnableWindow(win3, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnwim, true);

		EnableWindow(btnreboot, true);
		SetWindowText(protxt, NULL);
		mylogger->error("发生读取ISO的异常，异常信息：{:s}",e.what());
		MessageBoxA(hWnd, "无法搜索文件！\n这可能是镜像的分区格式不兼容，Windows无法识别这种分区格式。\n请尝试选择其他的ISO镜像", NULL, MB_ICONERROR);
		string unmountcmd = "imdisk -D -m " + MountedDisk;
		RunMyExec(unmountcmd.c_str());
		return;
	}
	if (foundFiles.size() == 1) {
		isloading = false;
		EnableWindow(win1, true);
		EnableWindow(win2, true);
		EnableWindow(win3, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnwim, true);

		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnreboot, true);
		SetWindowText(protxt, NULL);
		switch (imgtype) {
		case 0:
		{
			Edit_SetText(edit2, s2ws(foundFiles.at(0)).c_str());
			EnableWindow(btnwim, false);
			wstring filefrom = s2ws(foundFiles.at(0));
			GetWimSysInfo(filefrom.c_str());
			break;
		}
		case 1:
			Edit_SetText(edit, s2ws(foundFiles.at(0)).c_str());
			EnableWindow(btnghost,false);
			break;
		}
		return;
	}
	imglist = foundFiles;
	//SendMessage(hwlst, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ISOSunProc);
	
}
//void CreateUnattendSIF(string Username, string Password, string RegisterKey, string SysprepCommand, string FirstLogonCommand, bool SkipOOBE) {
//	inicpp::IniManager _ini(".\\WINNT.SIF");
//	_ini.modify("Data", "UnattendedInstall", "\"Yes\"");
//	_ini.modify("Data", "MsDosInitiated", "\"1\"");
//	_ini.modify("Data", "FloppyLess", "\"1\"");
//	_ini.modify("Unattended", "UnattendMode", "FullUnattended");
//	_ini.modify("Unattended", "TargetPath", "\\WINDOWS");
//	_ini.modify("Unattended", "AutoActivate", "Yes");
//	_ini.modify("UserData", "ProductKey", RegisterKey);
//	_ini.modify("UserData", "OrgName", "Microsoft");
//	_ini.modify("UserData", "FullName", Username);
//	_ini.modify("GuiUnattended", "OEMSkipRegional", SkipOOBE ? "Yes" : "No");
//	_ini.modify("GuiUnattended", "TimeZone", "210");
//	_ini.modify("Unattended", "OemSkipEula", SkipOOBE ? "Yes" : "No");
//	_ini.modify("GuiUnattended", "OemSkipWelcome", SkipOOBE ? "Yes" : "No");
//	_ini.modify("TapiLocation", "CountryCode", 86);
//	_ini.modify("Identification", "JoinWorkgroup", "WORKGROUP");
//}
void CreateUnattendXML(string Username, string Password, string RegisterKey, string SysprepCommand, string FirstLogonCommand, bool SkipOOBE) {
	mylogger->info("构建自定义无人值守文件...");
	TiXmlDocument* tinyXmlDoc = new TiXmlDocument();
	TiXmlDeclaration* tinyXmlDeclare = new TiXmlDeclaration("1.0", "utf-8", "");
	tinyXmlDoc->LinkEndChild(tinyXmlDeclare);
	TiXmlElement* Unattend = new TiXmlElement("Unattend");
	Unattend->SetAttribute("xmlns", "urn:schemas-microsoft-com:unattend");
	Unattend->SetAttribute("xmlns:wcm", "http://schemas.microsoft.com/WMIConfig/2002/State");
	tinyXmlDoc->LinkEndChild(Unattend);
	TiXmlElement* SettingoS = new TiXmlElement("Setting");
	SettingoS->SetAttribute("pass", "offlineServicing");
	Unattend->LinkEndChild(SettingoS);
	TiXmlElement* SettingPE = new TiXmlElement("Setting");
	SettingPE->SetAttribute("pass", "windowsPE");
	TiXmlElement* componentMWICW = new TiXmlElement("component");
	componentMWICW->SetAttribute("name", "Microsoft-Windows-International-Core-WinPE");
	componentMWICW->SetAttribute("processorArchitecture", "amd64");
	componentMWICW->SetAttribute("publicKeyToken", "31bf3856ad364e35");
	componentMWICW->SetAttribute("language", "neutral");
	componentMWICW->SetAttribute("versionScope", "nonSxS");
	TiXmlElement* setupUILanguage = new TiXmlElement("SetupUILanguage");
	componentMWICW->LinkEndChild(setupUILanguage);
	TiXmlElement* uiLanguage = new TiXmlElement("UILanguage");
	uiLanguage->LinkEndChild(new TiXmlText("zh-Hans-CN"));
	setupUILanguage->LinkEndChild(uiLanguage);
	TiXmlElement* inputLocale = new TiXmlElement("InputLocale");
	inputLocale->LinkEndChild(new TiXmlText("0804:00000804"));
	componentMWICW->LinkEndChild(inputLocale);
	TiXmlElement* systemLocale = new TiXmlElement("SystemLocale");
	systemLocale->LinkEndChild(new TiXmlText("zh-CN"));
	componentMWICW->LinkEndChild(systemLocale);
	TiXmlElement* uiLanguage_winPE = new TiXmlElement("UILanguage");
	uiLanguage_winPE->LinkEndChild(new TiXmlText("zh-Hans-CN"));
	componentMWICW->LinkEndChild(uiLanguage_winPE);
	TiXmlElement* userLocale = new TiXmlElement("UserLocale");
	userLocale->LinkEndChild(new TiXmlText("zh-CN"));
	componentMWICW->LinkEndChild(userLocale);
	SettingPE->LinkEndChild(componentMWICW);
	TiXmlElement* componentMWS = new TiXmlElement("component");
	componentMWS->SetAttribute("name", "Microsoft-Windows-Setup");
	componentMWS->SetAttribute("processorArchitecture", "amd64");
	componentMWS->SetAttribute("publicKeyToken", "31bf3856ad364e35");
	componentMWS->SetAttribute("language", "neutral");
	componentMWS->SetAttribute("versionScope", "nonSxS");
	TiXmlElement* userData = new TiXmlElement("UserData");
	TiXmlElement* productKey = new TiXmlElement("ProductKey");
	TiXmlElement* key = new TiXmlElement("Key");
	key->LinkEndChild(new TiXmlText(RegisterKey.c_str()));
	productKey->LinkEndChild(key);
	userData->LinkEndChild(productKey);
	TiXmlElement* acceptEula = new TiXmlElement("AcceptEula");
	acceptEula->LinkEndChild(new TiXmlText("true"));
	userData->LinkEndChild(acceptEula);
	componentMWS->LinkEndChild(userData);
	SettingPE->LinkEndChild(componentMWS);
	Unattend->LinkEndChild(SettingPE);
	TiXmlElement* Settingge = new TiXmlElement("Setting");
	Settingge->SetAttribute("pass", "generalize");
	Unattend->LinkEndChild(Settingge);
	TiXmlElement* settingssp = new TiXmlElement("settings");
	settingssp->SetAttribute("pass", "specialize");
	Unattend->LinkEndChild(settingssp);
	TiXmlElement* componentMWD = new TiXmlElement("component");
	componentMWD->SetAttribute("name", "Microsoft-Windows-Deployment");
	componentMWD->SetAttribute("processorArchitecture", "amd64");
	componentMWD->SetAttribute("publicKeyToken", "31bf3856ad364e35");
	componentMWD->SetAttribute("language", "neutral");
	componentMWD->SetAttribute("versionScope", "nonSxS");
	settingssp->LinkEndChild(componentMWD);
	if (SysprepCommand != "") {
		TiXmlElement* runSync = new TiXmlElement("RunSynchronous");
		componentMWD->LinkEndChild(runSync);
		TiXmlElement* runSyncCommand = new TiXmlElement("RunSynchronousCommand");
		runSyncCommand->SetAttribute("wcm:action", "add");
		runSync->LinkEndChild(runSyncCommand);
		TiXmlElement* order = new TiXmlElement("Order");
		TiXmlText* orderText = new TiXmlText("1");
		order->LinkEndChild(orderText);
		runSyncCommand->LinkEndChild(order);
		TiXmlElement* path = new TiXmlElement("Path");
		TiXmlText* pathText = new TiXmlText(SysprepCommand.c_str());
		path->LinkEndChild(pathText);
		runSyncCommand->LinkEndChild(path);
	}
	TiXmlElement* Settingas = new TiXmlElement("Setting");
	Settingas->SetAttribute("pass", "auditSystem");
	Unattend->LinkEndChild(Settingas);
	TiXmlElement* Settingau = new TiXmlElement("Setting");
	Settingau->SetAttribute("pass", "auditUser");
	Unattend->LinkEndChild(Settingau);
	TiXmlElement* settings = new TiXmlElement("settings");
	settings->SetAttribute("pass", "oobeSystem");
	Unattend->LinkEndChild(settings);
	TiXmlElement* componentIntl = new TiXmlElement("component");
	componentIntl->SetAttribute("name", "Microsoft-Windows-International-Core");
	componentIntl->SetAttribute("processorArchitecture", "amd64");
	componentIntl->SetAttribute("publicKeyToken", "31bf3856ad364e35");
	componentIntl->SetAttribute("language", "neutral");
	componentIntl->SetAttribute("versionScope", "nonSxS");
	settings->LinkEndChild(componentIntl);
	TiXmlElement* inputLocale2 = new TiXmlElement("InputLocale");
	TiXmlText* inputLocaleText2 = new TiXmlText("0804:00000804");
	inputLocale2->LinkEndChild(inputLocaleText2);
	componentIntl->LinkEndChild(inputLocale2);
	TiXmlElement* systemLocale2 = new TiXmlElement("SystemLocale");
	TiXmlText* systemLocaleText2 = new TiXmlText("zh-CN");
	systemLocale2->LinkEndChild(systemLocaleText2);
	componentIntl->LinkEndChild(systemLocale2);
	TiXmlElement* uiLanguage2 = new TiXmlElement("UILanguage");
	TiXmlText* uiLanguageText2 = new TiXmlText("zh-Hans-CN");
	uiLanguage2->LinkEndChild(uiLanguageText2);
	componentIntl->LinkEndChild(uiLanguage2);
	TiXmlElement* userLocale2 = new TiXmlElement("UserLocale");
	TiXmlText* userLocaleText2 = new TiXmlText("zh-CN");
	userLocale2->LinkEndChild(userLocaleText2);
	componentIntl->LinkEndChild(userLocale2);
	TiXmlElement* componentShell = new TiXmlElement("component");
	componentShell->SetAttribute("name", "Microsoft-Windows-Shell-Setup");
	componentShell->SetAttribute("processorArchitecture", "amd64");
	componentShell->SetAttribute("publicKeyToken", "31bf3856ad364e35");
	componentShell->SetAttribute("language", "neutral");
	componentShell->SetAttribute("versionScope", "nonSxS");
	settings->LinkEndChild(componentShell);
	TiXmlElement* userAccounts = new TiXmlElement("UserAccounts");
	componentShell->LinkEndChild(userAccounts);
	TiXmlElement* localAccounts = new TiXmlElement("LocalAccounts");
	userAccounts->LinkEndChild(localAccounts);
	TiXmlElement* localAccount = new TiXmlElement("LocalAccount");
	localAccount->SetAttribute("wcm:action", "add");
	localAccounts->LinkEndChild(localAccount);
	TiXmlElement* name = new TiXmlElement("Name");
	TiXmlText* nameText = new TiXmlText(Username.c_str());
	name->LinkEndChild(nameText);
	localAccount->LinkEndChild(name);
	TiXmlElement* group = new TiXmlElement("Group");
	TiXmlText* groupText = new TiXmlText("Administrators");
	group->LinkEndChild(groupText);
	localAccount->LinkEndChild(group);
	TiXmlElement* password = new TiXmlElement("Password");
	localAccount->LinkEndChild(password);
	TiXmlElement* value = new TiXmlElement("Value");
	TiXmlText* valueText = new TiXmlText(Password.c_str());
	value->LinkEndChild(valueText);
	password->LinkEndChild(value);
	TiXmlElement* plainText = new TiXmlElement("PlainText");
	TiXmlText* plainTextText = new TiXmlText("true");
	plainText->LinkEndChild(plainTextText);
	password->LinkEndChild(plainText);
	TiXmlElement* autoLogon = new TiXmlElement("AutoLogon");
	componentShell->LinkEndChild(autoLogon);
	TiXmlElement* username = new TiXmlElement("Username");
	TiXmlText* usernameText = new TiXmlText(Username.c_str());
	username->LinkEndChild(usernameText);
	autoLogon->LinkEndChild(username);
	TiXmlElement* enabled = new TiXmlElement("Enabled");
	TiXmlText* enabledText = new TiXmlText("true");
	enabled->LinkEndChild(enabledText);
	autoLogon->LinkEndChild(enabled);
	TiXmlElement* logonCount = new TiXmlElement("LogonCount");
	TiXmlText* logonCountText = new TiXmlText("1");
	logonCount->LinkEndChild(logonCountText);
	autoLogon->LinkEndChild(logonCount);
	TiXmlElement* autoLogonPassword = new TiXmlElement("Password");
	autoLogon->LinkEndChild(autoLogonPassword);
	TiXmlElement* autoLogonValue = new TiXmlElement("Value");
	TiXmlText* autoLogonValueText = new TiXmlText(Password.c_str());
	autoLogonValue->LinkEndChild(autoLogonValueText);
	autoLogonPassword->LinkEndChild(autoLogonValue);
	TiXmlElement* autoLogonPlainText = new TiXmlElement("PlainText");
	TiXmlText* autoLogonPlainTextText = new TiXmlText("true");
	autoLogonPlainText->LinkEndChild(autoLogonPlainTextText);
	autoLogonPassword->LinkEndChild(autoLogonPlainText);
	TiXmlElement* oobe = new TiXmlElement("OOBE");
	componentShell->LinkEndChild(oobe);
	TiXmlElement* protectYourPC = new TiXmlElement("ProtectYourPC");
	TiXmlText* protectYourPCText = new TiXmlText("3");
	protectYourPC->LinkEndChild(protectYourPCText);
	oobe->LinkEndChild(protectYourPC);
	TiXmlElement* hideEULAPage = new TiXmlElement("HideEULAPage");
	TiXmlText* hideEULAPageText = new TiXmlText(SkipOOBE ? "true" : "false");
	hideEULAPage->LinkEndChild(hideEULAPageText);
	oobe->LinkEndChild(hideEULAPage);
	TiXmlElement* hideUserOOBEPage = new TiXmlElement("SkipUserOOBE");
	TiXmlText* hideUserOOBEPageText = new TiXmlText(SkipOOBE ? "true" : "false");
	hideUserOOBEPage->LinkEndChild(hideUserOOBEPageText);
	oobe->LinkEndChild(hideUserOOBEPage);
	TiXmlElement* hideMOOBEPage = new TiXmlElement("SkipMachineOOBE");
	TiXmlText* hideMOOBEPageText = new TiXmlText(SkipOOBE ? "true" : "false");
	hideMOOBEPage->LinkEndChild(hideMOOBEPageText);
	oobe->LinkEndChild(hideMOOBEPage);
	if (FirstLogonCommand != "") {
		TiXmlElement* firstLogonCommands = new TiXmlElement("FirstLogonCommands");
		componentShell->LinkEndChild(firstLogonCommands);
		TiXmlElement* synchronousCommand = new TiXmlElement("SynchronousCommand");
		synchronousCommand->SetAttribute("wcm:action", "add");
		firstLogonCommands->LinkEndChild(synchronousCommand);
		TiXmlElement* order2 = new TiXmlElement("Order");
		TiXmlText* orderText2 = new TiXmlText("1");
		order2->LinkEndChild(orderText2);
		synchronousCommand->LinkEndChild(order2);
		TiXmlElement* commandLine = new TiXmlElement("CommandLine");
		TiXmlText* commandLineText = new TiXmlText(FirstLogonCommand.c_str());
		commandLine->LinkEndChild(commandLineText);
		synchronousCommand->LinkEndChild(commandLine);
	}
	tinyXmlDoc->SaveFile(".\\Unattend.xml");
	mylogger->info("构建完成。");
}

int InstallDriver(TCHAR* TargetPath, TCHAR* DriverPath) {
	DismLogLevel Level = DismLogErrorsWarningsInfo;
	DismString* ErrStr;
	HRESULT hr = DismInitialize(Level, NULL, NULL);
	if (FAILED(hr)) {
		DismGetLastErrorMessage(&ErrStr);
		mylogger->error("Disk使用发生问题，原因是：{:s}", ws2s(ErrStr->Value));
		MessageBoxA(hWnd, ("初始化Dism会话失败，错误原因：" + ws2s(ErrStr->Value)).c_str(), NULL, MB_ICONERROR);
		DismDelete(ErrStr);
		SetWindowText(protxt, NULL);
		return 1;
	}
	DismSession Session = DISM_SESSION_DEFAULT;
	hr = DismOpenSession(TargetPath, NULL, NULL, &Session);
	if (FAILED(hr)) {
		DismGetLastErrorMessage(&ErrStr);
		mylogger->error("Disk使用发生问题，原因是：{:s}", ws2s(ErrStr->Value));
		MessageBoxA(hWnd, ("打开Windows安装文件夹失败，错误原因：" + ws2s(ErrStr->Value)).c_str(), NULL, MB_ICONERROR);
		DismDelete(ErrStr);
		DismShutdown();
		SetWindowText(protxt, NULL);
		return 1;
	}
	vector<string> extensions = {".INF" };
	vector<string> driverList = findFilesWithExtensions(ws2s(DriverPath), extensions);
	for (string driver : driverList) {
		hr = DismAddDriver(Session, s2ws(driver).c_str(), FALSE);
		if (FAILED(hr)) {
			DismGetLastErrorMessage(&ErrStr);
			mylogger->error("Disk使用发生问题，原因是：{:s}", ws2s(ErrStr->Value));
			MessageBoxA(hWnd, ("安装驱动失败，错误原因：" + ws2s(ErrStr->Value)).c_str(), NULL, MB_ICONERROR);
			DismDelete(ErrStr);
			DismCloseSession(Session);
			DismShutdown();
			SetWindowText(protxt, NULL);
			return 1;
		}
	}
	DismCloseSession(Session);
	DismShutdown();
	return 0;
}

LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo) {

	DeleteMountedEFI();
	string unmountcmd = ".\\imdisk -D -m " + MountedDisk;
	RunMyExec(unmountcmd.c_str());
	UninstalledImDisk();
	RemoveFontResource(L".\\Fonts\\HarmonyOS_Sans_SC_Medium.ttf");
	RemoveFontResource(L".\\Fonts\\segoe_slboot.ttf");
	SendMessage(hWnd, WM_FONTCHANGE, 0, 0);
	DWORD Error = pExceptionInfo->ExceptionRecord->ExceptionCode;
	wstring em = to_wstring(Error);;
	wstring ErrorInfo = L"发生严重异常！无法继续执行程序，请将以下错误代码发给程序作者，按下“确定”退出程序。\n错误代码：" + em;
	mylogger->error("发生严重异常！无法继续执行任务，错误信息：", ws2s(ErrorInfo.c_str()));
	MessageBox(hWnd, ErrorInfo.c_str(), L"错误：", MB_ICONERROR | MB_OK);
	ExitProcess(1);
	return EXCEPTION_EXECUTE_HANDLER;
}
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	initLogger();
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	InstalledImDisk();
	//CreateUnattendSIF("NULL", "123", "11111-11111-11111-11111-11111", "NULL", "NULL", NULL);
	RemoveFontResource(L".\\Fonts\\HarmonyOS_Sans_SC_Regular.ttf");
	RemoveFontResource(L".\\Fonts\\segoe_slboot.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	AddFontResource(L".\\Fonts\\HarmonyOS_Sans_SC_Regular.ttf");
	AddFontResource(L".\\Fonts\\segoe_slboot.ttf");
	WNDCLASS wndcls; //创建一个窗体类
	wndcls.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);//设置光标
	wndcls.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));//设置窗体左上角的图标
	wndcls.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls.lpfnWndProc = WinSunProc;//设置窗体的回调函数
	wndcls.lpszClassName = L"CWinstWindowClass";//设置窗体的类名
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
	wndcls2.lpfnWndProc = InWin1Proc;//设置窗体的回调函数
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
	wndcls3.lpfnWndProc = InWin2Proc;//设置窗体的回调函数
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
	wndcls4.lpfnWndProc = InWin3Proc;//设置窗体的回调函数
	wndcls4.lpszClassName = L"INWIN3";//设置窗体的类名
	wndcls4.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls4.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls4);
	WNDCLASS wndcls7; //创建一个窗体类
	wndcls7.cbClsExtra = 0;//类的额外内存，默认为0即可
	wndcls7.hCursor = LoadCursor(NULL, IDC_ARROW);//设置
	wndcls7.cbWndExtra = 0;//窗口的额外内存，默认为0即可
	wndcls7.hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), NULL);//设置窗体左上角的图标
	wndcls7.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
	wndcls7.hInstance = hInstance;//设置窗体所属的应用程序实例
	wndcls7.lpfnWndProc = InWin4Proc;//设置窗体的回调函数
	wndcls7.lpszClassName = L"INWIN4";//设置窗体的类名
	wndcls7.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
	wndcls7.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
	RegisterClass(&wndcls7);//向操作系统注册窗体
	//产生一个窗体，并返回该窗体的句柄，第一个参数必须为要创建的窗体的类名，第二个参数为窗体标题名
	mylogger->info("创建窗口...");
	HWND hwnd = CreateWindow(L"CWinstWindowClass", L"CWInst系统安装工具",
		WS_OVERLAPPED^  WS_CAPTION ^ WS_MINIMIZEBOX ^ WS_SYSMENU, createx,
		createy, 640, 480,
		NULL, NULL, hInstance, NULL);
	//SendMessage(hiso, WM_FONTCHANGE, 0, 0);
	hWnd = hwnd;
	btnghost = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Ghost还原",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		107,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)ghobt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	barbg = CreateWindow(
		L"STATIC",
		NULL,
		WS_VISIBLE | WS_CHILD,
		0,
		428,
		640,
		14,
		hwnd,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	protxt = CreateWindow(
		L"STATIC",
		NULL,
		WS_VISIBLE | WS_CHILD,
		0,
		395,
		170,
		15,
		hwnd,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	protxt2 = CreateWindow(
		L"STATIC",
		NULL,
		WS_VISIBLE | WS_CHILD,
		200,
		395,
		17,
		17,
		hwnd,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	protxt3 = CreateWindow(
		L"STATIC",
		NULL,
		WS_VISIBLE | WS_CHILD,
		0,
		412,
		320,
		15,
		hwnd,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	barfw = CreateWindow(
		L"STATIC",
		NULL,
		WS_VISIBLE | WS_CHILD,
		0,
		428,
		0,
		14,
		hwnd,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	btnwim = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"WIM映像应用",      // Button text 
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
		L"NT5.x安装",      // Button text 
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
		L"备份系统",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,  // Styles 
		22,         // x position 
		281,         // y position 
		187,        // Button width
		56,        // Button height
		hwnd,     // Parent window
		(HMENU)diskbt,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
	HBITMAP logo = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
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
	hFont = CreateFont(19,                                    //   字体的高度   
		0,                                          //   字体的宽度  
		0,                                          //  nEscapement 
		0,                                          //  nOrientation   
		FW_BLACK,                                  //   nWeight   
		FALSE,                                      //   bItalic   
		FALSE,                                      //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("HarmonyOS Sans SC"));
	hFont2 = CreateFont(16,                                    //   字体的高度   
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
		_T("HarmonyOS Sans SC"));
	hFont3 = CreateFont(17,                                    //   字体的高度   
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
		_T("Segoe Boot Semilight"));
	hTabCtrl = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_VISIBLE | WS_CHILD | TCS_TABS,
		233, 45, 372, 355, hWnd, (HMENU)10001, hInstance, 0);
	SendMessage(btndisk, WM_SETFONT, (WPARAM)hFont, 1);
	SendMessage(protxt, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(protxt2, WM_SETFONT, (WPARAM)hFont3, 1);
	SendMessage(protxt3, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btnghost, WM_SETFONT, (WPARAM)hFont, 1);
	SendMessage(btnwim, WM_SETFONT, (WPARAM)hFont, 1);
	SendMessage(btnxp, WM_SETFONT, (WPARAM)hFont, 1);
	SendMessage(btnreboot, WM_SETFONT, (WPARAM)hFont, 1);
	SendMessage(btnlogo, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)logo);
	HICON ghologo = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON4));
	SendMessage(btnghost, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)ghologo);
	HICON wimlogo = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON6));
	SendMessage(btnwim, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)wimlogo);
	HICON xplogo = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON7));
	SendMessage(btnxp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)xplogo);
	HICON disklogo = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));
	SendMessage(btndisk, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)disklogo);
	HICON rebootlogo = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON5));
	SendMessage(btnreboot, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)rebootlogo);
	TCITEM item1, item2, item3, item4, item5;
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

	item5.mask = TCIF_TEXT | TCIF_IMAGE;
	item5.pszText = L"item5";
	item5.iImage = 4;


	// 将两个选项卡项插入到选项卡控件中
	TabCtrl_InsertItem(hTabCtrl, 0, &item1);
	TabCtrl_InsertItem(hTabCtrl, 1, &item2);
	TabCtrl_InsertItem(hTabCtrl, 2, &item3);
	TabCtrl_InsertItem(hTabCtrl, 3, &item4);
	TabCtrl_InsertItem(hTabCtrl, 4, &item5);
	EnableWindow(btnwim, true);
	EnableWindow(btnghost, false);
	EnableWindow(btnxp, true);
	EnableWindow(btndisk, true);


	win1 = CreateWindow(L"INWIN1", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),   // g_hInst is the global instance handle
		NULL);
	win2 = CreateWindow(L"INWIN2", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	win3 = CreateWindow(L"INWIN3", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	win5 = CreateWindow(L"INWIN4", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 372, 355,        // Position and dimensions; example only.
		hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	win4 = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 372, 355, hTabCtrl, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),    // g_hInst is the global instance handle
		NULL);
	SendMessage(win4, WM_SETFONT, (WPARAM)hFont2, 1);
	string program_info = "感谢您使用CWinst Windows安装工具！官方网站：win-compe.top\n\
本程序主要由C++ WinAPI编写，挂载镜像是第三方ImDisk。目前Windows的安装方式主要有Ghost系统安装，Windows Vista+的WIM/ESD安装和NT5.x类的I386三种安装方法，程序提供了三种方式以应对不同的操作系统的安装执行。\n\
为保证您的使用体验，使用时需注意以下几点：\n\
1.Ghost备份还原的时候由于Norton Ghost程序限制，程序路径请不要携带非英文字符，否则可能导致因无法识别目录导致安装失败。\n\
2.WIM/ESD安装（Legacy BIOS启动）和NT5.x安装方式要求，安装分区必须是一个活动主分区，否则将会因为没有PBR引导而启动失败，可通过DiskGenius或傲梅分区助手查看分区状态是否为活动主分区。\n\
3.NT5.x系统不支持UEFI启动和GPT硬盘格式，请确定自己的启动方式是Legacy启动并且硬盘格式是MBR，可通过Diskgenius或傲梅分区助手检查格式并更改成MBR格式磁盘。\n\
4.程序执行操作期间，不建议对计算机进行任何操作，否则有可能对您的设备造成损坏！\n\
5.程序提供无人值守的配置设置，允许您自定义一些在Windows安装期间自动执行的操作。本程序不提供盗版激活服务，如果要填写激活密钥请自行前往Microsoft官方网站购买。\n\
6.程序选择WIM/ESD的驱动安装的时候，您可以将多个驱动放在同一个目录下，以进行批量安装驱动。\n\
感谢您的使用，祝您使用愉快！";
	SetWindowText(win4, s2ws(program_info).c_str());
	HWND btfile1 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 119, 64, 22, win1, (HMENU)gholoadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btre1 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 150, 64, 22, win1, (HMENU)ghodiskbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btfile1, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btre1, WM_SETFONT, (WPARAM)hFont2, 1);
	edit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 119, 180, 21, win1, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(edit, WM_SETFONT, (WPARAM)hFont2, 1);
	hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 150, 180, 18, win1, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(hWndComboBox, WM_SETFONT, (WPARAM)hFont2, 1);
	ghostartbtn = CreateWindow(L"BUTTON", L"执行Ghost还原", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win1, (HMENU)ghostartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(ghostartbtn, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND savestartbtn = CreateWindow(L"BUTTON", L"备份系统", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win5, (HMENU)savestartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(savestartbtn, WM_SETFONT, (WPARAM)hFont2, 1);
	selectmodepar = CreateWindowEx(WS_EX_WINDOWEDGE,
		L"BUTTON",
		L"分区模式",
		WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,  // <---- WS_GROUP group the following radio buttons 1st,2nd button
		90, 179,
		100, 20,
		win1, //<----- Use main window handle
		(HMENU)parmode,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	selectmodedisk = CreateWindow(
		L"BUTTON",
		L"磁盘模式",
		WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,  // Styles
		190, 179,
		100, 20,
		win1,
		(HMENU)diskmode,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(selectmodedisk, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(selectmodepar, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(selectmodepar, BM_SETCHECK, 1, 0);
	SendMessage(selectmodedisk, BM_SETCHECK, 0, 0);
	HWND btfile2 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 94, 64, 22, win2, (HMENU)wimloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btre2 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 125, 64, 22, win2, (HMENU)wimdiskbt1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	btSWM = CreateWindow(L"BUTTON", L"SWM管理", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 155, 64, 22, win2, (HMENU)SWMBT, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btSWM, WM_SETFONT, (WPARAM)hFont2, 1);
	EnableWindow(btSWM, FALSE);
	HWND btfmt1 = CreateWindow(L"BUTTON", L"格式化", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		216, 125, 64, 22, win2, (HMENU)fmt1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	//SetWindowPos(barfw, NULL, 0, 425, 100, 15, SWP_NOMOVE);
	SendMessage(btfile2, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btfiled = CreateWindow(L"BUTTON", L"选择目录", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 218, 64, 22, win2, (HMENU)DRIVELOADBT, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btDownload = CreateWindow(L"BUTTON", L"下载映像", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		216, 94, 64, 22, win2, (HMENU)RUNIMGDOWNLOAD, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	editDrive = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 218, 180, 21, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(editDrive, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btfiled, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btDownload, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btre2, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btfmt1, WM_SETFONT, (WPARAM)hFont2, 1);
	edit2 = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 94, 115, 21, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	hWndComboBox2 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 125, 115, 18, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	hWndComboBox3 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 185, 115, 18, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btfmt2 = CreateWindow(L"BUTTON", L"格式化", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		216, 185, 64, 22, win2, (HMENU)fmt2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btfmt2, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(hWndComboBox3, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btre3 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 185, 64, 22, win2, (HMENU)wimdiskbt2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btre3, WM_SETFONT, (WPARAM)hFont2, 1);
	hWndComboBox4 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 155, 180, 18, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(edit2, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(hWndComboBox2, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(hWndComboBox4, WM_SETFONT, (WPARAM)hFont2, 1);
	btwimstart = CreateWindow(L"BUTTON", L"应用WIM/ESD映像", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win2, (HMENU)wimstartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btfile3 = CreateWindow(L"BUTTON", L"配置设置", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 250, 64, 22, win2, (HMENU)xmloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btwimstart, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btfile3, WM_SETFONT, (WPARAM)hFont2, 1);
	edit3 = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 250, 180, 21, win2, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(edit3, WM_SETFONT, (WPARAM)hFont2, 1);
	hsavepathedit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 99, 180, 21, win5, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(hsavepathedit, WM_SETFONT, (WPARAM)hFont2, 1);
	hsavediskcb = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 134, 180, 18, win5, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(hsavediskcb, WM_SETFONT, (WPARAM)hFont2, 1);
	hcompcbox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 170, 180, 18, win5, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(hcompcbox, WM_SETFONT, (WPARAM)hFont2, 1);
	ComboBox_AddString(hcompcbox, L"无压缩(None)");
	ComboBox_AddString(hcompcbox, L"最快压缩(XPRESS)");
	ComboBox_AddString(hcompcbox, L"普通压缩(LZX)");
	ComboBox_AddString(hcompcbox, L"极高压缩(LZMS非固实压缩)");
	ComboBox_AddString(hcompcbox, L"最大压缩(LZMS固实压缩)");
	ComboBox_SetCurSel(hcompcbox, 2);
	btxpstart = CreateWindow(L"BUTTON", L"安装旧版Windows", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		125, 300, 120, 50, win3, (HMENU)xpstartbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btxpstart, WM_SETFONT, (WPARAM)hFont2, 1);
	edit4 = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 99, 115, 21, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(edit4, WM_SETFONT, (WPARAM)hFont2, 1);
	edit5 = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		100, 169, 180, 21, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
	SendMessage(edit5, WM_SETFONT, (WPARAM)hFont2, 1);
	hWndComboBox5 = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
		100, 134, 115, 18, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btfmt3 = CreateWindow(L"BUTTON", L"格式化", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		216, 134, 64, 22, win3, (HMENU)fmt3, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	HWND btre4 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 134, 64, 22, win3, (HMENU)xpdiskbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btre4, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btre5 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 134, 64, 22, win5, (HMENU)savediskbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btre5, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(btfmt3, WM_SETFONT, (WPARAM)hFont2, 1);
	SendMessage(hWndComboBox5, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btbakdir = CreateWindow(L"BUTTON", L"保存到...", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 99, 64, 22, win5, (HMENU)saveloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btbakdir, WM_SETFONT, (WPARAM)hFont2, 1);
	string fm;
	if (GetFirmware() == "BIOS") {
		fm = "Legacy";
	}
	else {
		fm = GetFirmware();
		//MountEFIPartition();
	}


	bool secureBootEnabled = IsSecureBootEnabled();
	bool tpm2Enabled = IsTPM2Enabled();
	std::string secureBootEnabledstatus = secureBootEnabled ? "安全启动" : "非安全启动";
	std::string tpm2Enabledstatus = tpm2Enabled ? "已开启" : "未开启";
	HWND tpm = CreateWindow(L"STATIC", s2ws("TPM状态：" + tpm2Enabledstatus).c_str(), WS_VISIBLE | WS_CHILD, 320, 404, 200, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(tpm, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND firm = CreateWindow(L"STATIC", s2ws("当前启动类型：" + fm + secureBootEnabledstatus).c_str(), WS_VISIBLE | WS_CHILD, 438, 404, 200, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(firm, WM_SETFONT, (WPARAM)hFont2, 1);
	//HWND btre3 = CreateWindow(L"BUTTON", L"刷新列表", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
	//	280, 134, 64, 22, win3, (HMENU)xpdiskbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
	//	NULL);
	//SendMessage(btre3, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btdir = CreateWindow(L"BUTTON", L"选择文件夹", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 99, 64, 22, win3, (HMENU)xploadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btdir, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btxpiso = CreateWindow(L"BUTTON", L"选择镜像", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		216, 99, 64, 22, win3, (HMENU)xpisoloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btxpiso, WM_SETFONT, (WPARAM)hFont2, 1);
	HWND btfile4 = CreateWindow(L"BUTTON", L"选择文件", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		280, 169, 64, 22, win3, (HMENU)txtloadbt, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(btfile4, WM_SETFONT, (WPARAM)hFont2, 1);
	StcCPUMode = CreateWindow(WC_STATIC, NULL, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
		70, 249, 250, 22, win3, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL);
	SendMessage(StcCPUMode, WM_SETFONT, (WPARAM)hFont2, 1);
	//hwlst = CreateWindow(WC_COMBOBOX, TEXT(""),
	//	CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
	//	12, 55, 320, 20, hiso, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
	//	NULL);
	//hok = CreateWindow(L"BUTTON", L"确定", WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
	//	257, 106, 75, 23, hiso, (HMENU)btyes, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
	//	NULL);
	SetAllVolumeMountPoint();
	AddDiskList(hWndComboBox);
	AddDiskList(hWndComboBox2);
	AddDiskList(hWndComboBox3);
	AddDiskList(hWndComboBox5);
	AddDiskList(hsavediskcb);
	SetWindowShow();
	ShowWindow(hwnd, SW_SHOWNORMAL);//把窗体显示出来
	UpdateWindow(hwnd);//更新窗体
	UpdateWindow(win1);//更新窗体
	UpdateWindow(win2);//更新窗体
	UpdateWindow(win3);//更新窗体
	UpdateWindow(win4);//更新窗体
	UpdateWindow(win5);//更新窗体
	/*
	if (inPE()) {
		GetConfigJson();
	}*/
	mylogger->info("创建窗口完成！");
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
void ghost() {
	TCHAR dirs[1024] = { 0 };
	Edit_GetText(edit, dirs, 1024);
	string dirstr = ws2s(dirs).c_str();
	if (!isFileExists_ifstream(dirstr)) {
		MessageBox(hWnd, L"请选择正确的GHO文件！", L"错误：", MB_ICONERROR);
	}
	else {
		if (MessageBox(hWnd, L"确认应用Ghost映像吗？执行操作期间请勿操作电脑。", L"警告：", MB_YESNO | MB_ICONWARNING) == IDYES) {
			mylogger->info("Ghost还原任务被执行...");
			EnableWindow(ghostartbtn, false);
			if (ispar) {
				TCHAR disk[3] = { 0 };
				char roots[7] = "\\\\.\\";
				ComboBox_GetText(hWndComboBox, disk, 3);
				string target = ws2s(disk).c_str();
				const char* tg = strcat(roots, target.c_str());
				int pn = GetPartitionNumber(tg);
				string ghostexec = ".\\Ghost\\ghost64.exe -clone,mode=pload,src=" + dirstr + ":1,dst=" + to_string(GetDiskNum(target.c_str()) + 1) + ":" + to_string(pn) + " -sure";
				system(ghostexec.c_str());
			}
			else {
				TCHAR disk[MAX_PATH] = { 0 };
				ComboBox_GetText(hWndComboBox, disk, MAX_PATH);
				string target = ws2s(disk).c_str();
				int pos = target.find('.');
				// 截取从开始位置到 . 的前一个位置的子串
				string sub = target.substr(0, pos);
				// 将子串赋值给一个变量 string
				string result = sub;
				string ghostexec = ".\\Ghost\\ghost64.exe -clone,mode=load,src=" + ws2s(dirs) + ",dst=" + to_string(stoi(result)+1) + " -sure";
				RunMyExec(ghostexec.c_str());
			}
			EnableWindow(ghostartbtn, true);
			mylogger->info("任务结束！");
			MessageBox(hWnd, L"执行完成！重启计算机后将进行进一步安装Windows操作（以Ghost官方提示为准）！", L"成功：", MB_ICONINFORMATION);
		}
	}
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
		case gholoadbt: {
			TCHAR* file = GetGhoFile(L"Ghost映像文件(*.gho/*.iso)\0*.gho;*.iso\0\0", hwnd);
			string filepath = ws2s(file).c_str();
			filesystem::path path_obj(filepath);
			string files = filepath;

			// 检查路径是否存在并且是一个文件
			if (filesystem::exists(path_obj) && filesystem::is_regular_file(path_obj)) {
				// 获取并输出文件后缀
				if (path_obj.extension() == ".iso" || path_obj.extension() == ".ISO") {
					isopath = files;
					imgtype = 1;
					thread t(mountwimiso);
					t.detach();
					break;
				}
			}
			Edit_SetText(edit, file);
			break;
		}
		case ghodiskbt:
			if (ispar) {
				SetAllVolumeMountPoint();
				AddDiskList(hWndComboBox);
			}
			else {
				GetDriveList(hWndComboBox);
			}
			break;
		case parmode:
			SetAllVolumeMountPoint();
			AddDiskList(hWndComboBox);
			ispar = true;
			SendMessage(selectmodepar, BM_SETCHECK, 1, 0);
			SendMessage(selectmodedisk, BM_SETCHECK, 0, 0);
			break;
		case diskmode:
			GetDriveList(hWndComboBox);
			ispar = false;
			SendMessage(selectmodepar, BM_SETCHECK, 0, 0);
			SendMessage(selectmodedisk, BM_SETCHECK, 1, 0);
			break;
		case ghostartbt:
			ghost();
			break;
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}
void writewim() {
	isloading = true;
	thread t1(loading_anim);
	t1.detach();
	nowtime = 0;
	thread t(CountSeconds);
	t.detach();
	TCHAR dirs[MAX_PATH] = { 0 };
	Edit_GetText(edit2, dirs, MAX_PATH);
	string dirstr = ws2s(dirs).c_str();
	EnableWindow(win2, false);
	EnableWindow(btnghost, false);
	EnableWindow(btnxp, false);
	EnableWindow(btndisk, false);
	EnableWindow(btnreboot, false);
	WIMStruct* WIM;
	int result;
	result = wimlib_open_wim_with_progress(dirs, 0, &WIM, ApplyWimImage, NULL);
	if (result != 0) {
		isloading = false;
		mylogger->error("打开{:s}的时候失败了。", dirstr);
		MessageBox(hWnd, L"未能成功打开WIM/ESD文件，请检查文件是否是正确的映像文件！", 0, MB_ICONERROR);
		EnableWindow(win2, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnreboot, true);
		return;
	}
	int cs = ComboBox_GetCurSel(hWndComboBox4) + 1;
	TCHAR tar[MAX_PATH] = { 0 };
	ComboBox_GetText(hWndComboBox2, tar, MAX_PATH);
	if (!EnableSWMs.empty()) {
		size_t size = EnableSWMs.size();

		// 创建一个足够大的wstring数组来存储vector中的元素
		const wchar_t** wstringArray = new const wchar_t*[size];

		// 将vector中的元素复制到数组中
		for (size_t i = 0; i < size; ++i) {
			wstringArray[i] = EnableSWMs[i].c_str();
		}
		result = wimlib_reference_resource_files(WIM, wstringArray, 1, WIMLIB_REF_FLAG_GLOB_ERR_ON_NOMATCH, NULL);
		//
		if (result != 0) {
			wimlib_free(WIM);
			delete[] wstringArray;
			isloading = false;
			mylogger->error("swm整合期间发生错误，原因：{:s}", ws2s(GetWimErrorString((wimlib_error_code)result)));
			MessageBox(hWnd, s2ws("整合拆分swm时发生错误，错误原因：" + ws2s((TCHAR*)(GetWimErrorString((wimlib_error_code)result)))).c_str(), 0, MB_ICONERROR);
			EnableWindow(win2, true);
			EnableWindow(btnghost, true);
			EnableWindow(btnxp, true);
			EnableWindow(btndisk, true);
			EnableWindow(btnreboot, true);
			return;
		}
		delete[] wstringArray;
	}
	result = wimlib_extract_image(WIM, cs, tar, NULL);
	//
	if (result != 0) {
		wimlib_free(WIM);
		isloading = false;
		mylogger->error("应用WIM/ESD映像的时候发生错误，错误原因：{:s}", ws2s(GetWimErrorString((wimlib_error_code)result)));
		MessageBox(hWnd, s2ws("应用WIM/ESD映像的时候发生错误，错误原因：" + ws2s((TCHAR*)(GetWimErrorString((wimlib_error_code)result)))).c_str(), 0, MB_ICONERROR);
		EnableWindow(win2, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnreboot, true);
		return;
	}
	wimlib_free(WIM);
	TCHAR uad[MAX_PATH] = { 0 };
	Edit_GetText(edit3, uad, MAX_PATH);
	string uadfile = ws2s(uad).c_str();
	wstring Tar = tar;
	CreateDirectory((Tar + L"Windows\\Panther").c_str(),NULL);
	CopyFile(uad, (Tar + L"Windows\\Panther\\unattend.xml").c_str(), false);
	//
	SetWindowText(protxt, L"创建引导...");
	TCHAR boot[MAX_PATH] = { 0 };
	ComboBox_GetText(hWndComboBox3, boot, MAX_PATH);
	string cmd = "bcdboot " + ws2s(tar) + "Windows /s " + ws2s(boot).at(0) + ": /f ALL /l zh-cn";
	RunMyExec(cmd.c_str());
	cmd = ".\\bootsect.exe /nt60 " + to_string(ws2s(tar).at(0)) + ": /mbr";
	RunMyExec(cmd.c_str());
	//
	
	TCHAR driverpath[MAX_PATH] = { 0 };
	Edit_GetText(editDrive, driverpath, MAX_PATH);
	wstring dp = driverpath;
	if (driverpath != L"" && is_folder_path(ws2s(driverpath))) {
		SetWindowText(protxt, L"安装驱动...");
		if (InstallDriver(tar, driverpath) != 0) {
			isloading = false;
			EnableWindow(win2, true);
			EnableWindow(btnghost, true);
			EnableWindow(btnxp, true);
			EnableWindow(btndisk, true);
			EnableWindow(btnreboot, true);
			return;
		}
	}
	isloading = false;
	MessageBox(hWnd, L"应用完成！重启计算机后将进行进一步安装Windows操作！", L"成功：", MB_ICONINFORMATION);
	SetWindowText(protxt, NULL);
	EnableWindow(win2, true);
	EnableWindow(btnghost, true);
	EnableWindow(btnxp, true);
	EnableWindow(btndisk, true);
	EnableWindow(btnreboot, true);
	return;
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
		TCHAR msg4[] = L"                  SWM是WIM映像文件的拆分文件";
		TCHAR msg8[] = L"          本页面可用于安装对应操作系统以及还原备份。";
		TCHAR wimdir[] = L"   镜像位置：";
		TCHAR wimdisk[] = L"   安装目标：";
		TCHAR wimbisk[] = L"   引导分区：";
		TCHAR wimos[] = L"   系统选择：";
		TCHAR driver[] = L"   驱动文件：";
		TCHAR xmldir[] = L"   应答文件：";
		TCHAR msg7[] = L"   （留空或路径不正确则不进行无人值守模式和驱动安装）";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文  
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 10, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 25, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 40, msg3, _tcslen(msg3));
		TextOut(hdc, 32, 55, msg4, _tcslen(msg4));
		TextOut(hdc, 32, 70, msg8, _tcslen(msg8));
		TextOut(hdc, 31, 95, wimdir, _tcslen(wimdir));
		TextOut(hdc, 31, 128, wimdisk, _tcslen(wimdisk));
		TextOut(hdc, 31, 190, wimbisk, _tcslen(wimbisk));
		TextOut(hdc, 31, 160, wimos, _tcslen(wimos));
		TextOut(hdc, 31, 250, xmldir, _tcslen(xmldir));
		TextOut(hdc, 31, 220, driver, _tcslen(driver));
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
			try {
				EnableSWMs.clear();
				wstring file = GetGhoFile(L"Windows映像文件(*.wim/*.esd/*.swm/*.iso)\0*.wim;*.esd;*.swm;*.iso\0\0", hwnd);
				EnableWindow(btSWM, FALSE);
				string filepath = ws2s(file).c_str();
				filesystem::path path_obj(filepath);
				string files = filepath;
			// 检查路径是否存在并且是一个文件
				if (filesystem::exists(path_obj) && filesystem::is_regular_file(path_obj)) {
					if (path_obj.extension() == ".iso" || path_obj.extension() == ".ISO") {
						isopath = files;
						imgtype = 0;
						thread t(mountwimiso);
						t.detach();
						break;
					}if (path_obj.extension() == ".swm" || path_obj.extension() == ".SWM") {
						Edit_SetText(edit2, file.c_str());
						DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, SWMSunProc);
						GetWimSysInfo(file.c_str());
						EnableWindow(btSWM, TRUE);
						break;
					}
					Edit_SetText(edit2, file.c_str());
					GetWimSysInfo(file.c_str());
				}
				EnableWindow(btnwim, false);
				break;
			}
			catch (exception) {
				break;
			}
		}
		case wimdiskbt1:
		{
			SetAllVolumeMountPoint();
			AddDiskList(hWndComboBox2);
			break;
		}
		case wimdiskbt2:
		{
			SetAllVolumeMountPoint();
			AddDiskList(hWndComboBox3);
			break;
		}
		case xmloadbt:
		{
			DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, TWNSunProc);
			break;
		}
		case fmt1: {
			TCHAR drive[5] = { 0 };
			Edit_GetText(hWndComboBox2, drive, 5);
			char par = ws2s(drive).c_str()[0];
			SHFormatDrive(hwnd, par - 65, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);
			break;
		}
		case fmt2: {
			TCHAR drive[5] = { 0 };
			Edit_GetText(hWndComboBox3, drive, 5);
			char par = ws2s(drive).c_str()[0];
			SHFormatDrive(hwnd, par - 65, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);
			break;
		}
		case DRIVELOADBT:
		{
			TCHAR szBuffer[MAX_PATH] = { 0 };
			BROWSEINFO bi;
			ZeroMemory(&bi, sizeof(BROWSEINFO));
			bi.hwndOwner = NULL;
			bi.pszDisplayName = szBuffer;
			bi.lpszTitle = _T("选择包含驱动安装文件（*.inf）（可以包含在子目录中）的目录:");
			bi.ulFlags = BIF_RETURNFSANCESTORS;
			LPITEMIDLIST idl = SHBrowseForFolder(&bi);
			if (NULL == idl)
			{
				break;
			}
			SHGetPathFromIDList(idl, szBuffer);
			Edit_SetText(editDrive, szBuffer);
			break;
		}
		case SWMBT:
			DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, SWMSunProc);
			break;
		case RUNIMGDOWNLOAD:
		{
			string cline = ".\\Downloader\\CDowner.exe "+to_string((int)hWnd);
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;

			// 初始化STARTUPINFO结构体
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			// 创建cmd.exe进程
			if (!CreateProcessA(NULL,
				cline.data(), // 命令行参数
				NULL,       // 安全属性
				NULL,       // 安全属性
				FALSE,      // 继承句柄
				NULL, // 标志
				NULL,       // 环境变量
				NULL,       // 当前目录
				&si,        // 启动信息
				&pi))       // 接收进程信息
			{
				LPWSTR messageBuffer = nullptr;
				DWORD size = FormatMessageW(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&messageBuffer,
					0,
					NULL
				);
				wstring ErrStr = messageBuffer;
				MessageBox(hWnd, (L"未能启动下载器，错误原因：" + ErrStr).c_str(), NULL, MB_ICONERROR);
				break;
			}
			//MessageBoxA(NULL, cline.c_str(), NULL, NULL);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			break;
		}
		case wimstartbt:
		{
			TCHAR dirs[MAX_PATH] = { 0 };
			Edit_GetText(edit2, dirs, MAX_PATH);
			string dirstr = ws2s(dirs).c_str();
			if (!isFileExists_ifstream(dirstr))
			{
				MessageBox(hwnd, L"请选择正确的WIM文件！", L"错误：", MB_ICONERROR);
			}
			else
			{
				if (MessageBox(hwnd, L"确认应用WIM吗？执行操作期间请勿操作电脑。", L"警告：", MB_YESNO | MB_ICONWARNING) == IDYES)
				{
						thread t(writewim);
						t.detach();
						break;
					//}
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
void CopyXPFile() {
	isloading = true;
	thread t(loading_anim);
	t.detach();
	EnableWindow(win3, false);
	EnableWindow(btnghost, false);
	EnableWindow(btnwim, false);
	EnableWindow(btndisk, false);
	EnableWindow(btnreboot, false);
	TCHAR tar[MAX_PATH] = { 0 };
	ComboBox_GetText(hWndComboBox5, tar, MAX_PATH);
	string tarstr = ws2s(tar).c_str();
	TCHAR dirs[MAX_PATH] = { 0 };
	Edit_GetText(edit4, dirs, MAX_PATH);
	wstring nullstr;
	BOOL cpuresult = GetCPUOfWinXP(dirs,&nullstr);
	string dirstr;
	string CPUMode;
	if (cpuresult) {
		dirstr = (ws2s(dirs)+ "\\AMD64");
		CPUMode = "AMD64";
	}
	if (cpuresult == -1) {
		isloading = false;
		SetWindowText(protxt, NULL);
		MoveWindow(barfw, 0, 427, 0, 15, TRUE);
		EnableWindow(win3, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnwim, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnreboot, true);
		return;
	}
	if (!cpuresult) {
		dirstr = (ws2s(dirs) + "\\I386");
		CPUMode = "I386";
	}
	TCHAR i86fdr[MAX_PATH] = { 0 };
	string path = tarstr + "$WIN_NT$.~BT";
	bool flag = CreateDirectory(s2ws(path).c_str(), NULL);
	string filelist[] = { "1394BUS.SY_","ABP480N5.SY_","ACPI.SY_","ACPIEC.SY_","ADPU160M.SY_","AHA154X.SY_","AIC78U2.SY_","AIC78XX.SY_","ALIIDE.SY_","AMSINT.SY_","ASC.SY_","ASC3350P.SY_","ASC3550.SY_","ATAPI.SY_","BIOSINFO.INF","BOOTFONT.BIN","BOOTVID.DL_","CBIDF2K.SY_","CD20XRNT.SY_","CDFS.SY_","CDROM.SY_","CLASSPNP.SY_","CMDIDE.SY_","CPQARRAY.SY_","C_936.NL_","DAC2W2K.SY_","DAC960NT.SY_","DISK.SY_","disk1","DMBOOT.SY_","DMIO.SY_","DMLOAD.SY_","DPTI2O.SY_","DRVMAIN.SDB","FASTFAT.SY_","FDC.SY_","FLPYDISK.SY_","FTDISK.SY_","HAL.DL_","HALAACPI.DL_","HALACPI.DL_","HALAPIC.DL_","HIDCLASS.SY_","HIDPARSE.SY_","HIDUSB.SY_","HPN.SY_","I2OMGMT.SY_","I2OMP.SY_","I8042PRT.SY_","INI910U.SY_","INTELIDE.SY_","ISAPNP.SY_","KBDCLASS.SY_","KBDHID.SY_","KBDUS.DLL","KD1394.DL_","KDCOM.DL_","KSECDD.SYS","LBRTFDC.SY_","L_INTL.NL_","migrate.inf","MOUNTMGR.SY_","MRAID35X.SY_","NTDETECT.COM","NTFS.SYS","NTKRNLMP.EX_","NTLDR","OHCI1394.SY_","OPRGHDLR.SY_","PARTMGR.SY_","PCI.SY_","PCIIDE.SY_","PCIIDEX.SY_","PCMCIA.SY_","PERC2.SY_","PERC2HIB.SY_","QL1080.SY_","QL10WNT.SY_","QL12160.SY_","QL1240.SY_","QL1280.SY_","RAMDISK.SY_","SBP2PORT.SY_","SCSIPORT.SY_","SERENUM.SY_","SERIAL.SY_","SETUPDD.SY_","SETUPLDR.BIN","SETUPREG.HIV","SFLOPPY.SY_","SPARROW.SY_","SPCMDCON.SYS","SPDDLANG.SY_","SYMC810.SY_","SYMC8XX.SY_","SYM_HI.SY_","SYM_U3.SY_","TFFSPORT.SY_","TOSIDE.SY_","TXTSETUP.SIF","ULTRA.SY_","USBCCGP.SY_","USBD.SY_","USBEHCI.SY_","USBHUB.SY_","USBOHCI.SY_","USBPORT.SY_","USBSTOR.SY_","USBUHCI.SY_","VGA.SY_","VGAOEM.FO_","VIAIDE.SY_","VIDEOPRT.SY_","WINNT.SIF","WMILIB.SY_" };
	int a = 0;
	string dirstr2;
	for (string file : filelist) {
		try {
			a++;
			CopyFile(s2ws(dirstr + "\\" + file).c_str(), s2ws(path + "\\" + file).c_str(), FALSE);
			SetWindowText(protxt, s2ws("复制启动文件：" + file).c_str());
			mylogger->info("复制启动文件：" + file);
			MoveWindow(barfw, 0, 427, (float)a * (640/4 / 114), 14, TRUE);
		}
		catch (exception e) {
			isloading = false;
			mylogger->error("复制文件发生异常，原因：{:s}", e.what());
			MessageBox(hWnd, L"复制文件错误！未成功找到文件，请确认文件夹是否正确！", 0, MB_ICONERROR);
			SetWindowText(protxt, NULL);
			MoveWindow(barfw, 0, 427, 0, 15, TRUE);
			EnableWindow(win3, true);
			EnableWindow(btnghost, true);
			EnableWindow(btnwim, true);
			EnableWindow(btndisk, true);
			EnableWindow(btnreboot, true);
			return;
		}
	}
	try {
		mylogger->info("复制SYSTEM32文件夹...");
		SetWindowText(protxt, s2ws("复制SYSTEM32文件夹...").c_str());
		CreateDirectory(s2ws(path + "\\SYSTEM32").c_str(), NULL);
		filesystem::copy(dirstr + "\\SYSTEM32", path + "\\SYSTEM32", filesystem::copy_options::recursive);
		//copy_directory(dirstr + "\\SYSTEM32", path + "\\SYSTEM32");
		SetWindowText(protxt3, NULL);
		MoveWindow(barfw, 0, 427, 113 * (640/4*2 / 114), 15, TRUE);
		mylogger->info("复制安装文件夹...");
		SetWindowText(protxt, s2ws("复制安装文件夹...").c_str());
		CreateDirectory(s2ws(tarstr + "$WIN_NT$.~LS").c_str(), NULL);
		CreateDirectory(s2ws(tarstr + "$WIN_NT$.~LS\\"+CPUMode).c_str(), NULL);
		filesystem::copy(dirstr, tarstr + "$WIN_NT$.~LS\\" + CPUMode, filesystem::copy_options::recursive);
		//copy_directory(dirstr, tarstr + "$WIN_NT$.~LS\\" + CPUMode);
		SetWindowText(protxt3, NULL);
		if (CPUMode == "I386") {
			MoveWindow(barfw, 0, 427, 640, 15, TRUE);
			goto CopyOK;
		}
		else {
			CPUMode = "I386";
		}
		dirstr2 = ws2s(dirs);
		MoveWindow(barfw, 0, 427, 113 * (640 / 4 * 3 / 114), 15, TRUE);
		CreateDirectory(s2ws(tarstr + "$WIN_NT$.~LS\\" + CPUMode).c_str(), NULL);
		filesystem::copy(dirstr2+CPUMode, tarstr + "$WIN_NT$.~LS\\" + CPUMode, filesystem::copy_options::recursive);
		//copy_directory(dirstr2+ CPUMode, tarstr + "$WIN_NT$.~LS\\" + CPUMode);
		SetWindowText(protxt3, NULL);
		MoveWindow(barfw, 0, 427, 640, 15, TRUE);
	}
	catch (exception e) {
		isloading = false;
		mylogger->error("复制文件夹发生异常，原因：{:s}", e.what());
		MessageBox(hWnd, L"复制文件夹错误！未成功找到文件夹或已有重复目录。请确认文件夹是否正确！", 0, MB_ICONERROR);
		SetWindowText(protxt, NULL);
		MoveWindow(barfw, 0, 427, 0, 15, TRUE);
		EnableWindow(win3, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnwim, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnreboot, true);
		return;
	}
CopyOK:
	fstream f;
	string sifolder = tarstr + "$WIN_NT$.~BT\\WINNT.SIF";
	f.open(sifolder, ios::out);
	f << "[Data]\nFloppyLess = 1\nMsDosInitiated = 1\nUnattendedInstall = \"Yes\"\n\n[Unattended]\nTargetPath = Windows\nOemSkipEula = Yes" << endl;
	f.close();
	fstream f2;
	string sifolder2 = tarstr + "$WIN_NT$.~BT\\migrate.inf";
	f2.open(sifolder2, ios::out);
	string mig = "[Version]\n\
Signature = \"$Windows NT$\"\n\
\n\
[Addreg]\n\
HKLM, \"SYSTEM\\MountedDevices\", , 0x00000010\n\
HKLM, \"SYSTEM\\MountedDevices\", \"\\DosDevices\\C:\", 0x00030001, \\\
be, 1d, 6d, 24, 00, 00, 10, 00, 00, 00, 00, 00";
	f2 << mig << endl;
	f2.close();
	TCHAR txtfile[MAX_PATH] = { 0 };
	Edit_GetText(edit5, txtfile, MAX_PATH);
	if (isFileExists_ifstream(ws2s(txtfile))) {
		CopyFile(txtfile, s2ws(tarstr + "$WIN_NT$.~BT\\WINNT.SIF").c_str(), false);
	}
	SetWindowText(protxt, L"更新引导...");
	string cmd = ".\\bootsect.exe /nt52 " + to_string(ws2s(tar).at(0)) + ": /mbr";
	SetWindowText(protxt, s2ws("复制引导文件...").c_str());
	CopyFile(s2ws(dirstr2 + "\\I386\\NTDETECT.COM").c_str(), s2ws(tarstr + "NTDETECT.COM").c_str(), false);
	CopyFile(s2ws(dirstr + "\\NTDETECT.COM").c_str(), s2ws(tarstr + "NTDETECT.COM").c_str(), false);
	CopyFile(s2ws(dirstr2 + "\\I386\\TXTSETUP.SIF").c_str(), s2ws(tarstr + "TXTSETUP.SIF").c_str(), false);
	CopyFile(s2ws(dirstr + "\\TXTSETUP.SIF").c_str(), s2ws(tarstr + "TXTSETUP.SIF").c_str(), false);
	CopyFile(s2ws(dirstr2 + "\\I386\\BOOTFONT.BIN").c_str(), s2ws(tarstr + "BOOTFONT.BIN").c_str(), false);
	CopyFile(s2ws(dirstr + "\\BOOTFONT.BIN").c_str(), s2ws(tarstr + "BOOTFONT.BIN").c_str(), false);
	CopyFile(L".\\NTLDR", s2ws(tarstr + "NTLDR").c_str(), false);
	RunMyExec(cmd.c_str());
	isloading = false;
	MessageBox(hWnd, L"安装成功！重启后将进行进一步安装。", L"提示：", MB_ICONINFORMATION);
	SetWindowText(protxt, NULL);
	MoveWindow(barfw, 0, 427, 0, 15, TRUE);
	EnableWindow(win3, true);
	EnableWindow(btnghost, true);
	EnableWindow(btnwim, true);
	EnableWindow(btndisk, true);
	EnableWindow(btnreboot, true);
	return;
}

LRESULT CALLBACK InWin3Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //该子窗口消息不会被处理，测试阶段
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"           对于早期Windows操作系统（XP及更早版本）";
		TCHAR msg2[] = L"    光盘映像内由一个I386文件夹或AMD64文件夹存放安装信息";
		TCHAR msg3[] = L"                本页面可用于安装NT5.x的操作系统。";
		TCHAR i86dir[] = L"   系统目录：";
		TCHAR i86disk[] = L"   安装分区：";
		TCHAR txtdir[] = L"   应答文件：";
		TCHAR msg4[] = L"          （留空或路径不正确则不进行无人值守模式）";
		TCHAR msg5[] = L"      注：所有早期Windows操作系统均不支持UEFI启动";
		TCHAR msg6[] = L"文件夹目录格式示例：“D:\\”（其中有D:\\I386或D:\\AMD64）";
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
			Edit_SetText(edit5, GetGhoFile(L"应答文件(*.txt/*.sif)\0*.txt;*.sif\0\0",hwnd));
			break;
		}
		case xpdiskbt:
		{
			SetAllVolumeMountPoint();
			AddDiskList(hWndComboBox5);
			break;
		}
		case fmt3: {
			TCHAR drive[5] = { 0 };
			Edit_GetText(hWndComboBox5, drive, 5);
			char par = ws2s(drive).c_str()[0];
			SHFormatDrive(hwnd, par - 65, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);
			break;
		}
		case xploadbt:
		{
			TCHAR szBuffer[MAX_PATH] = { 0 };
			BROWSEINFO bi;
			ZeroMemory(&bi, sizeof(BROWSEINFO));
			bi.hwndOwner = NULL;
			bi.pszDisplayName = szBuffer;
			bi.lpszTitle = _T("选择包含I386或AMD64的目录:");
			bi.ulFlags = BIF_RETURNFSANCESTORS;
			LPITEMIDLIST idl = SHBrowseForFolder(&bi);
			if (NULL == idl)
			{
				break;
			}
			SHGetPathFromIDList(idl, szBuffer);
			Edit_SetText(edit4, szBuffer);
			wstring WinVer;
			BOOL cpuresult = GetCPUOfWinXP(szBuffer,&WinVer);
			if (cpuresult == -1) {
				SetWindowText(StcCPUMode, L"不是有效的Windows源文件！");
			}else if (cpuresult) {
				SetWindowText(StcCPUMode, WinVer.c_str());
			}
			else {
				SetWindowText(StcCPUMode, WinVer.c_str());
			}
			EnableWindow(btnxp, false);
			break;
		}
		case xpisoloadbt:
		{
			
			isopath = ws2s(GetGhoFile(L"镜像文件(*.iso)\0*.iso\0\0", hwnd));
			imgtype = 2;
			thread t(mountwimiso);
			t.detach();
			break;
		}
		case xpstartbt:
		{
			TCHAR dirs[1024] = { 0 };
			Edit_GetText(edit4, dirs, 1024);
			string dirstr = ws2s(dirs).c_str();
			if (!is_folder_path(dirstr))
			{
				MessageBox(hwnd, L"请输入正确的文件夹路径！", L"错误：", MB_ICONERROR);
			}
			else {
				if (MessageBox(hwnd, L"确认安装NT5.x的操作系统吗？期间请勿进行任何操作。", L"提示：", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
				{
					thread t(CopyXPFile);
					t.detach();
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

void Bak(){
	isloading = true;
	thread t1(loading_anim);
	t1.detach();
	nowtime = 0;
	thread t(CountSeconds);
	t.detach();
	TCHAR dirs[MAX_PATH] = { 0 };
	Edit_GetText(hsavepathedit, dirs, MAX_PATH);
	EnableWindow(win5, false);
	EnableWindow(btnghost, false);
	EnableWindow(btnxp, false);
	EnableWindow(btnwim, false);
	EnableWindow(btnreboot, false);
	WIMStruct* WIM;
	int result;
	int ct;
	if (ComboBox_GetCurSel(hcompcbox) == 4) {
		ct = 3;
	}
	else {
		ct = ComboBox_GetCurSel(hcompcbox);
	}
	result = wimlib_create_new_wim((wimlib_compression_type)ct,&WIM);
	if (result != 0) {
		isloading = false;
		mylogger->error("无法准备WIM文件，原因：" + ws2s((TCHAR*)GetWimErrorString((wimlib_error_code)result)));
		MessageBox(hWnd, s2ws("无法准备WIM文件，原因："+ws2s((TCHAR*)GetWimErrorString((wimlib_error_code)result))).c_str(), 0, MB_ICONERROR);
		EnableWindow(win5, true);

		SetWindowText(protxt, NULL);
		SetWindowTextA(protxt3, NULL);

		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnwim, true);
		EnableWindow(btnreboot, true);
		return;
	}
	TCHAR savedisk[MAX_PATH] = { 0 };
	ComboBox_GetText(hsavediskcb, savedisk, MAX_PATH);
	wimlib_register_progress_function(WIM, ApplyWimImage, NULL);
	result = wimlib_add_image(WIM, savedisk,L"CWinst Backup Image", NULL,NULL);
	if (result != 0) {
		wimlib_free(WIM);
		isloading = false;
		SetWindowText(protxt, NULL);
		SetWindowTextA(protxt3, NULL);
		mylogger->error("准备要备份的文件失败，原因：" + ws2s((TCHAR*)GetWimErrorString((wimlib_error_code)result)));
		MessageBox(hWnd, s2ws("准备要备份的文件的时候发生错误，错误原因：" + ws2s((TCHAR*)(GetWimErrorString((wimlib_error_code)result)))).c_str(), 0, MB_ICONERROR);
		EnableWindow(win5, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnwim, true);
		EnableWindow(btnreboot, true);
		return;
	}
	int flag;
	if (ComboBox_GetCurSel(hcompcbox) == 4) {
		flag = WIMLIB_WRITE_FLAG_SOLID;
	}
	else {
		flag = NULL;

	}
	result = wimlib_write(WIM, dirs, WIMLIB_ALL_IMAGES, flag, NULL);
	if (result != 0) {
		wimlib_free(WIM);
		isloading = false;
		SetWindowText(protxt, NULL);
		SetWindowTextA(protxt3, NULL);
		mylogger->error("无法备份WIM文件，原因：" + ws2s((TCHAR*)GetWimErrorString((wimlib_error_code)result)));
		MessageBox(hWnd, s2ws("备份的文件的时候发生错误，错误原因：" + ws2s((TCHAR*)(GetWimErrorString((wimlib_error_code)result)))).c_str(), 0, MB_ICONERROR);
		EnableWindow(win5, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnwim, true);
		EnableWindow(btnreboot, true);
		return;
	}
	wimlib_free(WIM);
	isloading = false;
	MessageBox(hWnd, L"备份完成！", L"成功：", MB_ICONINFORMATION);
	SetWindowText(protxt, NULL);
	SetWindowTextA(protxt3, NULL);

	EnableWindow(win5, true);
	EnableWindow(btnghost, true);
	EnableWindow(btnxp, true);
	EnableWindow(btnwim, true);
	EnableWindow(btnreboot, true);
	return;
}
LRESULT CALLBACK TWNSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_INITDIALOG:
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_CHECK1), FALSE);
		Button_SetCheck(GetDlgItem(hwnd,IDC_RADIO1),TRUE);
		return TRUE;
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK1), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), TRUE);
			return TRUE;
		case IDC_RADIO2:
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK1), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), FALSE);
			return TRUE;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		case IDOK:
		{
			if (Button_GetCheck(GetDlgItem(hwnd, IDC_RADIO2))) {
				TCHAR Username[MAX_PATH] = { 0 };
				TCHAR Password[MAX_PATH] = { 0 };
				TCHAR ActiveKey[MAX_PATH] = { 0 };
				TCHAR FstRun[MAX_PATH] = { 0 };
				TCHAR PreRun[MAX_PATH] = { 0 };
				Edit_GetText(GetDlgItem(hwnd, IDC_EDIT2), Username, MAX_PATH);
				Edit_GetText(GetDlgItem(hwnd, IDC_EDIT3), Password, MAX_PATH);
				Edit_GetText(GetDlgItem(hwnd, IDC_EDIT4), ActiveKey, MAX_PATH);
				Edit_GetText(GetDlgItem(hwnd, IDC_EDIT6), FstRun, MAX_PATH);
				Edit_GetText(GetDlgItem(hwnd, IDC_EDIT5), PreRun, MAX_PATH);
				bool Skiped = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK1));
				wstring un = Username;
				wstring pw = Password;
				wstring ak = ActiveKey;
				wstring fr = FstRun;
				wstring pr = PreRun;
				CreateUnattendXML(ws2s(un), ws2s(pw), ws2s(ak), ws2s(pr), ws2s(fr), Skiped);
				TCHAR rundir[MAX_PATH] = { 0 };
				GetCurrentDirectory(MAX_PATH, rundir);
				string rd = ws2s(rundir);
				wstring xml_path = s2ws(rd + "\\Unattend.xml");
				Edit_SetText(edit3, xml_path.c_str());
				EndDialog(hwnd, 0);
				return TRUE;
			}
			TCHAR xml_path[MAX_PATH] = { 0 };
			GetDlgItemTextW(hwnd, IDC_EDIT1, xml_path, MAX_PATH);
			Edit_SetText(edit3, xml_path);
			EndDialog(hwnd, 0);
			return TRUE;
		}
		case IDC_BUTTON1:
			Edit_SetText(GetDlgItem(hwnd, IDC_EDIT1), GetGhoFile(L"应答文件(*.xml)\0*.xml\0\0", hwnd));
			return TRUE;
		}
		return TRUE;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}
	return FALSE;
}
LRESULT CALLBACK InWin4Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //该子窗口消息不会被处理，测试阶段
{
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_PAINT: // 绘制消息
	{
		TCHAR msg1[] = L"                         出于对某些操作的考虑，";
		TCHAR msg2[] = L"                对操作系统进行备份是一个很重要的行为。";
		TCHAR msg3[] = L"         本页面可用于将操作系统备份成一个映像以供还原。";
		TCHAR bakpar[] = L"   备份分区：";
		TCHAR savpath[] = L"   保存位置：";
		TCHAR compmode[] = L"   压缩方式：";
		TCHAR msg4[] = L"                    保存映像为WIM，若想还原备份";
		TCHAR msg5[] = L"        可通过本程序“WIM映像应用”功能还原备份的映像";
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps); // 获取设备上下文 
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 64));
		SelectObject(hdc, hFont2);
		TextOut(hdc, 32, 34, msg1, _tcslen(msg1));
		TextOut(hdc, 32, 49, msg2, _tcslen(msg2));
		TextOut(hdc, 32, 64, msg3, _tcslen(msg3));
		TextOut(hdc, 32, 134, bakpar, _tcslen(bakpar));
		TextOut(hdc, 32, 99, savpath, _tcslen(savpath));
		TextOut(hdc, 32, 171, compmode, _tcslen(compmode));
		TextOut(hdc, 32, 204, msg4, _tcslen(msg4));
		TextOut(hdc, 32, 219, msg5, _tcslen(msg5));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case savediskbt:
		{
			SetAllVolumeMountPoint();
			AddDiskList(hsavediskcb);
			break;
		}
		case saveloadbt:
		{
			time_t t = time(nullptr);
			tm* now = localtime(&t);

			// 使用ostringstream来格式化字符串
			ostringstream time_stream;
			time_stream << setfill('0')
				<< setw(2) << now->tm_mon + 1
				<< setw(2) << now->tm_mday
				<< setw(2) << now->tm_hour
				<< setw(2) << now->tm_min;

			// 将格式化的时间赋值给string变量
			string time_str = time_stream.str();
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			string filenamehead = "CW-BAK_";
			wstring temp = s2ws(filenamehead + time_str + ".wim");
			const TCHAR* cszFilename = temp.c_str();
			// 创建一个足够大的TCHAR数组来存储可修改的字符串
			TCHAR szFilename[MAX_PATH];

			// 使用_tcscpy或相关函数复制字符串
			_tcscpy_s(szFilename, sizeof(szFilename) / sizeof(TCHAR), cszFilename);
			ofn.lpstrFile = szFilename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = _T("普通映像(.wim)\0*.wim\0高级压缩映像(.esd)\0*.esd\0\0");
			ofn.lpstrDefExt = _T("wim");
			ofn.lpstrTitle = _T("保存到...");
			ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			ofn.lStructSize = sizeof(OPENFILENAME);  // No OFN_ENABLEHOOK
			ofn.hwndOwner = hWnd;
			GetSaveFileName(&ofn);
			Edit_SetText(hsavepathedit,szFilename);
			if (endsWithEsd(szFilename)) {
				ComboBox_SetCurSel(hcompcbox,4);
				EnableWindow(hcompcbox, FALSE);
			}
			else {
				ComboBox_SetCurSel(hcompcbox, 2);
				EnableWindow(hcompcbox, TRUE);
			}
			break;
		}
		case savestartbt:
		{
			TCHAR dirs[1024] = { 0 };
			Edit_GetText(hsavepathedit, dirs, 1024);
			string dirstr = ws2s(dirs).c_str();
			if (MessageBox(hwnd, L"准备开始备份，过程请勿对备份分区进行操作。", L"提示：", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
			{
				thread t(Bak);
				t.detach();
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
	TCHAR msg3[] = L"纯净、简单、安全的Windows安装工具";
	TCHAR msg4[] = L"Copyright © 2024 CWInst-纯净且安全的Windows安装工具";
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
		//TextOut(hdc, 510, 405, msg, _tcslen(msg));
		//TextOut(hdc, 590, 405, msg2, _tcslen(msg2));
		TextOut(hdc, 12, 83, msg3, _tcslen(msg3));
		TextOut(hdc, 254, 16, msg4, _tcslen(msg4));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case diskbt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 4, 0);
			EnableWindow(btnwim, true);
			EnableWindow(btnghost, true);
			EnableWindow(hTabCtrl, TRUE);
			EnableWindow(btnxp, true);
			EnableWindow(btndisk, false);
			SetWindowShow();
			break;
		}
		case xpbt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 2, 0);
			EnableWindow(btnwim, true);
			EnableWindow(btnghost, true);
			EnableWindow(hTabCtrl, TRUE);
			EnableWindow(btnxp, false);
			EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		case wimbt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 1, 0);
			EnableWindow(btnwim, false);
			EnableWindow(hTabCtrl, TRUE);
			EnableWindow(btnghost, true);
			EnableWindow(btnxp, true);
			EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		case ghobt:
		{
			SendMessage(hTabCtrl, TCM_SETCURSEL, 0, 0);
			EnableWindow(hTabCtrl, TRUE);

			EnableWindow(btnwim, true);
			EnableWindow(btnghost, false);
			EnableWindow(btnxp, true);
			EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		case bootbt:
		{
			int ask = MessageBox(hwnd, L"确定要重启计算机，离开ComPE系统吗？若有操作正在进行，请确保其执行完毕后重启，否则可能造成设备损坏。\n重启请按“是”，否则请按“否”。", L"提示：", MB_YESNO | MB_ICONQUESTION);
			if (ask == IDYES) {
				powerOffProc();
				RunMyExec("wpeutil reboot");
				RunMyExec("pecmd shut r");
			}
			break;
		}
		case logobt:
			SendMessage(hTabCtrl, TCM_SETCURSEL, 3, 0);
			EnableWindow(hTabCtrl, FALSE);
			EnableWindow(btnwim, true);
			EnableWindow(btnghost, true);
			EnableWindow(btnxp, true);
			EnableWindow(btndisk, true);
			SetWindowShow();
			break;
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"确定要关闭程序吗？执行操作期间关闭可能导致设备损坏，确保所有操作结束后，选择“是”关闭程序。", L"提示：", MB_ICONQUESTION | MB_YESNO) == IDYES) {
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
	{
		mylogger->info("退出程序...");
		DeleteMountedEFI();
		string unmountcmd = "imdisk -D -m " + MountedDisk;
		RunMyExec(unmountcmd.c_str());
		UninstalledImDisk();
		RemoveFontResource(L".\\Fonts\\HarmonyOS_Sans_SC_Regular.ttf");
		RemoveFontResource(L".\\Fonts\\segoe_slboot.ttf");
		PostQuitMessage(0);//发出WM_QUIT消息，结束消息循环
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == barfw) {
			return (INT_PTR)CreateSolidBrush(RGB(0, 0, 64));
		}
		if ((HWND)lParam == barbg) {
			return (INT_PTR)CreateSolidBrush(RGB(236, 237, 255));
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
	}
	return 0;
}

LRESULT CALLBACK ISOSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_INITDIALOG://初始化对话框的代码添加到这
		for (string file : imglist) {
			ComboBox_AddString(GetDlgItem(hwnd, IDC_COMBO1), s2ws(file).c_str());
		}
		ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO1), 0);
		return TRUE;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK:
		{
			TCHAR wimfile[MAX_PATH] = { 0 };
			ComboBox_GetText(GetDlgItem(hwnd, IDC_COMBO1), wimfile, MAX_PATH);
			string wimuse = ws2s(wimfile);
			isloading = false;
			EnableWindow(win1, true);
			EnableWindow(win2, true);
			EnableWindow(win3, true);
			EnableWindow(btndisk, true);
			EnableWindow(btnghost, true);
			EnableWindow(btnxp, true);
			EnableWindow(btnreboot, true);
			SetWindowText(protxt, NULL);
			switch (imgtype) {
			case 0: {
				EnableWindow(btnwim, false);
				Edit_SetText(edit2, s2ws(wimuse).c_str());
				wstring filefrom = s2ws(wimuse);
				filesystem::path wpath_obj(filefrom);
				wstring files = filefrom;
				if (wpath_obj.extension() == L".swm" || wpath_obj.extension() == L".SWM") {
					DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, SWMSunProc);
					EnableWindow(btSWM, TRUE);
					GetWimSysInfo(filefrom.c_str());
					break;
				}
				GetWimSysInfo(filefrom.c_str());
				break;
			}
			case 1:
				Edit_SetText(edit, s2ws(imglist.at(0)).c_str());
				EnableWindow(btnghost, false);
				break;
			case 2:
				EnableWindow(btnxp, false);
			}
			EndDialog(hwnd, 0);
			return TRUE;
		}
		}
		return TRUE;
	}
	case WM_CLOSE:
	{
		TCHAR wimfile[MAX_PATH] = { 0 };
		ComboBox_GetText(GetDlgItem(hwnd, IDC_COMBO1), wimfile, MAX_PATH);
		string wimuse = ws2s(wimfile);
		isloading = false;
		EnableWindow(win1, true);
		EnableWindow(win2, true);
		EnableWindow(win3, true);
		EnableWindow(btndisk, true);
		EnableWindow(btnghost, true);
		EnableWindow(btnxp, true);
		EnableWindow(btnreboot, true);
		SetWindowText(protxt, NULL);
		switch (imgtype) {
		case 0: {
			EnableWindow(btnwim, false);
			Edit_SetText(edit2, s2ws(wimuse).c_str());
			wstring filefrom = s2ws(wimuse);
			filesystem::path wpath_obj(filefrom);
			wstring files = filefrom;
			if (wpath_obj.extension() == L".swm" || wpath_obj.extension() == L".SWM") {
				DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG3), hWnd, SWMSunProc);
				EnableWindow(btSWM, TRUE);
				GetWimSysInfo(filefrom.c_str());
				break;
			}
			GetWimSysInfo(filefrom.c_str());
			break;
		}
		case 1:
			Edit_SetText(edit, s2ws(imglist.at(0)).c_str());
			EnableWindow(btnghost, false);
			break;
		case 2:
			EnableWindow(btnxp, false);
		}
		EndDialog(hwnd, 0);
		return TRUE;
	}
	}
	return FALSE;
}

LRESULT CALLBACK SWMSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)//通过判断消息进行消息响应
	{
	case WM_INITDIALOG://初始化对话框的代码添加到这
	{
		wchar_t SWMPath[MAX_PATH] = { 0 };
		Edit_GetText(edit2, SWMPath, MAX_PATH);
 		SetWindowText(GetDlgItem(hwnd, IDC_STCSWM), SWMPath);
		for (wstring ESFS : EnableSWMs) {
			ListBox_AddString(GetDlgItem(hwnd, IDC_LIST1), ESFS.c_str());
		}
		EnableSWMs.clear();
		return TRUE;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK:
		{
			int allItem = ListBox_GetCount(GetDlgItem(hwnd, IDC_LIST1));
			for (int i = 0; i <= allItem; ++i) {
				wchar_t theSwm[MAX_PATH] = { 0 };
				ListBox_GetText(GetDlgItem(hwnd, IDC_LIST1), i, theSwm);
				wstring Tsm = theSwm;
				if (Tsm != L"") {
					EnableSWMs.push_back(theSwm);
				}
			}
			EndDialog(hwnd, 0);
			return TRUE;
		}
		case IDC_BUTTON1: {
			wstring file = GetGhoFile(L"拆分映像文件(*.swm)\0*.swm\0\0", hwnd);
			int allItem = ListBox_GetCount(GetDlgItem(hwnd, IDC_LIST1));
			for (int i = 0; i <= allItem; ++i) {
				wchar_t theSwm[MAX_PATH] = { 0 };
				ListBox_GetText(GetDlgItem(hwnd, IDC_LIST1), i, theSwm);
				wstring Tswm = theSwm;
				if (Tswm == file) {
					MessageBox(hwnd, L"不能重复添加swm文件！", NULL, MB_ICONERROR);
					return TRUE;
				}
			}
			wchar_t theSwm[MAX_PATH] = { 0 };
			Static_GetText(GetDlgItem(hwnd, IDC_STCSWM), theSwm, MAX_PATH);
			wstring Tswm = theSwm;
			if (Tswm == file) {
				MessageBox(hwnd, L"不能重复添加swm文件！", NULL, MB_ICONERROR);
				return TRUE;
			}
			ListBox_AddString(GetDlgItem(hwnd, IDC_LIST1), file.c_str());
			break;
		}case IDC_BUTTON2: {
			ListBox_DeleteString(GetDlgItem(hwnd, IDC_LIST1), ListBox_GetCurSel(GetDlgItem(hwnd, IDC_LIST1)));
		}
		}
		return TRUE;
	}
	case WM_CLOSE:
	{
		int allItem = ListBox_GetCount(GetDlgItem(hwnd, IDC_LIST1));
		for (int i = 0; i <= allItem; ++i) {
			wchar_t theSwm[MAX_PATH] = { 0 };
			ListBox_GetText(GetDlgItem(hwnd, IDC_LIST1), i, theSwm);
			wstring Tsm = theSwm;
			if (Tsm != L"") {
				EnableSWMs.push_back(theSwm);
			}
		}
		EndDialog(hwnd, 0);
		return TRUE;
	}
	}
	return FALSE;
}