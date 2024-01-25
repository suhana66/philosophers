/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 08:50:29 by susajid           #+#    #+#             */
/*   Updated: 2024/01/25 18:20:59 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	str_to_uint(char *str, unsigned int *result);
static int	philos_init(t_simulation *sim);

int	sim_init(t_simulation *sim, int argc, char **argv)
{
	if (argc != 4 && argc != 5)
		return (ft_perror(USAGE_ERR), 1);
	sim->quit = false;
	sim->number_of_meals = -1;
	if (str_to_uint(argv[0], &sim->number_of_philo) || !sim->number_of_philo
		|| str_to_uint(argv[1], &sim->time_to_die)
		|| str_to_uint(argv[2], &sim->time_to_eat)
		|| str_to_uint(argv[3], &sim->time_to_sleep)
		|| (argc == 5 && str_to_uint(argv[4], &sim->number_of_meals)))
		return (ft_perror(INVALID_ARGS_ERR), 2);
	if (pthread_mutex_init(&sim->write, NULL))
		return (ft_perror(MUTEX_INIT_ERR), 3);
	if (philos_init(sim))
		return (pthread_mutex_destroy(&sim->write), 4);
	return (0);
}

/*
	whitespace characters: '\t', '\n', '\v', '\f', '\r', ' '
					ascii: 9, 10, 11, 12, 13, 32
*/
static int	str_to_uint(char *str, unsigned int *result)
{
	unsigned int	prev_res;

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
	return (0);
}

static int	philos_init(t_simulation *sim)
{
	int	i;

	sim->philos = malloc(sizeof(t_philo) * sim->number_of_philo);
	if (!sim->philos)
		return (ft_perror(MALLOC_ERR), 1);
	i = -1;
	while (++i < (int)sim->number_of_philo)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].sim = sim;
		if (pthread_mutex_init(&sim->philos[i].fork, NULL))
		{
			while (--i)
				pthread_mutex_destroy(&sim->philos[i].fork);
			return (free(sim->philos), ft_perror(MUTEX_INIT_ERR), 2);
		}
	}
	return (0);
}

void	sim_destroy(t_simulation sim)
{
	unsigned int	i;

	pthread_mutex_destroy(&sim.write);
	i = 0;
	while (i < sim.number_of_philo)
		pthread_mutex_destroy(&sim.philos[i++].fork);
	free(sim.philos);
}
