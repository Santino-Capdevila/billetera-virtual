#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MovimientoVO.h"
#include "ContactoListaE.h"
#include "Contacto.h"
#include "Movimiento.h"
#define CONTACTOS_MAX 100
#define MONTO_MAX 100000

//Variables globales
static float saldo_unipago = 100000;       /* saldo inicial */
static int   next_id = 11;                   /* id autoincremental */
static const char alias_unipago[31] = "UNIPAGO_ALIAS";

//Funcion para borrar la pantalla
void limpiarPantalla(void)
{
    system("cls");
}

//Funcion para mostrar un mensaje al terminar una funcion
void pausar(void)
{
    system("pause");
}


//Funcion auxiliar para controlar el ingreso de fechas
int fecha_valida(int dia, int mes)
{
    int max_dia;
    if (mes < 10 || mes > 12)
    {
        return 0;
    }
    if (dia < 1)
    {
        return 0;
    }
    if (mes == 11)
    {
        max_dia = 30;
    }
    else
    {
        max_dia = 31;
    }
    if (dia > max_dia)
    {
        return 0;
    }
    if (mes == 10 && dia < 10)
    {
        return 0;
    }
    return 1;
}

//Funcion auxiliar para controlar que fecha es mas reciente
int es_mas_reciente(int d1, int m1, int d2, int m2)
{
    if (m1 != m2) return m1 > m2;
    return d1 > d2;
}

//Funcion auxiliar para insertar los movimientos por fecha de mayor a menor
void insertar_ordenado_por_fecha(lista_movimiento *lm, Movimiento nuevo)
{
    lista_movimiento it = *lm;
    reset_lista_movimiento(&it);
    while (!isOos_lista_movimiento(it))
    {
        Movimiento cur = copy_lista_movimiento(it);
        if ( es_mas_reciente(get_fecha_dia(nuevo), get_fecha_mes(nuevo),
                             get_fecha_dia(cur),   get_fecha_mes(cur)) )
        {
            break;
        }
        forward_lista_movimiento(&it);
    }

    reset_lista_movimiento(lm);
    while (!isOos_lista_movimiento(*lm))
    {
        Movimiento cur = copy_lista_movimiento(*lm);
        if ( es_mas_reciente(get_fecha_dia(nuevo), get_fecha_mes(nuevo),
                             get_fecha_dia(cur),   get_fecha_mes(cur)) )
        {
            break;
        }
        forward_lista_movimiento(lm);
    }
    insert_lista_movimiento(lm, nuevo);
}

//Funcion auxiliar para descontar o sumar al saldo dependiendo la operacion
void aplicar_efecto_sobre_saldo(const Movimiento *m)
{
    if (get_tipo_operacion(*m) == 1)               /* débito */
    {
        saldo_unipago = saldo_unipago - get_monto(*m);
    }
    else if (get_tipo_operacion(*m) == 2)          /* crédito */
    {
        saldo_unipago = saldo_unipago + get_monto(*m);
    }
}

//Funcion auxiliar para modificar el saldo en caso de anular un movimiento
void revertir_efecto_sobre_saldo(const Movimiento *m)
{
    if (get_tipo_operacion(*m) == 1)
    {
        saldo_unipago = saldo_unipago + get_monto(*m);
    }
    else if (get_tipo_operacion(*m) == 2)
    {
        saldo_unipago = saldo_unipago - get_monto(*m);
    }
}

//Funcion auxiliar que imprime los datos de un movimiento pasado por parametro
void mostrar_movimiento(Movimiento m)
{
    //Dependiendo de la operacion imprime "-" o "+"
    char signo;
    if (get_tipo_operacion(m) == 1)
    {
        signo = '-';
    }
    else if (get_tipo_operacion(m) == 2)
    {
        signo = '+';
    }
    else
    {
        signo = '?';
    }
    //Imprime los datos del movimiento
    printf("   %c ID:%d | Origen:%s | Destino:%s | Monto:%f | Fecha:%d/%d/2025 | Motivo:%s | Estado:",
           signo,
           get_id_mov(m),
           get_cuenta_Origen(&m),
           get_cuenta_Destino(&m),
           get_monto(m),
           get_fecha_dia(m),
           get_fecha_mes(m),
           get_motivo(&m));
    if (get_estado(m) == 1)
    {
        printf(" Realizado\n\n");
    }
    else
    {
        printf(" Anulado\n\n");
    }
}
//Funcion q)
void listar_movimientos(lista_movimiento lm)
{
    //Controla si hay movimientos
    if (isEmpty_lista_movimiento(lm))
    {
        printf("No hay movimientos registrados.\n");
        return;
    }
    //Imprime los movimientos con la funcion auxiliar
    lista_movimiento it = lm;
    reset_lista_movimiento(&it);
    while(!isOos_lista_movimiento(it))
    {
        Movimiento cur = copy_lista_movimiento(it);
        mostrar_movimiento(cur);
        forward_lista_movimiento(&it);
    }
}

//Funcion e)
void mostrarUltimos10(lista_movimiento lista)
{
    //Controla si hay movimientos
    if (isEmpty_lista_movimiento(lista))
    {
        printf("No hay movimientos registrados.\n");
        return;
    }
    //Imprime los movimientos y controla que imprima 10 con la variable contador
    Movimiento mov;
    int contador = 0;
    reset_lista_movimiento(&lista);
    printf("\n ULTIMOS 10 MOVIMIENTOS \n");
    while (!isOos_lista_movimiento(lista) && contador < 10)
    {
        mov = copy_lista_movimiento(lista);
        mostrar_movimiento(mov);
        forward_lista_movimiento(&lista);
        contador++;
    }
}

