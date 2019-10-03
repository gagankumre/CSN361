#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 

int parity_true(char* buff){
    int check=0; 
    for(int i=0;i<strlen(buff);i++){
        if(buff[i]=='1'){
            check=(check+1)%2 ;
        }
    }
    return check ;
}

int checkstr(char* c){
    int l=strlen(c) ;
    int check=0 ;
    for(int i=0;i<l;i++){
        if(c[i]=='1'){
            check=(check+1)%2 ;
        }
    }
    return check ;
}

char* xor(char* a, char* b){
    char* ret = malloc(sizeof(char)*strlen(a)) ;
    for(int i=0;i<strlen(a);i++){
        if((a[i]=='0'&&b[i]=='0') || (a[i]=='1'&&b[i]=='1')){
            ret[i]='0' ;
        } else if((a[i]=='0'&&b[i]=='1') || (a[i]=='1'&&b[i]=='0')){
            ret[i]='1';
        }
    }
    char* ret1 = malloc(sizeof(char)*(strlen(a)-1)) ;
    for(int i=0;i<strlen(a)-1;i++){
        ret1[i]=ret[i+1] ;
    }
    return (char *)ret1 ;
}

char* update_CRC(char* message, char* divi1, int len){
    
    char ch1[strlen(divi1)] ;
    for(int i=0;i<strlen(divi1)-1;i++){
        ch1[i]='0' ;
    }
    // strcat(message,ch1) ;

    strcat(ch1,"0") ;
    int pick=strlen(divi1) ;

    char* tmp=malloc(sizeof(char)*pick) ;
    for(int i=0;i<pick;i++){
        tmp[i]=message[i] ;
    }

    while(pick<strlen(message)){
        if(tmp[0]=='1'){
            tmp=xor(divi1, tmp);
            tmp[strlen(tmp)]=message[pick] ;
        } else {
            tmp=xor(ch1, tmp);
            tmp[strlen(tmp)]=message[pick] ;
        }
        pick++ ;
    }
    if(tmp[0]=='1'){
        tmp=xor(divi1,tmp) ;
    } else {
        tmp=xor(ch1, tmp) ;
    }
    return (char *)tmp ;   
}

int check_rem(char* buff){
    int check=0; 
    for(int i=0;i<strlen(buff);i++){
        if(buff[i]=='1'){
            return 0 ;
        }
    }
    return 1 ;
}

char* add_bin(char* m1, char* m2, int len){

    char* ans=malloc(sizeof(char)*len) ;
    char carry='0' ;
    for(int i=strlen(m1)-1;i>=0;i--){
        if(m1[i]=='0' && m2[i]=='0'){
            if(carry=='0'){
                ans[i]='0' ;
            } else{
                ans[i]='1' ;
                carry='0' ;
            }
        } else if(m1[i]=='0' && m2[i]=='1'){
            if(carry=='0'){
                ans[i]='1' ;
            } else{
                ans[i]='0' ;
                carry='1' ;
            }
        } else if(m1[i]=='1' && m2[i]=='0'){
            if(carry=='0'){
                ans[i]='1' ;
            } else{
                ans[i]='0' ;
                carry='1' ;
            }
        } else if(m1[i]=='1' && m2[i]=='1'){
            if(carry=='0'){
                ans[i]='0' ;
                carry='1' ;
            } else{
                ans[i]='1' ;
                carry='1' ;
            }
        } 
    }
    if(carry=='1'){
        for(int i=strlen(ans)-1;i>=0;i--){
            if(ans[i]=='1'){
                ans[i]='0' ;
            } else {
                ans[i]='1' ;
                break ;
            }
        }
    }
    
    return (char *)ans;
}

