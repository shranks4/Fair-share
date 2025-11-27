#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
using namespace std;

#define PORT "3490"

int main(){
    WSADATA wsaData;
    int wsaResult=WSAStartup(MAKEWORD(2,2),&wsaData);
    if(wsaResult!=0){
        cout<<"WSA Startup failed!"<<endl;
        return 1;
    }

    int sockfd,numbytes;
    struct addrinfo hints,*servinfo,*p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char yes=1;
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
            cout<<"Socket error"<<endl;
            continue;
        }
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(char))==-1){
            closesocket(sockfd);
            exit(1);
        }
    }


    return 0;
}