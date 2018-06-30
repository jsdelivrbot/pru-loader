#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "dd.h"
 
#define DATA_LENGHT 2048



int main(int argc, char *argv[])
{
    char * file_name = "/dev/pru_ctrl";
    char * data_name = "instructions.bin";

    unsigned char ibuf[2048];

    FILE * fd;
    FILE * fdb;
    
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("could not open device driver");
        return 2;
    }
    

    fdb = fopen(data_name, "rb");
    if (fdb == -1)
    {
        perror("could not open data binary");
        return 2;
    }

    fread(ibuf,sizeof(ibuf),1,fdb);


    //int fp = fopen("file.txt", "wb+");
    //fwrite(dbuf, sizeof(char), sizeof(dbuf), fp);
  

    int q;

    printf("Resettign PRU..\n"); 

    ioctl(fd, PRU_RESET, &q);

    printf("Copying Instructions\n"); 

    ioctl(fd, PRU_COPY_INSTRUCTIONS, &ibuf);

    printf("enabling PRU\n"); 

    ioctl(fd, PRU_START_EXECUTING, &q);

    printf("PRU execution started\n"); 
     
    //fclose(fp);
    close(fd);
    fclose(fdb);

    return 0;
}
