#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#define scrb 169
#define poweroff 0x04321fedc
#define reboot 0x01234567
#define magic1 0xfee1dead
#define magic2 672274793

void schl(int sig){
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void sys_poweroff(int sig){
	sync();
	syscall(scrb, magic1, magic2, poweroff, NULL);
}

void sys_reboot(int sig){
	sync();
	syscall(scrb, magic1, magic2, reboot, NULL);
}

int main(void){
	if(getpid() != 1){
		write(2, "PID 1 needed\n", 12);
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
