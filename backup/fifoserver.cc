#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <stdint.h>
int main(int argc, char* argv[])
{       
    int s2c, c2s; 
    char fifo_name1[] = "/tmp/fifo1";
    char fifo_name2[] = "/tmp/fifo2";
    char msg[80], buf[10];
    struct stat st;

    // if no fifos, create 'em
    if (stat(fifo_name1, &st) != 0)
        mkfifo(fifo_name1, 0666);
    if (stat(fifo_name2, &st) != 0)
        mkfifo(fifo_name2, 0666);

    
    //c2s= open(fifo_name2, O_RDONLY);

    // start sending messages, with 5s interval
    for (uint64_t i=0; i<5000; i++)
    {
      std::stringstream s;
        printf("Message #%d \n", i);

	s<<"Message #"<<i;
    s2c= open(fifo_name1, O_WRONLY); 
        //write(s2c, s.str().c_str(), s.str().length());
        write(s2c,&i,sizeof(uint64_t));
        ::close(s2c);
        ::usleep(1);
    }

    // delete fifos
    //unlink(fifo_name1);
    //unlink(fifo_name2);
    printf("server exit successfully");
    return EXIT_SUCCESS;
}
