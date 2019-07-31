#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
  
// Driver code 
int main() 
{ 
    int pid, pid1, pid2; 
  
    // variable pid will store the 
    // value returned from fork() system call 
    pid = fork(); 
  
    // If fork() returns zero then it 
    // means it is child process. 
    if (pid == 0) { 
  
        
        printf("child[1] --> pid = %d and ppid = %d\n", 
               getpid(), getppid()); 
        sleep(2);
        int gpid=fork();
        if(gpid==0){
         //sleep(3);
         printf("child-grand[1] --> pid = %d and ppid = %d\n", 
               getpid(), getppid());
        }
         else{
         int gpid2=fork(); 
         if(gpid2==0){
        // sleep(4);
         printf("child-grand[2] --> pid = %d and ppid = %d\n", 
               getpid(), getppid());
         }
        }
    } 
  
    else { 
        pid1 = fork(); 
        if (pid1 == 0) { 
            sleep(1); 
            printf("child[2] --> pid = %d and ppid = %d\n", 
                   getpid(), getppid()); 
         sleep(4);
          int gpid=fork();
        if(gpid==0){
         //sleep(3);
         printf("child-grand[3] --> pid = %d and ppid = %d\n", 
               getpid(), getppid());
        }
         else{
         int gpid2=fork(); 
         if(gpid2==0){
        // sleep(4);
         printf("child-grand[4] --> pid = %d and ppid = %d\n", 
               getpid(), getppid());
         }
        }
          
        } 
            // If value returned from fork() 
            // in not zero and >0 that means 
            // this is parent process. 
            else { 
                     sleep(10);
                // This is asked to be printed at last 
                // hence made to sleep for 3 seconds. 
                //sleep(10); 
                printf("parent --> pid = %d\n", getpid()); 
            } 
    } 
  
    return 0; 
}
