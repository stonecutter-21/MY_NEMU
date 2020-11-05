#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <elf.h>

enum {
	NOTYPE = 256, EQ, NEQ, AND, OR, NOT, NUMBER, HEX,REG, DEREF, NEG, SYMBOL, 
	/* TODO: Add more token types */
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},					    // equal
	{"!=", NEQ},                    // not equal
	{"&&", AND},                    // and
	{"\\|\\|", OR},                     // or
	{"!", NOT},                     // not
	{"-", '-'},                     // subtract
	{"\\*", '*'},                   // multiply
	{"/", '/'},                     // divide
	{"\\(", '('},                   // (
	{"\\)", ')'},                   // )
	{"\\$[a-eshilpx]{2,3}",REG},     // register
	{"0x[0-9a-f]+",HEX},            //hex-numbets
	{"[0-9]+", NUMBER},             // numbers
	{"\\b[a-zA-Z0-9_]+\\b", SYMBOL},// symbols (newly added from pa2...)
	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				// now, the token with type xxx is selected...
				switch(rules[i].token_type) {
					default: panic("please implement me");
					case NOTYPE:
					  break;
					case NUMBER:
					  tokens[nr_token].type = NUMBER;
					  strncpy(tokens[nr_token].str, substr_start,substr_len);
					  tokens [nr_token].str[substr_len]='\0';
					  nr_token ++;
					  break;
					case HEX:
					  tokens[nr_token].type = HEX;
					  strncpy(tokens[nr_token].str, substr_start,substr_len);
					  tokens[nr_token].str[substr_len] = '\0';
					  nr_token ++;
					  break;
					case REG:
					  tokens[nr_token].type = REG;
					  strncpy(tokens[nr_token].str, substr_start,substr_len);
					  tokens[nr_token].str[substr_len] = '\0';
					  nr_token ++;
					  break;
					case SYMBOL:
					   // printf("arrive here-- make token!!\n");
					   tokens[nr_token].type = SYMBOL;
					   strncpy(tokens[nr_token].str, substr_start,substr_len);
					   tokens[nr_token].str[substr_len] = '\0';
					   // printf("str:%s\n",tokens[nr_token].str);
					   // printf("substr_len: %d\n",substr_len);
					   nr_token ++;
					   break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case AND:
					case OR :
					case NOT:
					case DEREF:
					case NEG:
					case EQ :
					case NEQ:
					   tokens[nr_token].type = rules[i].token_type;
					   nr_token ++;
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}

bool check_parentheses(int p, int q){
	// first we check if the p and q is "(" and ")"
	if (tokens[p].type == '(' && tokens[q].type == ')') {
	}
	else {
		return false;
	}

	// then we check if all the parenteses match
	int count_num = 0;
	int i;
	for (i = p;  i <= q; i++) {
		if (count_num < 0) {
			return false;
		}
		else {
			if ( tokens[i].type == '(') {
				count_num ++;
			}
			else if (tokens[i].type == ')') {
				count_num --;
				if (count_num == 0 && i != q) 
				    return false;
			}
		}
	}
	if (count_num == 0) {
		return true;
	}
	else {
		return false;
	}
}

// return the index of dominant operator
int dominant_operator(int p, int q) {
	// from head to tail, iterate the p->q
	// method: we first find the low level as the dominant operator
	int i;
	int flag = 0; // count if there is a "()""
	int last_plus_sub = -1;
	int last_mut_div = -1;
	int last_and = -1;
	int last_or = -1;
	int last_eq = -1;
	int last_def = -1;
	int last_neg = -1;
	int last_not = -1;
	int last_neq = -1;
	for (i = p; i <= q; i++) {
		int now = tokens[i].type;
		switch (now)
		{   
			case '(':
				flag++;
				break;
			case ')':
				flag--;
				break;
			case '+':
			case '-':
				if(flag == 0) {
					last_plus_sub  = i;
					break;
				}
			case '*':
			case '/':
				if(flag == 0) {
					last_mut_div  = i;
					break;
				}
			case AND:
				if(flag == 0) {
					last_and  = i;
					break;
				}
			case OR:
				if(flag == 0) {
					last_or  = i;
					break;
				}
			case NEG:
				if(flag == 0) {
					last_neg  = i;
					break;
				}
			case DEREF:
				if(flag == 0) {
					last_def  = i;
					break;
				}
			case NOT:
				if(flag == 0) {
					last_not  = i;
					break;
				}
			case NEQ:
				if(flag == 0) {
					last_neq  = i;
					break;
				}
			case EQ:
				if(flag == 0) {
					last_eq  = i;
					break;
				}
			default:
				break;
		}
	}
		// now we rank these operators
		if (last_or != -1)	
			return last_or;
		if (last_and != -1)	
			return last_and;
		if (last_eq != -1 || last_neq != -1) 
			return last_neq > last_eq ? last_neq : last_eq;
		if (last_plus_sub != -1) 
			return last_plus_sub;
		if (last_mut_div != -1) 
			return last_mut_div;
		if (last_neg != -1 || last_def != -1)
			return last_neg > last_def ? last_neg : last_def;
		return 0;
}



