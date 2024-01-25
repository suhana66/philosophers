/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:05:44 by susajid           #+#    #+#             */
/*   Updated: 2024/01/25 12:39:12 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_perror(char *msg)
{
	if (!msg)
		return ;
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
}

unsigned long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		return (0);
	return ((unsigned long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
