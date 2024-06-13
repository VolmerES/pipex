/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:35:18 by volmer            #+#    #+#             */
/*   Updated: 2024/06/13 13:59:01 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../inc/pipex.h"


typedef enum
{
	FILE_PROBLEM,
	COMMAND_ERROR,
	MEMORY_ERROR
} t_error;

void exit_child(t_error code)
{
	if (code == FILE_PROBLEM)
		perror("File problem");	
	else if (code == COMMAND_ERROR)
		perror("Command error");
	else if (code == MEMORY_ERROR)
		perror("Memory error");
	else
		printf("Error");
	exit(1);
}

void	ft_execute(char *cmd, char **env)
{
	char	**command;
	char	*path;

	if (cmd == NULL || cmd[0] == '\0') {
		fprintf(stderr, "Command is empty\n");
		exit(1);
	}

	command = ft_split(cmd, ' ');
	path = ft_find_path(command, env);
	if (execve(path, command, env) == -1)
	{
		perror("Execve failed");
		exit(1);
	}
}

void	ft_child_process_one(char **argv, char **env, int *fd)
{
	int	filein;

	close(fd[0]);
	filein = open(argv[1], O_RDONLY);
	if (filein == -1)
	{
		perror("Filein crashed ");
		close(fd[1]);
		exit (1);
	}
	dup2(filein, STDIN_FILENO);
	close(filein);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	ft_execute(argv[2], env); 
}

void	ft_child_process_two(char **argv, char **env, int *fd)
{
	int	fileout;

	close(fd[1]);
	fileout = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
	if (fileout == -1)
	{
		perror("Filein crashed");
		close(fd[0]);
		exit(1);
	}
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ft_execute(argv[3], env);
}

int	main(int argc, char **argv, char **env)
{
	int	fd[2];
	int	pid1;
	int	pid2;
	int status2;

	if (argc != 5)
	{
		perror("Argumentos introducidos de manera incorrecta\n");
		exit(1); 
	}
	if (pipe(fd) == -1)
	{
		perror("Pipe crashed");
		exit(1);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Fork crashed ");
		exit(1);
	}
	if (pid1 == 0)
		ft_child_process_one(argv, env, fd);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("Fork crashed\n");
		exit(1);
	}
	if (pid2 == 0)
		ft_child_process_two(argv, env, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status2, 0);
	return (WEXITSTATUS(status2));
}
