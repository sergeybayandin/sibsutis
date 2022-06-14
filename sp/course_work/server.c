#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <pthread.h>

#define MAX_SLOT_COUNT 8

#define SERVER_IS_FULL_MSG "Server is full!"
#define SERVER_IS_DISCONNECT "Server is disconnect"

struct slot
{
    int active;
    struct sockaddr_in addr;
} slots[MAX_SLOT_COUNT];

volatile int running = 1;

static void sigint_handler(int);
static void *recv_handler(int *psrever_sock);

int main(void)
{
    int server_sock;

    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create server socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = 0;

    if (bind(server_sock, (struct sockaddr *)(&addr), sizeof(addr)) != 0) {
        perror("Failed to bind server socket");
        return EXIT_FAILURE;
    }

    socklen_t addrlen = sizeof(addr);

    if (getsockname(server_sock, (struct sockaddr *)(&addr), &addrlen) != 0) {
        perror("Failed while getting socket name");
        return EXIT_FAILURE;
    }

    FILE *config;

    if ((config = fopen(".config", "wb")) == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    if (fwrite(&addr, sizeof(addr), 1, config) < 1) {
        fprintf(stderr, "Failed while writting in server config\n");
        return EXIT_FAILURE;
    }
    
    if (fclose(config) != 0) {
        perror("Failed to close server config");
        return EXIT_FAILURE;
    }

    signal(SIGINT, sigint_handler);

    memset(&slots, sizeof(slots), 0);

    pthread_t tid;

    if (pthread_create(&tid, NULL, (void *(*)(void *))(recv_handler), &server_sock) != 0) {
        fprintf(stderr, "Failed to create recv thread\n");
        return EXIT_FAILURE;
    }

    while (running != 0) {
        
    }

    if (pthread_cancel(tid) != 0) {
        fprintf(stderr, "Failed to cancel recv thread\n");
        return EXIT_FAILURE;
    }

    int i, k;

    k = strlen(SERVER_IS_DISCONNECT) + 1;

    for (i = 0; i < MAX_SLOT_COUNT; ++i) {
        if (slots[i].active != 0 && sendto(server_sock, SERVER_IS_DISCONNECT, k, 0,
                                           (const struct sockaddr *)(&slots[i].addr), sizeof(slots[i].addr)) < 0) {

            fprintf(stderr, "Failed to send \"message server disconnect\"\n");
        }
    }

    if (remove(".config") != 0) {
        perror("Failed while deleting .config");
    }   

    if (close(server_sock) != 0) {
        perror("Failed while closing server socket");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void sigint_handler(int)
{
    running = 0;
}

static void *recv_handler(int *pserver_sock)
{
    struct sockaddr_in addr;
    socklen_t          addrlen = sizeof(addr);

    int i, k;

    ssize_t read;
    char    msg[256];

    for (;;) {
        read = recvfrom(*pserver_sock, msg, sizeof(msg), 0, (struct sockaddr *)(&addr), &addrlen);

        if (read < 0) {
            fprintf(stderr, "Failed while recieving frame from client\n");
            continue;
        }

        if (strncmp(msg, "Hi, Server", 10) == 0) {
            for (i = 0; i < MAX_SLOT_COUNT; ++i) {
                if (slots[i].active == 0) {
                    break;
                }
            }

            if (i == MAX_SLOT_COUNT &&
                sendto(*pserver_sock, SERVER_IS_FULL_MSG, strlen(SERVER_IS_FULL_MSG) + 1, 0,
                       (const struct sockaddr *)(&addr), sizeof(addr)) > 0) {

                continue;
            }

            sendto(*pserver_sock, "Ok", 2, 0, (const struct sockaddr *)(&addr), sizeof(addr));

            slots[i].active = 1;
            memcpy(&slots[i].addr, &addr, sizeof(addr));

            for (k = 0; k < MAX_SLOT_COUNT; ++k) {
                if (slots[k].active != 0 && k != i && sendto(*pserver_sock, msg + 4, strlen(msg + 4) + 1, 0,
                                                             (const struct sockaddr *)(&slots[k].addr), sizeof(slots[k].addr)) < 0) {
                
                    fprintf(stderr, "Failed while sending alert message\n");
                }
            }

            continue;
        } else if (strncmp(msg, "exit", 4) == 0) {
            for (i = 0; i < MAX_SLOT_COUNT; ++i) {
                if (slots[i].active != 0 && memcmp(&slots[i].addr, &addr, sizeof(addr)) == 0) {
                    break;
                }
            }   

            slots[i].active = 0;

            k = strlen(msg + 4) + 1;

            for (i = 0; i < MAX_SLOT_COUNT; ++i) {
                if (slots[i].active != 0 && sendto(*pserver_sock, msg + 4, k, 0, 
                                                   (const struct sockaddr *)(&slots[i].addr), sizeof(slots[i].addr)) < 0) {

                    fprintf(stderr, "Failed while sending alert message\n");
                }
            }
        
            continue;
        }

        for (i = 0; i < MAX_SLOT_COUNT; ++i) {
            if (slots[i].active != 0 && memcmp(&slots[i].addr, &addr, sizeof(addr)) == 0) {
                break;
            }
        }

        for (k = 0; k < MAX_SLOT_COUNT; ++k) {
            if (k != i && slots[k].active != 0 && sendto(*pserver_sock, msg, read, 0, 
                (const struct sockaddr *)(&slots[k].addr), sizeof(slots[k].addr)) < 0) {
                fprintf(stderr, "Failed while sending message\n");
            }
        }
    }
}
