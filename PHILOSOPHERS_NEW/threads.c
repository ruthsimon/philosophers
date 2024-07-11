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

void monitor(t_philo *philo)
{
   pthread_mutex_lock(&philo->data->meals);
   if (philo->data->eating_done >= philo->data->nb_philos)
   {
    pthread_mutex_lock(&philo->data->dead);
    philo->data->any_dead=1;
    pthread_mutex_unlock(&philo->data->dead);
   }    
   pthread_mutex_unlock(&philo->data->meals);
}

int is_philo_dead(t_data *data)
{
    int res=0;
    pthread_mutex_lock(&data->dead);
    res=data->any_dead;
    pthread_mutex_unlock(&data->dead);
    return (res);
}

int supervisor(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;

    while (1)
    {
        if (is_philo_dead(data))
            return (0);
        i = 0;
        while (i < data->nb_philos)
        {
            pthread_mutex_lock(&data->dead);
            if (data->philo[i].time_to_die < get_time())
            {
                data->any_dead = 1;
                pthread_mutex_unlock(&data->dead);
                return (0); // Philosopher died, stop monitoring
            }
            pthread_mutex_unlock(&data->dead);
            i++;
        }
        usleep(200);
    }
    return (0);
}


void *routine(void *pointer)
{
    t_philo *philo;

    philo= (t_philo *)pointer;
    
    if (philo->id %2 == 0)
    {
        message("is thinking", philo);
    }
    while (!is_philo_dead(philo->data))
    {
        if (!take_forks(philo))
            break;
        monitor(philo);
        if (!philo_sleeping(philo))
            break;
        if (!philo_thinking(philo))
            break;
    }
    return ((void *)0);     
}

int case_one(t_data *data)
{
    data->start_time = get_time();
    pthread_mutex_lock(&data->meals);
    message("has taken a fork",data->philo);
    pthread_mutex_unlock(&data->meals);
    ft_usleep(data->time_to_die);
    pthread_mutex_lock(&data->meals);
    message("has died",data->philo);
    pthread_mutex_unlock(&data->meals);
    ft_exit(data);
    return (0);
}


void create_threads(t_data *data)
{
   
    
    data->start_time=get_time();
    
    int i = 0;
    while (i < data->nb_philos)
    {
       
        if (pthread_create(&data->thread_id[i], NULL, &routine, &data->philo[i]))
            error_msg("Thread for philos failed\n",data);
        ft_usleep(1);
        i++;
    }
    supervisor(data);
    i =0;
    while (i < data->nb_philos )
    {
        pthread_join(data->thread_id[i], NULL);
        i++;
    }
    ft_exit(data);
}