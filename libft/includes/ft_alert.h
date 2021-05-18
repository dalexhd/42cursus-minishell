/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alert.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 17:55:19 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/05 20:41:35 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ALERT_H
# define FT_ALERT_H

# include "types.h"
# include "color.h"

void	ft_error(char *s, t_bool force, ...);
void	ft_warn(char *s, ...);
void	ft_success(char *s, t_bool force, ...);
void	ft_info(char *s, ...);

#endif
