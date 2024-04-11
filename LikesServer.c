/*
project: 02
author: Shianne Marbley
email: smarble2@umbc.edu
student id: MT91337
description: LikesServer creates rand num of likes and sends to PLS
then sleeps for a rand num of seconds and repeats for 5 minutes
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>


void logLikesServer(char serverNum[],char message[],int status);


int main(int argc, char *argv[]) {
  if(argc > 2){
      perror("argc error \n");
      return 1;
  }
  //each child process will live for 5 minutes
  //generate a random number 0-42
  //save number as var (likecount)
  //use rand 1-5 of seconds to notify the PrimaryLikesServer
  //first create and connecto to socket
   int client_socket; //create the socket
  struct sockaddr_in likes_server; //specify address for the socket
  //make socket()
  client_socket = socket(AF_INET, SOCK_STREAM, 0); //predefined,TCP socket,TCP
  if (client_socket == -1) {
      perror("Error creating socket");
      exit(1);
  }
  int reuse = 1;
  if(setsockopt(client_socket,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse)) == -1){
      perror("Reuse error \n");
      exit(1);
  }




  likes_server.sin_family = AF_INET;
  likes_server.sin_addr.s_addr = INADDR_ANY;//connect to any local host on this machine
  likes_server.sin_port = htons(1024); //in slides 1024 and below are well known and std services




  if (connect(client_socket, (struct sockaddr*)&likes_server, sizeof(likes_server)) == -1) {
      perror("Error connecting to PrimaryLikesServer");
      close(client_socket);
      exit(1);
  }
  //covert passed arg of child num for message
  int serverNum = atoi(argv[1]); //argv[1] is child number from execvp call ParentProcess
   time_t toStart = time(NULL);
  int fiveMin = 300; // 300s = 5 min
   while(1){
      if(time(NULL) - toStart >= fiveMin){
          break;
      }
    
      char message[1024]; // Allocate a larger buffer for the message
      char myTab[] = " ";
      char name[] = "LikesServer";
      char myNum[20];
      int likeCount = 42;
      char likes[40];
      int sec = 0;
      srand(time(0)); //for rand nums
      //use rand 1-5 of wait to notify the PrimaryLikesServer
      sec = rand() % (5 + 1 - 0) + 0;
      sleep(sec);
      //generate a random number 0-42 save number as var (likecount)
      likeCount = rand() % (42 + 1 - 1) + 1; //interval
      //convert ints to strings
      sprintf(myNum,"%d",serverNum);
      sprintf(likes,"%d",likeCount);
      strcpy(message, name); // Initialize message
      strcat(message, myNum);
      strcat(message, myTab);
      strcat(message, likes);
    
      //send message
      int IPSreturn = send(client_socket, message, strlen(message), 0);
      if(IPSreturn == -1){
          //log it
          logLikesServer(myNum,message,IPSreturn);
          perror("Error sending message.\n");
          close(client_socket);
          exit(1);
      }
      //log it
      logLikesServer(myNum,message,IPSreturn);


  }
  //close socket when done
  close(client_socket);
  exit(0);
 }




void logLikesServer(char serverNum[],char message[],int status){
  if(status >= 0){ status = 0;} //any successful sends will be noted w 0
  else{status = -1;}
 
  //build filename first
  char stat[8];
  char space[] = "  ";
  char nl[] = " \n";
  char filename[256] = "/tmp/LikesServer"; //begining of all LikesServer files are the same
  strcat(filename,serverNum); //attach server num to end of filename




  //add return status of IPS to end of the original LikesServer message
  sprintf(stat,"%d",status);
  strcat(message,space); //add a space before status
  strcat(message,stat);
  strcat(message,nl);
  //log example : LikesServer0 14  0




 //open file in append mode b/c write will delete contents everytime called
 FILE *log = fopen(filename,"a");




 if(log == NULL){
  printf("error opening %s\n",filename);
 }
 else{
  //log entry
  if(fputs(message,log) >= 0){
      //printf("LOGGED: %s",message);  
  }
  else{
      printf("Error Logging \n");
  }
 }
 fclose(log);
}



