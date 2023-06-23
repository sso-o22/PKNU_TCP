#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_REQUEST_SIZE 2048

void *handle_request(void *arg);
char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);

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
                     "<img src=\"data:image/jpeg;base64,";
                     
    char image_footer[] = "\"></center></body></html>\r\n";

    FILE *image_file = fopen("game.jpg", "rb");
    if (!image_file) {
        perror("Failed to open image file");
        close(client_socket);
        pthread_exit(NULL);
    }

    fseek(image_file, 0, SEEK_END);
    long image_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    unsigned char *image_data = malloc(image_size);
    if (!image_data) {
        perror("Failed to allocate memory for image data");
        fclose(image_file);
        close(client_socket);
        pthread_exit(NULL);
    }

    size_t bytes_read = fread(image_data, 1, image_size, image_file);
    if (bytes_read != image_size) {
        perror("Failed to read image data");
        fclose(image_file);
        free(image_data);
        close(client_socket);
        pthread_exit(NULL);
    }

    char *image_encoded;
    size_t image_encoded_length;
    image_encoded = base64_encode(image_data, image_size, &image_encoded_length);
    if (!image_encoded) {
        perror("Failed to encode image data");
        fclose(image_file);
        free(image_data);
        close(client_socket);
        pthread_exit(NULL);
    }

    size_t response_length = strlen(webpage) + image_encoded_length + strlen(image_footer);
    char *response = malloc(response_length);
    if (!response) {
        perror("Failed to allocate memory for response");
        fclose(image_file);
        free(image_data);
        free(image_encoded);
        close(client_socket);
        pthread_exit(NULL);
    }

    sprintf(response, "%s%s%s", webpage, image_encoded, image_footer);

    bytes_sent = send(client_socket, response, response_length, 0);
    if (bytes_sent < 0) {
        perror("Failed to send response");
    }

    fclose(image_file);
    free(image_data);
    free(image_encoded);
    free(response);
    close(client_socket);

    pthread_exit(NULL);
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
    // Base64 encoding 구현체를 사용하여 입력 데이터를 Base64로 인코딩하는 함수입니다.
    // 해당 함수는 구현체에 따라 다를 수 있으며, 필요한 경우 적절한 라이브러리나 코드를 사용해야 합니다.
    // 여기서는 예시로 OpenSSL의 Base64 인코딩 함수를 사용합니다.
    
    // input_length * 4 / 3 크기의 출력 버퍼를 할당합니다.
    size_t encoded_length = (input_length + 2) / 3 * 4;
    char *encoded_data = malloc(encoded_length);
    if (!encoded_data) {
        perror("Failed to allocate memory for base64 encoding");
        *output_length = 0;
        return NULL;
    }

    BIO *bio, *b64;
    BUF_MEM *buffer_ptr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    // 인코딩 시에 줄바꿈 문자를 사용하지 않도록 설정합니다.
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    BIO_write(bio, data, input_length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buffer_ptr);

    // 인코딩된 데이터를 복사합니다.
    memcpy(encoded_data, buffer_ptr->data, buffer_ptr->length);
    encoded_data[buffer_ptr->length] = '\0';
    *output_length = buffer_ptr->length;

    // BIO 및 할당된 자원을 정리합니다.
    BIO_free_all(bio);

    return encoded_data;
}
