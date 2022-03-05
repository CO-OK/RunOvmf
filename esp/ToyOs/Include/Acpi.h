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

typedef struct __attribute__((packed)){
    /*
        https://wiki.osdev.org/PCI_Express#Extended_PCI_Bus_Numbering
    */
    /*Base address of enhanced configuration mechanism */
    uint64_t BassAddress;
    /*PCI Segment Group Number */
    uint16_t SegmentGroup;
    /*Start PCI bus number decoded by this host bridge */
    uint8_t BusStart;
    /*End PCI bus number decoded by this host bridge */
    uint8_t BusEnd;
    /*Reserved */
    uint32_t Reserved;

}MCFGDeviceConfig;

/*除去header以后剩下的的东西*/
typedef struct __attribute__((packed)){
    uint32_t FIRMWARE_CTRL;
    uint32_t DSDT;//Physical memory address of the DSDT
    uint8_t Reserved;
    uint8_t Preferred_PM_Profile;
    uint16_t SCI_INT;
    uint32_t SMI_CMD;
    uint8_t ACPI_ENABLE;
    uint8_t ACPI_DISABLE;
    uint8_t S4BIOS_REQ;
    uint8_t PSTATE_CNT;
    uint32_t PM1a_EVT_BLK;
    uint32_t PM1b_EVT_BLK;
    uint32_t PM1a_CNT_BLK;
    uint32_t PM1b_CNT_BLK;
    uint32_t PM2_CNT_BLK;
    uint32_t PM_TMR_BLK;
    uint32_t GPE0_BLK;
    uint32_t GPE1_BLK;
    uint8_t PM1_EVT_LEN;
    uint8_t PM1_CNT_LEN;
    uint8_t PM2_CNT_LEN;
    uint8_t PM_TMR_LEN;
    uint8_t GPE0_BLK_LEN;
    uint8_t GPE1_BLK_LEN;
    uint8_t GPE1_BASE;
    uint8_t CST_CNT;
    uint16_t P_LVL2_LAT;
    uint16_t P_LVL3_LAT;
    uint16_t FLUSH_SIZE;
    uint16_t FLUSH_STRIDE;
    uint8_t DUTY_OFFSET;
    uint8_t DUTY_WIDTH;
    uint8_t DAY_ALRM;
    uint8_t MON_ALRM;
    uint8_t CENTURY;
    uint16_t IAPC_BOOT_ARCH;
    uint8_t Reserved1;
    uint32_t Flags;
    uint8_t RESET_REG[12];
    uint8_t RESET_VALUE;
    uint16_t ARM_BOOT_ARCH;
    uint8_t FADT_Minor_Version;
    uint64_t X_FIRMWARE_CTRL;
    uint64_t X_DSDT;
    uint32_t X_PM1a_EVT_BLK[3];
    uint32_t X_PM1b_EVT_BLK[3];
    uint32_t X_PM1a_CNT_BLK[3];
    uint32_t X_PM1b_CNT_BLK[3];
    uint32_t X_PM2_CNT_BLK[3];
    uint32_t X_PM_TMR_BLK[3];
    uint32_t X_GPE0_BLK[3];
    uint32_t X_GPE1_BLK[3];
    uint32_t SLEEP_CONTROL_REG[3];
    uint32_t SLEEP_STATUS_REG[3];
    uint64_t HypervisorIdentityVendor;

}FADTConfig;

/*得到Table*/
void* GetAcpiTable(SDTHeader*sdtHeader,uint32_t signature);