//Funcion f)
void mostrarMovimientosHistoricos(lista_movimiento lista)
{
    //Controla si la lista esta vacia
    if (isEmpty_lista_movimiento(lista))
    {
        printf("No hay movimientos registrados.\n");
        return;
    }
    //Iteracion para imprimir todos los movimientos de la lista
    Movimiento mov;
    reset_lista_movimiento(&lista);
    printf("\n -------------------------------HISTORIAL DE MOVIMIENTOS-------------------------------------\n");
    while (!isOos_lista_movimiento(lista))
    {
        mov = copy_lista_movimiento(lista);
        mostrar_movimiento(mov);
        forward_lista_movimiento(&lista);
    }
    printf("-------------------------------------------------------------------------------------------------\n");

}

//Funcion b)
int buscar_id_mov(lista_movimiento *p, int id_mov)
{
    //Itera buscando el movimiento mediante el ID
    Movimiento aux;
    reset_lista_movimiento(p);

    while (!isOos_lista_movimiento(*p))
    {
        aux = copy_lista_movimiento(*p);
        if (get_id_mov(aux) == id_mov)
            return 1;

        forward_lista_movimiento(p);
    }
    return 0;
    //retorna 1 si lo encuentra, 0 si no lo encuentra
}

//Funcion c)
void mostrar_datos_mov_buscado(lista_movimiento *p, int id_mov)
{
    //Si encuentra el movimiento, lo copia en aux y lo imprime campo a campo
    if (buscar_id_mov(p, id_mov))
    {
        Movimiento aux = copy_lista_movimiento(*p);
        printf("   DATOS DEL MOVIMIENTO ID: %d\n", get_id_mov(aux));
        printf("Cuenta de origen: %s\n", get_cuenta_Origen(&aux));
        printf("Cuenta de destino: %s\n", get_cuenta_Destino(&aux));
        switch(get_tipo_operacion(aux))
        {
        case 1:
            printf("Tipo de operacion: Debito\n");
            break;
        case 2:
            printf("Tipo de operacion: Credito\n");
            break;
        }
        switch(get_tipo_movimiento(aux))
        {
        case 1:
            printf("Tipo de movimiento: Transferencia\n");
            break;
        case 2:
            printf("Tipo de movimiento: Pago con QR\n");
            break;
        case 3:
            printf("Tipo de movimiento: Pago de servicio\n");
            break;
        case 4:
            printf("Tipo de movimiento: Retiro de efectivo\n");
            break;
        }
        printf("Monto: %f\n", get_monto(aux));
        printf("Motivo: %s\n", get_motivo(&aux));
        printf("Dia: %d\n", get_fecha_dia(aux));
        printf("Mes: %d\n", get_fecha_mes(aux));
        switch(get_estado(aux))
        {
        case 1:
            printf("Estado del movimiento: realizado\n");
            break;
        case 0:
            printf("Estado del movimiento: anulado\n");
            break;
        }
    }
    else
    {
        printf("No se encontro el id.\n");
    }


}

