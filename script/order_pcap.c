#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>

char order[1024][1024] = {0};

void    save_name_in_order(DIR *dir, const char *directory, const char *pattern)
{
    int fd = 0, n = 0, r = 0;
    char *ptr = 0;
    char name[0x1000] = {0};
    char buf[0x10000] = {0};
    struct dirent *file = 0;

    while ((file = readdir(dir))) {
        memset(name, 0, sizeof(name));
        snprintf(name, 0x1000, "%s/%s", directory, file->d_name);
        if ((fd = open(name, O_RDONLY)) >= 0) {
            if ((r = read(fd, buf, sizeof(buf))) >= 0) {
                buf[r] = 0;
                ptr = 0;
                if ((ptr = strstr(buf, pattern))) {
                    while (!isdigit(*ptr)) ++ptr;
                    n = atoi(ptr);
                    memcpy(&order[n], name, strlen(name));
                } 
            }
            close(fd);
        }
    }
}

void    print_all_file(const char *pattern)
{
    int fd = 0;
    char buf[0x10000] = {0};

    for (int i = 0; i < 1024; ++i) {
        if (order[i][0] != 0) {
            memset(buf, 0, sizeof(buf));
            if ((fd = open(order[i], O_RDONLY)) >= 0) {
                if (read(fd, buf, 0x10000) >= 0) {
                    for (int i = 0; buf[i]; ++i) {
                        if (!memcmp(&buf[i], pattern, strlen(pattern)-1)) {
                            i += sizeof(pattern);
                            while (isdigit(buf[i])) ++i;
                        } else {
                            putchar(buf[i]);
                        }
                    }
                }
                close(fd);
            }
        }
    }
}

int     main(int ac, char **av)
{
    char *directory = 0;
    const char pattern[] = "//file";
    DIR *dir = 0;

    if (ac != 2)
        return puts("./order_pcap <folder containing .pcap file>");
    
    directory = av[1];
    if (!(dir = opendir(directory)))
        return puts("[-] Error opendir");

    save_name_in_order(dir, directory, pattern);
    print_all_file(pattern);

    closedir(dir);
}
