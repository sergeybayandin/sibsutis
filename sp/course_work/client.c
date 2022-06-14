#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <pthread.h>

#include <ncurses.h>

#define HI_SERVER_MSG "Hi, Server"

int current_row;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *recv_handler(int *pclient_sock);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s [ your_name ]\n", *argv);
        return EXIT_FAILURE;
    }

    const char *username      = argv[1];
    const int   username_size = strlen(username);

    int client_sock;

    if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = 0;

    if (bind(client_sock, (struct sockaddr *)(&addr), sizeof(addr)) != 0) {
        perror("Failed to bind client socket");
        return EXIT_FAILURE;
    }
    
    FILE *config;

    if ((config = fopen(".config", "rb")) == NULL) {
        perror("Failed to open .config");
        return EXIT_FAILURE;
    }

    if (fread(&addr, sizeof(addr), 1, config) < 1 && ferror(config) != 0) {
        perror("Failed while reading .config");
        return EXIT_FAILURE;
    }

    if (fclose(config) != 0) {
        perror("Failed to close .config");
        return EXIT_FAILURE;
    }
    
    char msg[256];

    memset(msg, sizeof(msg), 0);

    sprintf(msg, "%s: %s entered the chat!", HI_SERVER_MSG, username);

    if (sendto(client_sock, msg, strlen(HI_SERVER_MSG) + username_size + 21, 0, (const struct sockaddr *)(&addr), sizeof(addr)) < 0) {
        fprintf(stderr, "Failed to send \"Hi, Server!\"\n");
        return EXIT_FAILURE;
    }

    socklen_t addrlen = sizeof(addr);
    
    if (recvfrom(client_sock, msg, sizeof(msg), 0, (struct sockaddr *)(&addr), &addrlen) < 0) {
        fprintf(stderr, "Failed while recieving response from server\n");
        return EXIT_FAILURE;
    }

    if (strcmp(msg, "Server is full!") == 0) {
        fprintf(stderr, "%s\n", msg);
        return EXIT_FAILURE;
    }

    if (initscr() == NULL) {
        fprintf(stderr, "Failed to init ncurses\n");
        return EXIT_FAILURE;
    } 
    
    current_row = 0;

    pthread_t tid;

    if (pthread_create(&tid, NULL, (void *(*)(void *))recv_handler, &client_sock) != 0) {
        endwin();
        fprintf(stderr, "Failed to create recv thread\n");
        return EXIT_FAILURE;
    }

    memcpy(msg, username, username_size);

    msg[username_size] = ':';
    msg[username_size + 1] = ' ';

    int    msg_content_size;
    char  *msg_content = msg + username_size + 2;

    ssize_t read;

    int i, offset;

    for (;;) {
        mvprintw(LINES - 1, 0, "Type: ");

        move(LINES - 1, 6);

        refresh();
            
        memset(msg_content, sizeof(msg) - username_size - 2, 0);

        if (getnstr(msg_content, sizeof(msg) - username_size - 2) == ERR) {
            continue;
        }

        if (*msg_content == 0) {
            continue;
        }
        
        if (strcmp(msg_content, "exit") == 0) {
            sprintf(msg_content + 4, "Server: %s left the chat", username);
            if (sendto(client_sock, msg_content, username_size + 27, 0, (const struct sockaddr *)(&addr), sizeof(addr)) > 0) {
                break;
            }
        }

        msg_content_size = strlen(msg_content);
        offset           = (msg_content_size + 4) / COLS + ((msg_content_size + 4) % COLS != 0);

        pthread_mutex_lock(&mutex);

        if (current_row + offset >= LINES - 1) {
            clear();
            current_row = 0;
            mvprintw(LINES - 1, 0, "Type: ");
            move(LINES - 1, 6);
            refresh();
        }

        mvprintw(current_row, 0, "Me: %s",  msg_content);

        current_row += offset;

        pthread_mutex_unlock(&mutex);

        move(LINES - 1, 6);

        clrtobot();

        refresh();

        if (sendto(client_sock, msg, username_size + 2 + msg_content_size + 1, 0, (const struct sockaddr *)(&addr), sizeof(addr)) < 0) {
            // Handle error
        }
    }

    endwin();

    if (pthread_cancel(tid) != 0) {
        fprintf(stderr, "Failed to cancel recv thread\n");
        return EXIT_FAILURE;
    }
    
    if (close(client_sock) != 0) {
        perror("Failed while closing client socket");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

static void *recv_handler(int *pclient_sock)
{
    ssize_t read;

    struct sockaddr_in server_addr;
    socklen_t          server_addrlen = sizeof(server_addr);

    char msg[256];
    int  msg_size;

    int offset;
    
    for (;;) {
        read = recvfrom(*pclient_sock, msg, sizeof(msg), 0,
                        (struct sockaddr *)(&server_addr), &server_addrlen);

        if (read < 0) {
            continue;
        }

        if (strcmp(msg, "Server is disconnect") == 0) {
            endwin();
            exit(0);
        }

        msg_size = strlen(msg);
        offset   = msg_size / COLS + (msg_size % COLS != 0);

        pthread_mutex_lock(&mutex);

        if (current_row + offset >= LINES - 1) {
            clear();
            current_row = 0;
            mvprintw(LINES - 1, 0, "Type: ");
            move(LINES - 1, 6);
            refresh();
        }

        mvprintw(current_row, 0, msg);

        current_row += offset;

        pthread_mutex_unlock(&mutex);

        move(LINES - 1, 6);

        refresh();
    }
}
