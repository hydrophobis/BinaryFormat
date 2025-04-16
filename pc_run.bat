gcc src/virtual_machine.c -o virtualmachine
gcc src/compiler.c -o compiler
.\compiler.exe file.pc program.vmbin
.\virtualmachine.exe program.vmbin