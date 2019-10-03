#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <math.h>
#define PORT 8080 
int power (int base, int exponent) {
    int result = 1;
    for (int i=0; i< exponent; i++){
        result = base*result;
    }
    return result;
}
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Listening for encoded message");
    valread = read( new_socket , buffer, 1024); 
    printf("\nMessage received is : %s\n",buffer );
    
    //int size = buffer.size()-1;
    int size = valread; 
    int parity_check[1024] = {0};
    // Checking error
    int j,x,min,max,p=0;
    int k=0;
    for (int i = 1; i <= size; i = power(2, k)){
      k++;
      int parity = 0;
      j = i;
      x = i;
      min = 1;
      max = i;
       while ( j <= size){
          for (x = j; max >= min && x <= size; min++, x++){
              if (buffer[x] == '1')
                  parity = parity + 1;
              //printf("%d",x);
          }
          j = x + i;
          min = 1;
      }
      
      //checking for even parity
      if (parity % 2 == 0){ 
         parity_check[p++] = 0;
      }
      else{
        parity_check[p++] = 1;
      }
    }
    int error_pos = 0; //position where error occured
    for(int f=0;f<1024;f++)
        {
          error_pos+=power(2,f)*parity_check[f];
          //printf("%d",parity_check[f]);
        }
    if(error_pos)
        {
          if(buffer[error_pos]=='1')
              buffer[error_pos]='0';
          else
              buffer[error_pos]='1';
          printf("\nCorrected message: %s",buffer);
        }
    //printf("\n%d",error_pos);
    //printf("%c",buffer[0]);
    return 0; 
} 
