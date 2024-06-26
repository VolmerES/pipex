/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:42:35 by jdelorme          #+#    #+#             */
/*   Updated: 2024/06/19 12:47:23 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	exit_child(t_error code)
{
	if (code == FILE_PROBLEM)
		perror("File problem");
	else if (code == COMMAND_ERROR)
		perror("Command error");
	else if (code == MEMORY_ERROR)
		perror("Memory error");
	else if (code == INVALID_ARGS)
		perror("Invalid arguments");
	else if (code == PIPE_ERROR)
		perror("Pipe error");
	else if (code == FORK_ERROR)
		perror("Fork error");
	else
		printf("Unknown error");
	exit(1);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*ft_access_check(char **cmd, char **path)
{
	char	*ac;
	int		i;
	char	*slash;

	i = 0;
	if (ft_strchr(*cmd, '/') && (access(*cmd, X_OK) == 0))
		return (*cmd);
	if (ft_strchr(*cmd, '/') && access(*cmd, X_OK))
		return (cmd[0]);
	while (path[i])
	{
		slash = ft_strjoin("/", cmd[0]);
		ac = ft_strjoin(path[i], slash);
		if ((access(ac, X_OK) == 0))
			return (ac);
		i++;
	}
	return (NULL);
}

char	*ft_find_path(char **cmd, char **env)
{
	char	**path;
	int		i;
	char	*res_path;

	i = 0;
	if (access(cmd[0], F_OK) == 0 && ft_strchr(cmd[0], '/'))
		return (ft_strdup(cmd[0]));
	while (env[i] && ft_strnstr(env[i], "PATH=", 5) == 0)
		i++;
	if (env[i] == NULL)
		return (ft_strdup(cmd[0]));
	path = ft_split(env[i] + 5, ':');
	res_path = ft_access_check(cmd, path);
	if (res_path == NULL)
	{
		ft_free_split(path);
		return (cmd[0]);
	}
	ft_free_split(path);
	return (res_path);
}
