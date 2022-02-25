#include<PCI.h>

/*
另一种访问配置空间的方式是通过寄存器进行访问。总线内部为配置空间准备了16M寻址空间，找到寻址空间的起始地址，
根据公式PCIE_BASE_ADDRESS + (bus<<20) + (dev<<15) +(fun<<12) + offset计算出总线地址，在该地址处读取数据。
*/

void EnumeratePCI(MCFGHeader*mcfg){
    int entries = ((mcfg->Header.Length)-sizeof(MCFGHeader))/sizeof(MCFGDeviceConfig);
    for(int i=0;i<entries;i++){
        MCFGDeviceConfig* newDeviceConfig = (MCFGDeviceConfig*)((uint64_t)mcfg+sizeof(MCFGHeader)+sizeof(MCFGDeviceConfig)*i);
        for(uint64_t bus=newDeviceConfig->BusStart;bus<newDeviceConfig->BusEnd;bus++){
            EnumerateBus(newDeviceConfig->BassAddress,bus);
        }
        
    }
}

void EnumerateBus(uint64_t baseAddress,uint64_t bus){
    uint64_t offset = bus << 20;//why
    uint64_t busAddress = baseAddress + offset;
    MapMemory((void*)busAddress, (void*)busAddress,&GlobalPageTableManager);

    PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)busAddress;

    if (pciDeviceHeader->DeviceID == 0) return;
    if (pciDeviceHeader->DeviceID == 0xFFFF) return;

    for (uint64_t device = 0; device < 32; device++){
            EnumerateDevice(busAddress, device);
    }  
}

void EnumerateDevice(uint64_t busAddress, uint64_t device){
    uint64_t offset = device << 15;//why
    uint64_t deviceAddress = busAddress + offset;
    MapMemory((void*)deviceAddress, (void*)deviceAddress,&GlobalPageTableManager);

    PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)deviceAddress;

    if (pciDeviceHeader->DeviceID == 0) return;
    if (pciDeviceHeader->DeviceID == 0xFFFF) return;

    for (uint64_t func = 0; func < 8; func++){
            EnumerateFunction(deviceAddress, func);
    }  
}
void EnumerateFunction(uint64_t deviceAddress,uint64_t func){
    uint64_t offset = func << 12;//why
    uint64_t funcAddress = deviceAddress + offset;
    MapMemory((void*)funcAddress, (void*)funcAddress,&GlobalPageTableManager);

    PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)funcAddress;

    if (pciDeviceHeader->DeviceID == 0) return;
    if (pciDeviceHeader->DeviceID == 0xFFFF) return;

    printf("DeviceID=%x\n",pciDeviceHeader->DeviceID);
    printf("VendorID=%x\n",pciDeviceHeader->VendorID);
}