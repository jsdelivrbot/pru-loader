#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "pru_ioctl.h"
 
#define DATA_LENGHT 2048





int main(int argc, char *argv[])
{
    char * file_name = "/dev/pru_ctrl";
    char * i_name    = "instructions.bin";
    char * d_name    = "data.bin";
    
    unsigned char ibuf[2048];
    unsigned char dbuf[2048];
    
    FILE * fd;
    FILE * fdbi;
    FILE * fdbd;
    
    int tmp = 0;
    
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("could not open device driver");
        return 2;
    }
    

    fdbi = fopen(i_name, "rb");
    if (fdbi == -1)
    {
        perror("could not open instructions binary");
        return 2;
    }
    
    fdbd = fopen(d_name, "rb");
    if (fdbd == -1)
    {
        perror("could not open data binary");
        return 2;
    }

    fread(ibuf,sizeof(ibuf),1,fdbi);
    fread(dbuf,sizeof(dbuf),1,fdbd);

/////
    #if 0
    tmp = 0;
    while(tmp!=2000)
    {
        dbuf[tmp] = 0xA5;
        tmp++;
    }
    #endif 
/////

    //int fp = fopen("file.txt", "wb+");
    //fwrite(dbuf, sizeof(char), sizeof(dbuf), fp);
 
    int q;

    printf("Resettign PRU..\n"); 

    ioctl(fd, PRU_RESET, &q);

    printf("Copying Instructions\n"); 

    ioctl(fd, PRU_COPY_INSTRUCTIONS, &ibuf);
    
    printf("Copying Data\n"); 

    ioctl(fd, PRU_COPY_DATA, &dbuf);

    printf("enabling PRU\n"); 

    ioctl(fd, PRU_START_EXECUTING, &q);

    printf("PRU execution started\n"); 
     
    //fclose(fp);
    close(fd);
    fclose(fdbi);
    fclose(fdbd);
    
    return 0;
}
