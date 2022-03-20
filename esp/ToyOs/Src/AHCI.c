#include<AHCI.h>

AHCIDriver* InitAHCIDriver(PCIDeviceHeader*header){
    AHCIDriver* AHCIPtr=(AHCIDriver*)malloc(sizeof(AHCIDriver));
    AHCIPtr->Header=header;
    AHCIPtr->ABAR=(HBA_MEM*)(((PCIDeviceHeaderFull*)(header))->BAR5);
	AHCIPtr->portCount=0;
    MapMemory(AHCIPtr->ABAR,AHCIPtr->ABAR,&GlobalPageTableManager);
    ProbePort(AHCIPtr);
	/*探测端口完成后要初始化端口了*/
	for(int i=0;i<AHCIPtr->portCount;i++){
		ConfigurePort(AHCIPtr->port[i]);
		AHCIPtr->port[i]->buffer=(uint8_t*)Allocator.pageCtrlTable.RequestPage();
		memset(AHCIPtr->port[i]->buffer,0,0x1000);
		// ReadDisk(AHCIPtr->port[i],0,4,AHCIPtr->port[i]->buffer);
		// for(int j=0;j<1024;j++){
		// 	printf("%c",AHCIPtr->port[i]->buffer[j]);
		// }
		// printf("\n");
	}	
	
}

