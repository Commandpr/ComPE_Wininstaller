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

    // ��ע�����
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\TPM\\WMI", 0, KEY_READ, &hKey);
    if (lResult == ERROR_SUCCESS) {
        // ��ȡTpmBackedDeviceID��ֵ
        lResult = RegQueryValueEx(hKey, L"TpmBackedDeviceID", NULL, NULL, (LPBYTE)&dwData, &dwSize);
        if (lResult == ERROR_SUCCESS) {
            // ͨ�������TpmBackedDeviceID�����Ҳ�Ϊ0��������ΪTPM�����õ�
            RegCloseKey(hKey);
            return dwData != 0;
        }
        else {
            // �����ȡʧ�ܣ�����������û������������������
            return false;
        }
        RegCloseKey(hKey);
    }
    else {
        return false;
    }
    return false;
}