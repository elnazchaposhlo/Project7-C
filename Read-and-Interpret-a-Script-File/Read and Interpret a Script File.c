/*
 * @author Elnaz Chaposhlo
 *
 * Read and Interpret a Script File
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int readLine(int, char *, int);
char** trimCmd(char *);
void crChild(pid_t, char *);

//This is the main function.

int main(int argc, char *argv[])
{
        char buffer [256] = "";
        int fd;

        if(argc != 2){
           fprintf(stderr, "Usage: %s <inFile>\n", argv[0]);
           exit(0);
        }
        if((fd = open (argv[1], O_RDONLY)) < 0){
           perror ("Unable to open the file.");
           exit (1);
        }
        int nChar = readLine(fd, buffer, 256); //Reading lines.
        printf("\n%d Characters read from file.\n", nChar);
        close (fd);
        exit (0);
        return 0;

}

//function to read every line into array line.

int readLine(int fd, char *line, int n)
{
	int len;
	char *cmd;
	int nChar;

	pid_t child_pid;
	len = read(fd, line, n);
	nChar = len;
	cmd = strtok(line, "\n");
	while(cmd != NULL)
	{
	crChild(child_pid, cmd); //Creating child function.
	cmd = strtok (NULL, "\n");
	}
	if(len == -1){
	   perror ("Unable to read file!");
	   return -1;
	}
	return nChar;
}

//terminate function.

char** trimCmd(char *cmd){
	char **args = NULL;
	char *p;
	size_t items = 0, q;

	p = cmd;
	while (1){
	   p += strspn(p, " ");
	   if(!(q = strcspn(p, " "))){
		break;
	   }
	   if(q){
		args = realloc(args, (items + 1) * sizeof(char *));
		args [items] = malloc(q + 1);
		strncpy (args [items], p, q);
		args [items][q] = 0;
		items ++;
		p = p + q;
	   }
	}
	args = realloc(args, (items + 1) * sizeof(char *));
	args[items] = NULL;
	return args;
}

//creating a chid.

void crChild(pid_t child_pid, char * cmd){
	child_pid = fork();
	if(child_pid < 0){ // Fork was not successful.
	   perror("fork() error");
	   exit(-1);
	}
	if(child_pid != 0){ //parent pocess.
	   printf("\nI am the parent %d, my child is %d.\n", getpid(), child_pid);
	   wait(NULL);
	}
	else{ //child process.
	   printf("\nI am the child %d (%s), my parent is %d.\n",getpid(), cmd, getppid());
	   char **args = NULL;
	   args = trimCmd(cmd); // function for terminating and storing into array.
	   printf("\nOutput - ");
	   execvp(args[0], args);
	   exit(1);
	}
}

