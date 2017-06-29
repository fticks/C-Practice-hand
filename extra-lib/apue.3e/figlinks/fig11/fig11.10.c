#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "apue.h"

struct foo {
	int             f_count;
	pthread_mutex_t f_lock;
	int             f_id;
	/* ... more stuff here ... */
};

struct foo *
foo_alloc(int id) /* allocate the object */
{
	struct foo *fp;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return(NULL);
		}
		/* ... continue initialization ... */
	}
	return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count == 0) { /* last reference */
     //in fact that it cant check the f_count == 1 or not, that is OK
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}

void *
thr_fn1(void *arg)
{
    printf("thread 1: ID is %lu\n", (unsigned long)pthread_self());
    
    struct foo * foo = arg;

    for(int i=0; i< 3; i++){
        usleep(500);
        foo_hold(foo);
        printf("thr_fn1, foo->f_count number: %d\n", foo->f_count);
        foo_rele(foo);
        printf("thr_fn1, foo->f_count number: %d\n", foo->f_count);
    }    
    //while(1);//add while want to check the pthread by pstree on bash 
    
    pthread_exit((void *)&foo);
}

void *
thr_fn2(void *arg)
{
    printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());

    struct foo * foo = arg;

    for(int i=0; i< 3; i++){
        usleep(300);
        foo_hold(foo);
        printf("thr_fn2, foo->f_count number: %d\n", foo->f_count);
        foo_rele(foo);
        printf("thr_fn2, foo->f_count number: %d\n", foo->f_count);
    }
    //while(1);
    
    pthread_exit((void *)0);
}


int
main(void)
{
    int			err;
    pthread_t	tid1, tid2;
    void		*tret;

    struct foo * foo = foo_alloc(1);
    if (NULL == foo)
    	err_exit((int)foo, "foo_alloc err ");

    err = pthread_create(&tid1, NULL, thr_fn1, foo);
    if (err != 0)
    	err_exit(err, "can't create thread 1");

    err = pthread_create(&tid2, NULL, thr_fn2, foo);
    if (err != 0)
    	err_exit(err, "can't create thread 2");

    sleep(1);
    /* wait for two thread run, and you can think if not used pthread_mutex_lock */
    
    err = pthread_join(tid1, (void *)&tret);
    if (err != 0)
    	err_exit(err, "can't join with thread 1");
    err = pthread_join(tid2, (void *)&tret);
    if (err != 0)
    	err_exit(err, "can't join with thread 2");

    foo_rele(foo);
    printf("parent, foo->f_count number: %d\n", foo->f_count);

    exit(0);
}