//Funcion a)
int realizar_movimiento(lista_movimiento *movs, Lista_E *contactos)
{
    //Definicion de variables auxiliares para usar los setter
    Movimiento m;
    int ok, tipo_op, tipo_mov, dia, mes, tipo_cta,respuesta;
    float monto;
    char destino[31], motivo[101],nombre_contacto[80];
    //Settea automaticamente el ID, y actualiza la variable con el ID para que siempre sea distinto
    set_id_mov(&m, next_id);
    next_id++;
    //Ingreso de datos con sus respectivos controles
    do
    {
        printf("Ingrese tipo de operacion (1=debito, 2=credito): ");
        ok = scanf("%d", &tipo_op);
        getchar();
        if (!ok || (tipo_op!=1 && tipo_op!=2))
        {
            printf("Opcion invalida.\n");
            ok=0;
        }
    }
    while(!ok);


    do
    {
        printf("Ingrese tipo de movimiento (1=transferencia, 2=pagoQR, 3=pagoServicio): ");
        ok = scanf("%d", &tipo_mov);
        getchar();

        if (!ok || tipo_mov<1 || tipo_mov>3)
        {
            printf("Opcion invalida.\n");
            ok=0;
        }
    }
    while(!ok);


    do
    {
        printf("Ingrese monto: ");
        ok = scanf("%f", &monto);
        getchar();
        if (!ok)
        {
            printf("Monto invalido.\n");

        }
        else if (monto<=0)
        {
            printf("Monto debe ser > 0.\n");
            ok=0;
        }
        else if (monto > MONTO_MAX)
        {
            printf("Monto demasiado grande.\n");
            ok=0;
        }
        else if (tipo_op==1 && monto>saldo_unipago)
        {
            printf("Saldo insuficiente. Saldo actual: %f\n", saldo_unipago);
            return 0;
        }
    }
    while(!ok);

    set_cuenta_Origen(&m, alias_unipago);


    if (tipo_mov == 2)
    {
        set_cuenta_Destino(&m, "QR");
    }
    else
    {

        do
        {
            printf("Ingrese CBU o Alias: ");
            scanf("%[^\n]", destino);
            getchar();
            if (strchr(destino, ' ') != NULL)
            {
                printf("El CBU o Alias no pueden tener espacios. Ingrese nuevamente.\n");
                ok = 0;
            }
            else
            {
                ok = 1;
            }
            if (strlen(destino)<5)
            {
                printf("El CBU o Alias no puede tener menos de 5 caracteres. Ingrese nuevamente.\n");
                ok = 0;
            }
            else
            {
                ok = 1;
            }

        }
        while(!ok);

        if(tipo_op == 1)
        {
            set_cuenta_Origen(&m,alias_unipago);
            set_cuenta_Destino(&m,destino);
        }
        else
        {
            set_cuenta_Origen(&m,destino);
            set_cuenta_Destino(&m,alias_unipago);
        }

    }
    do
    {
        printf("Desea ingresar un motivo? (1=si/0=no):");
        ok = scanf("%d",&respuesta);
        getchar();
        if(!ok || respuesta!= 1 && respuesta!= 0)
        {
            printf("Valor invalido.\n");
        }
        if(respuesta == 1)
        {
            do
            {
                printf("Ingrese motivo: ");
                scanf("%[^\n]", motivo);
                getchar();
                if(strlen(motivo)>MAX_MOTIVO)
                {
                    printf("El motivo no puede superar los 100 caracteres.\n");
                }
            }
            while(strlen(motivo)>MAX_MOTIVO);
        }
        else
        {
            strcpy(motivo,"-");

        }
    }
    while(respuesta !=1 && respuesta !=0 || !ok);
    set_motivo(&m, motivo);


    do
    {
        printf("Ingrese dia: ");
        ok = scanf("%d", &dia);
        getchar();
        if (!ok)
        {
            printf("Valor invalido.\n");
        }
        printf("Ingrese mes: ");
        ok = scanf("%d", &mes);
        getchar();
        if (!ok)
        {
            printf("Valor invalido.\n");

            continue;
        }
        if (!fecha_valida(dia, mes))
        {
            printf("Fecha fuera de rango.\n");
            ok=0;
        }
    }
    while(!ok);

    //Una vez que lee todo correctamente, settea cada campo
    set_tipo_operacion(&m, tipo_op);
    set_tipo_movimiento(&m, tipo_mov);
    set_monto(&m, monto);
    set_fecha_dia(&m, dia);
    set_fecha_mes(&m, mes);
    set_estado(&m, 1);

    //Pide confirmacion para hacer el movimiento y controla si es correcta la respuesta
    do
    {
        printf("Confirma el movimiento? (1=si / 0=no): ");
        scanf("%d", &respuesta);
        getchar();
    }
    while(respuesta != 0 && respuesta !=1);
    //Si cancela el movimiento, termina la funcion y retorna 0 (falso)
    if (respuesta == 0)
    {
        printf("Movimiento cancelado. Saldo intacto.\n");
        return 0;
    }
    //Si confirma el movimiento, se aplica el cambio en el saldo y se inserta el movimiento en la lista
    aplicar_efecto_sobre_saldo(&m);
    insertar_ordenado_por_fecha(movs, m);
    printf("Movimiento creado (id=%d). Saldo actual: %f\n", get_id_mov(m), saldo_unipago);
    //Control para saber si el alias al que se hace el movimiento ya existe. Si no existe, pregunta si desea agendarlo como contacto
    if(tipo_op == 1)
    {
        if (tipo_mov != 2)
        {
            int ya_existe = 0;
            reset_listaE(contactos);
            while (!isOos_listaE(*contactos))
            {
                contacto c = copy_listaE(*contactos);
                if ( strcmp(get_cbu_alias(c), get_cuenta_Destino(&m)) == 0 )
                {
                    ya_existe=1;
                    break;
                }
                forward_listaE(contactos);
            }
            if (!ya_existe)
            {
                do
                {
                    printf("El destino no esta agendado. Desea agendarlo? (1=si / 0=no): ");
                    scanf(" %d", &respuesta);
                }
                while(respuesta!= 1 && respuesta!=0);
                //Si el usuario responde que desea agregarlo, se hace el ingreso de todos los campos con los controles correspondientes
                if (respuesta == 1)
                {
                    contacto nuevo;
                    init(&nuevo);
                    printf("Ingrese nombre del contacto (puede tener espacios): ");
                    scanf(" %[^\n]", nombre_contacto);
                    set_nombre(&nuevo,nombre_contacto);
                    set_cbu_alias(&nuevo,get_cuenta_Destino(&m));
                    do
                    {
                        printf("Tipo de cuenta (1=caja ahorro, 2=cta corriente, 3=billetera virtual): ");
                        ok = scanf("%d", &tipo_cta);
                        getchar();
                        if (!ok || tipo_cta<1 || tipo_cta>3)
                        {
                            printf("Tipo invalido.\n");
                            ok=0;
                        }
                    }
                    while(!ok);
                    set_tipo(&nuevo, tipo_cta);
                    //Inserta el contacto en la lista
                    insert_listaE(contactos, nuevo);
                    printf("Contacto agregado.\n");
                }
            }
        }
    }
    //Retorna verdadero ya que se efectuo el movimiento
    return 1;
}

//Funcion d)
void ingresar_o_retirar(lista_movimiento *movs)
{
    //Definicion de variables auxiliares para usar en los setter
    Movimiento m;
    int conf,opcion, dia, mes, ok;
    float monto;
    char motivo[101];
    //Settea automaticamente el ID, y actualiza la variable con el ID para que siempre sea distinto
    set_id_mov(&m, next_id);
    next_id++;
    //El usuario elige que operacion desea hacer
    do
    {
        printf("Desea ingresar dinero (1) o retirar efectivo (2)? ");
        ok = scanf("%d", &opcion);
        getchar();
        if (!ok || (opcion!=1 && opcion!=2))
        {
            printf("Opcion invalida.\n");
            ok=0;
        }
    }
    while(!ok);
    //Dependiendo de la operacion elegida, settea el tipo de movimiento y de operacion
    if (opcion==1)
    {
        set_tipo_operacion(&m, 2);
        set_tipo_movimiento(&m, 1);
    }
    else
    {
        set_tipo_operacion(&m, 1);
        set_tipo_movimiento(&m, 4);
    }
    //Ingreso de datos con sus respectivos controles
    do
    {
        printf("Monto: ");
        ok = scanf("%f", &monto);
        getchar();
        if (!ok)
        {
            printf("Monto invalido.\n");
        }
        else if (monto<=0)
        {
            printf("Monto debe ser > 0.\n");
            ok=0;
        }
        else if (monto > MONTO_MAX)
        {
            printf("Monto demasiado grande.\n");
            ok=0;
        }
        else if (get_tipo_operacion(m)==1 && monto>saldo_unipago)
        {
            printf("Saldo insuficiente. Saldo actual: %f\n", saldo_unipago);
            ok=0;
        }
    }
    while(!ok);
    set_monto(&m, monto);

    printf("Motivo: ");
    scanf("%[^\n]", motivo);
    getchar();
    set_motivo(&m, motivo);

    do
    {
        printf("Dia: ");
        ok = scanf("%d", &dia);
        if (!ok)
        {
            printf("Valor invalido.\n");
            fflush(stdin);
            continue;
        }
        printf("Mes: ");
        ok = scanf("%d", &mes);
        if (!ok)
        {
            printf("Valor invalido.\n");
            fflush(stdin);
            continue;
        }
        if (!fecha_valida(dia, mes))
        {
            printf("Fecha fuera de rango.\n");
            ok=0;
        }
    }
    while(!ok);

    set_fecha_dia(&m, dia);
    set_fecha_mes(&m, mes);
    set_estado(&m, 1);
    set_cuenta_Origen(&m, alias_unipago);
    set_cuenta_Destino(&m, alias_unipago);
    //Confirmacion de la operacion
    do
    {
        printf("Confirma la operacion? (1=si / 0=no): ");
        scanf(" %d", &conf);
        getchar();
    }
    while(conf != 1 && conf != 0);
    //Si cancela la funcion, sale de la funcion
    if (conf == 0)
    {
        printf("Operacion cancelada. Saldo intacto.\n");
        return;
    }
    //En caso de que confirme la operacion, aplica el cambio en el saldo e inserta el movimiento en la lista
    aplicar_efecto_sobre_saldo(&m);
    insertar_ordenado_por_fecha(movs, m);
    printf("Operacion realizada. Saldo actual: %f\n", saldo_unipago);
}

