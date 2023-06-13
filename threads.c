/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlongo <mlongo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:28:12 by mlongo            #+#    #+#             */
/*   Updated: 2023/06/13 12:52:31 by mlongo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	*routinemonitor(void *data2)
{
	int		i;
	t_data	*data;

	i = 0;
	data = (t_data *)data2;
	while (!data->finished && !data->dead)
	{
		i = 0;
		while (i < data->philo_num)
		{
			if (data->philos[i].eat_cont <= data->meals_nb)
				break ;
			i++;
		}
		if (i == data->philo_num)
			data->finished = 1;
	}

	return (NULL);
}

void	*routineSupervisor(void *philo2)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)philo2;
	while (!philo->data->dead && !philo->data->finished)
	{
		if ((get_time() >= philo->time_to_die) && !philo->eating)
		{
			pthread_mutex_lock(&philo->data->lock);
			died(philo);
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->lock);
		}
	}
	return (NULL);
}

void	*routinePhilo(void	*philo2)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)philo2;
	if (pthread_create(&philo->t1, NULL, &routineSupervisor, (void *)philo))
		return (NULL);
	while (!philo->data->dead && !philo->data->finished)
	{
		eat(philo);
		mine_sleep(philo);
	}
	return (NULL);
}