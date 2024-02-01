/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 11:05:44 by susajid           #+#    #+#             */
/*   Updated: 2024/02/01 12:23:32 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_log(t_philo *philo, char *action);

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

bool	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->mutex);
	if (!philo->sim->quit && get_time() - philo->last_meal > philo->sim->time_to_die)
	{
		print_log(philo, DEAD);
		philo->sim->quit = true;
	}
	if (philo->sim->quit)
		return (pthread_mutex_unlock(&philo->sim->mutex), false);
	print_log(philo, action);
	return (pthread_mutex_unlock(&philo->sim->mutex), true);
}

static void	print_log(t_philo *philo, char *action)
{
	if (action)
		printf("%zu %zu %s\n",
			get_time() - philo->sim->start_time, philo->id, action);
}
