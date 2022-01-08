#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#include "PortScanner.h"

int check_port(int port, char *ip) {

    int connection_status, sockfd;
    struct sockaddr_in servaddr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
    
    connection_status = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    close(sockfd);
    return connection_status;
}

int main(int argc[], char *argv[]) {

    if(argc < 3) {
        printf("How to use: ./PortScanner <ip> <options>\n");
        exit(-1);
    }
    char *ip, *option, *optionOption;
    int sockfd, check;
    clock_t time;

    time = clock();

    ip = argv[1];
    option = argv[2];
    if(argc > 2)
        optionOption = argv[3];

    if(strcmp(ip, "-h") == 0 || strcmp(ip, "--help") == 0) {
        printf("Help\n");

    } else if(strcmp(option, "-pa") == 0) {
        for(int port = 1; port <= MAX_PORT; port++) {
            check = check_port(port, ip);
            if(check == 0) {
                printf("%s[+]%s Open port : %s%d%s\n", B_GREEN, RESET, B_CYAN, port, RESET);
            }
        }

    } else if(strcmp(option, "-p") == 0 && argc >3) {
        long port = strtol(argv[3], NULL, 10);

        if(port > MAX_PORT) {
            printf("[-] Port %d out of reach", port);
            exit(-1);
        }

        if(check_port(port, ip) == 0) {
            printf("[+] Open port : %d\n", port);
        } else {
            printf("[-] Port %d is closed\n", port);
        }
    }
    time = clock() - time;
    printf("Runtime: %fs\n", ((float)time) / CLOCKS_PER_SEC);
    return 0;
}