#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define SOCKETPAIR_FOR_PARENT 0
#define SOCKETPAIR_FOR_CHILD 1

#define MSG1 "msg1: hello world"
#define MSG2 "msg2: hello world"


int main()
{
	int sockets[2];
	int ret;
	char buf[256];

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
	if(ret){
		perror("create socketpair failed");
		exit(1);	
	}
	
	int child;
	child = fork();
	if(-1 == child){
		perror("fork faild");
		exit(1);	
	}
	else if(0 == child){// this is child
		sleep(3);
		ret = read(sockets[SOCKETPAIR_FOR_CHILD], buf, sizeof(buf));
		if(0 > ret){
			perror("child process read failed");
			exit(1);
		}
		cout << "child process read: " << buf << endl;
		close(sockets[SOCKETPAIR_FOR_CHILD]);
	}
	else{// this is parent
		ret = write(sockets[SOCKETPAIR_FOR_PARENT], MSG1, sizeof(MSG1));
		if(0 > ret){
			perror("parent process write failed");
			exit(1);
		}
		cout << "parent process write: " << MSG1 << endl;
		close(sockets[SOCKETPAIR_FOR_PARENT]);
	}
	return 0;	
}
