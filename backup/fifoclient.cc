/*
 * FIFO example using select.
 *
 * $ mkfifo /tmp/fifo
 * $ clang -Wall -o test ./test.c
 * $ ./test &
 * $ echo 'hello' > /tmp/fifo
 * $ echo 'hello world' > /tmp/fifo
 * $ killall test
 */

#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/dir.h>  
#include <sys/param.h>  
#include <string>
#include <iostream>
#include <errno.h>
int main()
{
    int fd;
    int n;
    fd_set set;
    ssize_t bytes;
    size_t total_bytes;

 char hname[1024]; int lenh;
    int ier=gethostname(hname,1024);
    std::string shn;
    shn.assign(hname);
    std::cout<<ier<<" "<<hname<<" "<<shn<<" "<<shn.length()<<"\n";
    if (ier<0) perror(" pas dhost");
    
    char buf[1024];
    int* ibuf=(int*) buf;
    std::string name="/tmp/fifo1";
    struct stat st;
    if (stat(name.c_str(), &st) != 0)
      mkfifo(name.c_str(), 0666);
    fd = open(name.c_str(), O_RDONLY);
    //fd = open("/tmp/fifo1", O_RDONLY| O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

   
    while (1)
    {
      bytes = read(fd, buf, sizeof(buf));
      if (bytes>0)
      {
        printf("done reading (%lu bytes) %d\n", bytes,ibuf[0]);
      }
    }
    FD_ZERO(&set);
    FD_SET(fd, &set);

    for (;;) {
        n = select(fd+1, &set, NULL, NULL, NULL);
        if (!n)
            continue;
        if (n == -1) {
            perror("select");
            return EXIT_FAILURE;
        }
        if (FD_ISSET(fd, &set)) {
            printf("Descriptor %d is ready.\n", fd);
            total_bytes = 0;
            for (;;) {
                bytes = read(fd, buf, sizeof(buf));
                if (bytes > 0) {
                    total_bytes += (size_t)bytes;
                } else {
                    if (errno == EWOULDBLOCK && total_bytes>0) {
                        /* Done reading */
		      buf[total_bytes]=0;
		      printf("done reading (%lu bytes) %s\n", total_bytes,buf);
                        break;
                    } else {
                        perror("read");
			::close(fd);
			fd = open("/tmp/fifo1", O_RDONLY| O_NONBLOCK);
			if (fd == -1) {
			  perror("open");
			  return EXIT_FAILURE;
			}
			
			FD_ZERO(&set);
			FD_SET(fd, &set);


			break;
                        return EXIT_FAILURE;
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
