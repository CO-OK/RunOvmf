run:
	qemu-system-x86_64 -m 4096 -drive if=pflash,format=raw,file=/home/v/program/EFI/runOvmf/OVMF_CODE.fd,readonly=on\
		-drive if=pflash,format=raw,file=/home/v/program/EFI/runOvmf/OVMF_VARS.fd,readonly=on\
		-drive format=raw,file=fat:rw:/home/v/program/EFI/runOvmf/esp\
		-net none