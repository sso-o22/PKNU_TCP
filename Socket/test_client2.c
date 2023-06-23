#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_RESPONSE_SIZE 4096

int main() {
    int sock = 0, bytes_read;
    struct sockaddr_in server_address;
    char response[MAX_RESPONSE_SIZE] = {0};

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("소켓 생성에 실패했습니다.");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    // 서버 주소 변환
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("서버 주소 변환에 실패했습니다.");
        return -1;
    }

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("서버에 연결에 실패했습니다.");
        return -1;
    }

    // HTTP GET 요청 전송
    char *request = "GET / HTTP/1.1\r\n\r\n";
    if (send(sock, request, strlen(request), 0) < 0) {
        perror("HTTP GET 요청 전송에 실패했습니다.");
        return -1;
    }

    // 서버 응답 수신
    if ((bytes_read = read(sock, response, sizeof(response))) < 0) {
        perror("서버 응답 수신에 실패했습니다.");
        return -1;
    }

    // 응답 출력
    printf("서버 응답:\n%s\n", response);

    close(sock);

    return 0;
}
