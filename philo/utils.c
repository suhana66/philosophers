/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:21:11 by susajid           #+#    #+#             */
/*   Updated: 2024/04/15 16:04:30 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_quit(t_simulation *sim)
{
	pthread_mutex_lock(&sim->dead_lock);
	if (sim->if_quit)
		return (pthread_mutex_unlock(&sim->dead_lock), 1);
	pthread_mutex_unlock(&sim->dead_lock);
	return (0);
}

int	check_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->sim->meal_lock);
	if (get_time() - philo->last_meal >= philo->sim->t_die)
		return (pthread_mutex_unlock(&philo->sim->meal_lock), 1);
	pthread_mutex_unlock(&philo->sim->meal_lock);
	return (0);
}

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
	size_t	start;

	start = get_time();
	while (get_time() - start < milliseconds)
		usleep(500);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((size_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
