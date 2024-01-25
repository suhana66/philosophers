/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:02:25 by susajid           #+#    #+#             */
/*   Updated: 2024/01/25 10:51:31 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	routine(t_philo *philo);

int	main(int argc, char **argv)
{
	t_simulation	sim;
	int				i;

	if (sim_init(&sim, argc - 1, &argv[1]))
		return (1);
	i = -1;
	while (++i < (int)sim.number_of_philo)
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *))routine, &sim.philos[i]))
			return (sim_destroy(sim), ft_perror(THREAD_CREATE_ERR), 1);
	while (--i >= 0)
		if (pthread_join(sim.philos[i].thread, NULL))
			return (sim_destroy(sim), ft_perror(THREAD_JOIN_ERR), 1);
	return (0);
}

static void	routine(t_philo *philo)
{
	(void)philo;
}
