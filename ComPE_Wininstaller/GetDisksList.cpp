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

double GetPhysicalDriveSizeInGB(int driveNumber) {
    if (driveNumber < 0) {
        return -1.0; // �Ƿ����������
    }

    // �����豸���ƣ���\\.\PhysicalDrive0
    std::wstring deviceName = L"\\\\.\\PhysicalDrive" + std::to_wstring(driveNumber);

    // �򿪴����豸
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
        return -1.0; // �޷����豸
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
        return -1.0; // �޷���ȡ���̴�С
    }

    // ������̴�С����GBΪ��λ��
    double sizeInBytes = static_cast<double>(diskGeometry.DiskSize.QuadPart);
    double sizeInGB = sizeInBytes / (1024.0 * 1024.0 * 1024.0);
    return sizeInGB;
}

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
            double size = GetPhysicalDriveSizeInGB(memberIndex);
            models.push_back(std::to_wstring(memberIndex) + L"." + buffer + L",Size=" + std::to_wstring(size) + L"GB");
        }

        memberIndex++;
    }
}

void GetDriveList(HWND hComboBox) {
    SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
    // Ӳ�̺�U��/�ƶ�Ӳ�̶����ڴ洢�豸��
    const GUID storageClassGuid = GUID_DEVCLASS_DISKDRIVE;
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&storageClassGuid, NULL, NULL, DIGCF_PRESENT);
    std::vector<std::wstring> diskModels;
    EnumerateStorageDevices(deviceInfoSet, &storageClassGuid, diskModels);

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    int a = 0;
    for (const auto& model : diskModels) {
        double size = GetPhysicalDriveSizeInGB(a);
        std::wstringstream wss;
        wss << model;
        ComboBox_AddString(hComboBox, wss.str().c_str());
        a++;
    }
    SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}