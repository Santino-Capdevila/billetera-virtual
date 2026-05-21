#ifndef CONTACTO_H_INCLUDED
#define CONTACTO_H_INCLUDED
#define MAX 10
#define MAX_NOMBRE     50 //Nombre, puede incluir espacios
#define MAX_CBU_ALIAS  23 // 22 dígitos + '\0'
#define TIPO_CAJA_AHORRO_PESOS       1
#define TIPO_CUENTA_CORRIENTE_PESOS  2
#define TIPO_BILLETERA_VIRTUAL       3

typedef struct
{
    char nombre[MAX_NOMBRE];
    char cbu_alias[MAX_CBU_ALIAS];
    int tipo; //1: caja de ahorro en $; 2: cuenta corriente $; 3: billetera virtual
} contacto;

void init(contacto *Contacto)
{
    (*Contacto).nombre[0] = '\0';
    (*Contacto).cbu_alias[0] = '\0';
    (*Contacto).tipo = 0;
}

void set_nombre(contacto *Contacto, char aux_nombre[])
{
    strcpy ((*Contacto).nombre, aux_nombre);
}

void set_cbu_alias(contacto *Contacto, char aux_cbu_alias[])
{
    strcpy ((*Contacto).cbu_alias, aux_cbu_alias);
}

void set_tipo(contacto *Contacto, int aux_tipo)
{
    (*Contacto).tipo = aux_tipo;
}

char* get_nombre(contacto *Contacto)
{
return (*Contacto).nombre;
}

char* get_cbu_alias(contacto Contacto)
{
return Contacto.cbu_alias;
}

int get_tipo(contacto Contacto)
{
    return Contacto.tipo;
}




#endif // CONTACTO_H_INCLUDED
