#include<iostream>
#include<cstdint>
#include<winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

#define PORT 9909

sockaddr_in srv{};
fd_set fr,fw,fe;

/*
#define AF_INET 2
#define SOCK_STREAM 1
#define IPPROTO_TCP 6
#define INADDR_ANY 0x00000000

struct in_addr {            //IPv4 address
    uint32_t s_addr; 
};

struct sockaddr_in {        // IPv4 Socket address
    short sin_family;
    u_short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

struct sockaddr {           //Generic Base structure
    uint16_t sa_family;
    char sa_data[14];
}
    */
int main(){
    int nRet=0;
    WSADATA ws;
    if(WSAStartup(MAKEWORD(2,2),&ws)!=0)
        cout<<endl<<"Connection failed";
    
    else cout<<endl<<"Connected";

    int nSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    
    
    if(nSocket<0) cout<<endl<<"Socket not opened";    
    else cout<<endl<<"Socket Opened";

    
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    // srv.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    srv.sin_addr.s_addr = INADDR_ANY;
    memset(&(srv.sin_zero),0,8);

    nRet= bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr_in));
    cout<<endl<<bind;
    if(nRet<0){
        cout<<endl<<"Failed to bind";
        exit(EXIT_FAILURE);
    }
    else cout<<endl<<"Binded";

    nRet=listen(nSocket, 5);
    if(nRet<0){
        cout<<endl<<"Failed to start Listen to local port";
        exit(EXIT_FAILURE);
    }
    else cout<<endl<<"Started listening to local port";
}