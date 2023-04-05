/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 06:56:23 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/05 09:45:33 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int	id;
	int	id2;
	int	fd;

	id = fork();
	if (id == 0)
	{
		fd = open("tests/std0", O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		fd = open("tests/std1", O_WRONLY | O_TRUNC | O_CREAT, 0777);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fd = open("tests/std2", O_WRONLY | O_TRUNC | O_CREAT, 0777);
		dup2(fd, STDERR_FILENO);
		close(fd);

		execlp("./minishell", "./minishell", NULL);
	}
	id2 = fork();
	if (id2 == 0)
	{
		fd = open("tests/std0", O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		fd = open("tests/bash1", O_WRONLY | O_TRUNC | O_CREAT, 0777);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		fd = open("tests/bash2", O_WRONLY | O_TRUNC | O_CREAT, 0777);
		dup2(fd, STDERR_FILENO);
		close(fd);

		execlp("/bin/bash", "/bin/bash", NULL);

	}
	sleep(5);
	kill(id, SIGKILL);
	kill(id2, SIGKILL);
	waitpid(id, NULL, 0);
	printf("\ndiff stdout :_________________________\n");
	system("diff tests/std1 tests/bash1");
	printf("\ndiff stderr :_________________________\n");
	system("diff tests/std2 tests/bash2");
	printf("\ndone\n");
}