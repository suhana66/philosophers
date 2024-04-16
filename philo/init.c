/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:20:34 by susajid           #+#    #+#             */
/*   Updated: 2024/04/15 20:14:15 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sim_init(t_simulation *sim, int argc, char **argv)
{
	if (argc != 4 && argc != 5)
		return (ft_perror(USAGE_ERR), 1);
	if (str_to_natural(argv[0], &sim->n_philo)
		|| str_to_natural(argv[1], &sim->t_die)
		|| str_to_natural(argv[2], &sim->t_eat)
		|| str_to_natural(argv[3], &sim->t_sleep)
		|| (argc == 5 && str_to_natural(argv[4], &sim->n_meal)))
		return (ft_perror(INVALID_ARGS_ERR), 2);
	sim->if_quit = 0;
	sim->if_limit = (argc == 5);
	if (pthread_mutex_init(&sim->write_lock, NULL))
		return (ft_perror(MUTEX_INIT_ERR), 3);
	if (pthread_mutex_init(&sim->dead_lock, NULL))
		return (pthread_mutex_destroy(&sim->write_lock),
			ft_perror(MUTEX_INIT_ERR), 4);
	if (pthread_mutex_init(&sim->meal_lock, NULL))
		return (pthread_mutex_destroy(&sim->write_lock),
			pthread_mutex_destroy(&sim->dead_lock),
			ft_perror(MUTEX_INIT_ERR), 5);
	if (philos_init(sim))
		return (pthread_mutex_destroy(&sim->write_lock),
			pthread_mutex_destroy(&sim->dead_lock),
			pthread_mutex_destroy(&sim->meal_lock),
			ft_perror(MUTEX_INIT_ERR), 6);
	return (0);
}

int	str_to_natural(char *str, unsigned int *result)
{
	size_t	prev_res;

	if (!str || !result)
		return (1);
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str < '0' || *str > '9')
		return (2);
	*result = (*str++ - '0');
	while (*str >= '0' && *str <= '9')
	{
		prev_res = *result;
		*result = *result * 10 + (*str++ - '0');
		if (prev_res > *result)
			return (3);
	}
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str)
		return (4);
	if (!*result)
		return (5);
	return (0);
}

int	philos_init(t_simulation *sim)
{
	size_t	i;

	sim->philos = malloc(sizeof(t_philo) * sim->n_philo);
	if (!sim->philos)
		return (ft_perror(MALLOC_ERR), 1);
	i = 0;
	while (i < sim->n_philo)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].eating = 0;
		sim->philos[i].sim = sim;
		sim->philos[i].meal_counter = 0;
		sim->philos[i].fork.value = 0;
		if (pthread_mutex_init(&sim->philos[i].fork.mutex, NULL))
		{
			while (i > 0)
				pthread_mutex_destroy(&sim->philos[--i].fork.mutex);
			return (free(sim->philos), ft_perror(MUTEX_INIT_ERR), 2);
		}
		i++;
	}
	return (0);
}

void	sim_destroy(t_simulation *sim)
{
	unsigned int	i;

	i = 0;
	pthread_mutex_destroy(&sim->write_lock);
	pthread_mutex_destroy(&sim->meal_lock);
	pthread_mutex_destroy(&sim->dead_lock);
	while (i < sim->n_philo)
		pthread_mutex_destroy(&sim->philos[i++].fork.mutex);
	free(sim->philos);
}
