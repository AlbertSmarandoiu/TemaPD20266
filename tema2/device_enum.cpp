#include <windows.h>
#include <setupapi.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

void PrintDevice(HDEVINFO list, SP_DEVINFO_DATA& data)
{
    BYTE buffer[512];
    DWORD size, type;

    auto Show = [&](DWORD prop, const wchar_t* name)
    {
        if (SetupDiGetDeviceRegistryPropertyW(list, &data, prop, &type, buffer, sizeof(buffer), &size))
        {
            std::wcout << name << L": " << (wchar_t*)buffer << std::endl;
        }
    };

    Show(SPDRP_DEVICEDESC, L"Descriere");
    Show(SPDRP_FRIENDLYNAME, L"Nume");
    Show(SPDRP_MFG, L"Producator");
}

int main()
{
    HDEVINFO devs = SetupDiGetClassDevsW(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);

    SP_DEVINFO_DATA data;
    data.cbSize = sizeof(data);

    int i = 0;

    while (SetupDiEnumDeviceInfo(devs, i, &data))
    {
        std::wcout << L"\nDevice " << i << L"\n";
        PrintDevice(devs, data);
        i++;
    }

    SetupDiDestroyDeviceInfoList(devs);

    std::wcout << L"\nTotal: " << i << std::endl;

    return 0;
}