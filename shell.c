#include "shell.h"

// updates given buffer with the current working directory
void getCwd(char *buf, size_t size)
{
	if (getcwd(buf,size) == NULL){
		perror("can't get cwd");
	}
}

char *read_line(void)
{
	char *buf;
	size_t buf_size;
	// curr working dir buffer
	char cwd[BUFSIZ];

	getCwd(cwd, sizeof(cwd));

	p("%s$>", cwd);
	if (getline(&buf, &buf_size, stdin) == -1){
		buf = NULL;
		p("ERROR OCCURED / EOL");
		p("%s\n", buf);
	}
	return buf;
}

int main(){
	char *line;
	while(1){
		line = read_line();
		p("%s\n", line);
	}

	printf("Hello world\n");
	return 0;
}
