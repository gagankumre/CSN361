
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <unistd.h>

using namespace std;

int main ()
{
   	int fd;
	
	struct ifreq ifr;
	char const *iface = "enp60s0"; //The interface name may be changed for different versions of linux
	char *mac;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy((char *)ifr.ifr_name , (const char *)iface , IFNAMSIZ-1);

	ioctl(fd, SIOCGIFHWADDR, &ifr);

	close(fd);
	
	mac = (char *)ifr.ifr_hwaddr.sa_data;

    //modifying the value for OUI
	int a = mac[0] + 256;
    int b = mac[1];
    int c = mac[2] + 256;
	
    //display mac address
	printf("Mac Address : %2x:%2x:%.2x:%.2x:%.2x:%.2x\n" , a, b, c, mac[3], mac[4], mac[5]);
	
}

