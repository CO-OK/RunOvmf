#include<PowerManage.h>

void PowerOff(SDTHeader* xsdt){
    /*
        先得到facp
        facp也叫Fixed ACPI Description Table (FADT)
    */
    SDTHeader*facp=GetAcpiTable(xsdt,FACP_SIGNATURE);
    FADTConfig*fadtp=(FADTConfig*)((uint64_t)facp+sizeof(SDTHeader));
    /*从facp得到dsdt*/
    SDTHeader* DSDTHeader=(SDTHeader*)((uint64_t)fadtp->DSDT);
    //printf("%x\n",DSDTHeader->Signature);
    
    /*找到_S5_*/
    int len=DSDTHeader->Length-sizeof(SDTHeader);
    char*S5Addr;
    for(int i=0;i<len;i++){
        S5Addr=(char*)((uint64_t)DSDTHeader+sizeof(SDTHeader)+i);
        if(*S5Addr=='_'&&*(S5Addr+1)=='S'&&*(S5Addr+2)=='5'&&*(S5Addr+3)=='_'){
            //printf("bingo\n");
            break;
        }
    }
    if ( ( *(S5Addr-1) == 0x08 || ( *(S5Addr-2) == 0x08 && *(S5Addr-1) == '\\') ) && *(S5Addr+4) == 0x12 ){
        //printf("bingo\n");
        S5Addr += 5;
        S5Addr += ((*S5Addr &0xC0)>>6) +2;   // calculate PkgLength size

        if (*S5Addr == 0x0A)
        S5Addr++;   // skip byteprefix
        // SLP_TYPa = *(S5Addr)<<10;
        S5Addr++;

        if (*S5Addr == 0x0A)
        S5Addr++;   // skip byteprefix
        // SLP_TYPb = *(S5Addr)<<10;

        // SMI_CMD = facp->SMI_CMD;

        // ACPI_ENABLE = facp->ACPI_ENABLE;
        // ACPI_DISABLE = facp->ACPI_DISABLE;

        // PM1a_CNT = facp->PM1a_CNT_BLK;
        // PM1b_CNT = facp->PM1b_CNT_BLK;
        
        // PM1_CNT_LEN = facp->PM1_CNT_LEN;

        // SLP_EN = 1<<13;
        // SCI_EN = 1;
        outw((unsigned int) fadtp->PM1a_CNT_BLK, *(S5Addr)<<10 | 1<<13 );
        if ( fadtp->PM1b_CNT_BLK != 0 )
            outw((unsigned int) fadtp->PM1b_CNT_BLK, *(S5Addr)<<10 | 1 );

    }
    printf("power off failed\n");
    // printf("%x\n",*(temp-1));
    // printf("%x\n",*(temp-2));
    // printf("%x\n",*(temp+4));
    // for(int i=0;i<1;i++){
    //     SDTHeader* newSDTHeader = (SDTHeader*)(*((uint64_t*)((uint64_t)DSDTHeader+sizeof(SDTHeader)+i*8)));
    //     //char* temp=(char*)newSDTHeader;
    //     printf("%x\n",newSDTHeader->Signature);
    //     printf("\n");
    // }
}