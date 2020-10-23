#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE answer = op_dest->val - op_src->val;
	int length = (DATA_BYTE << 3) - 1;
	cpu.CF = (op_dest->val < op_src->val);
	cpu.SF=answer >> length;
    int s1,s2;
	s1=op_dest->val>>length;
	s2=op_src->val>>length;
    cpu.OF = (s1 != s2 && s2 == cpu.SF);
    cpu.ZF = !answer;
	answer ^= answer >>4;
	answer ^= answer >>2;
	answer ^= answer >>1;
	cpu.PF=!(answer & 1);
	print_asm("cmp %s,%s", op_src->str, op_dest->str);
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"
