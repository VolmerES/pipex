/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:35:18 by volmer            #+#    #+#             */
/*   Updated: 2024/01/31 11:09:48 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "./libft/libft.h"

char    *ft_access_check(char **cmd, char **path)
{
    char    *ac;
    int     i;
    char     *slash;

    i = 0;
    printf("7. Comprobando que tenemos acceso a ese comando en cada ruta...\n");
    while (path[i])
    {
        //Protect -1 return form access;
        slash = ft_strjoin("/", cmd[0]);
        ac = ft_strjoin(path[i], slash);
        if ((access(ac, 0) == 0))
        {
            printf("\nComando encontrado en->%s\n", ac);
            return(ac);
        }
        printf("Comprobando path->%s\n", ac);
        i++;
    }
    return (NULL);
}
char *ft_find_path(char **cmd, char **env)
{
    char **path;
    char *found;
    int i;
    char    *res_path;

    printf("5. Buscando mediante strnstr PATH en env...\n");
    i = 0;
    while (ft_strnstr(env[i], "PATH=", 5) == 0)
    {
        i++;
    }
    printf("----------Encontrado en ENV[I]= %d----------\n\n", i);
    printf("6. Realizando un split de env para compobar ruta por ruta...\n");
    path = ft_split(env[i] + 5, ':');
    int j = 0;
    printf("Posibles paths: \n");
    while (path[j])
    {
        printf("----->%s\n", path[j]);
        j++;
    }
    printf("\n");
    res_path = ft_access_check(cmd, path);
    return (res_path);
}
void    ft_execute(char *cmd, char **env)
{
    /*
    - Divide el comando pasado como argumento (`argv`) utilizando `ft_split`
    dando lugar a la separacion en matriz en por ejemplo "ls", "-l" "-a".
    - Utiliza la función `find_path` para obtener la ruta completa del comando.
    - Si no encuentra la ruta, libera la memoria y llama a la función `error`.
    - Utiliza `execve` para ejecutar el comando con la ruta completa.*/

    char **command;
    char    *path;

    printf("4. Realizando un split de comandos, para separar de las flags...\n");
    command = ft_split(cmd, ' ');
    int i = 0;
    while (command[i])
    {
        printf("----------Split res= %s----------\n", command[i]);
        i++;
    }
    printf("\n");
// Crear find_path para obtetener la ruta completa del comando;
    path = ft_find_path(command, env);
    execve(path, command, env);
}

void    ft_child_process_one(char **argv, char **env, int *fd)
{
    int filein;
    
    filein = open(argv[1], O_RDONLY);
    if (filein == -1)
        perror("Filein crashed");
    dup2(filein, STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    printf("2. Duplicados de las salidas estándar, realizandose...\n");
    printf("3. Enviado el tercer agumento, para comprobar si el comando se encuentra en entorno...\n");
    printf("----------Tercer argcumento= %s----------\n\n", argv[2]);
    ft_execute(argv[2], env);
}
void    ft_child_process_two(char **argv, char **env, int *fd)
{
    int fileout;
    
    fileout = open(argv[4], O_RDONLY | O_CREAT | O_TRUNC, 0777);
    if (fileout == -1)
        perror("Filein crashed");
    dup2(fileout, STDOUT_FILENO);
    dup2(fd[0], STDIN_FILENO);
    close(fd[1]);
    close(fd[0]);
    ft_execute(argv[3], env);
}

int main(int argc, char **argv, char **env)
{
    int fd[2];
    int pid1;
    int pid2;
    
    if (argc != 5)
    {
        perror("Argumentos introducidos de manera incorrecta\n");
        printf("ejemplo de como tiene que ser la entrada\n");
    }
    if (pipe(fd) == -1)
        perror("Pipe crashed\n");
    pid1 = fork();
    if (pid1 == -1)
        perror("Fork crashed\n");
    if (pid1 == 0)
    {
        printf("1. El primer proceso hijo es creado...\n");
        printf("----------Process ID1= %d----------\n\n", pid1);
        ft_child_process_one(argv, env, fd);
    }
    pid2 = fork();
    if (pid2 == -1)
        perror("Fork crashed\n");
    if (pid2 == 0)
    {
        ft_child_process_two(argv, env, fd);
    }
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return (0);
}