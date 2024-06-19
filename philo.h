#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>
#include <string.h>


typedef struct s_data t_data;


typedef struct  s_philo{

    int             id;
    int             eat_count;
    int             status;
    u_int64_t       time_to_die;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
    t_data          *data;
    pthread_t       t1;
    pthread_mutex_t lock;
    int     eating;

}t_philo;


typedef struct s_data{
   
    int       nb_philos;
    int       nb_meals;
    u_int64_t        start_time;
    u_int64_t        time_to_die;
    u_int64_t        time_to_eat;
    u_int64_t        time_to_sleep;
    pthread_t        *thread_id;
    t_philo          *philo;
    pthread_mutex_t *fork_mutex;
    pthread_mutex_t lock;
    pthread_mutex_t write;
    int             dead;
    int             eating_done;
    int             *philosopher_states;
}t_data;


//main.c
int error_msg(char *msg, t_data *data);

//init.c
void parse(char **argv);
void init_data(int argc, char **argv);

//utils.c
int	ft_atoi(const char *str);
u_int64_t   get_time(void);
int			ft_strcmp(char *s1, char *s2);
int ft_usleep(u_int64_t time);
void	ft_exit(t_data *data);

//threads.c
void create_threads(t_data *data);
int case_one(t_data *data);

//
void message(char *str, t_philo *philo);

//tasks.c
void eat(t_philo *philo);





