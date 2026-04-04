#include <ntddk.h>

#define IOCTL_GET_MESSAGE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

void UnloadDriver(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("Driver unloaded\n");
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

    if (stack->Parameters.DeviceIoControl.IoControlCode == IOCTL_GET_MESSAGE)
    {
        char msg[] = "Salut din kernel!";
        memcpy(Irp->AssociatedIrp.SystemBuffer, msg, sizeof(msg));

        Irp->IoStatus.Information = sizeof(msg);

        DbgPrint("Mesaj trimis catre user\n");
    }

    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\TestDriver");
    UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\DosDevices\\TestDriver");

    PDEVICE_OBJECT device;

    IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &device);
    IoCreateSymbolicLink(&symLink, &devName);

    DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    DriverObject->DriverUnload = UnloadDriver;

    DbgPrint("Driver loaded\n");

    return STATUS_SUCCESS;
}