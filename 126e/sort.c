#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare(const void *a, const void *b)
{
	return *(const int *) a > *(const int *) b;
}

int main(int argc, char **argv)
{
	int i;
	int A[] = { 692, 1, 32, };
	int B[] = { 0, 0, 0, 14, 15, 123, 2431, };

	memcpy(B, A, sizeof(A));
	qsort(&B[0], sizeof(B)/sizeof(B[0]), sizeof(int), compare);

	for (i = 0; i < sizeof(B)/sizeof(B[0]); i++)
		printf("%d ", B[i]);

	putchar('\n');
	return (0);
}
