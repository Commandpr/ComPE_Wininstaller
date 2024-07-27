#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include <windowsx.h>
#include <comdef.h>
#include <string>
#include <iostream>
#include "getdiskslist.h"
#include <vector>
#include <map>

#define DISK_PATH_LEN 256
using namespace std;
struct diskInfo
{
	string parNum; //分区号
	int diskNum;   //磁盘号
	string serivalNum;
	string productId;
	string vendId;
	double parTotal; //分区总量
	double parFree;  //分区可用
};
wstring ins2ws(const string& s) {
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}
void QueryPropertyForDevice(HANDLE, diskInfo&);
void getParInfo(PSTORAGE_DEVICE_DESCRIPTOR, diskInfo&);
double GetPhysicalDriveSizeInGB(int driveNumber) {
	if (driveNumber < 0) {
		return -1.0; // 非法驱动器编号
	}

	// 构造设备名称，如\\.\PhysicalDrive0
	std::wstring deviceName = L"\\\\.\\PhysicalDrive" + std::to_wstring(driveNumber);

	// 打开磁盘设备
	HANDLE hDevice = CreateFileW(
		deviceName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (hDevice == INVALID_HANDLE_VALUE) {
		return -1.0; // 无法打开设备
	}

	DISK_GEOMETRY_EX diskGeometry;
	DWORD bytesReturned;
	BOOL result = DeviceIoControl(
		hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
		NULL, 0,
		&diskGeometry,
		sizeof(diskGeometry),
		&bytesReturned,
		NULL
	);
	CloseHandle(hDevice);

	if (!result) {
		return -1.0; // 无法获取磁盘大小
	}

	// 计算磁盘大小（以GB为单位）
	double sizeInBytes = static_cast<double>(diskGeometry.DiskSize.QuadPart);
	double sizeInGB = sizeInBytes / (1024.0 * 1024.0 * 1024.0);
	return sizeInGB;
}
string replaceString(const std::string& str, const std::string& from, const std::string& to) {
	std::string result = str;
	size_t startPos = 0;

	// 遍历字符串，查找需要替换的子字符串
	while ((startPos = result.find(from, startPos)) != std::string::npos) {
		// 替换子字符串
		result.replace(startPos, from.length(), to);
		startPos += to.length(); // 移动到替换后的子字符串的末尾
	}

	return result;
}
int GetDriveList(HWND hComboBox)
{

	vector<diskInfo> vec;
	for (int i = 0; i < 24; i++)
	{
		diskInfo info;
		DWORD num = i;
		info.diskNum = num;
		wchar_t path[DISK_PATH_LEN];
		memset(path, 0, DISK_PATH_LEN);
		swprintf_s(path, L"\\\\.\\PhysicalDrive%d", num);
		HANDLE hDevice = CreateFile(path,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (hDevice == INVALID_HANDLE_VALUE)
		{
			continue;
		}
		QueryPropertyForDevice(hDevice, info);
		CloseHandle(hDevice);
		vec.push_back(move(info));
	}
	ComboBox_ResetContent(hComboBox);
	for (auto v : vec)
	{
		string pId = replaceString(v.productId, " ", "");
		char PartitionInfo[256] = { 0 };

		sprintf(PartitionInfo, "%d.%s%s,Size=%.2fGB\n",
			v.diskNum,
			v.vendId.c_str(),
			pId.c_str(),
			GetPhysicalDriveSizeInGB(v.diskNum)
		);
		wstring Pifo = ins2ws(PartitionInfo);
		ComboBox_AddString(hComboBox, Pifo.c_str());
	}
	ComboBox_SetCurSel(hComboBox, 0);
	return 0;
}


void QueryPropertyForDevice(HANDLE hDevice, diskInfo& info)
{
	bool result;

	STORAGE_PROPERTY_QUERY query;
	query.PropertyId = StorageDeviceProperty;
	query.QueryType = PropertyStandardQuery;



	STORAGE_DESCRIPTOR_HEADER device_des_head = { 0 };
	DWORD readed;
	device_des_head.Size = sizeof(STORAGE_DESCRIPTOR_HEADER);
	result = DeviceIoControl(hDevice,     // device to be queried
		IOCTL_STORAGE_QUERY_PROPERTY,     // operation to perform
		&query, sizeof(query),               // no input buffer
		&device_des_head, sizeof(STORAGE_DESCRIPTOR_HEADER),     // output buffer
		&readed,                 // # bytes returned
		NULL);      // synchronous I/O
	if (!result)
	{
		return;
	}


	vector<char> outpu_buf(device_des_head.Size);

	result = DeviceIoControl(hDevice,     // device to be queried
		IOCTL_STORAGE_QUERY_PROPERTY,     // operation to perform
		&query, sizeof query,               // no input buffer
		&outpu_buf[0], device_des_head.Size,     // output buffer
		&readed,                 // # bytes returned
		NULL);      // synchronous I/O
	if (!result)
	{
		CloseHandle(hDevice);
		return;
	}
	STORAGE_DEVICE_DESCRIPTOR* device_descriptor = reinterpret_cast <STORAGE_DEVICE_DESCRIPTOR*> (&outpu_buf[0]);
	getParInfo(device_descriptor, info);

}

void  getParInfo(PSTORAGE_DEVICE_DESCRIPTOR DeviceDescriptor, diskInfo& info)
{
	LPCSTR vendorId = "";
	LPCSTR productId = "";
	LPCSTR productRevision = "";
	LPCSTR serialNumber = "";



	if ((DeviceDescriptor->ProductIdOffset != 0) &&
		(DeviceDescriptor->ProductIdOffset != -1)) {
		productId = (LPCSTR)(DeviceDescriptor);
		productId += (ULONG_PTR)DeviceDescriptor->ProductIdOffset;
	}
	if ((DeviceDescriptor->VendorIdOffset != 0) &&
		(DeviceDescriptor->VendorIdOffset != -1)) {
		vendorId = (LPCSTR)(DeviceDescriptor);
		vendorId += (ULONG_PTR)DeviceDescriptor->VendorIdOffset;
	}
	if ((DeviceDescriptor->SerialNumberOffset != 0) &&
		(DeviceDescriptor->SerialNumberOffset != -1))
	{
		serialNumber = (LPCSTR)(DeviceDescriptor);
		serialNumber += (ULONG_PTR)DeviceDescriptor->SerialNumberOffset;
	}

	info.serivalNum = serialNumber;
	info.productId = productId;
	info.vendId = vendorId;
}

