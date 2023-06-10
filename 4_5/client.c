#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int sock;
struct sockaddr_in addr;
socklen_t addr_size;
char buffer[1024];

void send_message() {
    if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, sizeof(addr)) != strlen(buffer)) {
        perror("send() failed");
        exit(1);
    }
}

void receive_message() {
    socklen_t server_len = sizeof(addr);
    bzero(buffer, 1024);
    if (recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, &server_len) < 0) {
        perror("recv() failed");
        exit(1);
    }
}

void enter_room() {
    srand(getpid() % 47);
    int gender = ((rand()) % 2) + 2; // 2 - женщина, 3 - мужчина

    sleep(rand() % 3);

    bzero(buffer, 1024);
    sprintf(buffer, "%d", gender);
    send_message();
    receive_message();
    if (atoi(buffer) > 0) {
        printf("Entered room #%s.\n", buffer);
    } else {
        printf("Could not enter a room.\n");
    }
    bzero(buffer, 1024);
    sprintf(buffer, "exit");
    send_message();
    exit(0);
}


int main(int argc, char *argv[]) {
    char *ip = argv[1];
    int port = atoi(argv[2]);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket error: ");
        exit(1);
    }

    printf("UDP server socket created\n");

    memset(&addr, '\0', sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_aton(ip, &addr.sin_addr) == 0) {
        perror("Invalid server IP address");
        exit(EXIT_FAILURE);
    }
    bzero(buffer, 1024);
    strcpy(buffer, "enter");
    send_message();
    bzero(buffer, 1024);
    receive_message();
    if (strcmp(buffer, "enter") == 0) {
        enter_room();
    }
    close(sock);
    exit(0);
}
