#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED
#define MAX_CVU_ALIAS 31
#define MAX_MOTIVO 101

typedef struct
{
    int dia;
    int mes;

} Fecha;

typedef struct
{
    int id_mov; // identificador, generado automáticamente, no se repite
    char cuenta_Origen[MAX_CVU_ALIAS]; //alias o CBU de quien realiza el movimiento
    char cuenta_Destino[MAX_CVU_ALIAS]; //alias o CBU del destinatario
    int tipo_operacion; // "transferencia", "pagoQR", "pagoServicio", "retiroEfectivo"
    int tipo_movimiento; // "transferencia", "pagoQR", "pagoServicio", "retiroEfectivo"
    float monto;
    char motivo[MAX_MOTIVO];
    Fecha fecha; //alias o CBU de quien realiza el movimiento
    int estado; //dos valores posibles ok, anulado (*)
} Movimiento;

void set_id_mov(Movimiento *p,int id_mov)
{
    (*p).id_mov = id_mov;
}

void set_cuenta_Origen(Movimiento *p,char cuenta_Origen[])
{
    strcpy ((*p).cuenta_Origen, cuenta_Origen);
}

void set_cuenta_Destino(Movimiento *p,char cuenta_Destino[])
{
    strcpy ((*p).cuenta_Destino, cuenta_Destino);
}

void set_tipo_operacion(Movimiento *p,int tipo_operacion)
{
    (*p).tipo_operacion = tipo_operacion;
}

void set_tipo_movimiento(Movimiento *p,int tipo_movimiento)
{
    (*p).tipo_movimiento = tipo_movimiento;
}

void set_monto(Movimiento *p,float monto)
{
    (*p).monto = monto;
}

void set_motivo(Movimiento *p,char motivo[])
{
    strcpy ((*p).motivo, motivo);
}

void set_fecha_dia(Movimiento *p,int dia)
{
    (*p).fecha.dia = dia;
}
void set_fecha_mes(Movimiento *p,int mes)
{
    (*p).fecha.mes = mes;
}
void set_estado(Movimiento *p,int estado)
{
    (*p).estado = estado;
}

int get_id_mov(Movimiento p)
{
    return p.id_mov;
}

char* get_cuenta_Origen(Movimiento *p)
{
    return p->cuenta_Origen;
}

char* get_cuenta_Destino(Movimiento *p)
{
    return p->cuenta_Destino;
}

int get_tipo_operacion(Movimiento p)
{
    return p.tipo_operacion;
}

int get_tipo_movimiento(Movimiento p)
{
    return p.tipo_movimiento;
}

float get_monto(Movimiento p)
{
    return p.monto;
}

char* get_motivo(Movimiento *p)
{

    return p->motivo;
}

int get_fecha_dia(Movimiento p)
{
    return p.fecha.dia;
}
int get_fecha_mes(Movimiento p)
{
    return p.fecha.mes;
}
int get_estado(Movimiento p)
{
    return p.estado;
}




#endif // MOVIMIENTO_H_INCLUDED
