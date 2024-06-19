/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:44:28 by jdelorme          #+#    #+#             */
/*   Updated: 2024/06/19 12:44:20 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

typedef enum e_error
{
	FILE_PROBLEM,
	COMMAND_ERROR,
	MEMORY_ERROR,
	INVALID_ARGS,
	PIPE_ERROR,
	FORK_ERROR
}	t_error;

char	*ft_access_check(char **cmd, char **path);
char	*ft_find_path(char **cmd, char **env);
void	ft_execute(char *cmd, char **env);
void	ft_child_process_one(char **argv, char **env, int *fd);
void	ft_child_process_two(char **argv, char **env, int *fd);
void	exit_child(t_error code);