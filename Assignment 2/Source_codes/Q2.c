
/**
* @file Q2.c
* @brief Program to demonstrate zombie and orphan process.
* 
*
*
* @author Gagan Kumre
*
* @date 7/31/2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h> 
int main() 
{ 
    printf("First Child is the Orphan.\nChild's Child will be the Zombie here.\n");
    int x; 
    x = fork(); 
  
    if (x > 0) 
        printf("IN PARENT PROCESS\nMY PROCESS ID: %d\n", getpid()); 
  
    else if (x == 0) { 
        sleep(5); 
        x = fork(); 
  
        if (x > 0) { 
   printf("CHILD PROCESS ID :%d\nNEW PARENT PROCESS ID : %d\n", getpid(), getppid()); 
  
   while(1) 
     sleep(1); 
  
   printf("IN CHILD PROCESS\nMY PARENT PROCESS ID: %d\n", getppid()); 
        } 
  
        else if (x == 0) 
            printf("IN CHILD'S CHILD PARENT PROCESS ID : %d\n", getppid()); 
    } 
  
    return 0; 
} 
