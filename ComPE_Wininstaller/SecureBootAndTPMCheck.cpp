#include "SecureBootAndTPMCheck.h"
#include <string>

bool IsSecureBootEnabled() {
    HKEY hKey;
    DWORD dwData = 0;
    DWORD dwSize = sizeof(DWORD);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\SecureBoot\\State", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, L"UEFISecureBootEnabled", NULL, NULL, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return dwData != 0;
        }
        RegCloseKey(hKey);
    }
    return false;
}

bool IsTPM2Enabled() {
    HKEY hKey;
    DWORD dwData = 0;
    DWORD dwSize = sizeof(DWORD);
    LONG lResult;

    // 打开注册表项
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\TPM\\WMI", 0, KEY_READ, &hKey);
    if (lResult == ERROR_SUCCESS) {
        // 读取TpmBackedDeviceID的值
        lResult = RegQueryValueEx(hKey, L"TpmBackedDeviceID", NULL, NULL, (LPBYTE)&dwData, &dwSize);
        if (lResult == ERROR_SUCCESS) {
            // 通常，如果TpmBackedDeviceID存在且不为0，可以认为TPM是启用的
            RegCloseKey(hKey);
            return dwData != 0;
        }
        else {
            // 如果读取失败，可能是由于没有这个项，或者其他错误
            return false;
        }
        RegCloseKey(hKey);
    }
    else {
        return false;
    }
    return false;
}