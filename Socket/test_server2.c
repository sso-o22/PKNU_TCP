#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_REQUEST_SIZE 4096

char webpage[] = "HTTP/1.1 200 OK\r\n"
                 "Server: Linux Web Server\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                 "<!DOCTYPE html>\r\n"
                 "<html><head><title>My Web Page</title></head>\r\n"
                 "<body><center><h1>Hello world!!</h1><br>\r\n"
                 "<img src=\"web.jpg\"></center></body></html>\r\n";

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("소켓 생성에 실패했습니다.");
        exit(EXIT_FAILURE);
    }

    // 소켓 옵션 설정
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("소켓 옵션 설정에 실패했습니다.");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("바인딩에 실패했습니다.");
        exit(EXIT_FAILURE);
    }
    printf("바인딩 완료. 포트 %d에서 접속 대기 중...\n", PORT);

    // 클라이언트 연결 대기
    if (listen(server_fd, 3) < 0) {
        perror("연결 대기에 실패했습니다.");
        exit(EXIT_FAILURE);
    }

    // 클라이언트 요청 처리
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("연결 수락에 실패했습니다.");
            exit(EXIT_FAILURE);
        }

        printf("연결 수락. 클라이언트 연결이 성공적으로 이루어졌습니다.\n");

        // HTTP 요청 수신
        char request[MAX_REQUEST_SIZE] = {0};
        int bytes_read = recv(new_socket, request, sizeof(request), 0);
        if (bytes_read < 0) {
            perror("HTTP 요청 수신에 실패했습니다.");
            exit(EXIT_FAILURE);
        }

        // GET 요청 확인
        if (strstr(request, "GET") != NULL) {
            // 이미지 요청 확인
            if (strstr(request, "game.jpg") != NULL) {
                // 이미지 파일 열기
                FILE *image_file = fopen("game.jpg", "rb");
                if (image_file == NULL) {
                    perror("이미지 파일 열기에 실패했습니다.");
                    exit(EXIT_FAILURE);
                }

                // 이미지 파일 크기 확인
                fseek(image_file, 0, SEEK_END);
                long image_size = ftell(image_file);
                fseek(image_file, 0, SEEK_SET);

                // 이미지 파일 응답 전송
                char response_header[512] = {0};
                snprintf(response_header, sizeof(response_header), "HTTP/1.1 200 OK\r\n"
                                                                    "Server: Linux Web Server\r\n"
                                                                    "Content-Type: image/jpeg\r\n"
                                                                    "Content-Length: %ld\r\n\r\n", image_size);

                // 헤더 전송
                send(new_socket, response_header, strlen(response_header), 0);
                printf("이미지 헤더를 클라이언트로 전송했습니다.\n");

                // 이미지 데이터 전송
                char buffer[1024];
                size_t bytes_sent;
                while ((bytes_sent = fread(buffer, sizeof(char), sizeof(buffer), image_file)) > 0) {
                    send(new_socket, buffer, bytes_sent, 0);
                }
                printf("이미지 데이터를 클라이언트로 전송했습니다.\n");

                fclose(image_file);
            } else {
                // 일반 HTML 응답 전송
                send(new_socket, webpage, strlen(webpage), 0);
                printf("웹페이지 응답을 클라이언트로 전송했습니다.\n");
            }
        }

        close(new_socket);
        printf("클라이언트 연결 종료.\n");
    }

    return 0;
}