//Funcion g)
void modificar_motivo_por_idmov(lista_movimiento *lm, int id_aux)
{
    //Variables auxiliares
    int respuesta, ok;
    //Controla si existe el ID buscado. Si no existe, sale de la funcion
    if (!buscar_id_mov(lm, id_aux))
    {
        printf("No se encontro el id: %d\n", id_aux);
        return;
    }
    //Ingreso de datos con sus respectivos controles
    Movimiento m = copy_lista_movimiento(*lm);
    char motivo[MAX_MOTIVO];
    do
    {
        //Pregunta si desea eliminar el motivo anterior. En caso de que si, settea un "-". En caso de que no, le pide al usuario ingresar el nuevo motivo
        printf("Desea eliminar el motivo anterior? (1=si/0=no):\n");
        ok = scanf("%d",&respuesta);
        getchar();
        if (!ok || respuesta!=1 && respuesta!=0)
        {
            printf("Valor invalido.\n");
        }
        if (respuesta == 0)
        {
            do
            {
                printf("Ingrese el nuevo motivo: ");
                scanf("%[^\n]", motivo);
                getchar();
                if(strlen(motivo)>MAX_MOTIVO)
                {
                    printf("El motivo no puede superar los 100 caracteres.\n");
                }
            }
            while(strlen(motivo)>MAX_MOTIVO);
        }
        if (respuesta == 1)
        {
            strcpy(motivo,"-");
        }
    }
    //Settea el nuevo motivo, suprime de la lista el motivo viejo e inserta el nuevo
    while(respuesta !=1 && respuesta !=0 || !ok);
    set_motivo(&m, motivo);
    supress_lista_movimiento(lm);
    insertar_ordenado_por_fecha(lm, m);
    printf("Motivo actualizado.\n");
}

//Funcion h)
void modificar_motivo_por_contacto(lista_movimiento *lm, Lista_E *le, char aux_nombre[])
{
    //Variables auxiliares
    char aux_alias[MAX_CBU_ALIAS];
    int i,arreglo_id[100],aux_id,cont = 0;
    contacto aux_cont;
    Movimiento movimiento_aux;
    //Busca el contacto del nombre pasado por parametro y copia su alias
    reset_listaE(le);
    while(!isOos_listaE(*le))
    {
        aux_cont = copy_listaE(*le);
        if(strcmp(get_nombre(&aux_cont),aux_nombre) == 0)
        {
            strcpy(aux_alias,get_cbu_alias(aux_cont));
            cont = 1;
        }
        forward_listaE(le);
    }
    //Si la variable contador es falsa, lo notifica y sale de la funcion
    if(!cont)
    {
        printf("No se encontro el contacto.\n");
        return;
    }
    cont = 0;
    i = 0;
    //Con el alias obtenido en la parte anterior, busca los movimientos relacionados a dicho alias y los imprime en pantalla
    reset_lista_movimiento(lm);
    while(!isOos_lista_movimiento(*lm))
    {
        movimiento_aux = copy_lista_movimiento(*lm);
        if(strcmp(get_cuenta_Destino(&movimiento_aux),aux_alias) == 0 || strcmp(get_cuenta_Origen(&movimiento_aux),aux_alias) == 0)
        {
            arreglo_id[i] = get_id_mov(movimiento_aux);
            i += 1;
            printf("ID del movimiento: %d\n", get_id_mov(movimiento_aux));
            printf("Fecha del movimiento: %d/%d/2025\n", get_fecha_dia(movimiento_aux), get_fecha_mes(movimiento_aux));
            printf("Monto del movimiento: %f\n", get_monto(movimiento_aux));
            switch(get_tipo_operacion(movimiento_aux))
            {
            case 1:
                printf("Tipo de operacion: Debito\n");
                break;
            case 2:
                printf("Tipo de operacion: Credito\n");
                break;
            }
            switch(get_tipo_movimiento(movimiento_aux))
            {
            case 1:
                printf("Tipo de movimiento: Transferencia\n");
                break;
            case 2:
                printf("Tipo de movimiento: Pago con QR\n");
                break;
            case 3:
                printf("Tipo de movimiento: Pago de servicio\n");
                break;
            case 4:
                printf("Tipo de movimiento: Retiro de efectivo\n");
                break;
            }
            switch(get_estado(movimiento_aux))
            {
            case 1:
                printf("Estado del movimiento: realizado\n");
                break;
            case 0:
                printf("Estado del movimiento: anulado\n");
                break;
            }
            printf("Motivo del movimiento: %s\n\n",movimiento_aux.motivo);
            cont = 1;
        }
        forward_lista_movimiento(lm);
    }
    //Si la variable cont es falsa, lo notifica y sale de la funcion
    if(!cont)
    {
        printf("No hay movimientos del contacto.\n");
        return;
    }
    int ok = 1;
    int cant_id = i;
    //Ingreso del ID del movimiento que el usuario quiere modificar con sus respectivos controles. Luego ejecuta la funcion anterior
    do
    {
        printf("Ingrese el id del movimiento que quiere modificar: ");
        ok = scanf("%d",&aux_id);
        getchar();
        if(!ok)
        {
            printf("Valor invalido.Ingrese nuevamente.\n");
        }
        else
        {
            ok=0;
            for(i = 0; i<cant_id; i++)
            {
                if(arreglo_id[i] == aux_id) ok = 1;
            }
        }
        if(!ok)
        {
            printf("No esta dentro de las opciones.Ingrese nuevamente.\n");
        }

    }
    while(!ok);
    modificar_motivo_por_idmov(lm,aux_id);
}

