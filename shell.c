#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80
#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"


int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}
void tryCommand(tok_t arg[])
{
  tok_t* paths=getToks(getenv("PATH"));
  char* testString=arg[0];
  char **stringPaths=paths;
  int test=execl(arg[0],arg[0],arg[1],NULL);
  if (test!=-1)
    {
      printf("This shell supports the commands in the table\n");
    }
  else
    {
      int i;
      for (i=0;i<30;i++)
	{
	  char *runnable=strcat(stringPaths[i],"/");
	  char *runnable2=strcat(stringPaths[i],arg[0]);
	  printf("%s\n",runnable2);
	  
	}
    }
  
 
} 
int cmd_help(tok_t arg[]);
int printDirectory()
{
  char directory[INPUT_STRING_SIZE];
  printf("%s ",getcwd(directory,INPUT_STRING_SIZE));
  return 0;
}
int cmd_cd(tok_t arg[])
{
  char *names[100];
  const char *fileNames;
  char directory[INPUT_STRING_SIZE];
  getcwd(directory,INPUT_STRING_SIZE);
  if (strcmp(arg[0],"..")==0)
    {
      chdir("..");
      
    }
  else
    {

	 
	
  fileNames=strtok(arg[0],"/");
  while (fileNames != NULL)
    {
      if (chdir(fileNames)==-1)
	{
	  printf("%s does not exist.\n",fileNames);
	  printDirectory();
	  return 0;
	}
	
      else
	{
	  fileNames=strtok(NULL,"/");
	}
    }
	
 
    }
    
      getcwd(directory,INPUT_STRING_SIZE);
      printDirectory();
      return 1;
  
}
/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_cd,"cd","changes directory"},
  /*{cmd_ls,"ls","lists all the subdirectories of the current directory"},*/
};

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
  /** YOUR CODE HERE */
  return NULL;
}



int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
  tok_t *t;			/* tokens parsed from input */
  int lineNum = 0;
  int fundex = -1;
  pid_t pid = getpid();		/* get current processes PID */
  pid_t ppid = getppid();	/* get parents PID */
  pid_t cpid, tcpid, cpgid;
  int status;
  init_shell();

  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

  lineNum=0;
  printDirectory();
  fprintf(stdout, "%d: ", lineNum);
  while ((s = freadln(stdin))){
    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if(fundex >= 0)
      {
      cmd_table[fundex].fun(&t[1]);
      }
	
    else {

      //This space is reserved for forking test
      pid_t cpid=fork();
      if (cpid==0)
	{
	  
	  tryCommand(t);
	}
      else
	{
	  tcpid=wait(&status);
	  while (tcpid!=cpid)
	    {
	      tcpid=wait(&status);
	      if (tcpid==cpid)
		{
		  printf("Child was terminated\n");
		}
	    }
	      
	      //fprintf(stdout, "This shell only supports built-ins.\n");
      printDirectory();
      //fprintf(stdout, "%d: ", lineNum++);
	}
       
    }
  };
}
  
