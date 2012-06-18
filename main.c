#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>
int main(int argc, char ** args) {
  int lineSize = 0;
  char * command = 0;
  char * commandPtr = NULL;
  for (int i = 1; i < argc; i++) {
    lineSize += strlen(args[i]) + 1; //the + 1 is for the space
  }
  if (lineSize != 0) {
    commandPtr = command = malloc(lineSize);
    commandPtr--;
    for (int i = 1; i < argc; i++) {
      strcpy(++commandPtr,args[i]);
      commandPtr += strlen(args[i]); 
      *commandPtr = ' ';
    }
    *commandPtr = '\0';
    char inputChar;
    pid_t forked;
    int exitStatus = 0;
    int state = 0;
    do { 
      forked = fork();
      waitpid(forked,&state,0);
    } while (forked != 0 && (WEXITSTATUS(state) == 0));
    FILE * pipe = 0;
    if (forked == 0){
      pipe = popen(command,"w");
   
      while ((inputChar = getchar()) != EOF) {
	fputc(inputChar,pipe);
	if (inputChar == '\n') {
	  fputc(4,pipe);
	  fputc(4,pipe);
	  pclose(pipe);
	  exit(0);
	}
      }
      pclose(pipe);
      exit(1);
    }
    
  }
}
