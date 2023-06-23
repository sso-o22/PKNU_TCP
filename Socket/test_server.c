#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7232 

char webpage[] = "HTTP/1.1 200 OK\r\n"
								 "Server:Linux Web Server\r\n"
								 "Content-Type: text/html: charset=UTF-8\r\n\r\n"
								 "<!DOCTYPE html>\r\n"
								 "<html><head><title> My Web Page </title>\r\n"
								 "<style>body {background-color: #FFFF00 }</style></head>\r\n"
								 "<body><center><h1>Hello world!!</hI><br>\r\n"
								 "<img src=\"web.jpg\"></center›</body></html>\r\n";

int main() {
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    char client_message[1024] = {0};

    // 소켓 생성
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("소켓 생성에 실패했습니다.\n");
        return 1;
    }

    // 소켓 주소 설정
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("바인딩에 실패했습니다.\n");
        return 1;
    }
    printf("바인딩 완료. 포트 %d에서 접속 대기 중...\n", PORT);

    // 클라이언트 연결 대기
    listen(socket_desc, 3);

    c = sizeof(struct sockaddr_in);

    while (1) {
        // 클라이언트 연결 수락
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        if (client_sock < 0) {
            printf("연결 수락에 실패했습니다.\n");
            return 1;
        }
        printf("연결 수락. 클라이언트 연결이 성공적으로 이루어졌습니다.\n");

        // 클라이언트로부터 데이터 수신
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0) {
            printf("데이터 수신에 실패했습니다.\n");
            return 1;
        }
        printf("클라이언트 요청:\n%s\n", client_message);

        // 응답 전송
        if (send(client_sock, webpage, strlen(webpage), 0) < 0) {
            printf("응답 전송에 실패했습니다.\n");
            return 1;
        }

        close(client_sock);
        printf("클라이언트 연결 종료.\n");
    }
}
