/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 18:29:39 by aborboll          #+#    #+#             */
/*   Updated: 2021/07/25 13:05:33 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_H
# define INTERNAL_H

/*
** Define args types
*/
# define CMD 1
# define FLAG 2
# define ARG 3
# define FILE 4
# define R_IN 5
# define R_OUT 6
# define R_AOUT 7

/*
** Define args types
*/
# define D_QUOT 1
# define S_QUOT 2
# define N_QUOT 3

/*
** Define chars
*/
# define DEL 127

/*
** Define errors
*/
# define ERR_MALLOC "malloc() error: failed to allocate bytes!"
# define ERR_RED "minishell: %s: %s\n"
# define ERR_RED_N "minishell: syntax error near redirect: %s\n"
# define ERR_P_DEN "Permission denied\n"
# define ERR_N_FILE_DIR "minishell: %s: No such file or directory\n"
# define ERR_C "minishell: %s: command not found\n"
# define ERR_I_DIR "minishell: %s: Is a directory\n"
# define ERR_FAU "usage: . filename [arguments]\n"
# define ERR_FA "minishell: %s: filename argument required\n.: "
# define ERR_UT "minishell: syntax error near unexpected token `%s'\n"
# define ERR_UQ "minishell: Unmaching opening quotes at col %d: %s\n"
# define ERR_UQ "minishell: Unmaching opening quotes at col %d: %s\n"
# define ERR_UN "minishell: unset: `%s': not a valid identifier\n"
# define ERR_EX "minishell: export: `%s=%s': not a valid identifier\n"
# define ERR_EXI "minishell: export: `%s': not a valid identifier\n"
#endif
