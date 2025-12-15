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
#define SIZE 1024

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void send_file(FILE *fp, int sockfd){
    int n;
    char data[SIZE]={0};

    while(fgets(data, SIZE, fp) != NULL){
        if(send(sockfd, data, strlen(data), 0)==-1){
            cerr<<"Error in reading file"<<endl;
            exit(1);
        }
        memset(&data, 0, SIZE);
    }
}

int main(int argc, char *argv[]){

    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int n;
    char s[INET6_ADDRSTRLEN];

    FILE *fp;
    char *filename="send.txt";
    
    if(argc != 2){
        cerr<<"Usage: client <hostname/IP/msg>"<<endl;
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

    fp=fopen(filename,"r");
    if(fp==NULL){
        cerr<<"Error sending data."<<endl;
        return 1;
    }

    send_file(fp,sockfd);
    cout<<"file data send successfully"<<endl;
    fclose(fp);
    // buf[numbytes] = '\0';
    // send(sockfd,argv[2],strlen(argv[2]),0);

    // numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);
    // buf[numbytes] = '\0';
    // if(numbytes==-1){
    //     cerr<<"recv"<<endl;
    //     exit(1);
    // }
    // cout<<"Client received: "<<buf<<endl;
    
    close(sockfd);

    return 0;
}