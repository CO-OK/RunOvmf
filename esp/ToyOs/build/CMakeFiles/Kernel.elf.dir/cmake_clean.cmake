file(REMOVE_RECURSE
  "Kernel.elf"
  "Kernel.elf.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM_NASM C)
  include(CMakeFiles/Kernel.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
