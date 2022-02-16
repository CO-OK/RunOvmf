#include<Uefi.h>
#pragma once
/*

    present
    : the present bit is set in order to show that the mapping in this entry is valid. The OS sets and deletes it.
    writeable
    : Used to control the access to this page. If not set, writing on this page will result in a pagefault. The OS sets and deletes it.
    user_access
    : Used to control the access to this page. If set the page is accessable from userspace, else it is only accessable from within the kernel. Controlled by the OS.
    write_through
    : Used to choose the caching strategy, which is either write-through or write-back. Controlled by the OS.
    cache_disabled
    : If this page is set the page will not be cached. Controlled by the OS.
    accessed
    : Set by the MMU when the page is either read from or written on. Has to be deleted by the OS.
    reserverd
    : This bit is not used in page directories.
    size
    : If set 2 MiB pages (instead of 4 KiB pages) are used. Normally not set. Controlled by the OS.
    global_page
    : This bit is not used in page directories.
    ignored bits
    : are also not used.
    page_ppm
    : contains the physical page number of the PDPT. Has to be multiplied with the page size in order to get the physical start address of the page. The OS has to set and reset this value.

*/


/*PDP*/
typedef struct
{
    /*the present bit is set in order to show that the mapping in this entry is valid. The OS sets and deletes it.*/
    uint64_t present                   :1;
    /*Used to control the access to this page. If not set, writing on this page will result in a pagefault. The OS sets and deletes it.*/
    uint64_t writeable                 :1;
    /*Used to control the access to this page. If set the page is accessable from userspace, else it is only accessable from within the kernel. Controlled by the OS*/
    uint64_t user_access               :1;
    /*Used to choose the caching strategy, which is either write-through or write-back. Controlled by the OS.*/
    uint64_t write_through             :1;
    /*If this page is set the page will not be cached. Controlled by the OS.*/
    uint64_t cache_disabled            :1;
    /*Set by the MMU when the page is either read from or written on. Has to be deleted by the OS.*/
    uint64_t accessed                  :1;
    /**/
    uint64_t ignored_3                 :1;
    /*If set 2 MiB pages (instead of 4 KiB pages) are used. Normally not set. Controlled by the OS.*/
    uint64_t size                      :1; // must be 0
    uint64_t ignored_2                 :4;
    /*contains the physical page number of the PDPT. Has to be multiplied with the page size in order to get the physical start address of the page. The OS has to set and reset this value.*/
    uint64_t page_ppn                  :28;
    /*This bit is not used in page directories.*/
    uint64_t reserved_1                :12; // must be 0
    uint64_t ignored_1                 :11;
    uint64_t execution_disabled        :1;
} PageMapLevel4Entry;

/*PD*/
typedef struct 
{
  uint64_t present                   :1;
  uint64_t writeable                 :1;
  uint64_t user_access               :1;
  uint64_t write_through             :1;
  uint64_t cache_disabled            :1;
  uint64_t accessed                  :1;
  uint64_t ignored_3                 :1;
  uint64_t size                      :1; // 0 means page directory mapped
  uint64_t ignored_2                 :4;
  uint64_t page_ppn                  :28;
  uint64_t reserved_1                :12; // must be 0
  uint64_t ignored_1                 :11;
  uint64_t execution_disabled        :1;
}PageDirPointerTablePageDirEntry ;

/*PT*/
typedef struct 
{
  uint64_t present                   :1;
  uint64_t writeable                 :1;
  uint64_t user_access               :1;
  uint64_t write_through             :1;
  uint64_t cache_disabled            :1;
  uint64_t accessed                  :1;
  uint64_t ignored_3                 :1;
  uint64_t size                      :1; // 0 means page table mapped
  uint64_t ignored_2                 :4;
  uint64_t page_ppn                  :28;
  uint64_t reserved_1                :12; // must be 0
  uint64_t ignored_1                 :11;
  uint64_t execution_disabled        :1;
}PageDirPageTableEntry  ;

/*

    dirty
    : Is set by the MMU
    when the the page is written on. Has to be deleted by the OS.
    global_page
    : If set the Translation Lookaside Buffer (TLB
    ) won’t update this address during the next context switch. Shouldn’t be used in this course. Controlled by the OS.
    page_ppn
    : contains the physical page number of the page mapped to the virtual address (i.e. physical address of the page divided by 4096).

*/

/*P*/
typedef struct
{
  uint64_t present                   :1;
  uint64_t writeable                 :1;
  uint64_t user_access               :1;
  uint64_t write_through             :1;
  uint64_t cache_disabled            :1;
  uint64_t accessed                  :1;
  uint64_t dirty                     :1;
  uint64_t size                      :1;
  uint64_t global                    :1;
  uint64_t ignored_2                 :3;
  uint64_t page_ppn                  :28;
  uint64_t reserved_1                :12; // must be 0
  uint64_t ignored_1                 :11;
  uint64_t execution_disabled        :1;
} PageTableEntry;