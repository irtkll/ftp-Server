#include <stdio.h>
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
#include <ifaddrs.h>
// struct asdfg
// {
//     int sock1;
//     XImage *image;
//     int datalen;
// };
struct 传输连接参数
{
    int sock;
    int 传输模式;
    int *传输是否中断;
    char *传输内容;
    char *传输类型;
    char *当前工作目录;
    struct sockaddr_in 传输端;
    int 传输端len;
};
void *传输连接(void *num)
{
    int sock3;
    if ((*(struct 传输连接参数 *)num).传输模式)
    {
        if ((sock3 = accept((*(struct 传输连接参数 *)num).sock, (struct sockaddr *)&(*(struct 传输连接参数 *)num).传输端, &(*(struct 传输连接参数 *)num).传输端len)) == -1)
        {
            perror("传输端口accept()函数出错");
            return 0;
        }
    }
    else
    {
        sock3 = (*(struct 传输连接参数 *)num).sock;
        if (connect((*(struct 传输连接参数 *)num).sock, (struct sockaddr *)&(*(struct 传输连接参数 *)num).传输端, (*(struct 传输连接参数 *)num).传输端len) == -1)
        {
            perror("传输端口connect()函数出错");
            return 0;
        }
    }
    if (!strcmp((*(struct 传输连接参数 *)num).传输内容, "list"))
    {
        if (send(sock3, "-rwx------ 1 user group 24 Jun 11 2009 autoexec.bat\r\n", 53, 0) == -1)
        {
            perror("传输端口数据传输出错");
            return 0;
        }

        close(sock3);
    }
    int 返回值 = 1;
    pthread_exit(&返回值);
}
void *控制连接(void *num1)
{
    struct sockaddr_in 传输端;
    传输端.sin_addr.s_addr = INADDR_ANY;
    传输端.sin_port = htons(0 /*(*(((struct num *)num1)->端口数))*/);
    传输端.sin_family = AF_INET;
    int 传输端len = sizeof(传输端);
    int 传输模式 = 1;
    int sock2;
    char *当前工作目录 = getcwd(NULL, 1024);
    char *传输类型 = realloc(NULL, 1);
    char s[100] = {0};
    char **字符串数;
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("传输端口套接字创建出错（if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)）");
        return 0;
    }
    if (send(*(int *)num1, "220 asdsad\r\n" /* Service ready for new user\n*/, 12, 0) == -1)
    {
        perror("控制端口发送错误（220 asdsad\r\n）");
        close(*(int *)num1);
        int 返回值 = 0;
        pthread_exit(&返回值);
    }
    while (1)
    {
        字符串数 = calloc(0, 0);
        int recv返回值 = recv(*(int *)num1, s, 100, 0);
        if (recv返回值 == -1)
        {
            perror("控制端口接收错误（ int recv返回值 = recv(*(int*)num1, s, 100, 0);）");
            break;
        }
        else if (recv返回值 == 0)
        {
            break;
        }
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
                if (send(*(int *)num1, "331 adawdawd3\r\n", 15, 0) == -1)
                {
                    perror("控制端口发送错误（331 adawdawd3\r\n）");
                    break;
                }
            }
        }
        else if (!strcmp(字符串数[0], "pass"))
        {
            if (send(*(int *)num1, "230 fgdf\r\n", 10, 0) == -1)
            {
                perror("控制端口发送错误（230 fgdf\r\n）");
                break;
            }
        }

        else if (!strcmp(字符串数[0], "syst"))
        {
            if (send(*(int *)num1, "215 UNIX type:L8\r\n", 18, 0) == -1)
            {
                perror("控制端口发送错误（215 UNIX type:L8\r\n）");
                break;
            }
        }
        else if (!strcmp(字符串数[0], "pwd"))
        {
            char *回复字符串 = realloc(NULL, 6 + strlen(当前工作目录));
            memcpy(回复字符串, "257 ", 4);
            memcpy(&回复字符串[4], 当前工作目录, strlen(当前工作目录));
            回复字符串[4 + strlen(当前工作目录)] = '\r';
            回复字符串[4 + strlen(当前工作目录) + 1] = '\n';
            if (send(*(int *)num1, 回复字符串, strlen(回复字符串), 0) == -1)
            {
                perror("控制端口发送错误（215 UNIX type:L8\r\n）");
                break;
            }
            free(回复字符串);
        }

        else if (!strcmp(字符串数[0], "cwd"))
        {
            // char* 回复字符串 = realloc(NULL, 3);
            struct stat num;
            if (stat(字符串数[1], &num) != 0)
            {
                if (send(*(int *)num1, "550 sdfa\r\n", 10, 0) == -1)
                {
                    perror("控制端口发送错误（550 sdfa\r\n）");
                    break;
                }
            }
            else
            {
                if (send(*(int *)num1, "250 adaseasdas\r\n", 16, 0) == -1)
                {
                    perror("控制端口发送错误（250 adaseasdas\r\n）");
                    break;
                }
                当前工作目录 = 字符串数[1];
            }
        }

        else if (!strcmp(字符串数[0], "type"))
        {
            传输类型[0] = 字符串数[1][0];
            if (send(*(int *)num1, "200 dfrsdfast\r\n", 15, 0) == -1)
            {
                perror("控制端口发送错误（200 dfrsdfast\r\n）");
                break;
            }
        }

        else if (!strcmp(字符串数[0], "pasv"))
        {
            // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
            // memcpy(回复字符串, "227 ", 4);
            传输模式 = 1;
            // 传输连接(被动模式)
            if (bind(sock2, (struct sockaddr *)&传输端, 传输端len) == -1)
            {
                perror("传输端口套接字绑定出错");
                return 0;
            }
            if (listen(sock2, 10) == -1)
            {
                perror("传输端口套接字监听出错");
                return 0;
            }
            struct sockaddr_in 临时变量;
            getsockname(*(int *)num1, (struct sockaddr *)&临时变量, &传输端len);
            char *ip地址 = inet_ntoa(临时变量.sin_addr);
            getsockname(sock2, (struct sockaddr *)&临时变量, &传输端len);
            for (int i = 0; i < strlen(ip地址); i++)
                if (ip地址[i] == '.')
                    ip地址[i] = ',';
            // getsockname(sock2, (struct sockaddr*)&传输端, &传输端len);
            int 端口号加斗号的长度 = 1;
            int 端口号高位字节 = ntohs(临时变量.sin_port) / 256;
            if (端口号高位字节 < 10)
                端口号加斗号的长度 += 1;
            else if (端口号高位字节 < 100)
                端口号加斗号的长度 += 2;
            else
                端口号加斗号的长度 += 3;
            int 端口号低位字节 = ntohs(临时变量.sin_port) % 256;
            if (端口号低位字节 < 10)
                端口号加斗号的长度 += 1;
            else if (端口号低位字节 < 100)
                端口号加斗号的长度 += 2;
            else
                端口号加斗号的长度 += 3;
            char *回复字符串 = realloc(NULL, 9 + strlen(ip地址) + 端口号加斗号的长度);
            memcpy(回复字符串, "227 (", 5);
            memcpy(&回复字符串[5], ip地址, strlen(ip地址));
            sprintf(&回复字符串[5 + strlen(ip地址)], ",%d,%d", 端口号高位字节, 端口号低位字节);
            memcpy(&回复字符串[6 + 端口号加斗号的长度 + strlen(ip地址)], ")\r\n", 3);
            // printf("%d", ntohs(传输端.sin_port));
            // printf(回复字符串);
            // printf(inet_ntoa(传输端.sin_addr));
            if (send(*(int *)num1, 回复字符串, 8 + strlen(ip地址) + 端口号加斗号的长度, 0) == -1)
            {
                perror("控制端口发送错误（if (send(*(int*)num1, 回复字符串, 8 + strlen(ip地址) + 端口号加斗号的长度, 0) == -1)）");
                break;
            }
            // free(ip地址);
            free(回复字符串);
            // pthread_t 线程ID;
            // pthread_create(线程ID, NULL, 传输连接, &sock3);
        }
        else if (!strcmp(字符串数[0], "port"))
        {
            // if (send(*(int *)num1, "202 fsdfaf\r\n", 12, 0) == -1)
            // {
            //     perror("控制端口发送错误(202 fsdfaf\r\n)");
            //     break;
            // }
            传输模式 = 0;
            unsigned char ip地址[4];
            int 端口[2];
            char 点分十进制ip地址[12];
            sscanf(字符串数[1], "%d,%d,%d,%d,%d,%d", &ip地址[0], &ip地址[1], &ip地址[2], &ip地址[3], &端口[0], &端口[1]);
            sprintf(点分十进制ip地址, "%d.%d.%d.%d", ip地址[0], ip地址[1], ip地址[2], ip地址[3]);
            传输端.sin_addr.s_addr = inet_addr((const char *)&点分十进制ip地址);
            传输端.sin_port = htons(端口[0] * 256 + 端口[1]);
            // for (int i = 0, j = 0,y=0; i < strlen(字符串数[1]); i++)
            // {
            //     char *num = realloc(NULL, j + 1);
            //     if (字符串数[1][i] == ','){
            //         ip地址_端口 = realloc(ip地址_端口, y+1);
            //         ip地址_端口[y] = num;
            //         y++;
            //         j=0;
            //         continue;
            //     }
            //     num[i] = 字符串数[1][i];
            //     j++;
            // }
            // char
            if (send(*(int *)num1, "200 fsdfaf\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误(200 fsdfaf\r\n)");
                break;
            }
        }
        else if (!strcmp(字符串数[0], "list"))
        {
            // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
            // memcpy(回复字符串, "227 ", 4);
            pthread_t 线程ID;
            struct 传输连接参数 参数;
            参数.sock = sock2;
            参数.传输内容 = 字符串数[0];
            参数.传输是否中断 = 0;
            参数.传输模式 = 传输模式;
            参数.传输类型 = 传输类型;
            参数.当前工作目录 = 当前工作目录;
            参数.传输端 = 传输端;
            参数.传输端len = 传输端len;
            pthread_create(&线程ID, NULL, 传输连接, &参数);
            if (send(*(int *)num1, "150 File status okay; about to open data connection.\r\n", 54, 0) == -1)
            {
                perror("控制端口发送错误（150 File status okay; about to open data connection.\r\n）");
                break;
            }
            // printf("asdad");

            // if (send(((struct num*)*(int*)num1)->sock, "125 dsfsdf\n", 11, 0) == -1)
            // {
            //     perror("");
            //     exit(1);
            // }
            int 传输连接返回值;
            pthread_join(线程ID, (void *)&传输连接返回值);
            if (send(*(int *)num1, "226 fsdfaf\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误(226 fsdfaf\r\n)");
                break;
            }
            // DIR *目录流 = opendir(当前工作目录);
            // struct stat num;
            // stat(当前工作目录, &num);
        }
        else if (!strcmp(字符串数[0], "quit"))
        {
            // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
            // memcpy(回复字符串, "227 ", 4);
            if (send(*(int *)num1, "221 srsaer\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误（221 srsaer\r\n）");
                break;
            }
            break;
        }
        else
        {
            if (send(*(int *)num1, "500 gtuii7\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误（500 gtuii7\r\n）");
                break;
            }
        }
        // pthread_create(&线程ID, NULL, 控制连接, &*((struct num*)*(int*)num1)->sock);
        // sleep(1);
        for (int i = 0; i < sizeof(字符串数) / sizeof(char **); i++)
        {
            free(字符串数[i]);
        }
        free(字符串数);
    }
    // char s[100] = {0};
    // while (1)
    // {
    //     if (recv(*(int *)sock, s, 100, 0) == -1)
    //     {
    //         perror("");
    //         exit(1);
    //     }
    //     char **字符串数 = realloc(NULL, 0);
    //     for (int i = -1, z = 1; s[i] != '\r\n'; z++)
    //     {
    //         i++;
    //         char *字符串 = realloc(NULL, 1);
    //         for (int j = 0; s[i] != ' ' && s[i] != '\r\n'; j++, i++)
    //         {
    //             字符串 = realloc(字符串, j + 2);
    //             字符串[j] = s[i];
    //         }
    //         字符串数 = realloc(字符串数, sizeof(char *) * z);
    //         字符串数[z - 1] = 字符串;
    //         // num[0]=字符串;
    //         free(字符串);
    //     }
    //     // free(字符串数);
    //     // char strd[2] = {0};
    //     // if (recv(((struct asdfg *)num)->*((struct num*)*(int*)num1)->sock, strd, 2, 0) == -1)
    //     // {
    //     //     perror("");
    //     // }
    for (int i = 0; i < sizeof(字符串数) / sizeof(char **); i++)
    {
        free(字符串数[i]);
    }
    free(字符串数);
    close(sock2);
    close(*(int *)num1);
    int 返回值 = 0;
    pthread_exit(&返回值);
}
int main()
{
    seteuid(0);
    // char *对应字符 = "0123456789";
    struct sockaddr_in 命令端;
    命令端.sin_addr.s_addr = 0x0;
    命令端.sin_port = htons(2223);
    命令端.sin_family = AF_INET;
    int 命令端len = sizeof(命令端);

    // 控制连接
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("控制端口套接字创建错误");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&命令端, sizeof(命令端)) == -1)
    {
        perror("控制端口套接字绑定错误");
        exit(1);
    }
    if (listen(sock, 10) == -1)
    {
        perror("控制端口监听错误");
        exit(1);
    }
    int sock1;
    pthread_t 线程ID;
    while (1)
    {
        if ((sock1 = accept(sock, (struct sockaddr *)&命令端, &命令端len)) == -1)
        {
            perror("控制端口accept()函数错误");
            exit(1);
        }
        // struct num 参数 = {sock1, 对应字符};
        pthread_create(&线程ID, NULL, 控制连接, &sock1);
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
