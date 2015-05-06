#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SERV_PORT 3000

void DieWithUserMessage(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

void DieWithSystemMessage(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {// Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Device ID> <FFF>");
    }
    char *DeviceID = argv[1]; // First arg:  local port/service
    char *Stats = argv[2];
    
    char send_buf[8];
    int send_buf_len = 8;

    // if ( sizeof(*DeviceID) == 2 ){
    //     memcpy(&send_buf[0], DeviceID, 2);
    // }else{
    //     DieWithSystemMessage("DeviceID must be 2 bytes.\n");
    // }
    // printf("===========%d", sizeof(*Stats));
    // if ( sizeof(*Stats) == 3 ){
    //     memcpy(&send_buf[2], Stats, 3);
    // }else{
    //     DieWithSystemMessage("Controller message must be 3 bytes.\n");
    // }

    memcpy(&send_buf[0], DeviceID, 2);
    memcpy(&send_buf[2], Stats, 3);
    memcpy(&send_buf[5], "\r\n\0", 3);

    printf("\n\n\n");
    printf("    Prepare send message is:%s", send_buf);
    
    int sock_fd;   //套接子描述符号
    int recv_num;
    int send_num;
    int client_len;
    char recv_buf[20];
    struct sockaddr_in  addr_serv;
    struct sockaddr_in  addr_client;//服务器和客户端地址
    sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_fd < 0){
            perror("socket");
            exit(1);
    } else{
            ;//printf("sock sucessful\n");
    }
    /*init server address*/
    memset(&addr_serv,0,sizeof(struct sockaddr_in));
    addr_serv.sin_family = AF_INET;//协议族
    addr_serv.sin_port = htons(SERV_PORT);
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);//任意本地址

    client_len = sizeof(struct sockaddr_in);
    
    /*bind socket*/
    if(bind(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in))<0 ){
            // perror("bind");
            exit(1);
    }else{
            ;//printf("bind sucess\n");
    }
    int i = 3;
    while(i > 1){
            recv_num = recvfrom(sock_fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&addr_client,&client_len);
            if(recv_num <  0){
                    printf("bad recv_num\n");
                    perror("again recvfrom");
                    exit(1);
            } else{
                    recv_buf[recv_num]='\0';
                    printf("recv sucess:%s\n",recv_buf);
            }

            printf("begin to send order\n");
            send_num = sendto(sock_fd,send_buf,send_buf_len, 0,(struct sockaddr *)&addr_client,client_len);
            if(send_num < 0){
                    perror("sendto");
                    exit(1);
            } else{
                    printf("send sucessful:%s\n", send_buf);
            }
            i--;
    }
    close(sock_fd);
    return 0;
}
