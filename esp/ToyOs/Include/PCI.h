#pragma once
#include<Acpi.h>
#include<PageTableManager.h>
#include<Char.h>


typedef struct {
    /*
        不同的 PCI有不同的结构，但表头大体一致
        https://wiki.osdev.org/PCI#Header_Type_0x0
    */
    /*  表头
        Register| Offset | Bits 31-24 |	Bits 23-16 | Bits 15-8 | Bits 7-0
          0x0 	   0x0 	    Device ID 	             Vendor ID
          0x1 	   0x4 	     Status 	             Command
          0x2 	   0x8 	   Class code 	 Subclass 	  Prog IF 	 Revision ID
          0x3 	   0xC 	      BIST 	    Header type Latency Timer Cache Line Size 
    */
    //Vendor ID: Identifies the manufacturer of the device. 
    //           Where valid IDs are allocated by PCI-SIG  to ensure uniqueness and 0xFFFF is an invalid value 
    //           that will be returned on read accesses to Configuration Space registers of non-existent devices. 
    uint16_t VendorID;
    //Device ID: Identifies the particular device. Where valid IDs are allocated by the vendor. 
    uint16_t DeviceID;
    //Command: Provides control over a device's ability to generate and respond to PCI cycles. 
    //         Where the only functionality guaranteed to be supported by all devices is, when a 0 is written to this register, 
    //         the device is disconnected from the PCI bus for all accesses except Configuration Space access. 
    uint16_t Command;
    //Status: A register used to record status information for PCI bus related events. 
    uint16_t Status;
    //Revision ID: Specifies a revision identifier for a particular device. Where valid IDs are allocated by the vendor. 
    uint8_t RevisionID;
    //Prog IF(Programming Interface Byte): A read-only register that specifies a register-level programming interface the device has, if it has any at all. 
    uint8_t ProgIF;
    //Subclass: A read-only register that specifies the specific function the device performs. 
    uint8_t Subclass;
    //Class Code: A read-only register that specifies the type of function the device performs. 
    uint8_t ClassCode;
    //Cache Line Size: Specifies the system cache line size in 32-bit units. A device can limit the number of cacheline sizes it can support, 
    //                 if a unsupported value is written to this field, the device will behave as if a value of 0 was written. 
    uint8_t CacheLineSize;
    //Latency Timer: Specifies the latency timer in units of PCI bus clocks.
    uint8_t LatencyTimer;
    //Header Type: Identifies the layout of the rest of the header beginning at byte 0x10 of the header and 
    //             also specifies whether or not the device has multiple functions. Where a value of 0x0 specifies a general device, 
    //             a value of 0x1 specifies a PCI-to-PCI bridge, and a value of 0x2 specifies a CardBus bridge. 
    //             If bit 7 of this register is set, the device has multiple functions; otherwise, it is a single function device. 
    uint8_t HeaderType;
    //BIST: Represents that status and allows control of a devices BIST (built-in self test). 
    uint8_t BIST;

}PCIDeviceHeader;


typedef struct{
    /*包括PCIDeviceHeader在内的完整结构 https://wiki.osdev.org/PCI#Header_Type_0x0 */
    PCIDeviceHeader Header;
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    /*Points to the Card Information Structure and is used by devices that share silicon between CardBus and PCI. */
    uint32_t CardbusCISPointer;
    uint16_t SubsystemVendorID;
    uint16_t SubsystemID;
    uint32_t ExpansionROMBaseAddress;
    /*
        Points (i.e. an offset into this function's configuration space) to a linked list of new capabilities implemented by the device. 
        Used if bit 4 of the status register (Capabilities List bit) is set to 1. 
        The bottom two bits are reserved and should be masked before the Pointer is used to access the Configuration Space.
     */
    uint8_t CapabilitiesPointer;
    uint8_t Reserved0;
    uint16_t Reserved1;
    uint32_t Reserved2;
    /*
        Specifies which input of the system interrupt controllers the device's interrupt pin is connected to and is implemented by 
        any device that makes use of an interrupt pin. For the x86 architecture this register corresponds to the 
        PIC IRQ numbers 0-15 (and not I/O APIC IRQ numbers) and a value of 0xFF defines no connection. 
    */
    uint8_t InterruptLine;
    /* Specifies which interrupt pin the device uses. Where a value of 0x1 is INTA#, 0x2 is INTB#, 0x3 is INTC#, 0x4 is INTD#, and 0x0 means the device does not use an interrupt pin. */
    uint8_t InterruptPIN;
    /*A read-only register that specifies the burst period length, in 1/4 microsecond units, that the device needs (assuming a 33 MHz clock rate). */
    uint8_t MinGrant;
    /*A read-only register that specifies how often the device needs access to the PCI bus (in 1/4 microsecond units). */
    uint8_t Maxlatency; 	

}PCIDeviceHeaderFull;

/*枚举PCI总线*/
void EnumeratePCI(MCFGHeader*mcfg);

/**/
void EnumerateBus(uint64_t bassAddress,uint64_t bus);

void EnumerateDevice(uint64_t busAddress, uint64_t device);

void EnumerateFunction(uint64_t deviceAddress, uint64_t func);


extern const char* DeviceClasses[];
const char* GetDeviceClassName(int i);
const char* GetVendorName(uint16_t vendorID);
const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID);
const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode);
const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);