void ProbePort(AHCIDriver* AHCIPtr)
{
	// Search disk in implemented ports
	uint32_t pi = AHCIPtr->ABAR->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = CheckType(&AHCIPtr->ABAR->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				printf("SATA drive found at port %d\n", i);
				InitilizeAHCIDriverPort(AHCIPtr,&AHCIPtr->ABAR->ports[i],dt);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				printf("SATAPI drive found at port %d\n", i);
				InitilizeAHCIDriverPort(AHCIPtr,&AHCIPtr->ABAR->ports[i],dt);
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

void InitilizeAHCIDriverPort(AHCIDriver* AHCIPtr,HBA_PORT *hbaport,int type){
	AHCIPtr->port[AHCIPtr->portCount]=(AHCIDriverPort*)malloc(sizeof(AHCIDriverPort));
	AHCIPtr->port[AHCIPtr->portCount]->portNumber=AHCIPtr->portCount;
	AHCIPtr->port[AHCIPtr->portCount]->portType=type;
	AHCIPtr->port[AHCIPtr->portCount]->hbaPort=hbaport;
	AHCIPtr->portCount++;
}

void StopCmd(HBA_PORT *port)
{
	/*这些位可以在 ahci文档的port register章节中找到*/
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;
 
	// Clear FRE (bit4)
	port->cmd &= ~HBA_PxCMD_FRE;
 
	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
 
}

void StartCmd(HBA_PORT*port){
	// Wait until CR (bit15) is cleared
	while (port->cmd & HBA_PxCMD_CR)
		;
 
	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST; 
}

void ConfigurePort(AHCIDriverPort*ahciport){
	StopCmd(ahciport->hbaPort);



	/*cmd list base*/
	void * cmdListBase=Allocator.pageCtrlTable.RequestPage();
	ahciport->hbaPort->clb=(uint32_t)(uint64_t)cmdListBase;
	ahciport->hbaPort->clbu=(uint32_t)(((uint64_t)cmdListBase)>>32);
	memset(cmdListBase,0,1024);
	/*FIS base*/
	void* fisbase=Allocator.pageCtrlTable.RequestPage();
	ahciport->hbaPort->fb=(uint32_t)(uint64_t)fisbase;
	ahciport->hbaPort->fbu=(uint32_t)(((uint64_t)fisbase)>>32);
	memset(fisbase,0,256);

	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)((uint64_t)ahciport->hbaPort->clb+(((uint64_t)ahciport->hbaPort->clbu)<<32));

	
	/*
		下面要初始化port 里的32个cmdheader的cmd table base 
		规定一个cmdheader有8个prdt entries
		而1个prdt entry是16byte,所以8个是16*8=128byte 
		所以cmdheader对应的cmd table中要有128byte存储这8个prdt entries
		另外一个cmd table还有64+16+48=128byte 参见结构体 HBA_CMD_TBL
		所以一个cmd table要256bytes
		全部32个要256×32=8192bytes要2页，所以按页来进行循环
	*/
	int cmdHeaderNum=0;
	for(int i=0;i<2;i++){
		//4个cmd table组成一页
		void * cmdTableAddress=Allocator.pageCtrlTable.RequestPage();
		memset(cmdTableAddress,0,4096);
		for(int j=0;j<16;j++){
			cmdheader[cmdHeaderNum].prdtl=8;//8 prdt entries per command table
			uint64_t address =(uint64_t)cmdTableAddress+j<<8;//右移8位相当于*sizeof(cmd table)=256
			cmdheader[cmdHeaderNum].ctba=(uint32_t)(uint64_t)(address);
			cmdheader[cmdHeaderNum].ctbau=(uint32_t)(((uint64_t)address)>>32);
			cmdHeaderNum++;
		}
	}
	// for (int i = 0; i < 32; i++){
	// 	cmdheader[i].prdtl = 8;

	// 	void* cmdTableAddress = Allocator.pageCtrlTable.RequestPage();
	// 	uint64_t address = (uint64_t)cmdTableAddress + (i << 8);
	// 	cmdheader[i].ctba = (uint32_t)(uint64_t)address;
	// 	cmdheader[i].ctbau = (uint32_t)((uint64_t)address >> 32);
	// 	memset(cmdTableAddress, 0, 256);
	// }



	StartCmd(ahciport->hbaPort);
}

bool ReadDisk(AHCIDriverPort* driverPort,uint64_t sector,uint32_t sectorCount,void* buffer){
	/*
		driverPort 端口
		sector 扇区号
		sectorCount 扇区数
		buffer 缓冲区地址
	*/
	

	uint32_t sectorL=(uint32_t)sector;
	uint32_t sectorH=(uint32_t)(sector>>32);
	driverPort->hbaPort->is = (uint32_t) -1;		// Clear pending interrupt bits

	/*get cmd header*/
	HBA_CMD_HEADER*cmdHeader=(HBA_CMD_HEADER*)(driverPort->hbaPort->clb+(((uint64_t)driverPort->hbaPort->clbu)<<32));
	cmdHeader->cfl=sizeof(FIS_REG_H2D)/sizeof(uint32_t);
	cmdHeader->w=0;//表示是读操作
	cmdHeader->prdtl=1;
	HBA_CMD_TBL*cmdTable=(HBA_CMD_TBL*)(cmdHeader->ctba+(((uint64_t)cmdHeader->ctbau)<<32));
	memset(cmdTable,0,sizeof(HBA_CMD_TBL)+sizeof(HBA_PRDT_ENTRY)*(cmdHeader->prdtl-1));

	/*设置prdt*/
	cmdTable->prdt_entry[0].dba=(uint32_t)(uint64_t)buffer;
	cmdTable->prdt_entry[0].dbau=(uint32_t)(((uint64_t)buffer)>>32);
	//读取的字节数
	cmdTable->prdt_entry[0].dbc=(sectorCount<<9)-1; // 512 bytes per sector
	cmdTable->prdt_entry[0].i=1;

	/*设置FIS*/

	FIS_REG_H2D* cmdFIS=(FIS_REG_H2D*)(&cmdTable->cfis);
	cmdFIS->fis_type=FIS_TYPE_REG_H2D;
	cmdFIS->c=1;//表示这是一个命令
	cmdFIS->command=ATA_CMD_READ_DMA_EX;

	cmdFIS->lba0 = (uint8_t)sectorL;
	cmdFIS->lba1 = (uint8_t)(sectorL >> 8);
	cmdFIS->lba2 = (uint8_t)(sectorL >> 16);
	cmdFIS->lba3 = (uint8_t)sectorH;
	cmdFIS->lba4 = (uint8_t)(sectorH >> 8);
	cmdFIS->lba4 = (uint8_t)(sectorH >> 16);

	cmdFIS->device = 1<<6; //set LBA mode

	cmdFIS->countl= sectorCount & 0xFF;
    cmdFIS->counth = (sectorCount >> 8) & 0xFF;

	// The below loop waits until the port is no longer busy before issuing a new command
	uint64_t spin = 0;
	while ((driverPort->hbaPort->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000){
		spin ++;
	}
	if (spin == 1000000) {
		return false;
	}

	driverPort->hbaPort->ci=1;//开始执行命令

	// Wait for completion
	while (true)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if (driverPort->hbaPort->ci == 0) 
			break;
		if (driverPort->hbaPort->is & HBA_PxIS_TFES)	// Task file error
		{
			printf("Read disk error\n");
			return false;
		}
	}
 
	// Check again
	if (driverPort->hbaPort->is & HBA_PxIS_TFES)
	{
		printf("Read disk error\n");
		return false;
	}
	
	//printf("read disk true\n");
	return true;
}