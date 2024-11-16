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
struct csljcs
{
    int sock;
    int csms;
    int *cssfzd;
    char *csnr;
    char cslx[2];
    char dqgzml[256];
    int dqgzmllen;
    struct sockaddr_in csd;
    int csdlen;
    // char wjm[100];
};
void *cslj(void *num1)
{
    printf("进入cslj\n");
    struct csljcs *num = num1;
    int sock3;
    if (num->csms)
    {
        if ((sock3 = accept(num->sock, (struct sockaddr *)&num->csd, &num->csdlen)) == -1)
        {
            perror("csdkaccept()函数出错");
            goto end;
        }
    }
    else
    {
        sock3 = num->sock;
        if ((sock3 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket()函数出错\n");
            printf("tccslj\n");
            int fhz = 1;
            pthread_exit(&fhz);
        }
        if (connect(sock3, (struct sockaddr *)&num->csd, num->csdlen) == -1)
        {
            perror("csdkconnect()函数出错");
            goto end;
        }
    }
    if (!strcmp(num->csnr, "stor"))
    {
        char str[1024] = {0};
        int recvfhz;
        FILE *f = *num->cslx == 'I' ? fopen(num->dqgzml, "wb") : fopen(num->dqgzml, "wt");
        if (!f)
        {
            perror("wj打开sb");
            goto end;
        }
        while (1)
        {
            if ((recvfhz = recv(sock3, str, 1024, 0)) == -1)
            {
                perror("csdkjscw");
                break;
            }
            else if (!recvfhz)
            {
                printf("wjcswc\n");
                break;
            }
            if (recvfhz == 1024)
            {
                printf("wjcsz: %d\n", recvfhz);
                fwrite(str, 1024, 1, f);
            }
            else
                fwrite(str, recvfhz, 1, f);
        }
        fclose(f);
    }

    if (!strcmp(num->csnr, "retr"))
    {
    // char wjlj[256] = {0};
    // if (num->dqgzml[num->dqgzmllen - 1] == '/')
    // {
    //     sprintf(wjlj, "%s%s", num->dqgzml, num->wjm);
    // }
    // else
    // {
    //     sprintf(wjlj, "%s/%s", num->dqgzml, num->wjm);
    // }
        char str[1024] = {0};
        FILE *f = *num->cslx == 'I' ? fopen(num->dqgzml, "rb") : fopen(num->dqgzml, "rt");
    wjcs:
        if (!f)
        {
            printf("%s\n", num->dqgzml);
            perror("wj打开sb");
            goto end;
        }
        fseek(f, 0, SEEK_END);
        unsigned wjdx = ftell(f);
        fseek(f, 0, SEEK_SET);
        printf("wjdx为%d\n", wjdx);
        for (int i = 0; i < wjdx; i += 1024)
        {
            if (wjdx - i < 1024)
            {
                fread(str, wjdx - i, 1, f);
                if (send(sock3, str, wjdx - i, 0) == -1)
                {
                    perror("csdk数据fscw");
                    break;
                }
                printf("yfs%dzj\n", wjdx - i);
            }
            else
            {
                fread(str, 1024, 1, f);
                if (send(sock3, str, 1024, 0) == -1)
                {
                    perror("csdk数据fscw");
                    break;
                }
                printf("yfs%dzj\n", 1024);
            }
        }
        // sleep(1);
    }
    else if (!strcmp(num->csnr, "list"))
    {
        DIR *dir = opendir(num->dqgzml);
        if (!dir)
        {
            perror("打开ml出错");
            printf("%s\n", num->dqgzml);
            goto end;
            // closedir(dir);
            // goto wjcs;
        }
        struct dirent *entry;
        while (entry = readdir(dir))
        {
            sprintf(&num->dqgzml[num->dqgzmllen], "/%s\0", entry->d_name);
            struct stat zt;
            if (stat(num->dqgzml, &zt))
            {
                perror("zt读取出错");
                printf("%s\n", num->dqgzml);
                continue;
            }
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            {
                continue;
            }

            char str[256] = {0};
            str[0] = (zt.st_mode & 0040000) ? 'd' : '-'; // wj类型：ml或普通wj
            str[1] = (zt.st_mode & S_IRUSR) ? 'r' : '-'; // 用户读权限
            str[2] = (zt.st_mode & S_IWUSR) ? 'w' : '-'; // 用户写权限
            str[3] = (zt.st_mode & S_IXUSR) ? 'x' : '-'; // 用户zx权限
            str[4] = (zt.st_mode & S_IRGRP) ? 'r' : '-'; // z读权限
            str[5] = (zt.st_mode & S_IWGRP) ? 'w' : '-'; // z写权限
            str[6] = (zt.st_mode & S_IXGRP) ? 'x' : '-'; // zzx权限
            str[7] = (zt.st_mode & S_IROTH) ? 'r' : '-'; // 其他用户读权限
            str[8] = (zt.st_mode & S_IWOTH) ? 'w' : '-'; // 其他用户写权限
            str[9] = (zt.st_mode & S_IXOTH) ? 'x' : '-'; // 其他用户zx权限
            str[10] = ' ';
            sprintf(&str[11], "%d ", zt.st_nlink);
            struct passwd *ssyh = getpwuid(zt.st_uid);
            struct group *ssyhz = getgrgid(zt.st_gid);
            sprintf(&str[strlen(str)], "%s ", ssyh->pw_name);
            sprintf(&str[strlen(str)], "%s ", ssyhz->gr_name);
            sprintf(&str[strlen(str)], "%d ", zt.st_size);
            strftime(&str[strlen(str)], 30, "%b %d %H:%M ", localtime(&zt.st_mtime));
            sprintf(&str[strlen(str)], "%s", entry->d_name);
            sprintf(&str[strlen(str)], "\r\n");
            if (send(sock3, str, strlen(str), 0) == -1)
            {
                perror("csdk数据cs出错");
                break;
            }
        }
        closedir(dir);
    }
end:
    // if (num->csms)
    // {
    close(sock3);
    // }
    printf("tccslj\n");
    int fhz = 1;
    pthread_exit(&fhz);
}
void *kzlj(void *num1)
{
    int sock = (int)num1;
    printf("进入kzlj\n");
    struct sockaddr_in csd;
    csd.sin_addr.s_addr = INADDR_ANY;
    csd.sin_port = htons(0 /*(*(((struct num *)num1)->dk数))*/);
    csd.sin_family = AF_INET;
    int csdlen = sizeof(csd);
    int csms = 0;
    int jsxh = 0;
    int sock2;
    char *dqgzml = getcwd(NULL, 1024);
    char cslx[2] = {0};
    char s[100] = {0};
    char **zfcs = calloc(0, 0);
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("csdk套接字cj出错(if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1))");
        close(sock);
        printf("tckzlj\n");
        int fhz = 0;
        pthread_exit(&fhz);
    }
    if (send(sock, "220 asdsad\r\n" /* Service ready for new user\n*/, 12, 0) == -1)
    {
        perror("kzdkfscw(220 asdsad\r\n)");
        close(sock);
        close(sock2);
        printf("tckzlj\n");
        int fhz = 0;
        pthread_exit(&fhz);
    }
    while (!jsxh)
    {
        int recvfhz = recv(sock, s, 100, 0);
        if (recvfhz == -1)
        {
            perror("kzdkjscw( int recvfhz = recv(*(int*)num1, s, 100, 0);)");
            break;
        }
        else if (recvfhz == 0)
        {
            printf("ljdk\n");
            break;
        }
        int mlcss = 0;
        for (int i = -1, z = 1, c = 1; s[i] != '\n' && s[i] != '\r'; z++)
        {
            i++;
            char *zfc = calloc(1, 2);
            for (int j = 0; s[i] != ' ' && s[i] != '\n' && s[i] != '\r' && c; j++, i++)
            {
                if (zfc[j] == '"')
                    ~c;
                zfc = realloc(zfc, j + 2);
                zfc[j] = s[i];
                zfc[j + 1] = 0;
            }
            zfcs = realloc(zfcs, sizeof(char *) * z);
            zfcs[z - 1] = zfc;
            mlcss = z;
            // num[0]=zfc;
            // free(zfc);
        }
        // zfcs内存是否y释放 = 0;
        // return 0;
        // zfc小写转换
        for (int i = 0; i < strlen(zfcs[0]); i++)
        {
            zfcs[0][i] = tolower(zfcs[0][i]);
        }

        if (!strcmp(zfcs[0], "user"))
        {
            if (!strcmp(zfcs[1], "anonymous") || !strcmp(zfcs[1], ".."))
            {
                if (send(sock, "331  yhmzq\r\n", 22, 0) == -1)
                {
                    perror("kzdkfscw(331 yhmzq\r\n)");
                    jsxh = 1;
                    goto xhmw;
                }
            }
            else
            {
                if (send(sock, "501 yhmcw\r\n", 21, 0) == -1)
                {
                    perror("kzdkfscw(501 yhmcw\r\n)");
                    jsxh = 1;
                    goto xhmw;
                }
            }
        }
        else if (!strcmp(zfcs[0], "pass"))
        {
            if (send(sock, "230 fgdf\r\n", 10, 0) == -1)
            {
                perror("kzdkfscw(230 fgdf\r\n)");
                jsxh = 1;
                goto xhmw;
            }
        }

        else if (!strcmp(zfcs[0], "syst"))
        {
            if (send(sock, "215 UNIX type:L8\r\n", 18, 0) == -1)
            {
                perror("kzdkfscw(215 UNIX type:L8\r\n)");
                jsxh = 1;
                goto xhmw;
            }
        }
        else if (!strcmp(zfcs[0], "pwd"))
        {
            // getcwd(dqgzml, 1024);
            char hfzfc[300] = {0};
            memcpy(hfzfc, "257 ", 4);
            sprintf(&hfzfc[4], "\"%s\"\r\n", dqgzml);
            if (send(sock, hfzfc, strlen(hfzfc), 0) == -1)
            {
                perror("kzdkfscw pwd");
                jsxh = 1;
                goto xhmw;
            }
            // printf("释放hfzfc内存\n");
            // free(hfzfc);
        }
        else if (!strcmp(zfcs[0], "cdup"))
        {
            char dqgzmlnum[1024] = {0};
            char ml[550] = {0};
            sprintf(ml, "cd \"%s\" && cd .. && pwd", dqgzml);
            printf("zxml：%s\n", ml);
            FILE *f = popen(ml, "r");
            if (!f)
            {
                perror("cdup popenzxmlsb");
                if (send(sock, "550 adaseasdas\r\n", 16, 0) == -1)
                {
                    perror("cdup kzdkfscw(550 adaseasdas\r\n)");
                    jsxh = 1;
                }
                goto xhmw;
            }
            // fseek(f, 0, SEEK_END);
            // unsigned wjdx = ftell(f);
            // fseek(f, 0, SEEK_SET);
            if (!fgets(dqgzmlnum, 1024, f))
            {
                printf("cdup zxmlsb\n");
                if (send(sock, "550 Requested file action okay, completed.\r\n", 44, 0) == -1)
                {
                    perror("cdup kzdkfscw(550 Requested file action okay, completed.\r\n)");
                    jsxh = 1;
                }
                fclose(f);
                goto xhmw;
            }
            dqgzmlnum[strlen(dqgzmlnum) - 1] = '\0';
            strcpy(dqgzml, dqgzmlnum);
            printf("dqgzml：%s\n", dqgzml);
            fclose(f);
            if (send(sock, "250 adaseasdas\r\n", 16, 0) == -1)
            {
                perror("kzdkfscw(250 adaseasdas\r\n)");
                jsxh = 1;
            }
            // getcwd(dqgzml, 1024);

            // else
            // {
            //     if (send(sock, "550 sdfa\r\n", 10, 0) == -1)
            //     {
            //         perror("kzdkfscw(550 sdfa\r\n)");
            //                         jsxh = 1;
            // goto xhmw;;
            //     }
            // }
        }
        else if (!strcmp(zfcs[0], "rmd"))
        {
            char lj[1024] = {0};
            if (zfcs[1][0] != '/')
            {
                sprintf(lj, "%s/", dqgzml);
            }
            strcpy(&lj[strlen(lj)], zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&lj[strlen(lj)], " %s", zfcs[j]);
            }
            if (rmdir(lj))
            {
                printf("scmlsb\n");
                if (send(sock, "450 scmlsb\r\n", 24, 0))
                {
                    perror("kzdkfscw(450 scmlsb\r\n)");
                    jsxh = 1;
                    goto xhmw;
                }
            }
            if (send(sock, "250 qqdwjczcgwc\r\n", 39, 0) == -1)
            {
                perror("kzdkfscw(250 qqdwjczcgwc)");
                jsxh = 1;
            }
            printf("scml：%s\n", lj);
        }
        else if (!strcmp(zfcs[0], "mkd"))
        {
            char lj[1024] = {0};
            if (zfcs[1][0] != '/')
            {
                sprintf(lj, "%s/", dqgzml);
            }
            strcpy(&lj[strlen(lj)], zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&lj[strlen(lj)], " %s", zfcs[j]);
            }

            if (mkdir(lj, 0777))
            {
                printf("cjmlsb\n");
                if (send(sock, "550 cjmlsb\r\n", 24, 0))
                {
                    perror("kzdkfscw(550 cjmlsb\r\n)");
                    jsxh = 1;
                    goto xhmw;
                }
            }
            char hfzfc[300] = {0};
            memcpy(hfzfc, "257 ", 4);
            sprintf(&hfzfc[4], "\"%s\"\r\n", lj);
            if (send(sock, hfzfc, strlen(hfzfc), 0) == -1)
            {
                perror("kzdkfscw mkd");
                jsxh = 1;
            }
            printf("cjmlcg\n");
        }
        else if (!strcmp(zfcs[0], "dele"))
        {
            char wjm[1024] = {0};
            if (zfcs[1][0] != '/')
            {
                sprintf(wjm, "%s/", dqgzml);
            }
            strcpy(&wjm[strlen(wjm)], zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&wjm[strlen(wjm)], " %s", zfcs[j]);
            }
            if (remove(wjm) == -1)
            {
                printf("scwjsb: %s\n", wjm);
                if (errno == ENOENT)
                {
                    if (send(sock, "550 File not found.\r\n", 21, 0))
                    {
                        perror("kzdkfscw(550 scwjsb\r\n)");
                        jsxh = 1;
                        goto xhmw;
                    }
                }
                else if (errno == EPERM)
                {
                    if (send(sock, "550 Permission denied.\r\n", 24, 0))
                    {
                        perror("kzdkfscw(550 Permission denied.\r\n)");
                        jsxh = 1;
                        goto xhmw;
                    }
                }
                else
                {
                    printf("scwjsb，未知cw\n");
                    if (send(sock, "550 Unknown error.\r\n", 21, 0))
                    {
                        perror("kzdkfscw(550 Unknown error.\r\n)");
                        jsxh = 1;
                        goto xhmw;
                    }
                }
            }
            else if (send(sock, "250 mlzxcg\r\n", 24, 0) == -1)
            {
                perror("kzdkfscw(250 mlzxcg\r\n)");
                jsxh = 1;
            }
        }
        else if (!strcmp(zfcs[0], "stor"))
        {
            pthread_t xcID;
            struct csljcs cs = {0};
            cs.sock = sock2;
            cs.csnr = zfcs[0];
            cs.cssfzd = 0;
            cs.csms = csms;
            strcpy(cs.cslx, cslx);
            if (zfcs[1][0] != '/')
            {
                if (dqgzml[strlen(dqgzml) - 1] == '/')
                {
                    sprintf(cs.dqgzml, "%s", dqgzml);
                }
                else
                    sprintf(cs.dqgzml, "%s/", dqgzml);
            }
            sprintf(&cs.dqgzml[strlen(cs.dqgzml)], "/%s", zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&cs.dqgzml[strlen(cs.dqgzml)], " %s", zfcs[j]);
            }
            cs.dqgzmllen = strlen(cs.dqgzml);
            cs.csd = csd;
            cs.csdlen = csdlen;
            pthread_create(&xcID, NULL, cslj, &cs);
            if (send(sock, "125 File status okay; about to open data connection.\r\n", 54, 0) == -1)
            {
                perror("kzdkfscw(125 File status okay; about to open data connection.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
            pthread_join(xcID, NULL);
            if (send(sock, "226 Transfer complete.\r\n", 24, 0) == -1)
            {
                perror("kzdkfscw(226 Transfer complete.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
        }
        else if (!strcmp(zfcs[0], "retr"))
        {
            pthread_t xcID;
            struct csljcs cs = {0};
            cs.sock = sock2;
            cs.csnr = zfcs[0];
            cs.cssfzd = 0;
            cs.csms = csms;
            strcpy(cs.cslx, cslx);
            if (zfcs[1][0] != '/')
            {
                if (dqgzml[strlen(dqgzml) - 1] == '/')
                {
                    sprintf(cs.dqgzml, "%s", dqgzml);
                }
                else
                    sprintf(cs.dqgzml, "%s/", dqgzml);
            }
            sprintf(&cs.dqgzml[strlen(cs.dqgzml)], "%s", zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&cs.dqgzml[strlen(cs.dqgzml)], " %s", zfcs[j]);
            }
            cs.dqgzmllen = strlen(cs.dqgzml);
            cs.csd = csd;
            cs.csdlen = csdlen;
            pthread_create(&xcID, NULL, cslj, &cs);
            if (send(sock, "125 File status okay; about to open data connection.\r\n", 54, 0) == -1)
            {
                perror("kzdkfscw(125 File status okay; about to open data connection.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
            pthread_join(xcID, NULL);
            if (send(sock, "226 Transfer complete.\r\n", 24, 0) == -1)
            {
                perror("kzdkfscw(226 Transfer complete.\r\n)");
                jsxh = 1;
            }
        }
        else if (!strcmp(zfcs[0], "cwd"))
        {
            char dqgzmlnum[1024] = {0};
            char ml[550] = {0};
            // if (zfcs[1][0] == '/')
            // {
            sprintf(ml, "cd \"%s\" && cd \"", dqgzml);
            strcpy(&ml[strlen(ml)], zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&ml[strlen(ml)], " %s", zfcs[j]);
            }
            sprintf(&ml[strlen(ml)], "\" && pwd");
            printf("ml：%s\n", ml);
            FILE *f = popen(ml, "r");
            if (!f)
            {
                perror("popen zxmlsb");
                if (send(sock, "550 Requested file action okay, completed.\r\n", 44, 0) == -1)
                {
                    perror("kzdkfscw(250 Requested file action okay, completed.\r\n)");
                    jsxh = 1;
                }
                goto xhmw;
            }
            // fseek(f, 0, SEEK_END);
            // unsigned wjdx = ftell(f);
            // fseek(f, 0, SEEK_SET);
            if (!fgets(dqgzmlnum, 1024, f))
            {
                printf("fgets zxmlsb\n");
                if (send(sock, "550 Requested file action okay, completed.\r\n", 44, 0) == -1)
                {
                    perror("kzdkfscw(550 Requested file action okay, completed.\r\n)");
                    jsxh = 1;
                }
                fclose(f);
                goto xhmw;
            }
            dqgzmlnum[strlen(dqgzmlnum) - 1] = '\0';
            strcpy(dqgzml, dqgzmlnum);
            fclose(f);
            // }
            // else
            // {
            //     if (dqgzml[strlen(dqgzml) - 1] == '/')
            //         sprintf(&dqgzml[strlen(dqgzml)], "%s", zfcs[1]);
            //     else
            //         sprintf(&dqgzml[strlen(dqgzml)], "/%s", zfcs[1]);
            //     for (int j = 2; j < mlcss; j++)
            //     {
            //         sprintf(&dqgzml[strlen(dqgzml)], " %s", zfcs[j]);
            //     }
            // }
            // chdir(dqgzml);
            printf("dqgzml：%s\n", dqgzml);
            if (send(sock, "250 Requested file action okay, completed.\r\n", 44, 0) == -1)
            {
                perror("kzdkfscw(250 Requested file action okay, completed.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
            // if (chdir(zfcs[1]) == 0)
            // {
            //     if (send((int )num1, "250 adaseasdas\r\n", 16, 0) == -1)
            //     {
            //         perror("kzdkfscw(250 adaseasdas\r\n)");
            //                         jsxh = 1;
            // goto xhmw;;
            //     }
            //     getcwd(dqgzml, 1024);
            // }
            // else
            // {

            //     // if (send((int )num1, "550 sdfa\r\n", 10, 0) == -1)
            //     // {
            //     //     perror("kzdkfscw(550 sdfa\r\n)");
            //     //                     jsxh = 1;
            // goto xhmw;;
            //     // }
            // }
        }
        else if (!strcmp(zfcs[0], "opts"))
        {
            if (!strcmp(zfcs[1], "UTF8") || !strcmp(zfcs[1], "utf8"))
            {
                if (!strcmp(zfcs[2], "ON") || !strcmp(zfcs[2], "on"))
                {
                    if (send(sock, "200 cg\r\n", 12, 0) == -1)
                    {
                        perror("kzdkfscw(200 cg\r\n)");
                        jsxh = 1;
                        goto xhmw;
                        ;
                    }
                }
                else
                {
                    if (send(sock, "504 sfas\r\n", 12, 0) == -1)
                    {
                        perror("kzdkfscw(504asdf\r\n)");
                        jsxh = 1;
                        goto xhmw;
                        ;
                    }
                }
            }
            else
            {
                if (send(sock, "504 sfas\r\n", 12, 0) == -1)
                {
                    perror("kzdkfscw(504asdf\r\n)");
                    jsxh = 1;
                    goto xhmw;
                    ;
                }
            }
        }
        else if (!strcmp(zfcs[0], "auth"))
        {
            // if (!strcmp(zfcs[1], "SSL"))
            // {
            //     if (send((int )num1, "234 dfrsdfast\r\n", 15, 0) == -1)
            //     {
            //         perror("kzdkfscw(234 dfrsdfast\r\n)");
            //         break;
            //     }
            // }
            // else
            // {
            if (send(sock, "504 cw\r\n", 12, 0) == -1)
            {
                perror("kzdkfscw(504 cw\r\n)");
                jsxh = 1;
                goto xhmw;
                ;
            }
            // }
        }
        else if (!strcmp(zfcs[0], "type"))
        {
            cslx[0] = zfcs[1][0];
            if (send(sock, "200 Command okay.\r\n", 19, 0) == -1)
            {
                perror("kzdkfscw(200 Command okay.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
        }
        else if (!strcmp(zfcs[0], "size"))
        {
            char str[50] = {0};
            char wjlj[256] = {0};
            if (zfcs[1][0] != '/')
            {
                sprintf(wjlj, "%s/", dqgzml);
            }
            strcpy(&wjlj[strlen(wjlj)], zfcs[1]);
            for (int j = 2; j < mlcss; j++)
            {
                sprintf(&wjlj[strlen(wjlj)], " %s", zfcs[j]);
            }
            FILE *f = fopen(wjlj, "rb");
            if (!f)
            {
                printf("wj不存在:%s\n", wjlj);
                if (send(sock, "550 wj不存在\r\n", 21, 0) == -1)
                {
                    perror("kzdkfscw(550 wj不存在\r\n)");
                    jsxh = 1;
                    goto xhmw;
                }
                goto xhmw;
            }

            fseek(f, 0, SEEK_END);
            sprintf(str, "213 %ld\r\n", ftell(f));
            if (send(sock, str, strlen(str), 0) == -1)
            {
                perror("kzdkfscw if (send((int )num1, str, strlen(str), 0) == -1)");
                jsxh = 1;
                goto xhmw;
            }
            fclose(f);
        }
        else if (!strcmp(zfcs[0], "pasv"))
        {
            // char *hfzfc = realloc(NULL, 5 + strlen(dqgzml));
            // memcpy(hfzfc, "227 ", 4);
            char hfzfc[100] = {0};
            if (!csms)
            {
                csms = 1;
                // cslj(被动模式)
                if (bind(sock2, (struct sockaddr *)&csd, sizeof(csd)) == -1)
                {
                    perror("csdk套接字绑定出错");
                    jsxh = 1;
                    goto xhmw;
                }
                if (listen(sock2, 10) == -1)
                {
                    perror("csdk套接字监听出错");
                    jsxh = 1;
                    goto xhmw;
                }
            }
            struct sockaddr_in lsbl;
            getsockname(sock, (struct sockaddr *)&lsbl, &csdlen);
            char *ipdz = inet_ntoa(lsbl.sin_addr);
            getsockname(sock2, (struct sockaddr *)&lsbl, &csdlen);
            // char ipdz[16] = {0};
            // strcpy(ipdz, 本机IPdz);
            // strcpy(ipdz, inet_ntoa(*(struct in_addr *)(gethostbyname(s))));
            for (int i = 0; i < strlen(ipdz); i++)
                if (ipdz[i] == '.')
                    ipdz[i] = ',';
            // getsockname(sock2, (struct sockaddr*)&csd, &csdlen);
            int dkhjdhdcd = 1;
            int dkhgwzj = ntohs(lsbl.sin_port) / 256;
            if (dkhgwzj < 10)
                dkhjdhdcd += 1;
            else if (dkhgwzj < 100)
                dkhjdhdcd += 2;
            else
                dkhjdhdcd += 3;
            int dkhdwzj = ntohs(lsbl.sin_port) % 256;
            if (dkhdwzj < 10)
                dkhjdhdcd += 1;
            else if (dkhdwzj < 100)
                dkhjdhdcd += 2;
            else
                dkhjdhdcd += 3;
            // char *hfzfc = realloc(NULL, 9 + strlen(ipdz) + dkhjdhdcd);
            sprintf(hfzfc, "227 Entering Passive Mode (%s,%d,%d).\r\n", ipdz, dkhgwzj, dkhdwzj);
            // memcpy(&hfzfc[5], ipdz, strlen(ipdz));
            // sprintf(&hfzfc[5 + strlen(ipdz)], ",%d,%d", dkhgwzj, dkhdwzj);
            // memcpy(&hfzfc[6 + dkhjdhdcd + strlen(ipdz)], ").\r\n", 3);
            // printf("%d", ntohs(csd.sin_port));
            // printf(hfzfc);
            // printf(inet_ntoa(csd.sin_addr));
            if (send(sock, hfzfc, strlen(hfzfc), 0) == -1)
            {
                perror("kzdkfscw(if (send(*(int*)num1, hfzfc, 8 + strlen(ipdz) + dkhjdhdcd, 0) == -1))");
                jsxh = 1;
            }
            // free(ipdz);
            // free(hfzfc);
            // pthread_t xcID;
            // pthread_create(xcID, NULL, cslj, &sock3);
        }
        else if (!strcmp(zfcs[0], "port"))
        {
            // if (send((int )num1, "202 fsdfaf\r\n", 12, 0) == -1)
            // {
            //     perror("kzdkfscw(202 fsdfaf\r\n)");
            //     break;
            // }
            // send(sock, "227 Entering Passive Mode (66,103,214,195,0,22).\r\n", 50, 0);
            // goto xhmw;
            // if (csms)
            // {
            csms = 0;
            //     close(sock2);
            //     if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            //     {
            //         perror("csdk套接字cj出错-----(if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1))");
            //         jsxh = 1;
            //         goto xhmw;
            //     }
            // }
            unsigned char ipdz[4];
            int dk[2];
            char dfsjzipdz[12];
            sscanf(zfcs[1], "%d,%d,%d,%d,%d,%d", &ipdz[0], &ipdz[1], &ipdz[2], &ipdz[3], &dk[0], &dk[1]);
            sprintf(dfsjzipdz, "%d.%d.%d.%d", ipdz[0], ipdz[1], ipdz[2], ipdz[3]);
            csd.sin_addr.s_addr = inet_addr((const char *)&dfsjzipdz);
            csd.sin_port = htons(dk[0] * 256 + dk[1]);
            // for (int i = 0, j = 0,y=0; i < strlen(zfcs[1]); i++)
            // {
            //     char *num = realloc(NULL, j + 1);
            //     if (zfcs[1][i] == ','){
            //         ipdz_dk = realloc(ipdz_dk, y+1);
            //         ipdz_dk[y] = num;
            //         y++;
            //         j=0;
            //         continue;
            //     }
            //     num[i] = zfcs[1][i];
            //     j++;
            // }
            // char
            if (send(sock, "200 fsdfaf\r\n", 12, 0) == -1)
            {
                perror("kzdkfscw(200 fsdfaf\r\n)");
                jsxh = 1;
            }
        }
        else if (!strcmp(zfcs[0], "list"))
        {
            // char *hfzfc = realloc(NULL, 5 + strlen(dqgzml));
            // memcpy(hfzfc, "227 ", 4);
            int num = 1;
            pthread_t xcID;
            struct csljcs cs = {0};
            cs.sock = sock2;
            cs.csnr = zfcs[0];
            cs.cssfzd = 0;
            cs.csms = csms;
            strcpy(cs.cslx, cslx);
            if (mlcss < 2 || (zfcs[1][0] == '-' && mlcss < 3))
                strcpy(cs.dqgzml, dqgzml);
            else
            {
                if (zfcs[1][0] == '-')
                {
                    num++;
                }
                if (zfcs[num][0] == '/')
                {
                    strcpy(cs.dqgzml, zfcs[num]);
                    for (int j = num + 1; j < mlcss; j++)
                    {
                        sprintf(&cs.dqgzml[strlen(cs.dqgzml)], " %s", zfcs[j]);
                    }
                }
                else
                {
                    strcpy(cs.dqgzml, dqgzml);
                    if (cs.dqgzml[strlen(cs.dqgzml) - 1] == '/')
                    {
                        sprintf(&cs.dqgzml[strlen(cs.dqgzml)], "%s", zfcs[num]);
                    }
                    else
                        sprintf(&cs.dqgzml[strlen(cs.dqgzml)], "/%s", zfcs[num]);
                    for (int j = num + 1; j < mlcss; j++)
                    {
                        sprintf(&cs.dqgzml[strlen(cs.dqgzml)], " %s", zfcs[j]);
                    }
                }
            }
            cs.dqgzmllen = strlen(cs.dqgzml);
            cs.csd = csd;
            cs.csdlen = csdlen;
            pthread_create(&xcID, NULL, cslj, &cs);
            if (send(sock, "125 Data connection already open; transfer starting.\r\n", 54, 0) == -1)
            {
                perror("kzdkfscw(125 File status okay; about to open data connection.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
            // printf("asdad");

            // if (send(((struct num*)*(int*)num1)->sock, "125 dsfsdf\n", 11, 0) == -1)
            // {
            //     perror("");
            //     exit(1);
            // }
            int csljfhz;
            pthread_join(xcID, (void **)&csljfhz);
            // close(sock2);
            // if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            // {
            //     perror("csdk套接字cj出错(if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1))");
            //     break;
            // }
            if (send(sock, "226  Transfer complete.\r\n", 25, 0) == -1)
            {
                perror("kzdkfscw(226 fsdfaf\r\n)");
                jsxh = 1;
            }
            // DIR *ml流 = opendir(dqgzml);
            // struct stat num;
            // stat(dqgzml, &num);
        }
        else if (!strcmp(zfcs[0], "noop"))
        {
            if (send(sock, "200 Command NOOP okay.\r\n", 24, 0) == -1)
            {
                perror("kzdkfscw(200 Command NOOP okay.\r\n)");
                jsxh = 1;
                goto xhmw;
            }
        }
        else if (!strcmp(zfcs[0], "quit"))
        {
            // char *hfzfc = realloc(NULL, 5 + strlen(dqgzml));
            // memcpy(hfzfc, "227 ", 4);
            if (send(sock, "221 srsaer\r\n", 12, 0) == -1)
            {
                perror("kzdkfscw(221 srsaer\r\n)");
                jsxh = 1;
                goto xhmw;
            }
            jsxh = 1;
            goto xhmw;
        }
        else
        {
            if (send(sock, "500 gtuii7\r\n", 12, 0) == -1)
            {
                perror("kzdkfscw(500 gtuii7\r\n)");
                jsxh = 1;
            }
        }
    // pthread_create(&xcID, NULL, kzlj, &*((struct num*)*(int*)num1)->sock);
    // sleep(1);
    xhmw:
        printf("%smlzxwc\n", zfcs[0]);
        // printf("释放内存\n");
        for (int i = 0; i < mlcss; i++)
        {
            free(zfcs[i]);
        }
        // zfcs内存是否y释放 = 1;
        printf("%d\n", jsxh);
    }
    free(zfcs);
    // char s[100] = {0};
    // while (1)
    // {
    //     if (recv(*(int *)sock, s, 100, 0) == -1)
    //     {
    //         perror("");
    //         exit(1);
    //     }
    //     char **zfcs = realloc(NULL, 0);
    //     for (int i = -1, z = 1; s[i] != '\r\n'; z++)
    //     {
    //         i++;
    //         char *zfc = realloc(NULL, 1);
    //         for (int j = 0; s[i] != ' ' && s[i] != '\r\n'; j++, i++)
    //         {
    //             zfc = realloc(zfc, j + 2);
    //             zfc[j] = s[i];
    //         }
    //         zfcs = realloc(zfcs, sizeof(char *) * z);
    //         zfcs[z - 1] = zfc;
    //         // num[0]=zfc;
    //         free(zfc);
    //     }
    //     // free(zfcs);
    //     // char strd[2] = {0};
    //     // if (recv(((struct asdfg *)num)->*((struct num*)*(int*)num1)->sock, strd, 2, 0) == -1)
    //     // {
    //     //     perror("");
    //     // }
    // if (!zfcs内存是否y释放)
    // {
    //     printf("释放内存--------\n");
    //     for (int i = 0; i < sizeof(zfcs) / sizeof(char **); i++)
    //     {
    //         free(zfcs[i]);
    //     }
    //     free(zfcs);
    // }
    close(sock2);
    close(sock);
    printf("tckzlj\n");
    int fhz = 0;
    pthread_exit(&fhz);
}
int main()
{
    printf("ks\n");
    // seteuid(0);
    // char *对应字符 = "0123456789";
    struct sockaddr_in mld;
    mld.sin_addr.s_addr = 0x0;
    mld.sin_port = htons(2234);
    mld.sin_family = AF_INET;
    int mldlen = sizeof(mld);

    // kzlj
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("kzdk套接字cjcw");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&mld, sizeof(mld)) == -1)
    {
        perror("kzdk套接字绑定cw");
        exit(1);
    }
    if (listen(sock, 10) == -1)
    {
        perror("kzdk监听cw");
        exit(1);
    }
    int sock1;
    pthread_t xcID;
    while (1)
    {
        if ((sock1 = accept(sock, (struct sockaddr *)&mld, &mldlen)) == -1)
        {
            perror("kzdkaccept()函数cw");
            exit(1);
        }
        // struct num cs = {sock1, 对应字符};
        pthread_create(&xcID, NULL, kzlj, (void *)sock1);
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
    printf("tc主函数\n");
    return 0;
}
