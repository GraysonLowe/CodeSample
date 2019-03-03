#ifndef	__MYDASH_H
#define	__MYDASH_H



/*gets the prompt for readline*/
char *getPrompt(void);
/*handles all commands except history*/
void handleCommand(char **tokens,int numTokens,char *list, char *currLine);
/*attempts to change to a specified directory*/
void changeDir(char *directory,int numTokens);    
/*handles the command line arguments*/
void handleArgs(int argc, char **argv);
/*takes in a char* and splits it into tokens on a delimter*/
char **parseInput(char *s, char *delimiter, int *numTokens);
void sigintHandler(int sig_num);
void sigstopHandler(int sig_num);
#endif	/* __MYDASH_H */
