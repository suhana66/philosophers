/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:02:25 by susajid           #+#    #+#             */
/*   Updated: 2024/01/24 18:28:34 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_unsigned(char *str, unsigned int *result);
t_philo		*init_philos(t_simulation sim);
int			init_threads(t_simulation sim);
void		destroy_sim(t_simulation sim);
void		routine(t_philo *philo);

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (argc != 5 && argc != 6)
		return (ft_perror(USAGE_ERR), 1);
	sim.quit = false;
	sim.number_of_meals = -1;
	if (parse_unsigned(argv[1], &sim.number_of_philo)
		|| sim.number_of_philo == 0
		|| parse_unsigned(argv[2], &sim.time_to_die)
		|| parse_unsigned(argv[3], &sim.time_to_eat)
		|| parse_unsigned(argv[4], &sim.time_to_sleep)
		|| (argc == 6 && parse_unsigned(argv[5], &sim.number_of_meals)))
		return (ft_perror(INVALID_ARGS_ERR), 2);
	if (pthread_mutex_init(&sim.write, NULL))
		return (ft_perror(MUTEX_INIT_ERR), 3);
	sim.philos = init_philos(sim);
	if (!sim.philos)
		return (pthread_mutex_destroy(&sim.write), 4);
	if (init_threads(sim))
		return (5);
	return (0);
}

/*
	whitespace characters: '\t', '\n', '\v', '\f', '\r', ' '
					ascii: 9, 10, 11, 12, 13, 32
*/
static int	parse_unsigned(char *str, unsigned int *result)
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

t_philo	*init_philos(t_simulation sim)
{
	t_philo			*philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * sim.number_of_philo);
	if (!sim.philos)
		return (ft_perror(MALLOC_ERR), NULL);
	i = 0;
	while (i < sim.number_of_philo)
	{
		philos[i].id = i + 1;
		philos[i].sim = sim;
		if (pthread_mutex_init(&philos[i].fork, NULL))
		{
			while (--i)
				pthread_mutex_destroy(&philos[i].fork);
			return (free(philos), NULL);
		}
		i++;
	}
	return (philos);
}

int	init_threads(t_simulation sim)
{
	int	i;

	i = -1;
	while (++i < (int)sim.number_of_philo)
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *)) & routine, &sim.philos[i]))
			return (destroy_sim(sim), ft_perror(THREAD_CREATE_ERR), 1);
	while (--i >= 0)
		if (pthread_join(sim.philos[i].thread, NULL))
			return (destroy_sim(sim), ft_perror(THREAD_JOIN_ERR), 1);
	return (0);
}

void	destroy_sim(t_simulation sim)
{
	unsigned int	i;

	pthread_mutex_destroy(&sim.write);
	i = 0;
	while (i < sim.number_of_philo)
		pthread_mutex_destroy(&sim.philos[i++].fork);
	free(sim.philos);
}

void	routine(t_philo *philo)
{
	(void)philo;
}
