#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define ERR -1

int main(){
    struct stat s;
    int fd;
    ssize_t ch;
    char c[1024];
    pid_t pid;
    char args[1024];
    char buf[32];
    int i,cmd=0,j=0;

    if(stat("/tmp/tube.fifo", &s) != ERR){
        unlink("/tmp/tube.fifo");
    }
    if(mkfifo("/tmp/tube.fifo",0760) == ERR)
        perror("Fifo "), exit(1);

    fd = open("/tmp/tube.fifo",O_RDWR);
    
    if(fd == ERR)
        perror("Fichier "), exit(3);
    
    ch = read(fd,&c,1024);
    while(ch != ERR && ch > 1){
        printf("%s",c);
        ch = read(fd,&c,1024);

        // ajout de la cmd dans buf et des arguments dans args
        for(i=0;i<strlen(c);i++){
            if(c[i] == ' '){
                buf[i] = '\0';
                cmd = 1;
            }
            if(c[i] != ' ' && cmd ==0){
                buf[i] = c[i];
            }
            else {
                args[j] = c[i];
                j++;
            }
        }
        execvp(buf,args);     
    }

    close(fd);

    return 0;
}