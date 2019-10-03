#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <math.h>
#include <stdlib.h> 
#include <time.h>
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
    //boilerplate

    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 




    //end boilerplate
    int m, data_bits[20],r = 0,parity;    //m = no. of data bits, r = no. of redundant bits

    //input number of bits
    printf("Enter the number of bits: ");
    scanf("%d", &m); 

    //input number
    printf("Enter the N bit message, bit by bit: \n");
    for(int i = 0; i < m; i++)
        scanf("%d", &data_bits[m-i]);

    //finding redundant bits
    while(power (2,r) < m + r + 1){
        r++;
    }
    printf("Redundant Bits: %d", r);
    
    int hamming[m + r + 1],j = 0,k = 1;    
    
    //finding positions of redundant bits.
    for(int i = 1; i <= m + r; i++){
        
        if( i == power( 2, j )){
            hamming[i] = -1;    //-1 is initial value of redundant bits
            j++;
        }
        else{
            hamming[i] = data_bits[k];
            k++;
        }
    }
    
    k = 0;
    int x, min, max = 0;
    //finding parity bit
    for (int i = 1; i <= m + r; i = power(2, k)){
      k++;
      parity = 0;
      j = i;
      x = i;
      min = 1;
      max = i;
       while ( j <= m + r){
          for (x = j; max >= min && x <= m + r; min++, x++){
              if (hamming[x] == 1)
                  parity = parity + 1;;
          }
          j = x + i;
          min = 1;
      }
      
      //checking for even parity
      if (parity % 2 == 0){ 
         hamming[i] = 0;
      }
      else{
        hamming[i] = 1;
      }
    }
    
    printf("\nM after encoding is :");
    for(int i= 0; i < m + r; i++)
        printf("%d",hamming[m+r-i]);

    int finalhamming[m+r];
    for (int i=0; i< m+r; i++) {
        finalhamming[i] = hamming[m+r-i];
    }

    //adding error
    printf("\nDo you wish to add an error?");
    printf("\nPress 0 for Manual error");
    printf("\nPress 1 for random errors");
    printf("\nPress any other key for no errors\n");

    int res;
    scanf("%d", &res);

    if (res==0){
        printf("\nPress the number of bits to be flipped :");
        int num;
        scanf("%d", &num);
        printf("\nEnter the indices of the bits to be flipped\n");
        for (int i =0; i< num; i++){
            int pos;
            scanf("%d", &pos);
            finalhamming[pos] = (finalhamming[pos]+1)%2;
        }
    }
    if (res==1){
        int index= rand()%(m+r);
        printf("\nBit%d is flipped\n", index);

        finalhamming[index] = (finalhamming[index]+1)%2;
    }
    printf("\nM transmitted (after adding errors): ");
    for(int i= 0; i < m + r; i++)
        printf("%d",finalhamming[i]);
    char message[m+r];
    for(int i= 0; i < m + r; i++){
        message[i]=(finalhamming[i]==0)?'0':'1';
    }
    

    send(sock , message , m+r , 0 ); 
    printf("\nEncoded message sent\n"); 



    // valread = read( sock , buffer, 1024); 
    // printf("%s\n",buffer ); 
    return 0; 
} 