int main(int argc, char const *argv[]) { 

    /// This is used to store the file descriptors of the socket.
    int sock = 0;
    
    /// This will contain the return value when message will be sent by the client.
    int valread; 

    /// This will store various socket attributes.
    struct sockaddr_in serv_addr; 
    
    /// 
    char buffer[1024] = {0}; 
    
    /// Creating a socket.
    sock = socket(AF_INET, SOCK_STREAM, 0) ;
    if (sock < 0) { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    /// Assigning various attribute values to the sockaddr_in struct.
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    

    /// Convert IPv4 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

    /// Connecting with the server socket.
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    /// Sending the message to the sever socket.
    // send(sock , hello , strlen(hello) , 0 ); 
    // printf("Hello message sent to the server.\n");  
    
    printf("Which algortihm to apply for error detection :- \n");
    printf("1. Single Parity Check\n2. Two-dimensional Parity Check\n3. Checksum\n4. Cyclic Redundancy Check (CRC)\n") ;

    int option ;
    scanf("%d",&option) ;

    if(option==1){
        valread = read( sock , buffer, 1024);
        printf("The received message is : %s\n",buffer); 
        if(parity_true(buffer)){
            printf("Error detected : Yes\n");
        } else {
            printf("Error detected : No\n");
        }
    } else if(option==2){
        valread = read( sock , buffer, 1024);
        printf("The received message is : %s\n",buffer);
        printf("Enter Number of Segments:\n");
        int s ;
        scanf("%d",&s) ;
        s++ ;
        int l=strlen(buffer) ;
        int len=l/s ;

        char a[len] ;
        for(int i=0;i<len;i++){
            a[i]='0' ;
        }
        a[len]='\0' ;
        int arr_row[s-1] ;
        int arr_col[len-1] ;
        for(int i=0;i<len-1;i++){
            arr_col[i]=0 ;
        }
        for(int i=0;i<s-1;i++){
            arr_col[i]=0 ;
        }
        int j=0 ;
        int index=0 ;
        for(int i=0;i<l-len;i++){
            a[j]=buffer[i] ;
            j++ ;
            if(j%len==0){
                j=0 ;
                arr_row[index++]=checkstr(a) ;
            }
        }
        printf("\n\n") ;
        char b[s] ;
        for(int i=0;i<s;i++){
            b[i]='0' ;
        }
        b[s]='\0' ;
        j=0 ;
        index=0 ;
        for(int x=0;x<len-1;x++){
            for(int y=x;y<l;y+=len){
                b[j]=buffer[y] ;
                j++ ;
                if(j%s==0){
                    j=0 ;
                    arr_col[index++]=checkstr(b) ;
                }
            }
        }
        int rcheck=0 ;
        int ccheck=0 ;
        for(int i=0;i<len-1;i++){
            if(arr_col[i]==1){
                ccheck++ ;
                break ;
            }
        }
        for(int i=0;i<s-1;i++){
            if(arr_row[i]==1){
                rcheck++ ;
                break ;
            }
        }
        if(rcheck!=0){
            printf("Found error in row :- ") ;
            for(int i=0;i<s-1;i++){
                if(arr_row[i]==1){
                    printf("%d ",i+1) ;
                }
            }
            printf("\n") ;
        }
        if(ccheck!=0){
            printf("Found error in column :- ") ;
            for(int i=0;i<len-1;i++){
                if(arr_col[i]==1){
                    printf("%d ",i+1) ;
                }
            }
            printf("\n"); 
        }
        if(rcheck==0 && ccheck==0){
            printf("No error found\n") ;
        }
    }  else if(option==3){
        valread = read( sock , buffer, 1024);
        printf("The received message is : %s\n",buffer);
        printf("Enter Number of Segments:\n");
        int s ;
        scanf("%d",&s) ;
        s++ ;
        int len=strlen(buffer)/s ;
        char a[len];
        for(int i=0;i<len;i++){
            a[i]='0';
        }
        a[len]='\0';
        char sume[len];
        char* sum = malloc(sizeof(char)*len) ;
        sum=sume ;
        for(int i=0;i<len;i++){
            sume[i]='0';
        }
        sume[len]='\0';
        int j=0 ;
        for(int i=0;i<strlen(buffer);i++){
            a[j]=buffer[i] ;
            j++ ;
            if(j%len==0){
                j=0 ;
                sum=add_bin(sum,a,len) ;
            }
        }
        int correct=1 ;
        for(int i=0;i<len;i++){
            if(sum[i]=='1'){
                sum[i]='0' ;
            } else {
                sum[i]='1' ;
                correct=0 ;
            }
        }
        if(correct){
            printf("Checksum : %s\n",sum);
            printf("Hence, no error\n");
        } else {
            printf("Checksum : %s\n",sum);
            printf("Error found\n");
        }
        
    } else if(option==4){
        printf("Enter the divisor:\n");
        char* div ;
        scanf("%s",div) ;
        valread = read( sock , buffer, 1024);
        printf("The received message is : %s\n",buffer);
        char* rem = malloc(sizeof(char)*(strlen(div)-1));
        rem = update_CRC(buffer,div,strlen(buffer)-strlen(div)+1) ;
        printf("Remainder : %s\n",rem);
        if(check_rem(rem)){
            printf("Hence, no error found.\n") ;
        } else {
            printf("Hence, error found.\n") ;
        }
    }

    return 0; 
} 