/**
* @file client.c
* @brief Program for initialization for client
* 
* 
* @author Gagan kumre
*
* @date 7/31/2019
*/
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	//setting memory
	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}else{
		char buffer[1024];
		int n;
		while(1){
			bzero(buffer, 1024);
			n=0;
			while((buffer[n++]=getchar())!='\n');
			write(sock , buffer , strlen(buffer)); 

			read( sock , buffer, 1024); 
	        	printf("server: %s\n",buffer );
		}
	     
	}
 
	return 0; 
} 
