#include "cpu/exec/template-start.h"
#define instr call


// call immediate number
make_helper (concat( call_i_, SUFFIX)) {
    // first we decode and get the length of the instr;
    int length = concat (decode_i_, SUFFIX)(eip);
    swaddr_write (reg_l (R_ESP), 4, cpu.eip + length);
    DATA_TYPE_S dis = op_src->val; 
    // then we deal with the esp
    reg_l (R_ESP) -= DATA_BYTE;
    // then we write the data into stack
    
    print_asm("call %x",cpu.eip + 1 + length + dis);
    printf("eip == %x  ",cpu.eip);
    
     printf("dis ==  %x",dis);
    cpu.eip += dis;
    return (length+1);
}

// the same, in the first task, we don't need this one
make_helper (concat(call_rm_, SUFFIX))
{
	int length = concat(decode_rm_, SUFFIX)(eip + 1);
    DATA_TYPE_S dis = op_src->val;
	reg_l (R_ESP) -= DATA_BYTE;
	swaddr_write (reg_l (R_ESP) , 4 , cpu.eip + length);
	print_asm("call %x",dis);
	cpu.eip = dis - length - 1;
	return (length + 1);
}

#include "cpu/exec/template-end.h"