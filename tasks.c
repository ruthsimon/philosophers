/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsimon <rsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:00:19 by rsimon            #+#    #+#             */
/*   Updated: 2024/06/01 21:01:40 by rsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void message(char *str, t_philo *philo)
{
    u_int64_t time;

    pthread_mutex_lock(&philo->data->write);
    time= get_time()-philo->data->start_time;
    // printf("time= %lu  gt=%lu , datat=%lu \n",time, get_time(), philo->data->start_time);
    if (ft_strcmp("has died", str)==0 && philo->data->dead==0)
    {
        printf("%lu %d %s\n", time, philo->id, str);
		philo->data->dead = 1;
	}
    if (!philo->data->dead)
		printf("%lu %d %s\n", time, philo->id, str);
    pthread_mutex_unlock(&philo->data->write);
}



void take_forks(t_philo *philo)
{
    pthread_mutex_lock(philo->r_fork);
    message("has taken a fork", philo);
    pthread_mutex_lock(philo->l_fork);
    message("has taken a fork", philo);
    philo->data->philosopher_states[philo->id - 1] = 1;
}

void drop_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->l_fork);
    pthread_mutex_unlock(philo->r_fork);
    philo->data->philosopher_states[philo->id - 1] = 0;
    message("is sleeping", philo);
    ft_usleep(philo->data->time_to_sleep);
}

void eat(t_philo *philo)
{
    
    take_forks(philo);
    pthread_mutex_lock(&philo->lock);
    philo->eating = 1;
    philo->time_to_die = get_time() + philo->data->time_to_die;
    //printf("it sh%li\n",philo->time_to_die);
    message("is eating", philo);
    philo->eat_count++;
    ft_usleep(philo->data->time_to_eat);
    //printf("eat done = %i  \n", philo->eat_count);
    philo->eating = 0;
    pthread_mutex_unlock(&philo->lock);
    drop_forks(philo);
    //printf("eat count is for [jilo]%i %i\n",philo->thread_id[1], philo->eat_count);

}

