#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_REQUEST_SIZE 2048

void *handle_request(void *arg);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    unsigned short server_port = 10103;
    int backlog_size = 10;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind");
        exit(1);
    }

    if (listen(server_socket, backlog_size) < 0) {
        perror("Failed to listen");
        exit(1);
    }

    printf("Server listening on port %d\n", server_port);

    while (1) {
        unsigned int client_address_length = sizeof(client_address);

        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            exit(1);
        }

        printf("Client connected\n");

        pthread_t thread;
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;

        if (pthread_create(&thread, NULL, handle_request, client_socket_ptr) != 0) {
            perror("Failed to create thread");
            exit(1);
        }

        pthread_detach(thread);
    }

    close(server_socket);

    return 0;
}

void *handle_request(void *arg) {
    int client_socket = *((int*)arg);
    free(arg);

    char response[MAX_REQUEST_SIZE];
    int bytes_sent;

    char webpage[] = "HTTP/1.1 200 OK\r\n"
                     "Server: Linux Web Server\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html><head><title>My Web Page</title>\r\n"
                     "<style>body {background-color: #FFFF00;}</style></head>\r\n"
                     "<body><center><h1>Hello world!!</h1><br>\r\n"
                     "<img src=\"game.jpg\"></center></body></html>\r\n";

    bytes_sent = send(client_socket, webpage, strlen(webpage), 0);
    if (bytes_sent < 0) {
        perror("Failed to send response");
        exit(1);
    }

    printf("Response sent\n");

    FILE *image_file = fopen("game.jpg", "rb");
    if (image_file == NULL) {
        perror("Failed to open image file");
        close(client_socket);
        pthread_exit(NULL);
    }

    fseek(image_file, 0, SEEK_END);
    long image_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    char image_header[128];
    snprintf(image_header, sizeof(image_header), "HTTP/1.1 200 OK\r\n"
                                                  "Server: Linux Web Server\r\n"
                                                  "Content-Type: image/jpeg\r\n"
                                                  "Content-Length: %ld\r\n\r\n", image_size);

    bytes_sent = send(client_socket, image_header, strlen(image_header), 0);
    if (bytes_sent < 0) {
        perror("Failed to send image header");
        fclose(image_file);
        close(client_socket);
        pthread_exit(NULL);
    }

    char image_buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(image_buffer, 1, sizeof(image_buffer), image_file)) > 0) {
        bytes_sent = send(client_socket, image_buffer, bytes_read, 0);
        if (bytes_sent < 0) {
            perror("Failed to send image data");
            fclose(image_file);
            close(client_socket);
            pthread_exit(NULL);
        }
    }

    fclose(image_file);
    close(client_socket);

    pthread_exit(NULL);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_REQUEST_SIZE 2048

void *handle_request(void *arg);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    unsigned short server_port = 10103;
    int backlog_size = 10;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind");
        exit(1);
    }

    if (listen(server_socket, backlog_size) < 0) {
        perror("Failed to listen");
        exit(1);
    }

    printf("Server listening on port %d\n", server_port);

    while (1) {
        unsigned int client_address_length = sizeof(client_address);

        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            exit(1);
        }

        printf("Client connected\n");

        pthread_t thread;
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;

        if (pthread_create(&thread, NULL, handle_request, client_socket_ptr) != 0) {
            perror("Failed to create thread");
            exit(1);
        }

        pthread_detach(thread);
    }

    close(server_socket);

    return 0;
}

void *handle_request(void *arg) {
    int client_socket = *((int*)arg);
    free(arg);

    char response[MAX_REQUEST_SIZE];
    int bytes_sent;

    char webpage[] = "HTTP/1.1 200 OK\r\n"
                     "Server: Linux Web Server\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html><head><title>My Web Page</title>\r\n"
                     "<style>body {background-color: #FFFF00;}</style></head>\r\n"
                     "<body><center><h1>Hello world!!</h1><br>\r\n"
                     "<img src=\"game.jpg\"></center></body></html>\r\n";

    bytes_sent = send(client_socket, webpage, strlen(webpage), 0);
    if (bytes_sent < 0) {
        perror("Failed to send response");
        exit(1);
    }

    printf("Response sent\n");

    FILE *image_file = fopen("game.jpg", "rb");
    if (image_file == NULL) {
        perror("Failed to open image file");
        close(client_socket);
        pthread_exit(NULL);
    }

    fseek(image_file, 0, SEEK_END);
    long image_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    char image_header[128];
    snprintf(image_header, sizeof(image_header), "HTTP/1.1 200 OK\r\n"
                                                  "Server: Linux Web Server\r\n"
                                                  "Content-Type: image/jpeg\r\n"
                                                  "Content-Length: %ld\r\n\r\n", image_size);

    bytes_sent = send(client_socket, image_header, strlen(image_header), 0);
    if (bytes_sent < 0) {
        perror("Failed to send image header");
        fclose(image_file);
        close(client_socket);
        pthread_exit(NULL);
    }

    char image_buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(image_buffer, 1, sizeof(image_buffer), image_file)) > 0) {
        bytes_sent = send(client_socket, image_buffer, bytes_read, 0);
        if (bytes_sent < 0) {
            perror("Failed to send image data");
            fclose(image_file);
            close(client_socket);
            pthread_exit(NULL);
        }
    }

    fclose(image_file);
    close(client_socket);

    pthread_exit(NULL);
}
