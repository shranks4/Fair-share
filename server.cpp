#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>
using namespace std;

#define PORT "3490"
#define BACKLOG 10

int main(){

    int sockfd, new_fd;
    struct addrinfo hints,*servinfo,*p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    rv=getaddrinfo(NULL,PORT,&hints,&servinfo);
    if(rv!=0){
        cerr<<"getaddrinfo"<<gai_strerror(rv)<<endl;
        return 1;
    }
    for(p=servinfo; p!=NULL; p=p->ai_next){
        sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(sockfd==-1){
            perror("server: socket");
            continue;
        }
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1){
            perror("setsocketopt");
            exit(1);
        }
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("server: bin");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if(p == NULL){
        cout<<"server: failed to bind"<<endl;
        exit(1);
    }
    if(listen(sockfd, BACKLOG)==-1){
        perror("listen");
        exit(1);
    }

    return 0;
}