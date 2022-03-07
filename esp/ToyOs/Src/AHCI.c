#include<AHCI.h>

AHCIDriver* InitAHCIDriver(PCIDeviceHeader*header){
    AHCIDriver* AHCIPtr=(AHCIDriver*)malloc(sizeof(AHCIDriver));
    AHCIPtr->Header=header;
    AHCIPtr->ABAR=(HBA_MEM*)(((PCIDeviceHeaderFull*)(header))->BAR5);
    MapMemory(AHCIPtr->ABAR,AHCIPtr->ABAR,&GlobalPageTableManager);
    ProbePort(AHCIPtr->ABAR);
}

void ProbePort(HBA_MEM *abar)
{
	// Search disk in implemented ports
	uint32_t pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = CheckType(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				printf("SATA drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				printf("SATAPI drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				printf("SEMB drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_PM)
			{
				printf("PM drive found at port %d\n", i);
			}
			else
			{
				//printf("No drive found at port %d\n", i);
			}
		}
 
		pi >>= 1;
		i ++;
	}
}

static int CheckType(HBA_PORT *port)
{
	uint32_t ssts = port->ssts;
 
    /*关于右移可以查看AHCI文档有关Port Register的说明 https://www.intel.com.au/content/www/au/en/io/serial-ata/serial-ata-ahci-spec-rev1-3-1.html */
	uint8_t ipm = (ssts >> 8) & 0x0F;// interface power management
	uint8_t det = ssts & 0x0F;//Device Detection
 
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;
 
	switch (port->sig)
	{
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
	}
}