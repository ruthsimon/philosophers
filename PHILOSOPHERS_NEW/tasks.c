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

int is_fork_available(t_philo *philo) {
    // Lock the left fork
    //int j=-1;

    
    pthread_mutex_lock(philo->l_fork_mutex);
    printf("keft fork %i\n", *philo->left_fork);
    // Check if the left fork is available
    if (*philo->left_fork && *philo->left_fork!=philo->id)
    {
         printf("left fork %i\n", *philo->left_fork);
        // If available, mark it as held by the current philosopher
        pthread_mutex_unlock(philo->l_fork_mutex);
        if (*philo->right_fork && *philo->right_fork != philo->id) 
        {
            // If available, mark it as held by the current philosopher
            *philo->right_fork = 0;
            *philo->left_fork = 0;
            pthread_mutex_unlock(philo->r_fork_mutex);
        }
        else
        {
            pthread_mutex_unlock(philo->r_fork_mutex);
            return (0);
        }
    } 
    else {
        // If not available, release the lock and return 0 (not available)
        pthread_mutex_unlock(philo->l_fork_mutex);
        return (0);
    }
    
    // Lock the right fork
   
    // If both forks are available, return 1 (available)
    //printf("both are available\n");
    return 1;
}

int philo_took_fork(t_philo *philo)
{
    if (!is_fork_available(philo))
    {
         return(0);
    }
    message("has taken a fork", philo);
    message("has taken a fork", philo);
    return (1);
}

void message(char *str, t_philo *philo)
{
    if (!is_philo_dead(philo->data))
    {
        pthread_mutex_lock(&philo->data->print);
        printf("%lu %d %s\n", get_time()-philo->data->start_time, philo->id, str);
        pthread_mutex_unlock(&philo->data->print);
    }
}

int take_forks(t_philo *philo)
{
    
    if (is_philo_dead(philo->data))
        return(0);
    while (!philo_took_fork(philo))
    {
        usleep(100);
        if (is_philo_dead(philo->data))
            return(0);
    }
    //printf("its nit even reaching here\n");
    if (!philo_eating(philo))
        return(0);
    return (1);
}


void drop_forks(t_philo *philo)
{
     pthread_mutex_lock(philo->r_fork_mutex);
    *philo->right_fork=philo->id;
    pthread_mutex_unlock(philo->r_fork_mutex);
    pthread_mutex_lock(philo->l_fork_mutex);
    *philo->left_fork=philo->id;
    pthread_mutex_unlock(philo->l_fork_mutex);
}

int philo_eating(t_philo *philo)
{
    message("is eating", philo);
    pthread_mutex_lock(&philo->data->eat);
    philo->time_to_die = get_time() + philo->data->time_to_die;   
    pthread_mutex_unlock(&philo->data->eat);
    if (is_philo_dead(philo->data))
        return(drop_forks(philo),0);
    ft_usleep(philo->data->time_to_eat);
    // printf("%lu\n", get_time()-philo->data->start_time);
    
    philo->eat_count++;
    if (philo->eat_count == philo->data->nb_meals)
    {
        pthread_mutex_lock(&philo->data->eat);
        philo->data->eating_done++;
        pthread_mutex_unlock(&philo->data->eat);
    }
    drop_forks(philo);
    return(1);        
}

int philo_sleeping(t_philo *philo)
{
    
    if (!is_philo_dead(philo->data))
    {
        message("is sleeping", philo);
        ft_usleep(philo->data->time_to_sleep);
          //printf("hi\n");
        return(1);
    }
    else 
        return (0);
}

int philo_thinking(t_philo *philo)
{
    if (!is_philo_dead(philo->data))
    {
        message("is thinking", philo);
        return(1);
    }
    else 
        return (0);
}