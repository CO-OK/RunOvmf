#pragma once



typedef void VOID;
typedef unsigned char UINT8;
typedef UINT8 CHAR8;
typedef unsigned int  UINT32;
typedef unsigned long long  UINT64;
typedef unsigned short UINT16;
typedef UINT64 UINTN;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;
typedef UINT8 uint8_t;
typedef UINT16  uint16_t;
typedef UINT32  uint32_t;
typedef UINT64  uint64_t;
#define IN
#define OUT
#define NULL   ((void* )0)

/**/



#pragma pack(1)
// typedef struct
// {
//   UINT8 Blue;
//   UINT8 Green;
//   UINT8 Red;
//   UINT8 Reserved;
// } BGR_PIXEL;



#pragma pack()




///
/// Enumeration of memory types introduced in UEFI.
///
typedef enum {
  ///
  /// Not used.
  ///
  EfiReservedMemoryType,
  ///
  /// The code portions of a loaded application.
  /// (Note that UEFI OS loaders are UEFI applications.)
  ///
  EfiLoaderCode,
  ///
  /// The data portions of a loaded application and the default data allocation
  /// type used by an application to allocate pool memory.
  ///
  EfiLoaderData,
  ///
  /// The code portions of a loaded Boot Services Driver.
  ///
  EfiBootServicesCode,
  ///
  /// The data portions of a loaded Boot Serves Driver, and the default data
  /// allocation type used by a Boot Services Driver to allocate pool memory.
  ///
  EfiBootServicesData,
  ///
  /// The code portions of a loaded Runtime Services Driver.
  ///
  EfiRuntimeServicesCode,
  ///
  /// The data portions of a loaded Runtime Services Driver and the default
  /// data allocation type used by a Runtime Services Driver to allocate pool memory.
  ///
  EfiRuntimeServicesData,
  ///
  /// Free (unallocated) memory.
  ///
  EfiConventionalMemory,
  ///
  /// Memory in which errors have been detected.
  ///
  EfiUnusableMemory,
  ///
  /// Memory that holds the ACPI tables.
  ///
  EfiACPIReclaimMemory,
  ///
  /// Address space reserved for use by the firmware.
  ///
  EfiACPIMemoryNVS,
  ///
  /// Used by system firmware to request that a memory-mapped IO region
  /// be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
  ///
  EfiMemoryMappedIO,
  ///
  /// System memory-mapped IO region that is used to translate memory
  /// cycles to IO cycles by the processor.
  ///
  EfiMemoryMappedIOPortSpace,
  ///
  /// Address space reserved by the firmware for code that is part of the processor.
  ///
  EfiPalCode,
  ///
  /// A memory region that operates as EfiConventionalMemory,
  /// however it happens to also support byte-addressable non-volatility.
  ///
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;