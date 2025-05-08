#include <stdio.h>
#include <stdlib.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
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
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
// #include <unistd.h>
#include <ifaddrs.h>
#include <errno.h>
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
    char 传输类型[2];
    char 当前工作目录[256];
    int 当前工作目录len;
    struct sockaddr_in 传输端;
    int 传输端len;
    // char 文件名[100];
};
void *传输连接(void *num1)
{
    printf("进入传输连接\n");
    struct 传输连接参数 *num = num1;
    int sock3;
    if (num->传输模式)
    {
        if ((sock3 = accept(num->sock, (struct sockaddr *)&num->传输端, &num->传输端len)) == -1)
        {
            perror("传输端口accept()函数出错");
            goto end;
        }
    }
    else
    {
        sock3 = num->sock;
        if ((sock3 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket()函数出错\n");
            printf("退出传输连接\n");
            int 返回值 = 1;
            pthread_exit(&返回值);
        }
        if (connect(sock3, (struct sockaddr *)&num->传输端, num->传输端len) == -1)
        {
            perror("传输端口connect()函数出错");
            goto end;
        }
    }
    if (!strcmp(num->传输内容, "stor"))
    {
        char str[1024] = {0};
        int recv返回值;
        FILE *f = *num->传输类型 == 'I' ? fopen(num->当前工作目录, "wb") : fopen(num->当前工作目录, "wt");
        if (!f)
        {
            perror("文件打开失败");
            goto end;
        }
        while (1)
        {
            if ((recv返回值 = recv(sock3, str, 1024, 0)) == -1)
            {
                perror("传输端口接收错误");
                break;
            }
            else if (!recv返回值)
            {
                printf("文件传输完成\n");
                break;
            }
            if (recv返回值 == 1024)
            {
                printf("文件传输中: %d\n", recv返回值);
                fwrite(str, 1024, 1, f);
            }
            else
                fwrite(str, recv返回值, 1, f);
        }
        fclose(f);
    }

    if (!strcmp(num->传输内容, "retr"))
    {
    // char 文件路径[256] = {0};
    // if (num->当前工作目录[num->当前工作目录len - 1] == '/')
    // {
    //     sprintf(文件路径, "%s%s", num->当前工作目录, num->文件名);
    // }
    // else
    // {
    //     sprintf(文件路径, "%s/%s", num->当前工作目录, num->文件名);
    // }
    文件传输:
        char str[1024] = {0};
        FILE *f = *num->传输类型 == 'I' ? fopen(num->当前工作目录, "rb") : fopen(num->当前工作目录, "rt");
        if (!f)
        {
            printf("%s\n", num->当前工作目录);
            perror("文件打开失败");
            goto end;
        }
        fseek(f, 0, SEEK_END);
        unsigned 文件大小 = ftell(f);
        fseek(f, 0, SEEK_SET);
        printf("文件大小为%d\n", 文件大小);
        for (int i = 0; i < 文件大小; i += 1024)
        {
            if (文件大小 - i < 1024)
            {
                fread(str, 文件大小 - i, 1, f);
                if (send(sock3, str, 文件大小 - i, 0) == -1)
                {
                    perror("传输端口数据发送错误");
                    break;
                }
                printf("已发送%d字节\n", 文件大小 - i);
            }
            else
            {
                fread(str, 1024, 1, f);
                if (send(sock3, str, 1024, 0) == -1)
                {
                    perror("传输端口数据发送错误");
                    break;
                }
                printf("已发送%d字节\n", 1024);
            }
        }
        // sleep(1);
    }
    else if (!strcmp(num->传输内容, "list"))
    {
        DIR *dir = opendir(num->当前工作目录);
        if (!dir)
        {
            perror("打开目录出错");
            printf("%s\n", num->当前工作目录);
            goto end;
            // closedir(dir);
            // goto 文件传输;
        }
        struct dirent *entry;
        while (entry = readdir(dir))
        {
            sprintf(&num->当前工作目录[num->当前工作目录len], "/%s\0", entry->d_name);
            struct stat 状态;
            if (stat(num->当前工作目录, &状态))
            {
                perror("状态读取出错");
                printf("%s\n", num->当前工作目录);
                continue;
            }
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            {
                continue;
            }

            char str[256] = {0};
            str[0] = (状态.st_mode & 0040000) ? 'd' : '-'; // 文件类型：目录或普通文件
            str[1] = (状态.st_mode & S_IRUSR) ? 'r' : '-'; // 用户读权限
            str[2] = (状态.st_mode & S_IWUSR) ? 'w' : '-'; // 用户写权限
            str[3] = (状态.st_mode & S_IXUSR) ? 'x' : '-'; // 用户执行权限
            str[4] = (状态.st_mode & S_IRGRP) ? 'r' : '-'; // 组读权限
            str[5] = (状态.st_mode & S_IWGRP) ? 'w' : '-'; // 组写权限
            str[6] = (状态.st_mode & S_IXGRP) ? 'x' : '-'; // 组执行权限
            str[7] = (状态.st_mode & S_IROTH) ? 'r' : '-'; // 其他用户读权限
            str[8] = (状态.st_mode & S_IWOTH) ? 'w' : '-'; // 其他用户写权限
            str[9] = (状态.st_mode & S_IXOTH) ? 'x' : '-'; // 其他用户执行权限
            str[10] = ' ';
            sprintf(&str[11], "%d ", 状态.st_nlink);
            struct passwd *所属用户 = getpwuid(状态.st_uid);
            struct group *所属用户组 = getgrgid(状态.st_gid);
            sprintf(&str[strlen(str)], "%s ", 所属用户->pw_name);
            sprintf(&str[strlen(str)], "%s ", 所属用户组->gr_name);
            sprintf(&str[strlen(str)], "%d ", 状态.st_size);
            strftime(&str[strlen(str)], 30, "%b %d %H:%M ", localtime(&状态.st_mtime));
            sprintf(&str[strlen(str)], "%s", entry->d_name);
            sprintf(&str[strlen(str)], "\r\n");
            if (send(sock3, str, strlen(str), 0) == -1)
            {
                perror("传输端口数据传输出错");
                break;
            }
        }
        closedir(dir);
    }
end:
    // if (num->传输模式)
    // {
    close(sock3);
    // }
    printf("退出传输连接\n");
    int 返回值 = 1;
    pthread_exit(&返回值);
}
void *控制连接(void *num1)
{
    int sock = (int)num1;
    printf("进入控制连接\n");
    struct sockaddr_in 传输端;
    传输端.sin_addr.s_addr = INADDR_ANY;
    传输端.sin_port = htons(0 /*(*(((struct num *)num1)->端口数))*/);
    传输端.sin_family = AF_INET;
    int 传输端len = sizeof(传输端);
    int 传输模式 = 0;
    int 结束循环 = 0;
    int sock2;
    char *当前工作目录 = getcwd(NULL, 1024);
    char 传输类型[2] = {0};
    char s[100] = {0};
    char **字符串数 = calloc(0, 0);
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("传输端口套接字创建出错（if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)）");
        close(sock);
        printf("退出控制连接\n");
        int 返回值 = 0;
        pthread_exit(&返回值);
    }
    if (send(sock, "220 asdsad\r\n" /* Service ready for new user\n*/, 12, 0) == -1)
    {
        perror("控制端口发送错误（220 asdsad\r\n）");
        close(sock);
        close(sock2);
        printf("退出控制连接\n");
        int 返回值 = 0;
        pthread_exit(&返回值);
    }
    while (!结束循环)
    {
        int recv返回值 = recv(sock, s, 100, 0);
        if (recv返回值 == -1)
        {
            perror("控制端口接收错误（ int recv返回值 = recv(*(int*)num1, s, 100, 0);）");
            break;
        }
        else if (recv返回值 == 0)
        {
            printf("连接断开\n");
            break;
        }
        int 命令参数数 = 0;
        for (int i = -1, z = 1, c = 1; s[i] != '\n' && s[i] != '\r'; z++)
        {
            i++;
            char *字符串 = calloc(1, 2);
            for (int j = 0; s[i] != ' ' && s[i] != '\n' && s[i] != '\r' && c; j++, i++)
            {
                if (字符串[j] == '"')
                    ~c;
                字符串 = realloc(字符串, j + 2);
                字符串[j] = s[i];
                字符串[j + 1] = 0;
            }
            字符串数 = realloc(字符串数, sizeof(char *) * z);
            字符串数[z - 1] = 字符串;
            命令参数数 = z;
            // num[0]=字符串;
            // free(字符串);
        }
        // 字符串数内存是否已释放 = 0;
        // return 0;
        // 字符串小写转换
        for (int i = 0; i < strlen(字符串数[0]); i++)
        {
            字符串数[0][i] = tolower(字符串数[0][i]);
        }

        if (!strcmp(字符串数[0], "user"))
        {
            if (!strcmp(字符串数[1], "anonymous") || !strcmp(字符串数[1], ".."))
            {
                if (send(sock, "331  用户名正确\r\n", 22, 0) == -1)
                {
                    perror("控制端口发送错误（331 用户名正确\r\n）");
                    结束循环 = 1;
                    goto 循环末尾;
                }
            }
            else
            {
                if (send(sock, "501 用户名错误\r\n", 21, 0) == -1)
                {
                    perror("控制端口发送错误（501 用户名错误\r\n）");
                    结束循环 = 1;
                    goto 循环末尾;
                }
            }
        }
        else if (!strcmp(字符串数[0], "pass"))
        {
            if (send(sock, "230 fgdf\r\n", 10, 0) == -1)
            {
                perror("控制端口发送错误（230 fgdf\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
        }

        else if (!strcmp(字符串数[0], "syst"))
        {
            if (send(sock, "215 UNIX type:L8\r\n", 18, 0) == -1)
            {
                perror("控制端口发送错误（215 UNIX type:L8\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
        }
        else if (!strcmp(字符串数[0], "pwd"))
        {
            // getcwd(当前工作目录, 1024);
            char 回复字符串[300] = {0};
            memcpy(回复字符串, "257 ", 4);
            sprintf(&回复字符串[4], "\"%s\"\r\n", 当前工作目录);
            if (send(sock, 回复字符串, strlen(回复字符串), 0) == -1)
            {
                perror("控制端口发送错误 pwd");
                结束循环 = 1;
                goto 循环末尾;
            }
            // printf("释放回复字符串内存\n");
            // free(回复字符串);
        }
        else if (!strcmp(字符串数[0], "cdup"))
        {
            char 当前工作目录num[1024] = {0};
            char 命令[550] = {0};
            sprintf(命令, "cd \"%s\" && cd .. && pwd", 当前工作目录);
            printf("执行命令：%s\n", 命令);
            FILE *f = popen(命令, "r");
            if (!f)
            {
                perror("cdup popen执行命令失败");
                if (send(sock, "550 adaseasdas\r\n", 16, 0) == -1)
                {
                    perror("cdup 控制端口发送错误（550 adaseasdas\r\n）");
                    结束循环 = 1;
                }
                goto 循环末尾;
            }
            // fseek(f, 0, SEEK_END);
            // unsigned 文件大小 = ftell(f);
            // fseek(f, 0, SEEK_SET);
            if (!fgets(当前工作目录num, 1024, f))
            {
                printf("cdup 执行命令失败\n");
                if (send(sock, "550 Requested file action okay, completed.\r\n", 44, 0) == -1)
                {
                    perror("cdup 控制端口发送错误（550 Requested file action okay, completed.\r\n）");
                    结束循环 = 1;
                }
                fclose(f);
                goto 循环末尾;
            }
            当前工作目录num[strlen(当前工作目录num) - 1] = '\0';
            strcpy(当前工作目录, 当前工作目录num);
            printf("当前工作目录：%s\n", 当前工作目录);
            fclose(f);
            if (send(sock, "250 adaseasdas\r\n", 16, 0) == -1)
            {
                perror("控制端口发送错误（250 adaseasdas\r\n）");
                结束循环 = 1;
            }
            // getcwd(当前工作目录, 1024);

            // else
            // {
            //     if (send(sock, "550 sdfa\r\n", 10, 0) == -1)
            //     {
            //         perror("控制端口发送错误（550 sdfa\r\n）");
            //                         结束循环 = 1;
            // goto 循环末尾;;
            //     }
            // }
        }
        else if (!strcmp(字符串数[0], "rmd"))
        {
            char 路径[1024] = {0};
            if (字符串数[1][0] != '/')
            {
                sprintf(路径, "%s/", 当前工作目录);
            }
            strcpy(&路径[strlen(路径)], 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&路径[strlen(路径)], " %s", 字符串数[j]);
            }
            if (rmdir(路径))
            {
                printf("删除目录失败\n");
                if (send(sock, "450 删除目录失败\r\n", 24, 0))
                {
                    perror("控制端口发送错误（450 删除目录失败\r\n）");
                    结束循环 = 1;
                    goto 循环末尾;
                }
            }
            if (send(sock, "250 请求的文件操作成功完成\r\n", 39, 0) == -1)
            {
                perror("控制端口发送错误（250 请求的文件操作成功完成）");
                结束循环 = 1;
            }
            printf("删除目录：%s\n", 路径);
        }
        else if (!strcmp(字符串数[0], "mkd"))
        {
            char 路径[1024] = {0};
            if (字符串数[1][0] != '/')
            {
                sprintf(路径, "%s/", 当前工作目录);
            }
            strcpy(&路径[strlen(路径)], 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&路径[strlen(路径)], " %s", 字符串数[j]);
            }

            if (mkdir(路径, 0777))
            {
                printf("创建目录失败\n");
                if (send(sock, "550 创建目录失败\r\n", 24, 0))
                {
                    perror("控制端口发送错误（550 创建目录失败\r\n）");
                    结束循环 = 1;
                    goto 循环末尾;
                }
            }
            char 回复字符串[300] = {0};
            memcpy(回复字符串, "257 ", 4);
            sprintf(&回复字符串[4], "\"%s\"\r\n", 路径);
            if (send(sock, 回复字符串, strlen(回复字符串), 0) == -1)
            {
                perror("控制端口发送错误 mkd");
                结束循环 = 1;
            }
            printf("创建目录成功\n");
        }
        else if (!strcmp(字符串数[0], "dele"))
        {
            char 文件名[1024] = {0};
            if (字符串数[1][0] != '/')
            {
                sprintf(文件名, "%s/", 当前工作目录);
            }
            strcpy(&文件名[strlen(文件名)], 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&文件名[strlen(文件名)], " %s", 字符串数[j]);
            }
            if (remove(文件名) == -1)
            {
                printf("删除文件失败: %s\n", 文件名);
                if (errno == ENOENT)
                {
                    if (send(sock, "550 File not found.\r\n", 21, 0))
                    {
                        perror("控制端口发送错误（550 删除文件失败\r\n）");
                        结束循环 = 1;
                        goto 循环末尾;
                    }
                }
                else if (errno == EPERM)
                {
                    if (send(sock, "550 Permission denied.\r\n", 24, 0))
                    {
                        perror("控制端口发送错误（550 Permission denied.\r\n）");
                        结束循环 = 1;
                        goto 循环末尾;
                    }
                }
                else
                {
                    printf("删除文件失败，未知错误\n");
                    if (send(sock, "550 Unknown error.\r\n", 21, 0))
                    {
                        perror("控制端口发送错误（550 Unknown error.\r\n）");
                        结束循环 = 1;
                        goto 循环末尾;
                    }
                }
            }
            else if (send(sock, "250 命令执行成功\r\n", 24, 0) == -1)
            {
                perror("控制端口发送错误（250 命令执行成功\r\n）");
                结束循环 = 1;
            }
        }
        else if (!strcmp(字符串数[0], "stor"))
        {
            pthread_t 线程ID;
            struct 传输连接参数 参数 = {0};
            参数.sock = sock2;
            参数.传输内容 = 字符串数[0];
            参数.传输是否中断 = 0;
            参数.传输模式 = 传输模式;
            strcpy(参数.传输类型, 传输类型);
            if (字符串数[1][0] != '/')
            {
                if (当前工作目录[strlen(当前工作目录) - 1] == '/')
                {
                    sprintf(参数.当前工作目录, "%s", 当前工作目录);
                }
                else
                    sprintf(参数.当前工作目录, "%s/", 当前工作目录);
            }
            sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], "/%s", 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], " %s", 字符串数[j]);
            }
            参数.当前工作目录len = strlen(参数.当前工作目录);
            参数.传输端 = 传输端;
            参数.传输端len = 传输端len;
            pthread_create(&线程ID, NULL, 传输连接, &参数);
            if (send(sock, "125 File status okay; about to open data connection.\r\n", 54, 0) == -1)
            {
                perror("控制端口发送错误（125 File status okay; about to open data connection.\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
            pthread_join(线程ID, NULL);
            if (send(sock, "226 Transfer complete.\r\n", 24, 0) == -1)
            {
                perror("控制端口发送错误（226 Transfer complete.\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
        }
        else if (!strcmp(字符串数[0], "retr"))
        {
            pthread_t 线程ID;
            struct 传输连接参数 参数 = {0};
            参数.sock = sock2;
            参数.传输内容 = 字符串数[0];
            参数.传输是否中断 = 0;
            参数.传输模式 = 传输模式;
            strcpy(参数.传输类型, 传输类型);
            if (字符串数[1][0] != '/')
            {
                if (当前工作目录[strlen(当前工作目录) - 1] == '/')
                {
                    sprintf(参数.当前工作目录, "%s", 当前工作目录);
                }
                else
                    sprintf(参数.当前工作目录, "%s/", 当前工作目录);
            }
            sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], "%s", 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], " %s", 字符串数[j]);
            }
            参数.当前工作目录len = strlen(参数.当前工作目录);
            参数.传输端 = 传输端;
            参数.传输端len = 传输端len;
            pthread_create(&线程ID, NULL, 传输连接, &参数);
            if (send(sock, "125 File status okay; about to open data connection.\r\n", 54, 0) == -1)
            {
                perror("控制端口发送错误（125 File status okay; about to open data connection.\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
            pthread_join(线程ID, NULL);
            if (send(sock, "226 Transfer complete.\r\n", 24, 0) == -1)
            {
                perror("控制端口发送错误（226 Transfer complete.\r\n）");
                结束循环 = 1;
            }
        }
        else if (!strcmp(字符串数[0], "cwd"))
        {
            char 当前工作目录num[1024] = {0};
            char 命令[550] = {0};
            // if (字符串数[1][0] == '/')
            // {
            sprintf(命令, "cd \"%s\" && cd \"", 当前工作目录);
            strcpy(&命令[strlen(命令)], 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&命令[strlen(命令)], " %s", 字符串数[j]);
            }
            sprintf(&命令[strlen(命令)], "\" && pwd");
            printf("命令：%s\n", 命令);
            FILE *f = popen(命令, "r");
            if (!f)
            {
                perror("popen 执行命令失败");
                if (send(sock, "550 Requested file action okay, completed.\r\n", 44, 0) == -1)
                {
                    perror("控制端口发送错误（250 Requested file action okay, completed.\r\n）");
                    结束循环 = 1;
                }
                goto 循环末尾;
            }
            // fseek(f, 0, SEEK_END);
            // unsigned 文件大小 = ftell(f);
            // fseek(f, 0, SEEK_SET);
            if (!fgets(当前工作目录num, 1024, f))
            {
                printf("fgets 执行命令失败\n");
                if (send(sock, "550 Requested file action okay, completed.\r\n", 44, 0) == -1)
                {
                    perror("控制端口发送错误（550 Requested file action okay, completed.\r\n）");
                    结束循环 = 1;
                }
                fclose(f);
                goto 循环末尾;
            }
            当前工作目录num[strlen(当前工作目录num) - 1] = '\0';
            strcpy(当前工作目录, 当前工作目录num);
            fclose(f);
            // }
            // else
            // {
            //     if (当前工作目录[strlen(当前工作目录) - 1] == '/')
            //         sprintf(&当前工作目录[strlen(当前工作目录)], "%s", 字符串数[1]);
            //     else
            //         sprintf(&当前工作目录[strlen(当前工作目录)], "/%s", 字符串数[1]);
            //     for (int j = 2; j < 命令参数数; j++)
            //     {
            //         sprintf(&当前工作目录[strlen(当前工作目录)], " %s", 字符串数[j]);
            //     }
            // }
            // chdir(当前工作目录);
            printf("当前工作目录：%s\n", 当前工作目录);
            if (send(sock, "250 Requested file action okay, completed.\r\n", 44, 0) == -1)
            {
                perror("控制端口发送错误（250 Requested file action okay, completed.\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
            // if (chdir(字符串数[1]) == 0)
            // {
            //     if (send((int )num1, "250 adaseasdas\r\n", 16, 0) == -1)
            //     {
            //         perror("控制端口发送错误（250 adaseasdas\r\n）");
            //                         结束循环 = 1;
            // goto 循环末尾;;
            //     }
            //     getcwd(当前工作目录, 1024);
            // }
            // else
            // {

            //     // if (send((int )num1, "550 sdfa\r\n", 10, 0) == -1)
            //     // {
            //     //     perror("控制端口发送错误（550 sdfa\r\n）");
            //     //                     结束循环 = 1;
            // goto 循环末尾;;
            //     // }
            // }
        }
        else if (!strcmp(字符串数[0], "opts"))
        {
            if (!strcmp(字符串数[1], "UTF8") || !strcmp(字符串数[1], "utf8"))
            {
                if (!strcmp(字符串数[2], "ON") || !strcmp(字符串数[2], "on"))
                {
                    if (send(sock, "200 成功\r\n", 12, 0) == -1)
                    {
                        perror("控制端口发送错误（200 成功\r\n）");
                        结束循环 = 1;
                        goto 循环末尾;
                        ;
                    }
                }
                else
                {
                    if (send(sock, "504 sfas\r\n", 12, 0) == -1)
                    {
                        perror("控制端口发送错误（504asdf\r\n）");
                        结束循环 = 1;
                        goto 循环末尾;
                        ;
                    }
                }
            }
            else
            {
                if (send(sock, "504 sfas\r\n", 12, 0) == -1)
                {
                    perror("控制端口发送错误（504asdf\r\n）");
                    结束循环 = 1;
                    goto 循环末尾;
                    ;
                }
            }
        }
        else if (!strcmp(字符串数[0], "auth"))
        {
            // if (!strcmp(字符串数[1], "SSL"))
            // {
            //     if (send((int )num1, "234 dfrsdfast\r\n", 15, 0) == -1)
            //     {
            //         perror("控制端口发送错误（234 dfrsdfast\r\n）");
            //         break;
            //     }
            // }
            // else
            // {
            if (send(sock, "504 错误\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误（504 错误\r\n）");
                结束循环 = 1;
                goto 循环末尾;
                ;
            }
            // }
        }
        else if (!strcmp(字符串数[0], "type"))
        {
            传输类型[0] = 字符串数[1][0];
            if (send(sock, "200 Command okay.\r\n", 19, 0) == -1)
            {
                perror("控制端口发送错误（200 Command okay.\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
        }
        else if (!strcmp(字符串数[0], "size"))
        {
            char str[50] = {0};
            char 文件路径[256] = {0};
            if (字符串数[1][0] != '/')
            {
                sprintf(文件路径, "%s/", 当前工作目录);
            }
            strcpy(&文件路径[strlen(文件路径)], 字符串数[1]);
            for (int j = 2; j < 命令参数数; j++)
            {
                sprintf(&文件路径[strlen(文件路径)], " %s", 字符串数[j]);
            }
            FILE *f = fopen(文件路径, "rb");
            if (!f)
            {
                printf("文件不存在:%s\n", 文件路径);
                if (send(sock, "550 文件不存在\r\n", 21, 0) == -1)
                {
                    perror("控制端口发送错误（550 文件不存在\r\n）");
                    结束循环 = 1;
                    goto 循环末尾;
                }
                goto 循环末尾;
            }

            fseek(f, 0, SEEK_END);
            sprintf(str, "213 %ld\r\n", ftell(f));
            if (send(sock, str, strlen(str), 0) == -1)
            {
                perror("控制端口发送错误 if (send((int )num1, str, strlen(str), 0) == -1)");
                结束循环 = 1;
                goto 循环末尾;
            }
            fclose(f);
        }
        else if (!strcmp(字符串数[0], "pasv"))
        {
            // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
            // memcpy(回复字符串, "227 ", 4);
            char 回复字符串[100] = {0};
            if (!传输模式)
            {
                传输模式 = 1;
                // 传输连接(被动模式)
                if (bind(sock2, (struct sockaddr *)&传输端, sizeof(传输端)) == -1)
                {
                    perror("传输端口套接字绑定出错");
                    结束循环 = 1;
                    goto 循环末尾;
                }
                if (listen(sock2, 10) == -1)
                {
                    perror("传输端口套接字监听出错");
                    结束循环 = 1;
                    goto 循环末尾;
                }
            }
            struct sockaddr_in 临时变量;
            getsockname(sock, (struct sockaddr *)&临时变量, &传输端len);
            char *ip地址 = inet_ntoa(临时变量.sin_addr);
            getsockname(sock2, (struct sockaddr *)&临时变量, &传输端len);
            // char ip地址[16] = {0};
            // strcpy(ip地址, 本机IP地址);
            // strcpy(ip地址, inet_ntoa(*(struct in_addr *)(gethostbyname(s))));
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
            // char *回复字符串 = realloc(NULL, 9 + strlen(ip地址) + 端口号加斗号的长度);
            sprintf(回复字符串, "227 Entering Passive Mode (%s,%d,%d).\r\n", ip地址, 端口号高位字节, 端口号低位字节);
            // memcpy(&回复字符串[5], ip地址, strlen(ip地址));
            // sprintf(&回复字符串[5 + strlen(ip地址)], ",%d,%d", 端口号高位字节, 端口号低位字节);
            // memcpy(&回复字符串[6 + 端口号加斗号的长度 + strlen(ip地址)], ").\r\n", 3);
            // printf("%d", ntohs(传输端.sin_port));
            // printf(回复字符串);
            // printf(inet_ntoa(传输端.sin_addr));
            if (send(sock, 回复字符串, strlen(回复字符串), 0) == -1)
            {
                perror("控制端口发送错误（if (send(*(int*)num1, 回复字符串, 8 + strlen(ip地址) + 端口号加斗号的长度, 0) == -1)）");
                结束循环 = 1;
            }
            // free(ip地址);
            // free(回复字符串);
            // pthread_t 线程ID;
            // pthread_create(线程ID, NULL, 传输连接, &sock3);
        }
        else if (!strcmp(字符串数[0], "port"))
        {
            // if (send((int )num1, "202 fsdfaf\r\n", 12, 0) == -1)
            // {
            //     perror("控制端口发送错误(202 fsdfaf\r\n)");
            //     break;
            // }
            // send(sock, "227 Entering Passive Mode (66,103,214,195,0,22).\r\n", 50, 0);
            // goto 循环末尾;
            // if (传输模式)
            // {
            传输模式 = 0;
            //     close(sock2);
            //     if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            //     {
            //         perror("传输端口套接字创建出错-----（if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)）");
            //         结束循环 = 1;
            //         goto 循环末尾;
            //     }
            // }
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
            if (send(sock, "200 fsdfaf\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误(200 fsdfaf\r\n)");
                结束循环 = 1;
            }
        }
        else if (!strcmp(字符串数[0], "list"))
        {
            // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
            // memcpy(回复字符串, "227 ", 4);
            int num = 1;
            pthread_t 线程ID;
            struct 传输连接参数 参数 = {0};
            参数.sock = sock2;
            参数.传输内容 = 字符串数[0];
            参数.传输是否中断 = 0;
            参数.传输模式 = 传输模式;
            strcpy(参数.传输类型, 传输类型);
            if (命令参数数 < 2 || (字符串数[1][0] == '-' && 命令参数数 < 3))
                strcpy(参数.当前工作目录, 当前工作目录);
            else
            {
                if (字符串数[1][0] == '-')
                {
                    num++;
                }
                if (字符串数[num][0] == '/')
                {
                    strcpy(参数.当前工作目录, 字符串数[num]);
                    for (int j = num + 1; j < 命令参数数; j++)
                    {
                        sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], " %s", 字符串数[j]);
                    }
                }
                else
                {
                    strcpy(参数.当前工作目录, 当前工作目录);
                    if (参数.当前工作目录[strlen(参数.当前工作目录) - 1] == '/')
                    {
                        sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], "%s", 字符串数[num]);
                    }
                    else
                        sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], "/%s", 字符串数[num]);
                    for (int j = num + 1; j < 命令参数数; j++)
                    {
                        sprintf(&参数.当前工作目录[strlen(参数.当前工作目录)], " %s", 字符串数[j]);
                    }
                }
            }
            参数.当前工作目录len = strlen(参数.当前工作目录);
            参数.传输端 = 传输端;
            参数.传输端len = 传输端len;
            pthread_create(&线程ID, NULL, 传输连接, &参数);
            if (send(sock, "125 Data connection already open; transfer starting.\r\n", 54, 0) == -1)
            {
                perror("控制端口发送错误（125 File status okay; about to open data connection.\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
            // printf("asdad");

            // if (send(((struct num*)*(int*)num1)->sock, "125 dsfsdf\n", 11, 0) == -1)
            // {
            //     perror("");
            //     exit(1);
            // }
            int 传输连接返回值;
            pthread_join(线程ID, (void **)&传输连接返回值);
            // close(sock2);
            // if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            // {
            //     perror("传输端口套接字创建出错（if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)）");
            //     break;
            // }
            if (send(sock, "226  Transfer complete.\r\n", 25, 0) == -1)
            {
                perror("控制端口发送错误(226 fsdfaf\r\n)");
                结束循环 = 1;
            }
            // DIR *目录流 = opendir(当前工作目录);
            // struct stat num;
            // stat(当前工作目录, &num);
        }
        else if (!strcmp(字符串数[0], "noop"))
        {
            if (send(sock, "200 Command NOOP okay.\r\n", 24, 0) == -1)
            {
                perror("控制端口发送错误(200 Command NOOP okay.\r\n)");
                结束循环 = 1;
                goto 循环末尾;
            }
        }
        else if (!strcmp(字符串数[0], "quit"))
        {
            // char *回复字符串 = realloc(NULL, 5 + strlen(当前工作目录));
            // memcpy(回复字符串, "227 ", 4);
            if (send(sock, "221 srsaer\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误（221 srsaer\r\n）");
                结束循环 = 1;
                goto 循环末尾;
            }
            结束循环 = 1;
            goto 循环末尾;
        }
        else
        {
            if (send(sock, "500 gtuii7\r\n", 12, 0) == -1)
            {
                perror("控制端口发送错误（500 gtuii7\r\n）");
                结束循环 = 1;
            }
        }
    // pthread_create(&线程ID, NULL, 控制连接, &*((struct num*)*(int*)num1)->sock);
    // sleep(1);
    循环末尾:
        printf("%s命令执行完成\n", 字符串数[0]);
        // printf("释放内存\n");
        for (int i = 0; i < 命令参数数; i++)
        {
            free(字符串数[i]);
        }
        // 字符串数内存是否已释放 = 1;
        printf("%d\n", 结束循环);
    }
    free(字符串数);
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
    // if (!字符串数内存是否已释放)
    // {
    //     printf("释放内存--------\n");
    //     for (int i = 0; i < sizeof(字符串数) / sizeof(char **); i++)
    //     {
    //         free(字符串数[i]);
    //     }
    //     free(字符串数);
    // }
    close(sock2);
    close(sock);
    printf("退出控制连接\n");
    int 返回值 = 0;
    pthread_exit(&返回值);
}
int main(int 命令行参数个数, char* 命令行参数[])
{
    printf("开始\n");
    // seteuid(0);
    // char *对应字符 = "0123456789";
    struct sockaddr_in 命令端;
    命令端.sin_addr.s_addr = 0x0;
    命令端.sin_port = htons(2235);
    命令端.sin_family = AF_INET;
    int 命令端len = sizeof(命令端);

    if (命令行参数个数){
        // printf("开始\n");
        for (int i = 0;i < 命令行参数个数;i++){
        printf("%s\n", 命令行参数[i]);
        char* num;
        if (num = strstr(命令行参数[i], "port=")){
            // printf("开始\n");
            // printf("%s\n", num);
            命令端.sin_port = htons(atoi(num+5));
            printf("端口被设置为%s\n", num+5);
        }
        // 命令端.sin_port = htons(atoi(命令行参数[0]));
        }
    }
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
        pthread_create(&线程ID, NULL, 控制连接, (void *)sock1);
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
    printf("退出主函数\n");
    return 0;
}