//Funcion i)
void anular_movimiento_por_idmov(lista_movimiento *lm, int id_aux)
{

    //Controla si existe el movimiento. En caso de que no se sale de la funcion
    if (!buscar_id_mov(lm, id_aux))
    {
        printf("No se encontro el id: %d\n", id_aux);
        return;
    }
    Movimiento m = copy_lista_movimiento(*lm);
    //Controla si el movimiento ya estaba anulado previamente
    if (get_estado(m) == 0)
    {
        printf("El movimiento ya estaba anulado.\n");
        return;
    }
    //Si no estaba anulado previamente, lo anula y hace el cambio correspondiente en el saldo y en la lista de movimientos
    revertir_efecto_sobre_saldo(&m);
    set_estado(&m, 0);

    supress_lista_movimiento(lm);
    insert_lista_movimiento(lm, m);

    printf("Movimiento %d anulado. Saldo actual: %f\n", id_aux, saldo_unipago);
}

//Funcion j)
void listar_movimientos_anulados(lista_movimiento lm)
{
    printf("Movimientos anulados:\n\n");
    //Variables auxiliares
    int alguno = 0;
    lista_movimiento it = lm;
    //Iteracion para buscar e imprimir los movimientos anulados
    reset_lista_movimiento(&it);
    while(!isOos_lista_movimiento(it))
    {
        Movimiento m = copy_lista_movimiento(it);
        if (get_estado(m)==0)
        {
            mostrar_movimiento(m);
            alguno=1;
        }
        forward_lista_movimiento(&it);
    }
    //En caso de no encotrar ninguno, lo notifica
    if (!alguno) printf("No hay movimientos anulados.\n");
}

//Funcion k)
void eliminar_mov_anulados(lista_movimiento *lm)
{
    //Controla si la lista esta vacia
    if (isEmpty_lista_movimiento(*lm))
    {
        printf("No hay movimientos registrados.\n");
        return;
    }
    //Variables auxiliares
    int opcion, ok=0;
    //Confirmacion del usuario para eliminar los movimientos anulados
    printf("\nEsta seguro de eliminar TODOS los movimientos anulados?\n1: Si\n0: No\n");
    do
    {
        ok = scanf("%d", &opcion);
        getchar();
        if (!ok || (opcion!=0 && opcion!=1))
        {
            printf("Opcion invalida.\n");
            ok=0;
        }
    }
    while(!ok);
    //Si el usuario decide no eliminarlos, se sale de la funcion
    if (!opcion) return;
    //Si decide eliminarlos, abre o crea el archivo e imprime los datos de los movimientos anulados
    FILE *fp = fopen("anulados.txt","a+");
    if (!fp)
    {
        printf("No se pudo abrir anulados.txt\n");
        return;
    }

    lista_movimiento it = *lm;
    reset_lista_movimiento(&it);
    while(!isOos_lista_movimiento(it))
    {
        Movimiento m = copy_lista_movimiento(it);
        if (get_estado(m)==0)
        {
            fprintf(fp, "ID:%d | Fecha:%d/%d/2025 | Monto:%f | Origen:%s | Destino:%s | TipoOp:%d TipoMov:%d | Motivo:%s\n",
                    get_id_mov(m), get_fecha_dia(m), get_fecha_mes(m), get_monto(m),
                    get_cuenta_Origen(&m), get_cuenta_Destino(&m),
                    get_tipo_operacion(m), get_tipo_movimiento(m), get_motivo(&m));
        }
        forward_lista_movimiento(&it);
    }
    fclose(fp);
    //Iteracion para suprimir de la lista los movimientos anulados
    reset_lista_movimiento(lm);
    while(!isOos_lista_movimiento(*lm))
    {
        Movimiento m = copy_lista_movimiento(*lm);
        if (get_estado(m)==0)
        {
            supress_lista_movimiento(lm);
        }
        else
        {
            forward_lista_movimiento(lm);
        }
    }
    printf("Movimientos anulados eliminados y volcados a anulados.txt\n");
}

//Funcion l)
int rec_mayores_350k(lista_movimiento it, int acum)
{
    //Caso base: si la lista esta fuera de estructura retorna el acumulador, el cual es igual a 1 si encontro un movimiento que cumple la condicion y 0 si no
    if (isOos_lista_movimiento(it))
        return acum;
    Movimiento m = copy_lista_movimiento(it);
    //Caso recursivo: Si el monto del movimiento es mayor a 350k, lo imprime, cambia el valor del acumulador y avanza en la lista. En caso de que no,
    if (get_monto(m) > 350000)
    {
        mostrar_movimiento(m);
        acum = 1;
    }
    forward_lista_movimiento(&it);
    rec_mayores_350k(it,acum);
}
//Funcion auxiliar de l) para notificar en caso de no encontrar movimientos mayores a 350k
void mostrar_mayores_350k(lista_movimiento lm)
{
    lista_movimiento it = lm;
    int acum = 0;
    reset_lista_movimiento(&it);
    if (!rec_mayores_350k(it,acum))
    {
        printf("Ningun movimiento supera los $350.000\n");
    }
}

