#include<Acpi.h>

void* GetAcpiTable(SDTHeader*sdtHeader,uint32_t signature){
    
    
    int entries = (sdtHeader->Length-sizeof(SDTHeader))/8;
   
    for(int i=0;i<entries;i++){
        SDTHeader* newSDTHeader = (SDTHeader*)(*((uint64_t*)((uint64_t)sdtHeader+sizeof(SDTHeader)+i*8)));
        if(newSDTHeader->Signature==signature)
            return newSDTHeader;
    }
    return NULL;
    
}