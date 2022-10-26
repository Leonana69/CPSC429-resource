#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define MAX_BUFFER_LENGTH (512 * 1024)
#define TEST_CASE 5
#define TRIALS 200

char buffer[MAX_BUFFER_LENGTH];
double read_time[TEST_CASE];
double write_time[TEST_CASE];
int test_len[TEST_CASE] = { 1, 64, 1024, 64 * 1024, 512 * 1024 };

int main() {
    int c, i;
    struct timeval start, end;
    int fd = open("/dev/mymem", O_RDWR);

    if (fd < 0) {
        printf("open file [FAILED]\n");
        return -1;
    }
    
    printf("file descriptor: %d\n", fd);
    
    for (c = 0; c < TEST_CASE; c++) {
        read_time[c] = 0;
        write_time[c] = 0;

        for (i = 0; i < TRIALS; i++) {
            lseek(fd, 0, SEEK_SET);
            gettimeofday(&start, NULL);
            read(fd, buffer, test_len[c]);
            gettimeofday(&end, NULL);
            read_time[c] += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
        }

        for (i = 0; i < TRIALS; i++) {
            lseek(fd, 0, SEEK_SET);
            gettimeofday(&start, NULL);
            write(fd, buffer, test_len[c]);
            gettimeofday(&end, NULL);
            write_time[c] += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
        }
        read_time[c] /= TRIALS;
        write_time[c] /= TRIALS;
    }
    
    for (c = 0; c < TEST_CASE; c++) {
        printf("read/write %6d bytes: %.3f / %.3f usec\n", test_len[c], read_time[c], write_time[c]);
    }

    return 0;
}