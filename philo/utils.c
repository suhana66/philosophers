/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:05:44 by susajid           #+#    #+#             */
/*   Updated: 2024/01/26 10:15:41 by susajid          ###   ########.fr       */
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

void	ft_usleep(size_t milliseconds)
{
	long int	time;

	time = get_time();
	while (get_time() - time < milliseconds)
		usleep(milliseconds / 10);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((size_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
