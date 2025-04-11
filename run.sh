gcc src/virtual_machine.c -o virtualmachine
gcc src/compiler.c -o compiler
./compiler file.asm program.bin
./virtualmachine program.bin
rm -rf virtualmachine
rm -rf compiler
#rm -rf program.bin