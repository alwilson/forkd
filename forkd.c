#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

int main(int argc, char **argv)
{
	int fd;
	char string[300];
	char old_cmd[300];
	int br;
	int ppid = 0, pid;

	// Open the silverware drawer
	fd = open("/proc/loadavg", O_RDONLY);
	if (fd < 0) {
		perror("/proc/loadavg");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Grab a fork
		memset(string, 0, 300);
		memset(old_cmd, 0, 300);
		br = pread(fd, string, 300, 0);
		if (!br) {
			fprintf(stderr, "read zero bytes!\n");
			close(fd);
			exit(EXIT_FAILURE);
		}
		sscanf(string, "%*f %*f %*f %*s %i", &pid);

		// Never use the same fork twice
		if (ppid != pid) {
			int x;
			size_t needed;
			char *buffer;

			ppid = pid;
			printf("%i: ", pid);
			fflush(stdout);

			needed = snprintf(NULL, 0, "/proc/%i/cmdline", pid) + 1;
			buffer = malloc(needed);
			snprintf(buffer, needed, "/proc/%i/cmdline", pid);
			for (x = 0; x < 10; x++) {
				int i, cfd;

				cfd = open(buffer, O_RDONLY);
				if (cfd < 0) {
					// TODO: ignore errors for now
					//perror(buffer);
					continue;
				}
				memset(string, 0, 300);
				br = pread(cfd, string, 300, 0);
				for (i = 0; i < br && i < (300-1); i++) {
					if (string[i] == '\0')
						string[i] = ' ';
				}
				if (strcmp(string, old_cmd)) {
					printf("\t%s\n", string);
				}
				memcpy(old_cmd, string, 300);
				close(cfd);
				usleep(100);
			}
			free(buffer);
		}
		usleep(1000);
	}

	close(fd);
	exit(EXIT_SUCCESS);
}

