#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 7232

int main() {
    int sock;
    struct sockaddr_in server;
    char server_response[4096] = {0};

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("소켓 생성에 실패했습니다.\n");
        return 1;
    }

    // 서버 주소 구성
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("서버 연결에 실패했습니다.\n");
        return 1;
    }
    printf("서버에 연결되었습니다.\n");

    // 서버에 GET 요청 전송
    char *request = "GET / HTTP/1.1\r\n\r\n";
    if (send(sock, request, strlen(request), 0) < 0) {
        printf("GET 요청 전송에 실패했습니다.\n");
        return 1;
    }
    printf("GET 요청을 서버로 전송했습니다.\n");

    // 서버 응답 수신
    if (recv(sock, server_response, sizeof(server_response), 0) < 0) {
        printf("서버 응답 수신에 실패했습니다.\n");
        return 1;
    }
    printf("서버 응답을 받았습니다:\n%s\n", server_response);

    close(sock);
    printf("연결이 종료되었습니다.\n");

    return 0;
}
