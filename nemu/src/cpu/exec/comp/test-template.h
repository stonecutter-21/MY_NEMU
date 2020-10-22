#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	DATA_TYPE answer = op_dest->val & op_src->val;
	int length = (DATA_BYTE << 3) - 1;
	cpu.CF=0;
	cpu.OF=0;
	cpu.SF=answer >> length;
    cpu.ZF=!answer;
    answer ^= answer >>4;
	answer ^= answer >>2;
	answer ^= answer >>1;
	cpu.PF=!(answer & 1);
	print_asm("test %s,%s", op_src->str, op_dest->str);
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