//Funcion m)
int contarMovimientosA(lista_movimiento lista, char *aliasContacto)
{
    if (isEmpty_lista_movimiento(lista) || isOos_lista_movimiento(lista)) return 0;
    Movimiento mov = copy_lista_movimiento(lista);
    lista_movimiento sig = lista;
    forward_lista_movimiento(&sig);
    if (strcmp(get_cuenta_Destino(&mov), aliasContacto)==0)
        return 1 + contarMovimientosA(sig, aliasContacto);
    else
        return contarMovimientosA(sig, aliasContacto);
}

//Funcion n)
void descargarMovimientosPeriodo(lista_movimiento lista, int diaInicio, int mesInicio, int diaFin, int mesFin)
{
    FILE *archivo = fopen("historicos.txt","w");
    if (!archivo)
    {
        printf("Error al crear historicos.txt\n");
        return;
    }
    if (isEmpty_lista_movimiento(lista))
    {
        printf("No hay movimientos registrados.\n");
        fclose(archivo);
        return;
    }
    int contador=0;
    reset_lista_movimiento(&lista);
    while(!isOos_lista_movimiento(lista))
    {
        Movimiento mov = copy_lista_movimiento(lista);
        int dia = get_fecha_dia(mov), mes = get_fecha_mes(mov);
        if ( (mes>mesInicio || (mes==mesInicio && dia>=diaInicio)) &&
                (mes<mesFin    || (mes==mesFin    && dia<=diaFin)) )
        {
            fprintf(archivo, "ID:%d | Origen:%s | Destino:%s | Monto:%f | Fecha:%d/%d | Motivo:%s\n",
                    get_id_mov(mov),
                    get_cuenta_Origen(&mov),
                    get_cuenta_Destino(&mov),
                    get_monto(mov),
                    dia, mes,
                    get_motivo(&mov));
            contador++;
        }
        forward_lista_movimiento(&lista);
    }
    fclose(archivo);
    printf("Se copiaron %d movimientos al archivo historicos.txt\n", contador);
}

//Funcion ñ)
void cargarContacto(Lista_E *lista)
{
    if (isFull_listaE(*lista))
    {
        printf("Lista de contactos llena.\n");
        return;
    }
    contacto nuevo,aux;
    char nombreAux[MAX_NOMBRE];
    char cbuAux[MAX_CBU_ALIAS];
    int tipoAux,ok,repetido;
    init(&nuevo);
    do
    {
        repetido = 1;
        printf("Ingrese nombre: ");
        ok = scanf("%[^\n]", nombreAux);
        getchar();
        if(strlen(nombreAux)>MAX_NOMBRE)
        {
            printf("El nombre no debe superar 30 caracteres.\n");
            ok = 0;
        }
        if(ok)
        {
            reset_listaE(lista);
            while (!isOos_listaE(*lista))
            {
                aux = copy_listaE(*lista);
                if (strcmp(get_nombre(&aux), nombreAux) == 0)
                {
                    printf("El nombre ya existe, ingrese otro.\n");
                    repetido = 0;

                }
                forward_listaE(lista);
            }
        }

    }
    while(!ok || !repetido);

    set_nombre(&nuevo, nombreAux);

    do
    {
        repetido = 1;
        printf("Ingrese CBU o Alias (sin espacios): ");
        ok = scanf("%[^\n]", cbuAux);
        getchar();
        if (strchr(cbuAux, ' ') != NULL)
        {
            printf("El CBU o Alias no pueden tener espacios. Ingrese nuevamente.\n");
            ok = 0;
        }
        else if (strlen(cbuAux)<5)
        {
            printf("El CBU o Alias debe tener mas de 5 caracteres. Ingrese nuevamente.\n");
            ok = 0;
        }
        else if (strlen(cbuAux)>30)
        {
            printf("El CBU o Alias debe tener menos de 30 caracteres. Ingrese nuevamente.\n");
            ok = 0;
        }
        if (ok)
        {
            reset_listaE(lista);
            while (!isOos_listaE(*lista))
            {
                aux = copy_listaE(*lista);
                if (strcmp(get_cbu_alias(aux), cbuAux) == 0)
                {
                    printf("El alias o CBU ya existe, ingrese otro.\n");
                    repetido = 0;
                }
                forward_listaE(lista);
            }
        }
    }
    while(!ok || !repetido);
    set_cbu_alias(&nuevo, cbuAux);
    do
    {
        printf("Tipo de cuenta (1=caja ahorro, 2=cta corriente, 3=billetera virtual): ");
        if (scanf("%d", &tipoAux)!=1 || tipoAux<1 || tipoAux>3)
        {
            printf("Tipo invalido.\n");
            tipoAux=0;
        }
        getchar();
    }
    while(!tipoAux);

    set_tipo(&nuevo, tipoAux);
    insert_listaE(lista, nuevo);
    printf("Contacto agregado.\n");
    return;
}

// Funcion o)
void eliminarContacto(Lista_E *lista)
{
    Lista_E l = *lista;

    if (isEmpty_listaE(l))
    {
        printf("No hay contactos.\n");
        return;
    }
    mostrarContactos(l);

    char buscado[MAX_NOMBRE];
    printf("Nombre o alias a eliminar: ");
    scanf("%[^\n]", buscado);
    getchar();
    reset_listaE(lista);
    while(!isOos_listaE(*lista))
    {
        contacto c = copy_listaE(*lista);
        if ( strcmp(get_nombre(&c), buscado)==0 || strcmp(get_cbu_alias(c), buscado)==0 )
        {
            suprimir_listaE(lista);
            printf("Contacto eliminado.\n");
            return;
        }
        forward_listaE(lista);
    }
    printf("No se encontro el contacto.\n");
}

//Funcion p)
void precargar_contactos(Lista_E *lista)
{
    FILE *fp = fopen("contactos.txt", "r");
    if (fp == NULL)
    {
        printf("No se pudo abrir el archivo contactos.txt\n");
        return;
    }

    contacto c;
    char nombre[40], alias[40];
    int tipo;

    while (fscanf(fp,"%39[^,],%39[^,],%d\n", nombre, alias, &tipo) == 3)
    {

        init_listaE(&c);
        set_nombre(&c, nombre);
        set_cbu_alias(&c, alias);
        set_tipo(&c, tipo);
        insert_listaE(lista, c);
    }

    fclose(fp);
    printf("Contactos precargados correctamente.\n");
}

