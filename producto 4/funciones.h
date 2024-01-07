#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> //librería para usar la estrutura time para la fecha y la hora


#define BUFFER_SIZE 1024

void fechaYHoraActual() {
    /*Definimos un puntero tipo FILE (fp) con fopen que abrirá el archivo "fecha.txt" en modo escritura "w". Si no existe lo crea */
    FILE* fp = fopen("archivoXml.txt", "w");

    if (fp == NULL) {  //Si el archivo no existe (es nulo) se lanza un mensaje indicando el error
        printf("Error al crear el archivo XML");
        return;
    }

    time_t t; /*declaramos una variable t de tipo time_t que es un tipo de dato que nos dará la fecha y hora a partir del epoch
    Se utiliza para almacenar la fecha y la hora actuales*/
    struct tm* tm;/*Declaramos un puntero a una estructura de tiempo tm. Esta estructura permite descomponer time_t en
    sus componente de tiempo (horas, minutos y segundos) y de fecha (año, mes, día)*/

    t = time(NULL); // con time(NULL) obtenemos la marca de hora y fecha actual y la almacena en la variable t
    tm = localtime(&t);/*Utilizamos la función localtime para descomponer la marca de tiempo t en una estructura tm que contiene
    los componentes individuales de fecha y hora local*/

    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<Informacion>\n");

    char buffer[BUFFER_SIZE];/*Creamos una variable donde se guardará lo que le vamos a pasar al archivo.
    Indicando un tamaño de buffer con la constante BUFFER_SIZE que se define e iniciliza previamente como constante*/
    strftime(buffer, BUFFER_SIZE, "    <timestamp>%d/%m/%Y %H:%M</timestamp>\n", tm);/*La funcion strftime formatea la estrcutura tm en una cadena de caracteres
    en el formato día mes y año para sacar la fecha. Utilizamos el buffer donde se guarda la fecha, indicamos un tamaño de buffer con
    BUFFER_SIZE, el formato especificado y la estructura que utilizamos (tm)*/
    fprintf(fp, "%s", buffer);

    printf("%s", buffer);

    fclose(fp);//cerramos el archivo de texto
}

void infoAdaptador() {
    system("ipconfig /all > temp.txt");//asignamos la salida del comando ipconfig /all al archivo temp.txt
    FILE* fp = fopen("temp.txt", "r"); //abrimos el archivo temp.txt en modo lectura

    if (fp == NULL) { //si el archivo no se encuentra lanzamos el error de lectura por consola
        printf("Error al leer el archivo temporal");
        return;
    }

    char adaptador[100]; //variable para guardar el adaptador
    char buffer[BUFFER_SIZE]; //variable para guardar lo que se almacene en el buffer

    int imprimir = 0; //variable para comprobar si se encuentra el adaptador o no

    FILE* xml_fp = fopen("archivoXml.txt", "a"); //abrimos el archivo xml en modo escritura después de lo que hay

    if (xml_fp == NULL) { //si no se puede encontrar el archivo generamos un error por consola
        printf("Error al escribir en el archivo XML");
        fclose(fp);
        return;
    }

    // Mostrar por consola los nombres de adaptadores usando un bucle while con la función fgets() 
    //usando strstr para buscar la palabra Adaptador en la líneas del archivo
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, "Adaptador") != NULL) {
            printf("%s", buffer);
        }
    }

    printf("\nIndica el nombre del adaptador: ");
    if (fgets(adaptador, sizeof(adaptador), stdin) == NULL) {//lanzamos un error si no se puede leer la entrada del nombre del archivo
        printf("Error al leer el nombre del adaptador");
        fclose(fp); 
        fclose(xml_fp);
        return;
    }

    adaptador[strcspn(adaptador, "\n")] = '\0'; //quitamos el espacio final de la cadena del adaptador

    rewind(fp); //volvemos al inicio del archivo

    while (fgets(buffer, sizeof(buffer), fp)) { //bucle while donde buscaremos el nombre del adaptador y desde ahí empezaremos a leer
        if (strstr(buffer, adaptador) != NULL) { 
            //usamos fprintf para escribir en al archivo el adaptador con le formato xml
            fprintf(xml_fp, "    <AdaptadorSeleccionado>%s</AdaptadorSeleccionado>\n", adaptador);
        }
        if (strstr(buffer, adaptador) != NULL) { //si se encuentra damos la opcion de que se puede imprimir en el archivo
            imprimir = 1;
        }

        if (imprimir) { //una vez se puede imprimir procedemos a buscar las opciones de ip, mascara de subred y puerta de enlace
            //usamos strstr para comparar lo que hay en cada línea del archivo con lo que necesitamos buscar
            //si se encuentra lo escribimos en el archivo con formato xml
            if (strstr(buffer, "IPv4") != NULL || strstr(buffer, "subred") != NULL || strstr(buffer, "Puerta de enlace predeterminada") != NULL) {
                printf("%s", buffer);
                fprintf(xml_fp, "    <Info>%s</Info>\n", buffer);
            }
        }
    }

    fprintf(xml_fp, "</Informacion>\n"); //cerramo la etiquta del archivo xml

    fclose(fp); //cerramos el archivo de lectura
    fclose(xml_fp); //cerramos el archivo xml de escritura
}




	



    