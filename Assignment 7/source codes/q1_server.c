#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h>    // time()
#define PORT 8080 

void add_parity(char message[]){
    int parity=0 ;
    for(int i=0;i<strlen(message);i++){
        if(message[i]=='1'){
            parity=(parity+1)%2 ;
        }
    }
    if(parity==1){
        strcat(message,"1") ;
    } else {
        strcat(message,"0") ;
    }
}

char* add_2dparity(char** message,int segments,int len){
    for(int i=0;i<segments;i++){
        add_parity(message[i]) ;
    }
    int parity=0 ;
    char* add = malloc(sizeof(char)*(len+1));
    for(int i=0;i<len+1;i++){
        add[i]='0' ;
    }
    for(int i=0;i<len+1;i++){
        parity=0 ;
        for(int j=0;j<segments;j++){
            if(message[j][i]=='1'){
                parity=(parity+1)%2 ;
            }
        }
        if(parity==1){
            add[i]='1' ;
        }
    }
    return (char *)add ;
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

char* add_sum(char** message,int segments,int len){
    
    if(segments==1){
        return (char *)message[0] ;
    }
    char* sum = malloc(sizeof(char)*len) ;
    for(int i=1;i<segments;i++){
        if(i==1){
            sum=add_bin(message[0],message[1],len) ;
        } else {
            sum=add_bin(sum,message[i],len) ;
        }
    }
    for(int i=0;i<len;i++){
        if(sum[i]=='1'){
            sum[i]='0' ;
        } else {
            sum[i]='1' ;
        }
    }

    return (char *)sum ;
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
    strcat(message,ch1) ;

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

int main(int argc, char const *argv[]) 
{ 
    /// This is used to store the file descriptors of the socket.
    int server_fd ; 

    /// This will create a new socket for the client when input will be given by client.
    int new_socket ;

    /// This will contain the return value when message will be sent by the client.
    int valread ; 

    /// This will store various socket attributes.
    struct sockaddr_in address;

    int opt = 1; 
    int addrlen = sizeof(address); 
    
    /// This will contian the message that the client has sent.
    char buffer[1024] = {0}; 

    /// This is the message that will be sent to the client.
    printf("Which algorithm do you want to use for checking :-\n") ;
    printf("1. Single Parity Check\n2. Two-dimensional Parity Check\n3. Checksum\n4. Cyclic Redundancy Check (CRC)\n") ;
    int option ;
    scanf("%d",&option) ;

    int len=0 ; 
   	int segments=0 ;
	char* divisor;

	char* message1 ;
	char** message23 ;
    char* message4 ;

    switch(option){
        case 1 :
            printf("Length of the Message:") ;
            scanf("%d",&len) ;
            printf("N bits binary message:");
            message1 = calloc(sizeof(char),len) ;
            scanf("%s",message1) ;
            add_parity(message1) ;
            break ;
        case 2 :
            printf("Length of the Message:") ;
            scanf("%d",&len) ;
            printf("Number of segments of message:") ;
            scanf("%d",&segments) ;
            len=len/segments ;
            message23 = calloc(sizeof(char*),segments) ;
            for(int i=0;i<segments;i++){
                printf("%dth segment of %d bits:\n",(i+1),len);
                message23[i] = calloc(sizeof(char),len) ;
                scanf("%s",message23[i]) ;
            }
            message23[segments]=add_2dparity(message23,segments,len) ;
            segments++ ;
            break ;
        case 3 :
        	printf("Length of the Message:") ;
        	scanf("%d",&len) ;
        	printf("Number of segments of message:") ;
        	scanf("%d",&segments) ;
            len=len/segments ;
        	message23 = calloc(sizeof(char*),segments) ;
        	for(int i=0;i<segments;i++){
        		printf("%dth segment of %d bits:\n",(i+1),len);
        		message23[i] = calloc(sizeof(char),len) ;
        		scanf("%s",message23[i]) ;
        	}
            message23[segments]=add_sum(message23,segments,len) ;
            segments++ ;
        	break ;
        case 4 :
        	printf("Divisor: "); 
        	scanf("%s",divisor) ;
        	printf("Length of the Message:") ;
            scanf("%d",&len) ;
            message4 = calloc(sizeof(char),len) ;
            printf("N bits binary message:");
            scanf("%s",message4) ;
            char* rem = malloc(sizeof(char)*(strlen(divisor)-1));
            rem=update_CRC(message4,divisor,len) ;
            for(int i=len;i<strlen(message4);i++){
			    message4[i]=rem[i-len] ;
			}
            len=strlen(message4) ;
            break ;
        default:
        	printf("Enter a valid option !!!");
		
    }
    /// Creating socket file descriptor 
    server_fd = socket(AF_INET, SOCK_STREAM, 0) ;
    if (server_fd < 0) { 
        printf("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    /// Assigning various attribute values to the sockaddr_in struct.
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    
    ///printing original message
    printf("\nThe Original message is :- \n");
    if(option==1){
        printf("%s\n",message1);
    } else if(option==4){
        printf("%s\n",message4);
    } else if(option==2 || option==3){
        for(int i=0;i<segments;i++){
            printf("%s\n",message23[i]);
        }
    } 

    int er_opt ;
    printf("\nHow you want to add an error\n1.Manually add an error\n2.Randomly add an error\n") ;
    scanf("%d",&er_opt) ;

    int n_flips=0 ;
    int** flip_indices ;
	float pe=0.0;

    switch(er_opt) {

    	case 1 :
    		if(option==1){

    			printf("a. Enter number of bits to be flipped:");
	    		scanf("%d",&n_flips) ;
	    		flip_indices = calloc(sizeof(int*),n_flips) ;
	    		printf("b. Enter bits (B indices) to be flipped:-\n") ;
	    		for(int i=0;i<n_flips;i++){
	    			flip_indices[i] = calloc(sizeof(int),1) ;
	    			scanf("%d",flip_indices[i]) ;
	    		} 

		    	for(int i=0;i<n_flips;i++){
		    		if(message1[*flip_indices[i]]=='1'){
		    			message1[*flip_indices[i]]='0' ;
		    		} else {
		    			message1[*flip_indices[i]]='1' ;
		    		}
		    	}

		    	printf("Message to be transmitted is : %s\n",message1);

		    } else if(option==4) {
			    
			    printf("a. Enter number of bits to be flipped:");
	    		scanf("%d",&n_flips) ;
	    		flip_indices = calloc(sizeof(int*),n_flips) ;
	    		printf("b. Enter bits (B indices) to be flipped:-\n") ;
	    		for(int i=0;i<n_flips;i++){
	    			flip_indices[i] = calloc(sizeof(int),1) ;
	    			scanf("%d",flip_indices[i]) ;
	    		} 

	    		for(int i=0;i<n_flips;i++){
		    		if(message4[*flip_indices[i]]=='1'){
		    			message4[*flip_indices[i]]='0' ;
		    		} else {
		    			message4[*flip_indices[i]]='1' ;
		    		}
		    	} 

		    	printf("Message to be transmitted is : %s\n",message4);

		    } else {
		    	for(int i=0;i<segments;i++){
		    	
		    		printf("For %dth segment\n",(i+1));
		    		printf("a. Enter number of bits to be flipped:");
		    		scanf("%d",&n_flips) ;
		    		flip_indices = calloc(sizeof(int*),n_flips) ;
		    		printf("b. Enter bits (B indices) to be flipped:-\n") ;
		    		for(int i=0;i<n_flips;i++){
		    			flip_indices[i] = calloc(sizeof(int),1) ;
		    			scanf("%d",flip_indices[i]) ;
		    		}

		    		for(int j=0;j<n_flips;j++){
			    		if(message23[i][*flip_indices[j]]=='1'){
			    			message23[i][*flip_indices[j]]='0' ;
			    		} else {
			    			message23[i][*flip_indices[j]]='1' ;
			    		}
		    		}
	    		}

	    		for(int i=0;i<segments;i++){
	    			printf("Message to be transmitted is : %s\n",message23[i]);
	    		}
   			}
   			break ;

   			case 2 :
   				printf("Enter probability of induced error:") ;
   				scanf("%f",&pe) ;

   				srand(time(0));

   				if(option==1){
	    			for(int i=0;i<len;i++){
	    				if(pe<=(float)rand()/(float)RAND_MAX){
	    					if(message1[i]=='1'){
				    			message1[i]='0' ;
				    		} else {
				    			message1[i]='1' ;
				    		}
	    				}
			    	}
			    	printf("Message to be transmitted is : %s\n",message1);
			    } else if(option==4) {
			    	for(int i=0;i<len;i++){
	    				if(pe<=(float)rand()/(float)RAND_MAX){
	    					if(message4[i]=='1'){
				    			message4[i]='0' ;
				    		} else {
				    			message4[i]='1' ;
				    		}
	    				}
			    	}
			    	printf("Message to be transmitted is : %s\n",message4);
			    } else {
			    	for(int i=0;i<segments;i++){
			    		for(int j=0;j<len;j++){
		    				if(pe<=(float)rand()/(float)RAND_MAX){
		    					if(message23[i][j]=='1'){
					    			message23[i][j]='0' ;
					    		} else {
					    			message23[i][j]='1' ;
					    		}
		    				}
			    		}
   					}
   					for(int i=0;i<segments;i++){
   						printf("Message to be transmitted is : %s\n",message23[i]);
   					}
				}
				break ;
				
			default:
				printf("INVALID OPTION");
	}

 /// Forcefully binding socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) { 
        printf("bind failed\n"); 
        exit(EXIT_FAILURE); 
    } 

    /// Waiting for the client to connect
    if (listen(server_fd, 3) < 0) { 
        printf("listen\number"); 
        exit(EXIT_FAILURE); 
    } 

    /// creating a new socket for the client 
    new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen) ;
    if(new_socket<0) { 
        printf("accept\n"); 
        exit(EXIT_FAILURE); 
    } 

    /// Sending message to the client.
    if(option==1){
    	send(new_socket , message1 , strlen(message1) , 0 ); 
    } else if(option==4) {
    	send(new_socket , message4 , strlen(message4) , 0 ); 
    } else {
    	for(int i=0;i<segments;i++){
    		send(new_socket , message23[i] , strlen(message23[i]) , 0 );
    	}
    }
    // printf("Hello message sent to the client\n"); 

    /// Reading the value from the client
    // valread = read( new_socket , buffer, 1024); 
    // printf("%s\n",buffer ); 
    
    return 0; 
} 
