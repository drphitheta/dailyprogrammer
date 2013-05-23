/* http://www.reddit.com/r/dailyprogrammer/1euacb/ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Cand(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return 0;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg1] & reg[arg2];

	return 0;
}

int Cor(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return 0;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg1] | reg[arg2];

	return 0;
}

int Cxor(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return 0;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg1] ^ reg[arg2];

	return 0;
}

int Cnot(char *reg, int num_args, int *args)
{
	int arg1;
	if (num_args != 1)
		return 0;

	arg1 = args[0];
	reg[arg1] = ~(reg[arg1]);

	return 0;
}

int Cmov(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return 0;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg2];

	return 0;
}

int Cset(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return 0;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = arg2;

	return 0;
}

int Crand(char *reg, int num_args, int *args)
{
	int arg1;
	if (num_args != 1)
		return 0;

	arg1 = args[0];
	reg[arg1] = rand() % 1;

	return 0;
}

/* XXX */
int Cjmp(char *reg, int num_args, int *args)
{
	return 0;
}
/* XXX */
int Cjz(char *reg, int num_args, int *args)
{
	return 0;
}

int Chalt(char *reg, int num_args, int *args)
{
	return -1;
}

typedef struct cmd_table_s {
	char *cmd;
	int (*fcn)(char *, int, int *);
} cmd_table_t;

static cmd_table_t cmd_table[] = {
	{"AND", Cand},
	{"OR", Cor},
	{"XOR", Cxor},
	{"NOT", Cnot},
	{"MOV", Cmov},
	{"SET", Cset},
	{"RANDOM", Crand},
	{"JMP", Cjmp},
	{"JZ", Cjz},
	{"HALT", Chalt},
	{NULL, NULL},
};

int parseFile(char *filename)
{
	FILE *in = fopen(filename, "rb");
	int st; /* return value */
	int i;
	char lbuf[80]; /* current line */

	char *buf;
	char instr[16]; /* instruction */
	/* 'arguments' of the instructions */
	int num_args, args[2];
	int instructions = 0;

	/* 32 1-bit registers */
	char reg[32];
	memset(reg, 0x00, sizeof(reg)/sizeof(reg[0]));

	if (!in) {
		fprintf(stderr, "Unable to read file: %s\n", filename);
		return -1;
	}

	do {
		fgets(lbuf, sizeof(lbuf), in);
		num_args = sscanf(lbuf, " %s %d %d ", instr, &args[0], &args[1]);

		if (instr[0] == '\0')
			continue;

		printf("'%s' executed.\n", instr);

		for (i = 0; cmd_table[i].cmd != NULL; i++)
			if (strstr(cmd_table[i].cmd, buf) == cmd_table[i].cmd)
				break;

		if (cmd_table[i].cmd == NULL) {
			fprintf(stderr, "Invalid command '%s' found.\n", buf);
			continue;
		}

		st = cmd_table[i].fcn(reg, num_args, &args[0]);
		instructions++;

		if (instructions >= 100000)
			break;

		/* halt signal receieved */
		if (st == -1)
			break;

		/* jmp to a location in the file */
		if (st >= 1)
			;

	} while(1);

	return instructions;
}

int main(int argc, char **argv)
{
	char *data = "halt.asm";
	int instructions;

	if ((instructions = parseFile(data)) == -1)
		return 1;

	printf("Program halts! %d instructions executed!\n");

	return (0);
}
