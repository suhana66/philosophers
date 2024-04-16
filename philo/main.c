/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:20:38 by susajid           #+#    #+#             */
/*   Updated: 2024/04/15 20:02:15 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;
	unsigned int	i;

	if (sim_init(&sim, argc - 1, &argv[1]))
		return (1);
	sim.start_time = get_time();
	i = 0;
	while (i < sim.n_philo)
	{
		sim.philos[i].last_meal = get_time();
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *))routine, &sim.philos[i]) || !++i)
			return (ft_perror(THREAD_CREATE_ERR), sim_destroy(&sim), 2);
	}
	while (!sim_quit(&sim))
		philo_sleep(1, &sim);
	i = 0;
	while (i < sim.n_philo)
		if (pthread_join(sim.philos[i++].thread, NULL))
			return (ft_perror(THREAD_JOIN_ERR), sim_destroy(&sim), 3);
	return (sim_destroy(&sim), 0);
}

void	*routine(t_philo *philo)
{
	t_fork	*fork1;
	t_fork	*fork2;

	if (philo->id % 2)
		fork1 = &philo->fork;
	else
		fork1 = &philo->sim->philos[philo->id % philo->sim->n_philo].fork;
	if (philo->id % 2)
		fork2 = &philo->sim->philos[philo->id % philo->sim->n_philo].fork;
	else
		fork2 = &philo->fork;
	while (!check_quit(philo->sim))
	{
		if (eat(philo, fork1, fork2))
			break ;
		print(philo, SLEEPING);
		philo_sleep(philo->sim->t_sleep, philo->sim);
		print(philo, THINKING);
	}
	return (NULL);
}

int	eat(t_philo *philo, t_fork	*fork1, t_fork	*fork2)
{
	if (fork1 == fork2)
		return (print(philo, TAKEN_FORK), 1);
	while (1)
	{
		pthread_mutex_lock(&fork1->mutex);
		if (philo->id == fork1->value)
		{
			pthread_mutex_unlock(&fork1->mutex);
			continue ;
		}
		fork1->value = philo->id;
		print(philo, TAKEN_FORK);
		break ;
	}
	while (1)
	{
		pthread_mutex_lock(&fork2->mutex);
		if (philo->id == fork2->value)
		{
			pthread_mutex_unlock(&fork2->mutex);
			continue ;
		}
		fork2->value = philo->id;
		print(philo, TAKEN_FORK);
		break ;
	}
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->sim->meal_lock);
	print(philo, EATING);
	philo_sleep(philo->sim->t_eat, philo->sim);
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->meal_counter++;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	pthread_mutex_unlock(&fork1->mutex);
	pthread_mutex_unlock(&fork2->mutex);
	return (0);
}

void	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->write_lock);
	if (check_quit(philo->sim))
		return ((void)pthread_mutex_unlock(&philo->sim->write_lock));
	printf("%zu %d %s\n", get_time() - philo->sim->start_time,
		philo->id, action);
	pthread_mutex_unlock(&philo->sim->write_lock);
}

int	sim_quit(t_simulation *sim)
{
	unsigned int	i;
	unsigned int	satisfied_philos;
	int				if_dead;

	i = 0;
	satisfied_philos = 0;
	if_dead = 0;
	while (++i <= sim->n_philo)
	{
		if_dead = check_last_meal(&sim->philos[i - 1]);
		if (if_dead || (pthread_mutex_lock(&sim->meal_lock), 0))
			break ;
		if (sim->if_limit && sim->philos[i - 1].meal_counter >= sim->n_meal)
			satisfied_philos++;
		pthread_mutex_unlock(&sim->meal_lock);
	}
	if (if_dead)
		print(&sim->philos[i - 1], DEAD);
	if (if_dead || (sim->if_limit && satisfied_philos == sim->n_philo))
	{
		pthread_mutex_lock(&sim->dead_lock);
		sim->if_quit = 1;
		return (pthread_mutex_unlock(&sim->dead_lock), 1);
	}
	return (0);
}
