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

void sigchld_handler(int s){
    (void)s;
    int saved_errno=errno;
    while(waitpid(-1,NULL,WNOHANG)>0);
    errno=saved_errno;
}
void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(){

    int sockfd, new_fd;
    struct addrinfo hints,*servinfo,*p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    char buf[100];
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
            perror("server: bind");
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
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL)==-1){
        perror("sigaction");
        exit(1);
    }
    cout<<"Server: waiting for connections..."<<endl;

    while(1){
        sin_size=sizeof(their_addr);
        new_fd= accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if(new_fd == -1){
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        cout<<"server: got connection from "<<s<<endl;
       
        

        int bytes = recv(new_fd, buf, 99, 0);
        if(bytes > 0){
            buf[bytes] = '\0';
            cout << "Client: " << buf << endl;
        }

        const char *msg="hi from server";
        send(new_fd, msg, strlen(msg), 0);

        close(new_fd);
    }
    return 0;
}