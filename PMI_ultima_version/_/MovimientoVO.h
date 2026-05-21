#ifndef MOVIMIENTOVO_H_INCLUDED
#define MOVIMIENTOVO_H_INCLUDED
#include "Movimiento.h"
#include "Movimiento.h"
typedef struct nodo
{
    Movimiento vipd;
    struct nodo *siguiente;
} Nodo;

typedef struct
{
    Nodo *acc;
    Nodo *cur;
    Nodo *aux;
} lista_movimiento;

void init_lista_movimiento(lista_movimiento *lst)
{
    (*lst).acc = NULL;
    (*lst).cur = NULL;
    (*lst).aux = NULL;
}

int isEmpty_lista_movimiento(lista_movimiento lst)
{
    return (lst.acc == NULL);
}

int isFull_lista_movimiento(lista_movimiento lst)
{
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    free(n);
    return 0;
}

void reset_lista_movimiento(lista_movimiento *lst)
{
    (*lst).aux = (*lst).acc;
    (*lst).cur = (*lst).acc;
}

void forward_lista_movimiento(lista_movimiento *lst)
{
    (*lst).aux = (*lst).cur;
    (*lst).cur = (*(*lst).cur).siguiente;

}

int isOos_lista_movimiento(lista_movimiento lst)
{
    return (lst.cur == NULL);
}

Movimiento copy_lista_movimiento(lista_movimiento lst)
{
    return (*lst.cur).vipd;
}

void insert_lista_movimiento(lista_movimiento *lst, Movimiento nuevomov)
{
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL)
    {
        return;
    }
    (*nuevo).vipd = nuevomov;
    (*nuevo).siguiente = NULL;

    if ((*lst).cur == (*lst).acc)
    {
        (*nuevo).siguiente = (*lst).acc;
        (*lst).acc = nuevo;
        (*lst).aux = nuevo;
        (*lst).cur = nuevo;
        return;
    }
    else
    {
        (*nuevo).siguiente = (*lst).cur;
        (*(*lst).aux).siguiente = nuevo;
        (*lst).cur = nuevo;
    }
}

void supress_lista_movimiento(lista_movimiento *lst)
{
    if((*lst).acc == (*lst).cur)
    {
        (*lst).acc = (*(*lst).aux).siguiente;
        (*lst).aux = (*(*lst).aux).siguiente;
        free((*lst).cur);
        (*lst).cur = (*lst).aux;
    }
    else
    {
        (*(*lst).aux).siguiente = (*(*lst).cur).siguiente;
        free((*lst).cur);
        (*lst).cur = (*(*lst).aux).siguiente;
    }
}


#endif // MOVIMIENTOVO_H_INCLUDED
