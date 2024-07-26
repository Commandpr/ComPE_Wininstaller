#include <windows.h>
#include <setupapi.h>
#include <CommCtrl.h>
#include <devguid.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <windowsx.h>
#include <cstring>
#include "getdiskslist.h"

#pragma comment(lib, "setupapi.lib")

void EnumerateStorageDevices(HDEVINFO deviceInfoSet, const GUID* classGuid, std::vector<std::wstring>& models) {
    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD memberIndex = 0;
    while (SetupDiEnumDeviceInfo(deviceInfoSet, memberIndex, &deviceInfoData)) {
        DWORD dataType;
        wchar_t buffer[256];
        DWORD bufferSize = sizeof(buffer);

        if (!SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, &dataType, (PBYTE)buffer, bufferSize, NULL)) {
            DWORD error = GetLastError();
        }
        else {
            models.push_back(buffer);
        }

        memberIndex++;
    }
}

void GetDriveList(HWND hComboBox) {
    SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
    // 硬盘和U盘/移动硬盘都属于存储设备类
    const GUID storageClassGuid = GUID_DEVCLASS_DISKDRIVE;
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&storageClassGuid, NULL, NULL, DIGCF_PRESENT);
    std::vector<std::wstring> diskModels;
    EnumerateStorageDevices(deviceInfoSet, &storageClassGuid, diskModels);

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    int a = 0;
    for (const auto& model : diskModels) {
        std::wcout << L"." << model << std::endl;
        std::wstringstream wss;
        wss << a << L"." << model << std::endl;
        ComboBox_AddString(hComboBox, wss.str().c_str());
        a++;
    }
    SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}