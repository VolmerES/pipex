/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:44:28 by jdelorme          #+#    #+#             */
/*   Updated: 2024/06/13 14:44:26 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
typedef enum
{
	FILE_PROBLEM,
	COMMAND_ERROR,
	MEMORY_ERROR,
	INVALID_ARGS,
	PIPE_ERROR,
	FORK_ERROR
} t_error;

char	*ft_access_check(char **cmd, char **path);
char	*ft_find_path(char **cmd, char **env);
void	ft_execute(char *cmd, char **env);
void	ft_child_process_one(char **argv, char **env, int *fd);
void	ft_child_process_two(char **argv, char **env, int *fd);