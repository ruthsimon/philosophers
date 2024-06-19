/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsimon <rsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:00:40 by rsimon            #+#    #+#             */
/*   Updated: 2024/06/01 19:00:41 by rsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

u_int64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_msg("getting time failed\n", NULL);
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

static int	ft_checkmnumbers(const char *str, long int result, int sign)
{
	int	digit;

	digit = 0;
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if ((result > (LLONG_MAX - digit) / 10) && sign > 0)
			return (-1);
		else if ((result > (LLONG_MAX - digit) / 10) && sign < 0)
			return (0);
		result *= 10;
		result = result + digit;
		str++;
	}
	return ((int)result * sign);
}

int	ft_atoi(const char *str)
{
	long long int	result;
	int				sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	if (*str == '-' || *str == '+')
		return (0);
	return (ft_checkmnumbers(str, result, sign));
}


int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 != '\0' && (*s1 == *s2))
	{
		s1++;
		s2++;	
	}
	return (*(char *)s1 - *(char *)s2);
}


int ft_usleep(u_int64_t time)
{
	u_int64_t start;
	
	start = get_time();

	while ((get_time() - start) < time)
		usleep(time/10);
	
	return (0);
}



void	ft_exit(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->philo->r_fork[i]);
		pthread_mutex_destroy(&data->philo->l_fork[i]);
		pthread_mutex_destroy(&data->fork_mutex[i]);
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	pthread_mutex_destroy(&data->philo->lock);


	free(data);
}