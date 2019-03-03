#include <stdio.h>
#include <pthread.h>
#include "list_mt.h"
#include "include/list.h"

struct list_mt{
        struct list* wrapped;
        int poolsize;
        int finished;
        pthread_mutex_t mutex;
        pthread_cond_t add;
        pthread_cond_t remove;
};

int createList_mt(struct list_mt **lst,
                void (*freeObject)(void *),
                int poolsize)
{
        struct list_mt *list = malloc(sizeof(struct list_mt));
        list->wrapped = createList(freeObject);
        list->poolsize=poolsize;
        list->finished=0;
        pthread_mutex_init(&list->mutex,NULL);
        pthread_cond_init(&list->add,NULL);
        pthread_cond_init(&list->remove,NULL);
        *lst=list;
        return 0;
}

void freeList_mt(struct list_mt *list)
{
        pthread_mutex_lock(&list->mutex);
        freeList(list->wrapped);
        pthread_mutex_unlock(&list->mutex);
        free(list);
}

int getSize_mt(const struct list_mt *list)
{
        return getSize(list->wrapped);
}

int isEmpty_mt(const struct list_mt *list)
{
        return isEmpty(list->wrapped);
}

void addAtFront_mt(struct list_mt *list, void *data)
{
        pthread_mutex_lock(&list->mutex);
        while(list->poolsize < getSize(list->wrapped) && !list->finished){
                pthread_cond_wait(&(list->add), &list->mutex);
        }
        struct node *temp = createNode(data);
        addAtFront(list->wrapped,temp);
        pthread_cond_broadcast(&list->remove);
        pthread_mutex_unlock(&list->mutex);
}

void addAtRear_mt(struct list_mt *list, void *data)
{
        pthread_mutex_lock(&list->mutex);
        while(list->poolsize < getSize(list->wrapped) && !list->finished){
                pthread_cond_wait(&(list->add), &list->mutex);
        }
        struct node *temp = createNode(data);
        addAtRear(list->wrapped,temp);
        pthread_cond_broadcast(&list->remove);
        pthread_mutex_unlock(&list->mutex);

}

void *removeFront_mt(struct list_mt *list)
{
        void *ret;
        pthread_mutex_lock(&list->mutex);
        while(isEmpty(list->wrapped) && !list->finished){
                pthread_cond_wait(&(list->remove), &list->mutex);
        }
        struct node *temp = removeFront(list->wrapped);
        if(temp==NULL){
                pthread_mutex_unlock(&list->mutex);
                return NULL;
        }
        ret = (void *)(temp->data);
        freeNode(temp);
        pthread_cond_broadcast(&list->add);
        pthread_mutex_unlock(&list->mutex);
        return ret;
}


void *removeRear_mt(struct list_mt *list)
{
        void *ret;
        pthread_mutex_lock(&list->mutex);
        while(isEmpty(list->wrapped) && !list->finished){
                pthread_cond_wait(&(list->remove), &list->mutex);
        }
        struct node *temp = removeRear(list->wrapped);
        if(temp==NULL){
                pthread_mutex_unlock(&list->mutex);
                return NULL;
        }
        ret = (void *)(temp->data);
        freeNode(temp);
        pthread_cond_broadcast(&list->add);
        pthread_mutex_unlock(&list->mutex);
        return ret;
}

void finishUp_mt(struct list_mt *list)
{
        pthread_mutex_lock(&list->mutex);
        list->finished = 1;
        pthread_cond_broadcast(&list->remove);
        pthread_mutex_unlock(&list->mutex);
}
