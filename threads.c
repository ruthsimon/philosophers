/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsimon <rsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:00:36 by rsimon            #+#    #+#             */
/*   Updated: 2024/06/01 21:00:07 by rsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor(void *pointer)
{
    t_philo *philo;

	philo = (t_philo *)pointer;

    while (philo->data->dead == 0)
    {
        pthread_mutex_lock(&philo->lock);
        if (philo->data->eating_done >= philo->data->nb_philos)
            philo->data->dead=1;
        pthread_mutex_unlock(&philo->lock);
    }
    return ((void *)0);
}


void *supervisor(void *pointer)
{
    t_philo *philo;
	philo = (t_philo *) pointer;

    while (philo->data->dead==0)
    {
        pthread_mutex_lock(&philo->lock);
        if ( get_time() > philo->time_to_die  && philo->eating==0)
        {
            //printf("%lu- when it shoudl die , %llu, when it died",philo->time_to_die,get_time() );
            message("has died", philo);           
        }
        if (philo->eat_count == philo->data->nb_meals)
       {
            pthread_mutex_lock(&philo->data->lock);
            philo->data->eating_done++;
            philo->eat_count++;
            pthread_mutex_unlock(&philo->data->lock);
        }
        pthread_mutex_unlock(&philo->lock);
    }
    return ((void *)0);
    
}

void *routine(void *pointer)
{
    t_philo *philo;

    philo= (t_philo *)pointer;

    philo->time_to_die = philo->data->time_to_die + get_time();

    if (pthread_create(&philo->t1, NULL, &supervisor,(void *)philo))
        error_msg("Thread creation t1 failed\n", philo->data);
    while (philo->data->dead==0)
    {
        eat(philo);
        message("is thinking", philo);

    }
    if (pthread_join(philo->t1, NULL))
		return ((void *)1);
    return ((void *)0);     
}

int case_one(t_data *data)
{
    
    data->start_time = get_time();
    
    if (pthread_create(&data->thread_id[0], NULL, &routine, &data->philo[0]))
        error_msg("thread creation case1 failed\n", data);
    pthread_detach(data->thread_id[0]);
    while(data->dead==0)
        ft_usleep(0);
    return (0);
}


void create_threads(t_data *data)
{
    data->start_time=get_time();
    int i = 0;
    pthread_t	t0;
    if (data->nb_meals>0)
    {
        if ( pthread_create(&t0, NULL, &monitor, &data->philo[0]))
            error_msg("Thread creation t0 failed\n",data);   
    }
    while (i < data->nb_philos)
    {
        if (pthread_create(&data->thread_id[i], NULL, &routine, &data->philo[i]))
            error_msg("Thread for philos failed\n",data);
        ft_usleep(1);
        i++;
    }

    i =0;
    while (i < data->nb_philos )
    {
        pthread_join(data->thread_id[i], NULL);
        i++;
    }
    ft_exit(data);
}