#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char **argv) {
    //pp2c: parent to child父进程对子进程
    //pc2p: child to parent子进程对父进程
    int pp2c[2], pc2p[2];
    pipe(pp2c);
    pipe(pc2p);
    int pid = fork();
    if(pid != 0) {
        //父进程写入一个字符
        write(pp2c[1], ".", 1);
        close(pp2c[1]);
        //从子进程读取字符
        char buf;
        read(pc2p[0], &buf, 1);
        printf("%d: received pong\n", getpid());  
        wait(0);
    } else {
        char buf;
        read(pp2c[0], &buf, 1);
        printf("%d: received ping\n", getpid());      

        write(pc2p[1], &buf, 1);
        close(pc2p[1]);
    }
    close(pp2c[0]);
    close(pp2c[1]);
    exit(0);
}