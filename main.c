/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsimon <rsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:00:45 by rsimon            #+#    #+#             */
/*   Updated: 2024/06/01 19:00:46 by rsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
int error_msg(char *msg, t_data *data)
{
    write(1, msg, ft_strlen(msg));
    if (data)
        ft_exit(data);
    return(1);
}

int main(int argc, char **argv)
{
    if (argc !=5  && argc !=6)
       write(1,"Wrong num of inputs\n",ft_strlen("Wrong num of inputs\n"));
    parse(argv);
    init_data(argc, argv);
   
}