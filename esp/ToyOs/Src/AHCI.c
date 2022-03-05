#include<AHCI.h>
AHCIDriver* InitAHCIDriver(PCIDeviceHeader*header){
    AHCIDriver* AHCIPtr=(AHCIDriver*)malloc(sizeof(AHCIDriver));
    AHCIPtr->Header=header;
}