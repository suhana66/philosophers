/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:21:11 by susajid           #+#    #+#             */
/*   Updated: 2024/05/08 13:20:59 by susajid          ###   ########.fr       */
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
	if (get_time() - philo->last_meal >= philo->sim->t_die && !philo->eating)
		return (pthread_mutex_unlock(&philo->sim->meal_lock), 1);
	pthread_mutex_unlock(&philo->sim->meal_lock);
	return (0);
}

void	do_sleep(size_t milliseconds, t_simulation *sim)
{
	size_t	start;

	start = get_time();
	while (!check_quit(sim) && get_time() - start < milliseconds)
		usleep(500);
}

size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		ft_perror(TIME_ERR);
	return ((size_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_perror(char *msg)
{
	size_t	len;

	if (!msg)
		return ;
	len = 0;
	while (msg[len])
		len++;
	write(2, msg, len);
}
