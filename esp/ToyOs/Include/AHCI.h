#pragma once
#include<Uefi.h>
#include<PCI.h>
#include<Heap.h>
/*
    AHCI说明：
    https://wiki.osdev.org/AHCI
*/
typedef struct {
    PCIDeviceHeader* Header;
}AHCIDriver;

/*
初始化AHCI Driver
*/
AHCIDriver* InitAHCIDriver(PCIDeviceHeader*header);