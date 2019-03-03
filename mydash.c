#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "version.h"
#include "mydash.h"
#include "list.h"
#include "common.h"
#include <errno.h>
#include <signal.h>
#include "jobs.h"
#include "mydash.h"

int main(int argc, char **argv)
{
        int i;
        char    *currLine;
        char **tokens;
        int numTokens = 0;
        void *list = NULL;  
        char *delimiter = " ;";
        /*Initialize history*/
        HIST_ENTRY **myhistory;
        using_history();
        handleArgs(argc,argv);
        vector_alloc(&list);
        /*Setup Signals*/
        signal(SIGTTOU,SIG_IGN);
        signal(SIGINT,sigintHandler);
        signal(SIGTSTP,sigstopHandler);
        /*Prompt user for input*/
        while ((currLine=readline(getPrompt()))) {
                add_history(currLine);
                myhistory = history_list();
                jobStatus(list,1);
                tokens = parseInput(currLine, delimiter, &numTokens);
                if(numTokens>0){
                        if(strcmp("history",tokens[0])==0){
                                if(myhistory)
                                        for (i=0;myhistory[i];i++)
                                                fprintf(stderr,"%d: %s\n",i+history_base,myhistory[i]->line); 
                        }
                        else
                        handleCommand(tokens,numTokens,list,currLine);
                }
                freeToken(tokens,numTokens);
        }
        exit(0);
}

char *getPrompt(void){
        char    *prompt;
        prompt = getenv("DASH_PROMPT");
        if(prompt==NULL){
                prompt = "mydash:";
        }
        return prompt;
}

void handleCommand(char **tokens,int numTokens, char *list, char *currLine){
        char *lastToken;
        lastToken = tokens[numTokens-1];
        /*Get job status*/
        if(strcmp("jobs",tokens[0])==0){
                jobStatus(list,0);
        }
        /*Start a background job*/
        else if(strcmp("bg",tokens[0])==0){
                if(tokens[1])
                        bgCmd(list,atoi(tokens[1]));
                else
                        bgCmd(list,-1);
        }
        /*Start a foreground job*/
        else if(strcmp("fg",tokens[0])==0){
                if(tokens[1])
                        fgCmd(list,atoi(tokens[1]));
                else
                        fgCmd(list,-1);
        }
        else if(strcmp("cd",tokens[0])==0){
                changeDir(tokens[1],numTokens);
        }
        else if(strcmp("exit",tokens[0])==0){
                freeJobs(list);
                freeToken(tokens,numTokens);
                free(currLine);
                exit(0);
        }
        /*Execute command*/
        else{
                if(strcmp(lastToken,"&")==0){
                        execute(tokens[0],tokens,1,list);
                        strcpy(tokens[numTokens-1],"");
                }
                else if(strcmp(lastToken+(strlen(lastToken)-1),"&")==0){
                        memmove(tokens[numTokens-1]+(strlen(lastToken)-1),"",1);
                        execute(tokens[0],tokens,1,list);
                }
                else{
                        execute(tokens[0],tokens,0,list);
                }
        }
}

char **parseInput(char *s, char *delimiter, int *numTokens)
{   
        char *nextToken;
        char **token = (char **) calloc (MAX_TOKENS+1,sizeof(char *));
        /* tokenize the string s */
        nextToken = strtok(s, delimiter);
        *numTokens = 0;
        while (nextToken != NULL) {
                token[*numTokens] = (char *) calloc( strlen(nextToken)+1, sizeof(char));
                strcpy(token[*numTokens], nextToken);
                (*numTokens)++;
                nextToken = strtok(NULL, delimiter);
        }
        /*Now the tokens are copied into token[0..numTokens-1];*/
        return token;
}

void freeToken(char **token, int numTokens)
{
        int i;
        for(i=0;i<numTokens;i++){
                free(token[i]);
        }
        free(token);
}

void changeDir(char *directory,int numTokens){    
        uid_t   user;
        struct passwd uInfo;
        struct passwd *structPointer;
        char start[MAXLINE];
        char end[MAXLINE];
        if(getcwd(start,sizeof(start))==NULL){
                fprintf(stderr,"getcwd() error");
        }
        /*Goes home*/
        if(numTokens==1){
                user = getuid();
                structPointer = getpwuid(user);
                uInfo = *structPointer;
                chdir(uInfo.pw_dir);
        }
        else{
                chdir(directory);
        }
        if(getcwd(end,sizeof(end))!=NULL){
                if(strcmp(start,end)==0 && strcmp(directory,".")!=0)
                        fprintf(stderr,"failed to cd to target: %s\n",directory);
        }
        else{
                fprintf(stderr,"getcwd() error");
        }
}                        
void handleArgs(int argc, char **argv){
        if(argc>2){
                fprintf(stderr,"invalid usage:use ./mydash or ./mydash -v for version\n");
                exit(0);
        }
        if(argc>1){
                if(strcmp(argv[1],"-v")==0)
                        fprintf(stderr,"Version 1:Revision %s User: %s\n",version(),getlogin());
                exit(0);
        }
}
void sigintHandler(int sig_num){
        signal(sig_num,sigintHandler);
        printf("\n%s",getPrompt());
}

void sigstopHandler(int sig_num){
        signal(sig_num,sigstopHandler);
        printf("\n%s",getPrompt());
}

