#include <windows.h>
#include <iostream>

#define IOCTL_GET_MESSAGE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main()
{
    HANDLE hDevice = CreateFile(L"\\\\.\\TestDriver",
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);

    if (hDevice == INVALID_HANDLE_VALUE)
    {
        std::cout << "Nu s-a deschis driverul\n";
        return 1;
    }

    char buffer[100];
    DWORD bytes;

    DeviceIoControl(hDevice, IOCTL_GET_MESSAGE,
        NULL, 0,
        buffer, sizeof(buffer),
        &bytes, NULL);

    std::cout << "Raspuns driver: " << buffer << std::endl;

    CloseHandle(hDevice);
    return 0;
}