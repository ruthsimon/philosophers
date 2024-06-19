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

void parse(char **argv)
{
    int i=1;
    int j=0;
    while (argv[i])
    {
        j=0;
        while(argv[i][j]!='\0') 
        {
            if (argv[i][j] == ' ')
                j++;
            if (argv[i][j]<48 || argv[i][j]>57)
            {
                printf("Ensure that all inputs consist of numerical values!\n");
                return ;
            }
            j++;
        }
        i++;
    }
}


void malloc_data(t_data *data)
{
    data->thread_id= malloc(sizeof(pthread_t)* data->nb_philos);
    data->philo= malloc(sizeof(t_philo)*data->nb_philos);
    data->fork_mutex = (pthread_mutex_t*)malloc(data->nb_philos * sizeof(pthread_mutex_t));
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philos)
		pthread_mutex_init(&data->fork_mutex[i], NULL);
	i = 0;
	data->philo[0].l_fork = &data->fork_mutex[0];
	data->philo[0].r_fork = &data->fork_mutex[data->nb_philos - 1];
	i = 1;
	while (i < data->nb_philos)
	{
		data->philo[i].l_fork = &data->fork_mutex[i];
		data->philo[i].r_fork = &data->fork_mutex[i - 1];
		i++;
	}
	return (0);
}
// void init_forks(t_data *data)
// {
//     int i = 0;
    
//     // Initialize mutexes for the forks
//     while (i < data->nb_philos) {
//         pthread_mutex_init(&data->fork_mutex[i], NULL);
//         i++;
//     }
    
//     i = 0;
//     // Assign forks to each philosopher
//     while (i < data->nb_philos) {
//         // Left fork for philosopher i
//         data->philo[i].l_fork = &data->fork_mutex[i];
//         // Right fork for philosopher i
//         data->philo[i].r_fork = &data->fork_mutex[(i + 1) % data->nb_philos];
//         i++;
//     }
// }

// void init_forks(t_data *data)
// {
//     int i=0;
 
//     while (i < data->nb_philos)
//     {
//         pthread_mutex_init(&data->fork_mutex[i], NULL);
//         i++;
//     }
//     data->philo[0].l_fork=&data->fork_mutex[0];
//     data->philo[0].r_fork=&data->fork_mutex[data->nb_philos-1];
    
//     i=1;
//     while (i < data->nb_philos)
//     {
//         data->philo[i].l_fork=&data->fork_mutex[i];
//         data->philo[i].r_fork=&data->fork_mutex[i-1];
//         i++;
//     }
// }

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
        pthread_mutex_init(&data->philo[i].lock, NULL);
        i++;
    }
    
}

void init_data(int argc, char **argv)
{
    t_data *data;
    data = malloc(sizeof(t_data));
    data->nb_philos= ft_atoi(argv[1]);
    data->time_to_die= ft_atoi(argv[2]);
    data->time_to_eat= ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        data->nb_meals= ft_atoi(argv[5]);
    else
        data->nb_meals = -1;

    data->start_time=0;
    data->dead=0;
    data->eating_done=0;
    data->philosopher_states = (int *)malloc(data->nb_philos* sizeof(int));
    memset(data->philosopher_states, 0, data->nb_philos * sizeof(int));

    
    pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
    malloc_data(data);
    init_philo(data);
    init_forks(data);
    if (data->nb_philos==1)
        case_one(data);
    create_threads(data);
}