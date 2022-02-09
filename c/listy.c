/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ics.h"
#include "emalloc.h"
#include "listy.h"

node_t *new_node(event_t *val)
{
    assert(val != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    temp->val = val;
    temp->next = NULL;

    return temp;
}

node_t *add_front(node_t *list, node_t *new)
{
    new->next = list;
    return new;
}

node_t *add_end(node_t *list, node_t *new)
{
    node_t *curr;

    if (list == NULL)
    {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next)
        ;
    curr->next = new;
    new->next = NULL;
    return list;
}


node_t *add_inorder(node_t *list, node_t *new)
{
    node_t *prev = NULL;
    node_t *curr = NULL;
    //If list is empty. set new node as the head of the list. 
    if (list == NULL)
    {
        return new;
    }
    //Iterate through list to find correct position for new node. 
    for (curr = list; curr != NULL; curr = curr->next)
    {
        //Add new node into correct position based on start date when date is unique.
        if (strcmp(new->val->dtstart, curr->val->dtstart) > 0)
        {
            prev = curr;
        }
        //Add new node into correct position based on start time when start date is duplicate.
        else if (strcmp(new->val->dtstart, curr->val->dtstart) == 0)
        {
            if (strcmp(new->val->start_time, curr->val->start_time) > 0)
            {
                prev = curr;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
    //Update pointers based on insertion position. 
    new->next = curr;

    if (prev == NULL)
    {
        return (new);
    }
    else
    {
        prev->next = new;
        return list;
    }
}

node_t *peek_front(node_t *list)
{
    return list;
}

node_t *remove_front(node_t *list)
{
    if (list == NULL)
    {
        return NULL;
    }

    return list->next;
}

void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for (; list != NULL; list = list->next)
    {
        (*fn)(list, arg);
    }
}

void inccounter(node_t *p, void *arg)
{
    int *ip = (int *)arg;
    (*ip)++;
}

void print_node(node_t *p, void *arg)
{
    char *fmt = (char *)arg;
    printf(fmt, p->val->dtstart, p->val->start_time, p->val->dtend, p->val->end_time, p->val->summary, p->val->location, p->val->frequency, p->val->repeat_until, p->next);
}

void analysis(node_t *l)
{
    int len = 0;

    apply(l, inccounter, &len);
    printf("Number of nodes: %d\n", len);

    apply(l, print_node, "dtstart: %s\nstart time: %s\ndtend: %s\nend_time: %s\nsummary: %s\nlocation: %s\nfrequency: %s\nrepeat until: %s\nnext: %s\n");
}
