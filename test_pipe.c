#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int p1[2], p2[2];
	int id1, id2, id3, id4, id5;

	pipe(p1);
	id1 = fork();
	if (!id1)
	{
		pipe(p2);
		id2 = fork();
		
		if (!id2)
		{
			id4 = fork();
			if (!id4)
			{
				dprintf(2, "in sleep\n");
				// 1
				close(p2[0]);
				dup2(p2[1], 1);
				close(p2[1]);
			execl("/bin/sleep", "5", "5", NULL);
			}
			else
			{
				id5 = fork();
				if(!id5)
				{
				dprintf(2, "in ls\n");
					//2
					close(p2[1]);
					dup2(p2[0], 0);
					dup2(p1[1], 1);
					close(p2[0]);
					execl("/bin/ls", "-l", "-l", NULL);
				dprintf(2, "in  after ls\n");
				}
			}
		}
		else
		{
			close(p2[0]);
			close(p2[1]);
		}
	}
	else 
	{
		id3 = fork();
		if (!id3)
		{   
			//3
			dprintf(2, "in grep\n");
			close(p1[1]);
			dup2(p1[0], 0);
			close(p1[0]);
				printf("printf in grep\n");
			execl("/usr/bin/grep", "/usr/bin/grep", "a.out", NULL);
				printf("printf in grep2\n");
		}
		else
		{
			close(p1[0]);
			close(p1[1]);
			while (wait(NULL) != -1)
				;
		}
	}
}