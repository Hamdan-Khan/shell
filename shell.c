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
		cwd[size-1] = '\0';
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

char **tokenize_input(char *input)
{
	size_t bufSize;
	char *savePtr;
	char **tokens;

	bufSize = BUFSIZ;
	tokens = malloc(bufSize * sizeof(char *));
	if (tokens == NULL) return NULL;

	int i = 0;
	tokens[i] = strtok_r(input, " ", &savePtr);
	while (tokens[i] != NULL){
		tokens[++i] = strtok_r(NULL, " ", &savePtr);
		// reallocate double memory if it exceeds the current buffer size
		if (i >= bufSize) {
			bufSize *= 2;
			tokens = realloc(tokens, bufSize * sizeof(char *));
		}
	}
	return tokens;
}

int main(){
	char *line;
	char **tokens;
	while(1){
		line = read_line();
		// ignores crlf at the end of the line
		line[strlen(line)-1] = '\0';

		if (strcmp(line,"exit") == 0){
			p("Exiting!\n");
			break;
		}
		tokens = tokenize_input(line);

		for (int i = 0; tokens[i]; i++){
			p("%s\n", tokens[i]);
		}

		free(line);
		free(tokens);
	}

	return 0;
}
