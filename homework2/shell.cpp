/*
 * Jim Blaney
 * CS464
 * Spring 2014
 * Homework 2
 * Implementation of a stripped-down shell
 */
 
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

const int MAX_LENGTH = 256;

void type_prompt();
void read_command(char*, char**);

int main() {
  char  *command    = new char[MAX_LENGTH];
  char **parameters = new char*[MAX_LENGTH];
  int    status     = 0;

  // display the banner
  cout << "Welcome to JimShell" << endl;
  cout << "ctrl-c or ctrl-\\ to quit" << endl;

  while (true) {
    type_prompt();
    read_command(command, parameters);

    if (fork() != 0) {
      waitpid(-1, &status, 0);
    } else {
      // using execvp instead of execve so that the path is resolved automatically
      execvp(command, parameters); 
    }
  }

  return 0;
}

void type_prompt() {
  cout << "JimShell> ";
}

void read_command(char* command, char** parameters) {
  char *tmp   = new char[MAX_LENGTH];
  char *token;
  int   count = 0;
  int   i     = 0;
  
  // set all parameters to null so that commands without parameters 
  // following commands with parameters do not receive the old parameters
  for (i = 0; i < MAX_LENGTH; i++) {
    parameters[i] = NULL;
  }
  
  // read the user's input
  cin.getline(tmp, MAX_LENGTH - 1);

  // parse the user's input into the command and parameters variables
  token = strtok(tmp, " ");
  while (token != NULL) {
    // copy the argument into the parameters array
    parameters[count] = new char[MAX_LENGTH];
    strcpy(parameters[count], token);
    token = strtok(NULL, " ");
    
    // make sure we don't overflow the parameters array
    count++;
    if (count == MAX_LENGTH - 1) {
      break;
    }
  }
  
  // if no command was provided, provide a message to the user
  if (count == 0) {
    parameters[0] = new char[MAX_LENGTH];
    strcpy(parameters[0], "echo");
    parameters[1] = new char[MAX_LENGTH];
    strcpy(parameters[1], "No input received");
  }
  
  // copy the first parameter to the command
  strcpy(command, parameters[0]);
}
