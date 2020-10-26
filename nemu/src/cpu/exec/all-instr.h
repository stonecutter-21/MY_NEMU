#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/movzb.h"



// we add these
#include "tran/call.h"
#include "tran/jmp.h"
#include "tran/jne.h"
#include "tran/jbe.h"
#include "tran/leave.h"
#include "push-pop-ret/push.h"
#include "comp/test.h"
#include "comp/je.h"
#include "comp/cmp.h"
#include "push-pop-ret/pop.h"
#include "push-pop-ret/ret.h"



#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"
#include "arith/add.h"



#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/setne.h"



#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"
