#include<iostream>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <unistd.h>
#include<arpa/inet.h>
using namespace std;

#define PORT "3490"
#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]){


    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    
    if(argc != 2){
        cerr<<"Usage: client <hostname/IP>"<<endl;
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    rv=getaddrinfo(argv[1], PORT, &hints, &servinfo);

    if(rv != 0){
        cerr<<"getaddrinfo:"<<gai_strerror(rv) << endl;
        return 1;
    }
    for(p = servinfo; p!=NULL; p=p->ai_next){
        sockfd=socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sockfd==-1){
            cout<<"Socket error"<<endl;
            continue;
        }
        inet_ntop(p->ai_family,get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
        cout<<"Client attempting connection to "<<s<<endl;

        if(connect(sockfd, p->ai_addr, p->ai_addrlen)==-1){
            cout<<"Connection error"<<endl;
            close(sockfd);
            continue;
        }
        break;
    }

    if(p == NULL){
        cerr<<"Client failed to connect"<<endl;
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof(s));
    cout<<"client connected to "<<s<<endl;

    freeaddrinfo(servinfo);
    
    numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);
    if(numbytes==-1){
        cerr<<"recv"<<endl;
        exit(1);
    }

    buf[numbytes] = '\0';
    
    cout<<"Client received"<<buf<<endl;
    
    close(sockfd);

    return 0;
}