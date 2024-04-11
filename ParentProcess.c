/*
project: 02
author: Shianne Marbley
email: smarble2@umbc.edu
student id: MT91337
description: ParentProcess forks 10 child processes that create 0-9 likesservers
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void startLogFiles(char serverNum[]);
void endLogFiles(char serverNum[],int status);


int main() {
  //ParentProcess forks 10 child processes
  //after each fork indicate that child process has started in log
  //each child is made 1 second after the previous child
  //each child process will live for 5 minutes
   //ParentProcess forks 10 child processes to call ./LikesServer
  int MAX_CHILD = 10;
  //array of child process for return code logging
  int forkArray[MAX_CHILD];


  int ii = 0;
  int jj = 0;
  for(ii = 0; ii < MAX_CHILD; ii++){
    pid_t LikesServer = fork(); //envoke fork call
    //process_id < 0 failed
    if(LikesServer < 0){
        printf("fork failed \n");
        exit(1);
    }
    //process_id = 0 child process success
    else if(LikesServer == 0){
      
      // call execvp to start each LikesServer
      // char LikesServer[] = "./LikesServer";
       char serverNum[20]; //each LikesServer number
      //convert int to string
      sprintf(serverNum,"%d",ii);
      //indicate that the child process has started                           
      startLogFiles(serverNum);
      //Build cmd for execvp
      char *cmd[]= {"./LikesServer",serverNum,NULL};
    
      //start the LikesServers
      if(execvp(cmd[0],cmd) == -1){
        //*********might need to log  if an execvp fails*********
        printf("[%d] execvp failed \n", ii);
        exit(1);
      }
     }
      //always store the child process # whether fail/success
      forkArray[ii] = LikesServer;
      //wait 1 sec before  creating next child
      sleep(1);
  }
  //use collected child process #'s to see exit status
  for(jj = 0; jj < MAX_CHILD; jj++){
    int status;
    waitpid(forkArray[jj], &status,0);
    if(WIFEXITED(status)){
    
      char serverNum[20];//each LikesServer number
      //convert jj to string
      sprintf(serverNum,"%d",jj);
      
      int forkStatus = WEXITSTATUS(status); //exit status of child
      
      //call log end fxn
      endLogFiles(serverNum,forkStatus);
      //printf("Child process #%d exited with status %d\n", jj, forkStatus);
    }
  }
  return 0;
}
void startLogFiles(char serverNum[])
{
  time_t current = time(NULL); //curr time
  struct tm *currTime = localtime(&current);
  char start[] = " started  ";
  char newEntry[1024] = "Child";

  //build the start message
 
  strcat(newEntry,serverNum);
  strcat(newEntry,start);
  strcat(newEntry,asctime(currTime));


  //open file in append mode b/c write will delete contents everytime called
  FILE *log = fopen("/tmp/ParentProcessStatus","a");
  if(log == NULL){
      perror("error opening /tmp/ParentProcessStatus\n");
  }
  else{
      //log entry
    if(fputs(newEntry,log) >= 0){
      printf("%s",newEntry);     
    }
    else{
      printf("Error Logging \n");
    }
  }
 fclose(log);

}


void endLogFiles(char serverNum[],int status){
 time_t current = time(NULL); //curr time
 struct tm *currTime = localtime(&current);
 
 char end[] = " ended with status ";
 char newEntry[256] = "Child";
 char stat[8]; //will hold str of exit status
 char s[]="  ";

 sprintf(stat,"%d",status); //convert exit status to string

 //build the ended message
 strcat(newEntry,serverNum);
 strcat(newEntry,end);
 strcat(newEntry,stat);
 strcat(newEntry,s);
 strcat(newEntry,asctime(currTime));
 

 //open file in append mode b/c write will delete contents everytime called
 FILE *log = fopen("/tmp/ParentProcessStatus","a");
 if(log == NULL){
     perror("error opening /tmp/ParentProcessStatus\n");
 }
 else{
     //log entry
   if(fputs(newEntry,log) >= 0){
     printf("%s",newEntry);     
   }
   else{
     printf("Error Logging \n");
   }
 }
fclose(log);
 }


