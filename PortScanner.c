#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#include "PortScanner.h"

struct sockaddr_in servaddr;
servaddr.sin_family = AF_INET;


int check_port(int sockfd, int port, char *ip) {

    int connection_status;

    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
    
    connection_status = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    return connection_status;
}

int main(int argc[], char *argv[]) {

    int sockfd;

    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("too lazy to add anything here sorry.\n");
        exit(0);
    }

    if(argc < 3) {
        printf("How to use: ./PortScanner <ip> <options>\n");
        exit(-1);
    }

    char *ip, *option, *optionOption;
    int check, openPorts;
    clock_t time;
    
    time = clock();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    ip = argv[1];
    option = argv[2];
    if(argc > 2)
        optionOption = argv[3];


    if(strcmp(option, "-pa") == 0) {
        printf("[%s*%s] Scanning %sALL%s ports on %s%s%s\n", B_GREEN, RESET, B_CYAN, RESET, B_CYAN, ip, RESET);
        for(int port = 1; port <= MAX_PORT; port++) {
            check = check_port(sockfd, port, ip);
            if(check == 0) {
                printf("[%s+%s] Open port: %s%d%s\n", B_GREEN, RESET, B_CYAN, port, RESET);
                openPorts++;
            }
        }
        if(openPorts == 0)
            printf("[%s-%s] All ports are closed\n", B_RED, RESET);
        
    } else if(strcmp(option, "-p") == 0 && argc >3) {
        long port = strtol(argv[3], NULL, 10);

        if(port > MAX_PORT) {
            printf("[%s-%s] Port %s%ld%s out of reach\n", B_RED, RESET, B_CYAN, port, RESET);
            exit(-1);
        }
        check = check_port(sockfd, port, ip);
        if(check == 0) {
            printf("[%s+%s] Open port: %s%ld%s\n", B_GREEN, RESET, B_CYAN, port, RESET);
        } else {
            printf("[%s-%s] Port %s%ld%s is closed\n", B_RED, RESET, B_CYAN, port, RESET);
        }
    }

    // Closes the socket
    close(sockfd);

    time = clock() - time;
    printf("Runtime: %fs\n", ((float)time) / CLOCKS_PER_SEC);
 
    return 0;
}
