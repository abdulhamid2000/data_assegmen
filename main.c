#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#define PORT 8000
#define MAX_BUFFER_SIZE 1024

void start_client() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_data;
    char buffer[MAX_BUFFER_SIZE];
    int total_categories, category_choice, answer_choice;
    int recv_size;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Winsock initialization failed. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Exiting...\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(PORT);
    server_data.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&server_data, sizeof(server_data)) == SOCKET_ERROR) {
        printf("Connection failed. Exiting...\n");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, MAX_BUFFER_SIZE);
    recv_size = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
    if (recv_size == SOCKET_ERROR) {
        printf("Failed to receive data from the server. Exiting...\n");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buffer);

    char *line = strtok(buffer, "\n");
    sscanf(line, "Categories: %d", &total_categories);

    while (1) {
        printf("Choose a category (enter the number): ");
        if (scanf("%d", &category_choice) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');  // Clear the input buffer
            continue;
        }

        if (category_choice < 1 || category_choice > total_categories) {
            printf("Invalid category number. Please choose a valid number.\n");
        } else {
            break;
        }
    }

    memset(buffer, 0, MAX_BUFFER_SIZE);
    sprintf(buffer, "%d", category_choice);
    send(client_socket, buffer, strlen(buffer), 0);

    memset(buffer, 0, MAX_BUFFER_SIZE);
    recv_size = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
    if (recv_size == SOCKET_ERROR) {
        printf("Failed to receive data from the server. Exiting...\n");
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buffer);

    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        recv_size = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);

        if (recv_size == SOCKET_ERROR) {
            printf("Failed to receive data from the server. Exiting...\n");
            closesocket(client_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        if (strstr(buffer, "Your final score")) {
            printf("%s\n", buffer);
            break;
        }

        printf("%s\n", buffer);

        while (1) {
            printf("Your answer (Choose a number): ");
            if (scanf("%d", &answer_choice) != 1) {
                printf("Invalid input. Please enter a valid number.\n");
                while (getchar() != '\n');
                continue;
            }

            if (answer_choice < 1 || answer_choice > 4) {
                printf("Invalid answer number. Please choose a valid option.\n");
            } else {
                break;
            }
        }

        memset(buffer, 0, MAX_BUFFER_SIZE);
        sprintf(buffer, "%d", answer_choice);
        send(client_socket, buffer, strlen(buffer), 0);

        memset(buffer, 0, MAX_BUFFER_SIZE);
        recv_size = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Failed to receive data from the server. Exiting...\n");
            closesocket(client_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buffer);
    }

    closesocket(client_socket);
    WSACleanup();
}

int main() {
    start_client();
    return 0;
}