int help_find_reg(char *arg, bool *success) {
	if (strcmp(arg,"$eax") == 0 ) return cpu.eax;
	if (strcmp(arg,"$ebp") == 0)  return cpu.ebp;
	if (strcmp(arg,"$ebx") == 0)  return cpu.ebx;
    if (strcmp(arg,"$ecx") == 0)  return cpu.ecx;
	if (strcmp(arg,"$edi") == 0)  return cpu.edi;
	if (strcmp(arg,"$edx") == 0)  return cpu.edx;
	if (strcmp(arg,"$eip") == 0)  return cpu.eip;
	if (strcmp(arg,"$esi") == 0)  return cpu.esi;
    if (strcmp(arg,"$esp") == 0)  return cpu.esp;
	*success = false;
	return -1;
}


uint32_t get_value_of_symbol(char *str,bool *success);


uint32_t eval(int p, int q, bool *success, int *format) {
	if (p > q) {
		//printf ("here in p > q?\n");
		*success = false;
		return -1;
		// it means the expression is wrong...
	}
	
	else if (p == q) {
		if (tokens[p].type == SYMBOL){
		int answer;
		// printf("tokens[p].str == %s\n",tokens[p].str);
		answer = get_value_of_symbol(tokens[p].str, success);
		if (*success == false) {
			    printf("can't get the value of %s\n",tokens[p].str);
				return 0;
			}
		return answer;
		}
		if (tokens[p].type == NUMBER) {
			int answer;
			sscanf(tokens[p].str,"%d",&answer);
			return answer;
		}
		else if (tokens[p].type == HEX){
			int answer;
			sscanf(tokens[p].str,"%x",&answer);
			return answer;
		}
		else if (tokens[p].type == REG){

			return help_find_reg(tokens[p].str, success);
		}
		else {
			// printf ("here in p == q?\n");
			*success = false;
			return -1;
		}
	}
	if (check_parentheses(p ,q) == true) {

		return eval (p+1, q-1, success, format);
	}
	if (tokens[p].type == NEG) {
		return -eval(p+1,q,success, format);
	}
	if (tokens[p].type == DEREF) {
		return swaddr_read( eval(p+1,q,success, format), 4);
	}
	if (tokens[p].type == NOT) {
		return !(eval(q,q,success, format));
	}

	int index = dominant_operator(p, q);
	// printf ("index == %d\n", index);
	//printf ("p == %d  ", p);
	//printf ("q == %d  ", q);
	int op = tokens[index].type;
	//printf ("op == %c\n", op);

	uint32_t v1 = eval(p, index-1, success, format);
	//printf ("v1 == %d\n", v1);
	uint32_t v2 = eval(index+1, q, success, format);
	//printf ("v2 == %d\n", v2);
	switch (op)
	{
	case '+':   return v1 + v2;
	case '-':   return v1 - v2;
	case '*':   return v1 * v2;
	case '/':   return v1 / v2;
	case EQ :   return v1 == v2;
	case AND:   return v1 && v2;
	case OR :   return v1 || v2;
	case NEQ:   return v1 != v2;
	default:
		assert(0);
	}

}

uint32_t expr(char *e, bool *success, int* format) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int i;
	for (i = 0 ; i < nr_token; i++) {
		if (tokens[i].type == '*' && (i == 0 || (tokens[i-1].type !=NUMBER && tokens[i-1].type != ')' ))) {
			tokens[i].type = DEREF;
		}
		if (tokens[i].type == '-' && (i == 0 || (tokens[i-1].type !=NUMBER && tokens[i-1].type != ')' ))) {
			tokens[i].type = NEG;
		}
	}
	if (tokens[0].type == REG || tokens[0].type == SYMBOL) {
		*format = 1;
	}
	/* TODO: Insert codes to evaluate the expression. */
	return eval(0, nr_token-1, success, format); // call this recursive funcition to compute the answer
	// panic("please implement me");
	// return 0;
}

