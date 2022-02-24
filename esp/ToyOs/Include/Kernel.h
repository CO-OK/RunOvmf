#pragma once
#include"Shell.h"
#include<Memory32.h>
#include<Video.h>
#include<Char.h>
#include<BootConfig.h>
#include<PagInit.h>
#include<Assert.h>
#include<VideoInit.h>
#include<GDT.h>
#include<Interrupts.h>
#include<Acpi.h>
void KernelInit(BOOT_CONFIG* BootConfig);

void InitACPI(BOOT_CONFIG *BootConfig);