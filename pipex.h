/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:10:04 by shechong          #+#    #+#             */
/*   Updated: 2023/07/20 11:46:11 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>

char	*get_path(char *cmd, char **env);
void	run_cmd(char *cmd, char **env);
void	run_cmd_redir(char *cmd, char **env);
void	run_pipex(int ac, char **av, char **env);
int		open_files(char *filename, char mode);
int		scan_cmd(int ac, char **av, char **env, int min);
int		array_count(char **a, int index, char *str);

#endif
