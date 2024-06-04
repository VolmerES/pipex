/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:42:35 by jdelorme          #+#    #+#             */
/*   Updated: 2024/06/04 14:28:55 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

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
		return (NULL); 
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
	while (env[i] && ft_strnstr(env[i], "PATH=", 5) == 0)
		i++;
	if (env[i] == NULL)
		return NULL;
	path = ft_split(env[i] + 5, ':');
	res_path = ft_access_check(cmd, path);
	ft_free_split(path);
	return (res_path);
}
