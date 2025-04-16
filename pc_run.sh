gcc src/virtual_machine.c -o virtualmachine
gcc src/pc_compiler.c -o compiler
./compiler file.asm program.vmbin
./virtualmachine program.vmbin
rm -rf virtualmachine
rm -rf compiler
rm -rf program.vmbin