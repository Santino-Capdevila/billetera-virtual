#ifndef CONTACTOLISTAE_H_INCLUDED
#define CONTACTOLISTAE_H_INCLUDED
#define LISTA_CONTACTOS_MAX 100
#include "Contacto.h"
typedef struct
{
    contacto lista_cont[LISTA_CONTACTOS_MAX];
    int cur;
    int ultimo;
} Lista_E;

void init_listaE(Lista_E *p)
{
    (*p).cur = -1;
    (*p).ultimo = -1;
}

void forward_listaE(Lista_E *p)
{
    (*p).cur = (*p).cur + 1;
}

void insert_listaE(Lista_E *p, contacto nuevo_contacto)
{
    int i;

    if ((*p).ultimo == -1)
    {
        (*p).cur = 0;
        (*p).ultimo = 0;
        (*p).lista_cont[0] = nuevo_contacto;
        return;
    }
    for (i = (*p).ultimo; i >= (*p).cur; i--)
    {
        (*p).lista_cont[i + 1] = (*p).lista_cont[i];
    }
    (*p).lista_cont[(*p).cur] = nuevo_contacto;
    (*p).ultimo++;
}

int isOos_listaE(Lista_E p)
{
    return p.cur == p.ultimo + 1;
}

contacto copy_listaE(Lista_E p)
{
    return p.lista_cont[p.cur];
}

int isFull_listaE(Lista_E p)
{
    return p.ultimo == LISTA_CONTACTOS_MAX-1;
}

void reset_listaE(Lista_E *p)
{
    (*p).cur = 0;
}

int isEmpty_listaE(Lista_E p)
{
    return p.ultimo == -1;
}

void suprimir_listaE(Lista_E *p)
{
    int i;

    for (i = (*p).cur; i < (*p).ultimo; i++)
    {
        (*p).lista_cont[i] = (*p).lista_cont[i + 1];
    }
    (*p).ultimo--;

    if ((*p).cur > (*p).ultimo && (*p).ultimo >= 0)
    {
        (*p).cur = (*p).ultimo;
    }
    if ((*p).ultimo == -1)
    {
        (*p).cur = -1;
    }
}


#endif // CONTACTOLISTAE_H_INCLUDED
