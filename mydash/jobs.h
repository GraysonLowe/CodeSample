#ifndef	__JOBS_H
#define	__JOBS_H

/*Used to keep track of a job*/
struct job{
        pid_t id;
        char *cmd;
        char *status;
        int num;
};

/*moves a job into the foreground*/
void foreground(struct job *j);
/*frees all of the tokens in a token array along with the array itself*/
void freeToken(char **token, int numTokens);
/*uses execvp to execute the cmd with the given params*/
void execute(const char *cmd, char *const params[],int bg,char *line);
void freeJobs(char *list);
/*defines kill for compiler complaint*/
int kill(pid_t pid, int sig);
/*handles background command*/
void bgCmd(char *list, int process);
/*handles foreground command*/
void fgCmd(char *list, int process);
/*prints out the status of all jobs with a boolean justDone for only Done jobs*/
void jobStatus(void *list,int justDone);
void freeJob(struct job *j);

#endif	
