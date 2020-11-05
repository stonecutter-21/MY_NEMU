#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/movzb.h"
#include "data-mov/movs.h"
#include "data-mov/movsx.h"
#include "data-mov/cltd.h"




// we add these
#include "tran/call.h"
#include "tran/jmp.h"
#include "tran/jne.h"
#include "tran/jle.h"
#include "tran/jbe.h"
#include "tran/jg.h"
#include "tran/jl.h"
#include "tran/js.h"
#include "tran/jns.h"
#include "tran/jge.h"
#include "tran/ja.h"




#include "tran/leave.h"
#include "push-pop-ret/push.h"
#include "comp/test.h"
#include "comp/je.h"
#include "comp/cmp.h"
#include "push-pop-ret/pop.h"
#include "push-pop-ret/ret.h"



#include "arith/dec.h"
#include "arith/adc.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"
#include "arith/sbb.h"
#include "arith/add.h"



#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/sete.h"
#include "logic/setne.h"




#include "string/rep.h"
#include "string/stos.h"
#include "string/scas.h"
#include "string/lods.h"

#include "misc/misc.h"

#include "special/special.h"
