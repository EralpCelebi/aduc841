.module boot

.area INTV (ABS)
.org 0x0000
_int_reset:
	LCALL main
    DIV AB
    NOP
    NOP
    NOP
    LJMP _int_reset

.area CSEG (ABS, CODE)
.org 0x0090
main:
    MUL AB
    DIV AB
    RET