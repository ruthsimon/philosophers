/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsimon <rsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:00:48 by rsimon            #+#    #+#             */
/*   Updated: 2024/06/01 20:08:00 by rsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int parse(char **argv)
{
    int i=1;
    int j=0;
    while (argv[i])
    {
        if (ft_atoi(argv[i])> INT_MAX)
        {
            ft_putstr_fd("Input should be below int max\n", 2);
            return(0);
        }
        if (ft_atoi(argv[i])<0)
        {
            ft_putstr_fd("Input should be positive\n", 2);
            return(0);
        }
        j=0;
        while(argv[i][j]!='\0') 
        {
            if (argv[i][j] == ' ')
                j++;
            if (argv[i][j]<48 || argv[i][j]>57)
            {
                ft_putstr_fd("Ensure inputs is numerical/positive!\n",2);
                return (0);
            }
            j++;
        }
        i++;
    }
    return (1);
}


void malloc_data(t_data *data)
{
    data->thread_id= malloc(sizeof(pthread_t)* data->nb_philos);
    data->philo= malloc(sizeof(t_philo)*data->nb_philos);
    data->fork_mutex = (pthread_mutex_t*)malloc(data->nb_philos * sizeof(pthread_mutex_t));
}

void init_forks(t_data *data)
{
    
    int i =0;

    while (i < data->nb_philos)
    {
        data->philo[i].left_fork= &data->forks_philo[i];
        data->philo[i].l_fork_mutex= &data->fork_mutex[i];
        if (data->philo[i].id== data->nb_philos)
        {
            data->philo[i].right_fork = &data->forks_philo[0];
            data->philo[i].r_fork_mutex= &data->fork_mutex[0];
        }
        else
        {
            data->philo[i].right_fork= &data->forks_philo[i+1];
            data->philo[i].r_fork_mutex= &data->fork_mutex[i+1];
        }
        i++;
    }
 
}

int	init_fork_mutex(t_data *data)
{
	int	i;
    int j;


	i = -1;
    j=-1;

    data->fork_mutex= malloc(sizeof(pthread_mutex_t)*data->nb_philos);
    if (!data->fork_mutex)
		return 0;
    data->forks_philo =  malloc(data->nb_philos* sizeof(int));
    if (!data->forks_philo)
        return 0;
    data->philo= malloc(sizeof(t_philo)*data->nb_philos);

	while (++i < data->nb_philos)
		pthread_mutex_init(&data->fork_mutex[i], NULL);
	while (++j <data->nb_philos)
    {
         data->forks_philo[j]=-1;
         //printf("after setting %i", data->forks_philo[j]=-1);
    }
       
    return 1;
}




void init_philo(t_data *data)
{
    int i = 0;

    while (i < data->nb_philos)
    {
        data->philo[i].id=i+1;
        data->philo[i].eat_count=0;
        data->philo[i].status=0;
        data->philo[i].eating=0;
        data->philo[i].data= data;
        data->philo[i].time_to_die=0;
        data->philo[i].dead=0;
        //pthread_mutex_init(&data->philo[i].lock, NULL);
        i++;
    }
    
}

int init_data(int argc, char **argv)
{
    t_data *data;
    data = malloc(sizeof(t_data));
    data->nb_philos= ft_atoi(argv[1]);
    data->time_to_die= ft_atoi(argv[2]);
    data->time_to_eat= ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    if (data->nb_meals < 0 )
    {
        ft_putstr_fd("Requested number of meals is 0\n", 2);
        return(0);
    }
    if (argc == 6)
        data->nb_meals= ft_atoi(argv[5]);
    else
        data->nb_meals = -1;
    data->start_time=0;
    data->eating_done=0;
    data->any_dead=0;
    data->thread_id= malloc(sizeof(pthread_t)* data->nb_philos);

    
    init_fork_mutex(data);
    
    init_philo(data);
   
    init_forks(data);
   
    pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->dead, NULL);
    pthread_mutex_init(&data->eat, NULL);
    pthread_mutex_init(&data->meals, NULL);
           
    if (data->nb_philos==1)
    {
        case_one(data);
        return(0);
    }
    
    create_threads(data);
    return(1);
}