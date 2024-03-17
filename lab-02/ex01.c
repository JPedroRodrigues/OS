#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	// position 0: read channel
	// position 1: write channel
	int fileDescriptor[2];
	pid_t pid;

	// creating a pipe to establish a communication between parent and child
	if (pipe(fileDescriptor) < 0) {
		perror("pipe\n");
		return -1;
	}

	// creating a child
	pid = fork();

	if (pid < 0) {
		// error message
		fprintf(stderr, "Fork failed\n");
		return -1;
	} else if (pid == 0) {
		// shutting child write
		close(fileDescriptor[1]);

		// receiving the parent message sent
		char receivedMsg[256];
		read(fileDescriptor[0], receivedMsg, sizeof(receivedMsg));

		printf("String received: \"%s\"\n", receivedMsg);
		exit(0);
	} else {
		// shutting parent read
		close(fileDescriptor[0]);

		// sending a message to child through our pipe
		char message[256] = "Hello, child!\0";
		write(fileDescriptor[1], message, sizeof(message));

		// waiting child resolve
		wait(NULL);

		printf("Message sent from parent: \"%s\"\n", message);
		exit(0);
	}
	return 0;
}
