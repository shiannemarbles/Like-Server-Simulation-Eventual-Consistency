/*
project: 02
author: Shianne Marbley
email: smarble2@umbc.edu
student id: MT91337
description:PrimaryLikesServer recieves likes sent by LikesServer0-9
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

int parse_string(char str[]);
void logLikesServer(char message[]);
void logTotalLikes(int total);


int main() {
//PrimaryLikesServer
//recieve data from a LikesServer "LikesServer0 23"
//respond to recieving data "Likes Recieved"
//note in log
int totalLikes = 0; //will hold total number of likes from all servers
 //create sockets
int server_socket;
int client_socket;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
//define socket() vals
server_socket = socket(AF_INET, SOCK_STREAM, 0); //predefined,TCP socket,TCP
if (server_socket == -1) {
    perror("Error creating socket");
    exit(1);
}
int reuse = 1;
if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse)) == -1){
    perror("Reuse error \n");
    exit(1);
}
//server_address vals
server_address.sin_family = AF_INET;
server_address.sin_addr.s_addr = INADDR_ANY;//connect to any local host on this machine
server_address.sin_port = htons(1024); //in slides 1024 and below are well known and std services
///bind socket to port
if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
    perror("Error binding");
    close(server_socket);
    close(client_socket);
    exit(1);
}
 //listen for connections
 if (listen(server_socket, 10) == -1) {
     perror("Error listening");
     close(server_socket);
     close(client_socket);
     exit(1);
 }
while(1){
    printf("PrimaryLikesServer listening...\n");
    //create int to hold client socket
    socklen_t length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &length);
     if (client_socket == -1) {
         perror("accepting fail \n");
         close(server_socket);
         exit(1);
     }
    //recieve the data and store in r_m
    char recieved_message[1024]; //used for printing confirmation


    while(recv(client_socket, recieved_message, sizeof(recieved_message), 0) > 0){


         //log the message recieved
         logLikesServer(recieved_message);


         //after likes are recieved send to be parsed and added to running total
         totalLikes += parse_string(recieved_message);
         printf("Total: %d \n",totalLikes);
         logTotalLikes(totalLikes);


    }
}
//close sockets
close(client_socket);
close(server_socket);
 return 0;
}




//log each message rcieved
//add up the likes by parsing numbers
//ex) LikesServer0 14 --> take 14 add to runnign total
int parse_string(char str[])
{
char num[40]; //will store double digit likes
int likes = 0; //used for conversion
if((str[0] == 'L') && (str[1] == 'i') && (str[2] == 'k') && (str[3] == 'e') &&
(str[4] == 's') && (str[5] == 'S') && (str[6] == 'e') && (str[7] == 'r') &&
(str[8] == 'v') && (str[9] == 'e') && (str[10] == 'r') && str[12] == ' '){
     //now check for the actual numbers
    if(!(str[13] == '\0') && !(str[14] == '\0')){
        //concat together
        //covert str to int
        //printf("two digit : %c%c \n",str[13],str[14]);
        num[0] = str[13];
        num[1] = str[14];
        num[3] = '\0';
    
        likes = atoi(num);
    }
    else{
        //means 1 digit number
        //covert str to int
        //printf("one digit : %c \n",str[13]);
        likes = atoi(&str[13]);
    }
}
//return likes from message for totalLikes
if (likes > 42){
   likes = 0;
}

return likes;
}




void logLikesServer(char message[])
{
  char filename[256] = "/tmp/PrimaryLikesLog";


  //open file in append mode b/c write will delete contents everytime called
  FILE *log = fopen(filename,"a");

  if(log == NULL){
      perror("error opening /tmp/PrimaryLikesLog\n");
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




void logTotalLikes(int total){
  //build message
  char message[1024] = "Total: ";
  char nl[] = " \n";
  char likes[1024];


  sprintf(likes,"%d", total);
  strcat(message,likes);
  strcat(message,nl);


  //open file in append mode b/c write will delete contents everytime called
  char filename[256] = "/tmp/PrimaryLikesLog";
  FILE *log = fopen(filename,"a");




  if(log == NULL){
      perror("error opening /tmp/PrimaryLikesLog\n");
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




