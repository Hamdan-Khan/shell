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
	char *buf = NULL;
	size_t buf_size = 0;
	// curr working dir buffer
	char cwd[BUFSIZ];

	char *homeDir = getenv("HOME");
	getCwd(cwd, sizeof(cwd));
	replaceHomeDir(cwd,sizeof(cwd),homeDir);

	p(C"%s$> "RST, cwd);
	if (getline(&buf, &buf_size, stdin) == -1){
		free(buf);
		return NULL;
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

void executeCommand(char **tokens)
{
	// for builtin commands:
	int i = 0;
	char *curr;
	while ((curr = builtinCmds[i].cmdName)){
		if (strcmp(curr, tokens[0]) == 0){
			builtinCmds[i].cmd(tokens);
			return;
		}
		i++;
	}

	// for external commands:
	int pid = fork();
	switch (pid){
		// couldn't fork
		case -1:
			p(RED"An error occured before executing the command"RST);
			exit(EXIT_FAILURE);
		// succesfully forked, this branch is for child process
		case 0:
			execvp(tokens[0], tokens);
		// parent process branch, waits for child process to be executed
		default:
			wait();
	}
}

int main(){
	char *line;
	char **tokens;
	while(1){
		// READ
		line = read_line();
		if (line == NULL){
			break;
		}

		size_t lineSize = strlen(line);
		if (lineSize > 0 && line[lineSize-1] == '\n'){
			// removes trailing crlf
			line[lineSize-1] = '\0';
		}

		if (line[0] != '\0'){
			// EVAL & PRINT
			tokens = tokenize_input(line);
			if (tokens != NULL){
				executeCommand(tokens);
			}
			free(tokens);
		}

		free(line);
	}

	return 0;
}