//Funcion q)
void mostrarContactos(Lista_E lista)
{
    contacto c;
    if (isEmpty_listaE(lista))
    {
        printf("No hay contactos cargados.\n");
        return;
    }
    reset_listaE(&lista);
    printf("\n -----LISTA DE CONTACTOS -----\n\n");
    while(!isOos_listaE(lista))
    {
        c = copy_listaE(lista);
        printf(" Nombre: %s | Alias/CBU: %s | Tipo: ", get_nombre(&c), get_cbu_alias(c));
        int t = get_tipo(c);
        if (t==1) printf("Caja de ahorro $\n");
        else if (t==2) printf("Cuenta corriente $\n");
        else printf("Billetera virtual\n");
        forward_listaE(&lista);
    }
    printf("--------------------------------\n");
}

//Funcion r)
void calcularIngresosYDebitosMes(lista_movimiento lista, int mesBuscado)
{
    if (isEmpty_lista_movimiento(lista))
    {
        printf("No hay movimientos.\n");
        return;
    }
    float totalIng=0, totalDeb=0;
    reset_lista_movimiento(&lista);
    while(!isOos_lista_movimiento(lista))
    {
        Movimiento m = copy_lista_movimiento(lista);
        if (get_fecha_mes(m)==mesBuscado)
        {
            if (get_tipo_operacion(m)==1) totalDeb += get_monto(m);
            else if (get_tipo_operacion(m)==2) totalIng += get_monto(m);
        }
        forward_lista_movimiento(&lista);
    }
    printf("\nRESUMEN DEL MES %d/2025\n", mesBuscado);
    printf("Total ingresado (+): %f\n", totalIng);
    printf("Total debitado (-):  %f\n", totalDeb);

}

//Funcion s)
void precargar_movimientos(lista_movimiento*lista)
{
    FILE *fp = fopen("movimientos.txt", "r");
    if (fp == NULL)
    {
        printf("No se pudo abrir el archivo movimientos.txt\n");
        return;
    }

    Movimiento m;
    char cuenta_origen[31],cuenta_destino[31],motivo[101];
    int id,tipo_op,tipo_mov,dia,mes,estado;
    float monto;
    int max_id = 0;

    while (fscanf(fp, "%d,%30[^,],%30[^,],%d,%d,%f,%100[^,],%d,%d,%d\n",&id,cuenta_origen,cuenta_destino,&tipo_op,&tipo_mov,&monto,motivo,&dia,&mes,&estado) == 10)
    {
        init_lista_movimiento(&m);
        set_id_mov(&m,id);
        set_cuenta_Origen(&m,cuenta_origen);
        set_cuenta_Destino(&m,cuenta_destino);
        set_tipo_operacion(&m,tipo_op);
        set_tipo_movimiento(&m,tipo_mov);
        set_monto(&m,monto);
        set_motivo(&m,motivo);
        set_fecha_dia(&m,dia);
        set_fecha_mes(&m,mes);
        set_estado(&m,estado);


        reset_lista_movimiento(lista);
        while (!isOos_lista_movimiento(*lista))
            forward_lista_movimiento(lista);

        insert_lista_movimiento(lista, m);



    }

    fclose(fp);
    printf("Movimientos precargados correctamente.\n");
}

