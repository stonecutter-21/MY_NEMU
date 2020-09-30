#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_scan_mem(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "The program executes N instructions in a single step and then pauses. When N is not given, the default is 1", cmd_si},
	{ "info", "'-r',print register state. '-w', print monitor point information", cmd_info},
	{ "x","'x N EXPR', find the expression EXPR", cmd_scan_mem},

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

// my code: cmd_si[N]
static int cmd_si(char *args) {
	char *arg = strtok(args, " "); // it should be a string of number, such as 1 or 2
	uint32_t count_number;
	if (arg == NULL) {
		count_number = 1;
	}
	else{
		sscanf(arg,"%d",&count_number);
	}
	// i don't know why, if number is bigger than 9, it does excecute, but doesn't show anything...
	cpu_exec(count_number);
	// printf("test: the number is:%d\n",count_number);
	return 0;
}

// my code: cmd_info()
static int cmd_info(char *args) {
	char *arg = strtok(NULL, " ");
	if (arg == NULL){
		printf("too few arguments. type 'help' for more detiles\n" );
		return 0;
	}
	if (*arg == 'r'){
		printf("eax: 0x%x\n",cpu.eax);
	    printf("ecx: 0x%x\n",cpu.ecx);
	    printf("edx: 0x%x\n",cpu.edx);
	    printf("ebx: 0x%x\n",cpu.ebx);
	    printf("esp: 0x%x\n",cpu.esp);
	    printf("ebp: 0x%x\n",cpu.ebp);
	    printf("esi: 0x%x\n",cpu.esi);
	    printf("edi: 0x%x\n",cpu.edi);
	}
	return 0;	
}

static int cmd_scan_mem(char *args) {
	char *arg1 = strtok(NULL, " "); // first arg -- number 
	char *arg2 = strtok (NULL, " "); // second arg -- addr
	uint32_t N_temp;  // the 'N'
	uint32_t EX_temp;  // the 'N'

	if (arg1 == NULL){
		printf("Too few arguments. Type \"help\" for more infomations\n");
		return 0;
	}
	if (arg2 == NULL){
		printf("Too few arguments. Type \"help\" for more infomations\n");
		return 0;
	}
	sscanf(arg1,"%d",&N_temp);
	sscanf(arg2,"%x",&EX_temp);
	int i ;
	uint32_t addr;
	for(i = 0; i< N_temp; i++) {
		addr = swaddr_read(EX_temp + i, 1);
		printf("%x: %2x\n",EX_temp+i,addr);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
