#pragma once
#include<Uefi.h>

/*一些signature*/
#define FACP_SIGNATURE 0x50434146
#define APIC_SIGNATURE 0x43495041
#define HPET_SIGNATURE 0x54455048
#define MCFG_SIGNATURE 0x4746434D
#define WAET_SIGNATURE 0x54454157
#define BGRT_SIGNATURE 0x54524742
///
/// Root System Description Pointer Structure 
/// defined in/home/v/program/EFI/edk2/MdePkg/Include/IndustryStandard/Acpi64.h
///
typedef struct __attribute__((packed)) EFI_ACPI_6_4_ROOT_SYSTEM_DESCRIPTION_POINTER {
  uint64_t    Signature;
  uint8_t     Checksum;
  uint8_t     OemId[6];
  uint8_t     Revision;
  uint32_t    RsdtAddress;
  uint32_t    Length;
  uint64_t    XsdtAddress;
  uint8_t     ExtendedChecksum;
  uint8_t     Reserved[3];
} RSDP;

/*System Description Table Header*/
typedef struct __attribute__((packed)) {
    uint32_t Signature;
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    uint8_t OEMID[6];
    uint8_t OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
}SDTHeader;

/*
    "MCFG" PCI Express Memory Mapped Configuration Space Base Address Description Table
*/
typedef struct __attribute__((packed)){
    SDTHeader Header;
    uint64_t Reserved;
}MCFGHeader;

/*得到Table*/
void* GetAcpiTable(SDTHeader*sdtHeader,uint32_t signature);