/* See LICENSE file for license details */
/* init - minimal init for x86_64 unix-like systems */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#define version "0.2"

void schl(int sig){
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void sys_poweroff(int sig){
	sync();
	syscall(169, 0xfee1dead, 672274793, 0x04321fedc, NULL);
}

void sys_reboot(int sig){
	sync();
	syscall(169, 0xfee1dead, 672274793, 0x01234567, NULL);
}

void help(const char *s){
	printf("usage: %s [options]..\n", s);
	printf("options:\n");
	printf("  -v	show version information\n");
	printf("  -h	display this\n");
}

int main(int argc, char *argv[]){
	if(argc >= 2){
		if(strcmp(argv[1], "-h") == 0){
			help(argv[0]);
			return 0;
		}

		if(strcmp(argv[1], "-v") == 0){
			printf("init-%s\n", version);
			return 0;
		}

		printf("usage: %s [options]\n", argv[0]);
		printf("try '%s -h' for more information\n", argv[0]);

		return 1;
	}

	if(getpid() != 1){
		write(2, "PID 1 needed\n", 14);
		_exit(1);
	}

	signal(SIGCHLD, schl);
	signal(SIGUSR1, sys_poweroff);
	signal(SIGUSR2, sys_reboot);

	mount("proc", "/proc", "proc", 0, "");
	if(fork() == 0){
		char *argv[] = { "/bin/sh", NULL };
		execv(argv[0], argv);
		write(2, "null\n", 4);
		_exit(1);
	}

	while(1) pause();
}
