#include <stdio.h>//sdfsdf
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
// #include<time.h>
#include <unistd.h>
// struct asdfg
// {
//     int sock1;
//     XImage *image;
//     int datalen;
// };
void *控制连接(void *sock)
{
    char s[100] = {0};
    while (1)
    {
        if (recv(*(int *)sock, s, 100, 0) == -1)
        {
            perror("");
            exit(1);
        }
        char **字符串数 = realloc(NULL, 0);
        for (int i = -1, z = 1; s[i] != '\r\n'; z++)
        {
            i++;
            char *字符串 = realloc(NULL, 1);
            for (int j = 0; s[i] != ' ' && s[i] != '\r\n'; j++, i++)
            {
                字符串 = realloc(字符串, j + 2);
                字符串[j] = s[i];
            }
            字符串数 = realloc(字符串数, sizeof(char *) * z);
            字符串数[z - 1] = 字符串;
            // num[0]=字符串;
            free(字符串);
        }
        // free(字符串数);
        // char strd[2] = {0};
        // if (recv(((struct asdfg *)num)->sock1, strd, 2, 0) == -1)
        // {
        //     perror("");
        // }
    }
}
int main()
{
    seteuid(0);
    char *当前工作目录 = getcwd(NULL, 1024);
    char *传输模式 = realloc(NULL, 1);
    struct sockaddr_in 传输端;
    传输端.sin_addr.s_addr = 0x0;
    传输端.sin_port = htons(2221);
    传输端.sin_family = AF_INET;
    int 传输端len = sizeof(传输端);
    struct sockaddr_in 命令端;
    命令端.sin_addr.s_addr = 0x0;
    命令端.sin_port = htons(2223);
    命令端.sin_family = AF_INET;
    int 命令端len = sizeof(命令端);

    int sock3;
    // 传输连接(被动模式)
    int sock2;
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("");
        exit(1);
    }
    if (bind(sock2, (struct sockaddr *)&传输端, sizeof(传输端)) == -1)
    {
        perror("");
        exit(1);
    }
    if (listen(sock2, 10) == -1)
    {
        perror("");
        exit(1);
    }
    // 控制连接
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&命令端, sizeof(命令端)) == -1)
    {
        perror("");
        exit(1);
    }
    if (listen(sock, 10) == -1)
    {
        perror("");
        exit(1);
    }
    int sock1;
    pthread_t 线程ID;
    while (1)
    {
        if ((sock1 = accept(sock, (struct sockaddr *)&命令端, &命令端len)) == -1)
        {
            perror("");
            exit(1);
        }
        if (send(sock1, "220\n", 4, 0) == -1)
        {
            perror("");
            exit(1);
        }
        char s[100] = {0};
        while (1)
        {
            if (recv(sock1, s, 100, 0) == -1)
            {
                perror("");
                exit(1);
            }
            char **字符串数 = calloc(0, 0);
            for (int i = -1, z = 1; s[i] != '\n' && s[i] != '\r'; z++)
            {
                i++;
                char *字符串 = calloc(0, 0);
                for (int j = 0; s[i] != ' ' && s[i] != '\n' && s[i] != '\r'; j++, i++)
                {
                    字符串 = realloc(字符串, j + 2);
                    字符串[j] = s[i];
                    字符串[j + 1] = 0;
                }
                字符串数 = realloc(字符串数, sizeof(char *) * z);
                字符串数[z - 1] = 字符串;
                // num[0]=字符串;
                // free(字符串);
            }
            // return 0;
            // 字符串小写转换
            for (int i = 0; i < strlen(字符串数[0]); i++)
            {
                字符串数[0][i] = tolower(字符串数[0][i]);
            }

            if (!strcmp(字符串数[0], "user"))
            {
                if (!strcmp(字符串数[1], "anonymous"))
                {
                    if (send(sock1, "331\n", 4, 0) == -1)
                    {
                        perror("");
                        exit(1);
                    }
                }
            }
            else if (!strcmp(字符串数[0], "pass"))
            {
                if (send(sock1, "230\n", 4, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
            }

            else if (!strcmp(字符串数[0], "syst"))
            {
                if (send(sock1, "215 UNIX type:L8\n", 17, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
            }
            else if (!strcmp(字符串数[0], "pwd"))
            {
                char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
                memcpy(回复字符串, "257 ", 4);
                memcpy(&回复字符串[4], 当前工作目录, strlen(当前工作目录));
                回复字符串[4 + strlen(当前工作目录)] = '\n';
                if (send(sock1, 回复字符串, strlen(回复字符串), 0) == -1)
                {
                    perror("");
                    exit(1);
                }
                free(回复字符串);
            }

            else if (!strcmp(字符串数[0], "cwd"))
            {
                // char* 回复字符串 = realloc(NULL, 3);
                struct stat num;
                if (stat(字符串数[1], &num) != 0)
                {
                    if (send(sock1, "550\n", 4, 0) == -1)
                    {
                        perror("");
                        exit(1);
                    }
                }
                else
                {
                    if (send(sock1, "250\n", 4, 0) == -1)
                    {
                        perror("");
                        exit(1);
                    }
                    当前工作目录 = 字符串数[1];
                }
            }

            else if (!strcmp(字符串数[0], "type"))
            {
                传输模式[0] = 字符串数[1][0];
                if (send(sock1, "200\n", 4, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
            }

            else if (!strcmp(字符串数[0], "pasv"))
            {
                // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
                // memcpy(回复字符串, "227 ", 4);
                if (send(sock1, "227 (192,168,2,25,8,173)\n", 25, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
                if ((sock3 = accept(sock2, (struct sockaddr *)&传输端, &传输端len)) == -1)
                {
                    perror("");
                    exit(1);
                }
                printf("asdad");
            }
            else if (!strcmp(字符串数[0], "list"))
            {
                // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
                // memcpy(回复字符串, "227 ", 4);
                if (send(sock1, "150\n", 4, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
                if (send(sock1, "125\n", 4, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
                                if (send(sock3, "adasdfasd\n", 10, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
                if (send(sock1, "226\n", 4, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
                DIR *目录流 = opendir(当前工作目录);
                struct stat num;
                stat(当前工作目录, &num);
            }
            else if (!strcmp(字符串数[0], "quit"))
            {
                // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
                // memcpy(回复字符串, "227 ", 4);
                if (send(sock3, "221\n", 4, 0) == -1)
                {
                    perror("");
                    exit(1);
                }
            }
            // pthread_create(&线程ID, NULL, 控制连接, &sock1);
            // sleep(1);
            for (int i = 0; i < sizeof(字符串数) / sizeof(char **); i++)
            {
                free(字符串数[i]);
            }
            free(字符串数);
        }
    }

    // if (recv(sock1, image, sizeof(XImage), 0) == -1)
    // {
    //     perror("");
    //     exit(1);
    // }

    //         int num1 = 0;
    //         while (num1<datalen)
    //         {

    //         if ((num1 += recv(sock1, &image->data[num1], datalen, 0)) == -1)
    //         {
    //             perror("");
    //             exit(1);
    //         }
    //         }

    return 0;
}
