#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

// 요청의 최대 크기를 나타냄
#define MAX_REQUEST_SIZE 2048

void *handle_request(void *arg);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    // 포트 번호
    unsigned short server_port = 10103;
    int backlog_size = 10;

    // ipv4, 연결 지향형 방식, 프로토콜
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        // perror는 에러 메세지를 출력할 때 시스템 에러메세지도 출력한다
        perror("Failed to create socket");
        exit(1);
    }

    // IPv4주소체계를의미
    server_address.sin_family = AF_INET;
    // 서버가 어떤 네트워크 인터페이스에도 바인딩이 되어 들어오는 모든 IP주소를
    // 수신 할 수 있도록 지정
    server_address.sin_addr.s_addr = INADDR_ANY;
    // htons는 호스트바이트 순서를 네트워크 바이트 순서로 변환
    server_address.sin_port = htons(server_port);

    // Bind the server socket to a specific address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, backlog_size) < 0) {
        perror("Failed to listen");
        exit(1);
    }

    printf("Server listening on port %d\n", server_port);

    while (1) {
        unsigned int client_address_length = sizeof(client_address);

        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            exit(1);
        }

        printf("Client connected\n");

        // Create a new thread to handle the client request
        pthread_t thread;
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;

        if (pthread_create(&thread, NULL, handle_request, client_socket_ptr) != 0) {
            perror("Failed to create thread");
            exit(1);
        }

        // Detach the thread to clean up automatically
        pthread_detach(thread);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

void *handle_request(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);

    char response[MAX_REQUEST_SIZE];
    int bytes_sent;

    // Create the HTML response
    char webpage[] = "HTTP/1.1 200 OK\r\n"
                     "Server: Linux Web Server\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html><head><title>My Web Page</title>\r\n"
                     "<style>body {background-color: #FFFF00;}</style></head>\r\n"
                     "<body><center><h1>Hello world!!</h1><br>\r\n"
                     "<img src=\"game.jpg\"></center></body></html>\r\n";

    // Send the response to the client
    bytes_sent = write(client_socket, webpage, strlen(webpage));
    if (bytes_sent < 0) {
        perror("Failed to send response");
        exit(1);
    }

    printf("Response sent\n");

    // Close the client socket
    close(client_socket);

    pthread_exit(NULL);
}

