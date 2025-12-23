#include "shell.h"

// updates given buffer with the current working directory
void getCwd(char *buf, size_t size)
{
	if (getcwd(buf,size) == NULL){
		perror("can't get cwd");
	}
}

bool startsWith(const char *a, const char *b)
{
	return strncmp(a,b,strlen(b)) == 0;
}

void replaceHomeDir(char *cwd, size_t size, char *homeDir)
{
	if (homeDir == NULL){
		return;
	}
	int homeDirLen = strlen(homeDir);

	// home directory can be substituted with ~
	if (startsWith(cwd,homeDir) &&
	    (cwd[homeDirLen] == '/' || cwd[homeDirLen] == '\0')){
		char temp[BUFSIZ];
		/*
		 * Updates temp instead of printing.
		 * Formatter adds an ~ at the start of the modified string
		 * i.e. cwd + home directory (pointer at the / after homeDir)
		 */
		snprintf(temp, sizeof(temp), "~%s", cwd + homeDirLen);

		strncpy(cwd, temp, size-1);
		cwd[size-1] = "\0";
	}else{
		p("debug");
	}
}

char *read_line(void)
{
	char *buf;
	size_t buf_size;
	// curr working dir buffer
	char cwd[BUFSIZ];

	char *homeDir = getenv("HOME");
	getCwd(cwd, sizeof(cwd));
	replaceHomeDir(cwd,sizeof(cwd),homeDir);

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
		if (strcmp(line,"exit\n") == 0){
			p("Exiting!\n");
			break;
		}
		p("%s", line);
	}

	return 0;
}
