/* http://www.reddit.com/r/dailyprogrammer/1euacb/ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INSTRUCTIONS 100000
static int INDEX; /* next line to read from in file */

int Cand(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return -1;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg1] & reg[arg2];

	return 1;
}

int Cor(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return -1;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg1] | reg[arg2];

	return 1;
}

int Cxor(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return -1;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg1] ^ reg[arg2];

	return 1;
}

int Cnot(char *reg, int num_args, int *args)
{
	int arg1;
	if (num_args != 1)
		return -1;

	arg1 = args[0];
	reg[arg1] = ~(reg[arg1]);

	return 1;
}

int Cmov(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return -1;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = reg[arg2];

	return 1;
}

int Cset(char *reg, int num_args, int *args)
{
	int arg1, arg2;
	if (num_args != 2)
		return -1;

	arg1 = args[0];
	arg2 = args[1];
	reg[arg1] = arg2;

	return 1;
}

int Crand(char *reg, int num_args, int *args)
{
	int arg1;
	if (num_args != 1)
		return -1;

	arg1 = args[0];
	reg[arg1] = rand() % 1;

	return 1;
}

/* JMP to INDEX args[0] */
int Cjmp(char *reg, int num_args, int *args)
{
	if (num_args != 1)
		return -1;

	INDEX = args[0];
	return 1;
}

/* if args[1] == 0, JMP to INDEX args[0] */
int Cjz(char *reg, int num_args, int *args)
{
	if (num_args != 2)
		return -1;

	if (reg[args[1]] == 0)
		INDEX = args[0];

	return 1;
}

/* Do nothing, return halt signal */
int Chalt(char *reg, int num_args, int *args)
{
	return 0;
}

struct cmd_table_t {
	char *cmd;
	int (*fcn)(char *, int, int *);
} cmd_table[] = {
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

#if 0
char *fileToBuf(char *filename)
{
	FILE *in;
	char *buf;
	int length = 0;

	in = fopen(filename, "rb");
	fseek(in, 0, SEEK_END);

	/* Get lenght of file */
	length = ftell(in);

	/* rewind to beginning of file */
	fseek(in, 0, SEEK_SET);

	/* Allocate enough memory for entire file */
	buf = calloc(length, sizeof(char));

	/* Read file into buffer */
	fread(buf, sizeof(char), length, in);
	fclose(in);

	return buf;
}
#endif

int main(int argc, char **argv)
{
	char *buf;
	int st, i, instructions;

	char lbuf[16];
	int num_args, args[2];

	/* 32 1-bit registers */
	char reg[4] = { 0x00, 0x00, 0x00, 0x00 };

	srand(time(NULL));
	buf = fileToBuf("halt.asm");

	do {
		/* read in the next INDEX line from buffer */
		num_args = sscanf(buf[INDEX], "%s %d %d ", lbuf, &args[0], &args[1]);

		/* Find which command to execute */
		for (i = 0; cmd_table[i].cmd != NULL; i++)
			if (strstr(cmd_table[i].cmd, lbuf) == cmd_table[i].cmd)
				break;

		if (cmd_table[i].cmd == NULL) {
			fprintf(stderr, "Invalid command '%s' found.\n", lbuf);
			continue;
		}

		/* on next read start at following line, unless executing a JMP or JZ
		 * instruction, in which case those are handeled automatically. */
		instructions++, INDEX++;
		st = cmd_table[i].fcn(reg, num_args, &args[0]);

		if (instructions >= MAX_INSTRUCTIONS) {
			fprintf(stderr, "Program does not halt.\n");
			exit(1);
		}

		/* halt signal receieved */
		if (st == 0)
			break;
		else if (st == -1) /* error recieved */
			fprintf(stderr, "line %d: Unexpected number of arguments: '%s'",
					INDEX, lbuf);

	} while(1);

	free(buf);
	printf("Program halts! %d instructions executed!\n", instructions);

	return (0);
}
