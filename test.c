#include <stdio.h>
#include <string.h>

void tok_test(char **argv);

int main(int argc, char **argv)
{
	printf("hello world\n");
	return 0;
}

void tok_test(char **argv)
{
	char *str, *token;
	int j;
	const char *delim = "e";

	str = argv[1];
	token = strtok(str, delim);
	printf("%s\n", token);
	printf("%d\n", strcmp(token, "h"));
	while (token = strtok(NULL, delim)) {
		printf("%s\n", token);
	}
}
