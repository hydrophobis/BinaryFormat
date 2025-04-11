gcc src/virtual_machine.c -o virtualmachine
gcc src/compiler.c -o compiler
.\compiler.exe file.asm program.bin
.\virtualmachine.exe program.bin