int main(void)
{
    lista_movimiento movimientos;
    Lista_E contactosE;
    int opcion;
    char c;

    init_lista_movimiento(&movimientos);
    init_listaE(&contactosE);


    precargar_contactos(&contactosE);
    precargar_movimientos(&movimientos);

    do
    {
        limpiarPantalla();
        printf("\n=== Menu Principal ===\n");
        printf("Saldo actual: %f\n", saldo_unipago);
        printf("1) Mostrar contactos\n");
        printf("2) Cargar contacto\n");
        printf("3) Eliminar contacto\n");
        printf("4) Menu Movimientos\n");
        printf("0) Salir\n");
        printf("Opcion: ");
        if (scanf(" %d", &opcion)!=1)
        {
            opcion=-1;
            fflush(stdin);
        }
        getchar();

        switch(opcion)
        {
        case 1:
            limpiarPantalla();
            mostrarContactos(contactosE);
            pausar();
            break;

        case 2:
            limpiarPantalla();
            cargarContacto(&contactosE);
            pausar();
            break;

        case 3:
            limpiarPantalla();
            eliminarContacto(&contactosE);
            pausar();
            break;

        case 4:
        {
            int opm;
            do
            {
                limpiarPantalla();
                printf("\n=== Menu Movimientos ===\n");
                printf("Saldo actual: %f\n", saldo_unipago);
                printf("1) Realizar movimiento\n");
                printf("2) Listar movimientos\n");
                printf("3) Mostrar ultimos 10\n");
                printf("4) Buscar y mostrar por id\n");
                printf("5) Ingresar/Retirar efectivo\n");
                printf("6) Modificar motivo por id\n");
                printf("7) Anular por id\n");
                printf("8) Listar anulados \n");
                printf("9) Eliminar anulados \n");
                printf("10) Mostrar > 350k \n");
                printf("11) Contar envios a un alias \n");
                printf("12) Descargar periodo a historicos.txt \n");
                printf("13) Resumen mes 10-12 \n");
                printf("14) Modificar motivo por contacto \n");
                printf("0) Volver\n");
                printf("Opcion: ");
                if (scanf("%d", &opm)!=1)
                {
                    opm=-1;
                    fflush(stdin);
                }
                getchar();

                switch(opm)
                {
                case 1:
                {
                    limpiarPantalla();
                    int ok = realizar_movimiento(&movimientos, &contactosE);
                    if (ok)
                    {
                        printf("Movimiento insertado.\n");
                    }
                    else    printf("No se pudo crear el movimiento.\n");
                    pausar();
                }
                break;

                case 2:
                    limpiarPantalla();
                    mostrarMovimientosHistoricos(movimientos);
                    pausar();
                    break;

                case 3:
                    limpiarPantalla();
                    mostrarUltimos10(movimientos);
                    pausar();
                    break;

                case 4:
                {
                    limpiarPantalla();
                    int id, ok;
                    do
                    {
                        printf("Ingrese id_mov a buscar: ");
                        ok = scanf("%d", &id);
                        getchar();
                        if (!ok)
                        {
                            printf("Valor invalido.\n");
                            fflush(stdin);
                        }
                    }
                    while(!ok);
                    mostrar_datos_mov_buscado(&movimientos, id);
                    pausar();
                }
                break;

                case 5:
                    limpiarPantalla();
                    ingresar_o_retirar(&movimientos);
                    pausar();
                    break;

                case 6:
                {
                    limpiarPantalla();
                    int id, ok;
                    do
                    {
                        printf("Ingrese id_mov a modificar motivo: ");
                        ok = scanf("%d", &id);
                        getchar();
                        if (!ok)
                        {
                            printf("Valor invalido.\n");
                            fflush(stdin);
                        }
                    }
                    while(!ok);
                    modificar_motivo_por_idmov(&movimientos, id);
                    pausar();
                }
                break;

                case 7:
                {
                    limpiarPantalla();
                    int id, ok;
                    listar_movimientos(movimientos);
                    do
                    {
                        printf("Ingrese id_mov a anular: ");
                        ok = scanf("%d", &id);
                        getchar();
                        if (!ok)
                        {
                            printf("Valor invalido.\n");
                            fflush(stdin);
                        }
                    }
                    while(!ok);
                    anular_movimiento_por_idmov(&movimientos, id);
                    pausar();
                }
                break;

                case 8:
                    limpiarPantalla();
                    listar_movimientos_anulados(movimientos);
                    pausar();
                    break;

                case 9:
                    limpiarPantalla();
                    eliminar_mov_anulados(&movimientos);
                    pausar();
                    break;

                case 10:
                    limpiarPantalla();
                    mostrar_mayores_350k(movimientos);
                    pausar();
                    break;

                case 11:
                {
                    limpiarPantalla();
                    int ok;
                    char aliasBuscado[31];
                    do
                    {
                        printf("Alias/CBU destino a contar: ");
                        ok = scanf("%[^\n]", aliasBuscado);
                        getchar();
                        if (strchr(aliasBuscado, ' ') != NULL)
                        {
                            printf("El CBU o Alias no pueden tener espacios. Ingrese nuevamente.\n");
                            ok = 0;
                        }
                        else if (strlen(aliasBuscado)<5)
                        {
                            printf("El CBU o Alias no puede tener menos de 5 caracteres. Ingrese nuevamente.\n");
                            ok = 0;
                        }
                        else if(strlen(aliasBuscado)>30)
                        {
                            printf("El CBU o Alias no puede tener mas de 30 caracteres. Ingrese nuevamente.\n");
                            ok = 0;
                        }
                        else
                        {
                            ok = 1;
                        }

                    }
                    while(!ok);
                    lista_movimiento it = movimientos;
                    reset_lista_movimiento(&it);
                    int total = contarMovimientosA(it, aliasBuscado);
                    printf("Cantidad de movimientos a %s: %d\n", aliasBuscado, total);
                    pausar();
                }
                break;

                case 12:
                {
                    limpiarPantalla();
                    int d1,m1,d2,m2, ok;
                    do
                    {
                        printf("Dia/Mes inicio: ");
                        ok = (scanf("%d %d",&d1,&m1)==2);
                        getchar();
                        if (!ok)
                        {
                            printf("Invalido.\n");
                            fflush(stdin);
                            continue;
                        }
                        printf("Dia/Mes fin: ");
                        ok = (scanf("%d %d",&d2,&m2)==2);
                        getchar();
                        if (!ok)
                        {
                            printf("Invalido.\n");
                            fflush(stdin);
                            continue;
                        }
                        if (!fecha_valida(d1,m1) || !fecha_valida(d2,m2))
                        {
                            printf("Fuera de la ventana permitida.\n");
                            ok = 0;
                        }
                    }
                    while(!ok);
                    descargarMovimientosPeriodo(movimientos, d1,m1,d2,m2);
                    pausar();
                }
                break;

                case 13:
                {
                    limpiarPantalla();
                    int mes, ok;
                    do
                    {
                        printf("Mes (10-12): ");
                        ok = scanf("%d",&mes);
                        getchar();
                        if(!ok)
                        {
                            printf("Valor Invalido. Ingrese un numero entero del 10 al 12\n");
                        }
                        if (mes<10 || mes>12)
                        {
                            printf("Mes invalido (solo 10,11 o 12).\n");
                            ok = 0;
                        }
                    }
                    while(!ok);
                    calcularIngresosYDebitosMes(movimientos, mes);
                    pausar();
                }
                break;

                case 14:
                {
                    limpiarPantalla();
                    char nombrebus[MAX_NOMBRE];
                    int ok;

                    do
                    {
                        printf("Ingrese el nombre del contacto que desea buscar:");
                        ok = scanf("%[^\n]",nombrebus);
                        getchar();
                        if(!ok)
                        {
                            printf("\nInvalido, intente nuevamente.\n");
                        }
                    }
                    while(!ok);
                    modificar_motivo_por_contacto(&movimientos,&contactosE,nombrebus);
                    pausar();

                }
                break;

                case 0:

                    break;

                default:
                    printf("Opcion no valida.\n");
                    pausar();
                    break;
                }

            }
            while(opm!=0);
        }
        break;

        case 0:
            limpiarPantalla();
            printf("Saliendo...\n");
            break;

        default:
            printf("Opcion no valida.\n");
            pausar();
            break;
        }

    }
    while(opcion!=0);

    return 0;
}
