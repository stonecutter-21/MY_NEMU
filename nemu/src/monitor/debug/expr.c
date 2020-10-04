#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUMBER, 

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
	{"-", '-'},                     // subtract
	{"\\*", '*'},                   // multiply
	{"/", '/'},                     // divide
	{"\\(", '('},                   // (
	{"\\)", ')'},                   // )
	{"[0-9]+", NUMBER},             // numbers
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
					case '+':
					  tokens[nr_token].type = '+';
					  nr_token ++;
					  break;
					case '-':
					  tokens[nr_token].type = '-';
					  nr_token ++;
					  break;
					case '*':
					  tokens[nr_token].type = '*';
					  nr_token ++;
					  break;
					case '/':
					  tokens[nr_token].type = '/';
					  nr_token ++;
					  break;
					case '(':
					  tokens[nr_token].type = '(';
					  nr_token ++;
					  break;
					case ')':
					  tokens[nr_token].type = ')';
					  nr_token ++;
					  break;
					case NUMBER:
					  tokens[nr_token].type = NUMBER;
					  strncpy(tokens[nr_token].str, substr_start,substr_len);
					  nr_token ++;
					  break;
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
	int first_m_d = q;  // for the case that there is no '+' or '/'
	int count = 0; // count the first time of '*' or '/' appears
	int i;
    int flag = 0; // we think there is no parenthese at first
	for (i = q; i >= p; i--) {
		// if there is a right parenthese, just escape these until meet a left parenthese
		
		char now = tokens[i].type;
		// printf("%c\n", now);
		switch (now)
		{
		case ')':
		    flag ++;
			break;
		case '(':
		    flag --;
			break;
		case '+':
		    if (flag == 0) {
				return i;  // if is this case, it must be this one
			}
			break;
		case '-':
		// it is a little special -- to judge if it is negative
		    if (flag == 0) {
				// if is normal .such as '1-1' or '(1+1)-2'
				if (tokens[i-1].type == NUMBER || tokens[i-1].type ==')') {
					return i;
				}
				// if is negative, just ignore it
			}
			break;
		case '*':
		case '/':
		    if (flag == 0) {
			   count ++;
			   // do the record
			   if (count == 1) {
				   first_m_d = i;
			    //printf("first_m_d == %d\n", first_m_d);
			    }
			}
			break;
		default:
			break;
		}
	}
	return first_m_d;
}

uint32_t eval(int p, int q, bool *success) {
	if (p > q) {
		//printf ("here in p > q?\n");
		*success = false;
		return -1;
		// it means the expression is wrong...
	}
	else if (p == q) {
		if (tokens[p].type == NUMBER) {
			int answer;
			sscanf(tokens[p].str,"%d",&answer);
			return answer;
		}
		else {
			//printf ("here in p == q?\n");
			*success = false;
			return -1;
		}
	}

	else if (check_parentheses(p ,q) == true) {

		return eval (p+1, q-1, success);
	}
	// specially deal with the case like '-1'  or '- (1+1)'
	else if (tokens[p].type == '-' && tokens[p+1].type == '(') {
		return -(eval(p+1,q,success));
	}
	else {
		int index = dominant_operator(p, q);
		// printf ("p == %d  ", p);
		// printf ("q == %d  ", q);
		char op = tokens[index].type;
	    //printf ("op == %c\n", op);

		uint32_t v1 = eval(p, index-1, success);
		//printf ("v1 == %d\n", v1);
		uint32_t v2 = eval(index+1, q, success);
		//printf ("v2 == %d\n", v2);
		switch (op)
		{
		case '+':   return v1 + v2;
		case '-':   return v1 - v2;
		case '*':   return v1 * v2;
		case '/':   return v1 / v2;
		default:
			assert(0);
		}
	}
}



uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	return eval(0, nr_token-1, success); // call this recursive funcition to compute the answer
	panic("please implement me");
	return 0;
}

