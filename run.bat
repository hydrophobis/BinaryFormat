gcc src/virtual_machine.c -o virtualmachine
gcc src/asm_compiler.c -o compiler
.\compiler.exe file.asm program.vmbin
.\virtualmachine.exe program.vmbin