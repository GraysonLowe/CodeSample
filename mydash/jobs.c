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


/*Print status of each job, justDone flag only prints out jobs that have finished since last call*/
void jobStatus(void *list,int justDone){
        struct job *j = NULL;
        int i;
        int w;
        int status;
        int size = vector_size(list);
        int removed = 0;


        for(i=0;i<size;i++){
                j = (struct job*)vector_get(list,i);
                if(strcmp(j->status,"Killed")==0){
                        erase_at(list,i);
                        freeJob(j);
                        i--;
                        size--;
                }
                if(strcmp(j->status,"Failed")==0){
                        erase_at(list,i);
                        freeJob(j);
                        i--;
                        size--; 
                }
                if(( w = waitpid(j->id,&status,WNOHANG|WCONTINUED))>0){
                        if(strcmp("Running",j->status)==0){
                                strcpy(j->status,"Done");
                                if(justDone){
                                        fprintf(stderr,"[%d]%s %s\n",j->num,j->status,j->cmd);
                                        erase_at(list,i);        
                                        freeJob(j);
                                        i--;
                                        size--;
                                        removed=1;
                                }
                        }
                        if(WIFCONTINUED(status)){
                                strcpy(j->status,"Running");
                        }
                }

                if(!justDone&&!removed){
                        if(strcmp(j->status,"Done")==0)
                        {
                                fprintf(stderr,"[%d]%s %s\n",j->num,j->status,j->cmd);
                                erase_at(list,i);
                                freeJob(j);
                                i--;
                                size--;
                        }
                        else
                                fprintf(stderr,"[%d]%d %s %s \n",j->num,j->id,j->status,j->cmd);
                }
        }
}

/*Executes a command*/
void  execute(const char *cmd, char *const params[],int bg,char *list){
        struct job *j = NULL;
        struct job *lastJob;
        pid_t   pid;

        /*Create a child*/
        if ( (pid = fork()) < 0)
                err_sys("fork error");
        else if (pid == 0) {    
                /*Create signals*/
                signal(SIGTTOU,SIG_IGN);
                signal(SIGINT,sigintHandler);
                signal(SIGTSTP,sigstopHandler);
                setpgid(0,0);
                
                execvp(cmd,params);
                err_ret("couldn't execute: %s", cmd);
                exit(-1);
        }

        /*Create job info*/
        setpgid(pid,pid);
        j = malloc(sizeof(struct job));
        j->cmd = calloc(strlen(history_get(history_length)->line)+1,sizeof(char));
        j->status = malloc(20);
        strcpy(j->status,"Running");
        j->id = pid;
        strcpy(j->cmd, history_get(history_length)->line); 

        if(vector_size(list)==0)
                j->num = 1;
        else{
                lastJob = vector_get(list,vector_size(list)-1);
                j->num = lastJob->num+1;
        }
        vector_add(list,j);
        /* parent waits if not background*/
        if(!bg){
                foreground(j);
        }
        else
                fprintf(stderr,"[%d]%d %s\n",j->num,j->id,j->cmd);
}

/*Brings a process to the foreground*/
void foreground(struct job *j){
        int status;
        pid_t w;
        w = j->id;
        tcsetpgrp(STDIN_FILENO,j->id);
        w  = waitpid(w,&status,WUNTRACED|WCONTINUED);
        tcsetpgrp(STDIN_FILENO,getpid());
        if(w==-1){
                if(errno==EINTR){
                        foreground(j);
                }
        }
        /*Handles signal interuptions*/
        else if(WIFEXITED(status)){
                strcpy(j->status,"Done");
                if(WEXITSTATUS(status)==255){
                        strcpy(j->status,"Failed");
                }

        }
        else if(WIFSIGNALED(status)){
                strcpy(j->status,"Killed");
                printf("\n");
        }
        else if(WIFSTOPPED(status)){
                strcpy(j->status, "Stopped");
                fprintf(stderr,"\n[%d] Stopped %s\n",j->num,j->cmd);
        }
        else if(WIFCONTINUED(status)){
                strcpy(j->status,"Resumed");
        }
}

/*Puts a job in the background*/
void bgCmd(char *list,int process){
        struct job *currJob = NULL;
        int i;
        int first = 0;
        if(process==-1){
                for(i=vector_size(list)-1;i>=0;i--){
                        currJob = (struct job*)vector_get(list,i);
                        if (strcmp("Stopped",currJob->status)==0&&!first){
                                kill(currJob->id,SIGCONT);
                                first=1;
                        }
                }
        }
        else{
                for(i=0;i<vector_size(list);i++){
                        currJob = (struct job*)vector_get(list,i);
                        if (strcmp("Stopped",currJob->status)==0&&process==currJob->num){
                                kill(currJob->id,SIGCONT);
                        }
                }
        }
}

/*Puts a job in the foreground*/
void fgCmd(char *list,int process){
        struct job *currJob = NULL;
        int i;
        int first = 0;
        if(process==-1){
                for(i=vector_size(list)-1;i>=0;i--){
                        currJob = (struct job*)vector_get(list,i);
                        if (strcmp("Stopped",currJob->status)==0&&!first){
                                kill(currJob->id,SIGCONT);
                                first=1;
                                foreground(currJob);
                                foreground(currJob);
                        }
                        else if (strcmp("Running",currJob->status)==0&&!first){
                                first=1;
                                foreground(currJob);
                        }
                }
        }
        else{
                for(i=0;i<vector_size(list);i++){
                        currJob = (struct job*)vector_get(list,i);
                        if (strcmp("Stopped",currJob->status)==0&&process==currJob->num){
                                kill(currJob->id,SIGCONT);
                                setpgid(currJob->id,getpid());
                                foreground(currJob);
                                foreground(currJob);
                        }
                        else if (strcmp("Running",currJob->status)==0&&process==currJob->num){
                                setpgid(currJob->id,0);
                                foreground(currJob);
                        }
                }
        }
}

void freeJob(struct job *j){
        free(j->status);
        free(j->cmd);
        free(j);
}

void freeJobs(char *list){
        struct job *j=NULL;
        while(vector_size(list)>0){
                j = (struct job*)vector_remove(list);
                freeJob(j);
        }
        vector_destroy(list);
}

