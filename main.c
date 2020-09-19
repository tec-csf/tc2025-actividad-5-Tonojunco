/*
 *  Actividad 5
 *  Programación avanzada
 *
 *  Antonio Junco de Haas
 *  A01339695
 *
 *  Este ejercicio se trabajó con Sergio Hernández, Luis Daniel Roa, Christian Alberto Dalma, Luis Revilla
 */
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void manual();

int main(int argc, char *const *argv)
{

    char *numberVal = NULL;
    int dat, updateValue = 0, man = 0, cont = 0, childCount = 0, contExit = 1, darth;
    pid_t pid;
    char witness = 'T';
    char wBoys;
    int pidpadre=getpid();




    while ((dat = getopt(argc, argv, "n:m")) != -1)
    {
        switch (dat)
        {
        case 'n':
            numberVal = optarg;

            if (isdigit(*numberVal) > 0)
            {
                updateValue = atoi(numberVal);

                if (updateValue > 0)
                {
                    printf("Se van a crear %d procesos\n\n", updateValue);
                }
                else if (updateValue == 0)
                {
                    printf("Inserte un valor mayor a %d\n", updateValue);
                    exit(0);
                }
            }
            else
            {
                printf("Inserte un valor numerico valido.\n");
            }

            break;

        case 'm':

            manual();

            break;

        case '?':
            if (optopt == 'n')
            {
                fprintf(stderr, "Inserte un argumento a la opción -%c\n", optopt);
            }
            else if (isprint(optopt))
            {
                fprintf(stderr, "La opción -%c no es válida.\n", optopt);
            }
            else
            {
                fprintf(stderr, "Opción desconocida '\\x%x'.", optopt);
            }

            break;

        default:
            abort();
        }
    }
    /* -- Considering covering the father process*/
    updateValue--;
    int *pipeline = (int *)malloc(sizeof(int[2]) * (updateValue));

    for (; cont < updateValue; ++cont)
    {
        pipe((pipeline + (2 * cont)));
    }

    childCount = 1;
    /* Exception when only one process is made*/
    if (updateValue == 0)
    {
        close(*(pipeline));
        write(*(pipeline + 1), &witness, sizeof(char));

        printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo %c.\n", getpid(), witness);

        darth = getpid();
        //printf("%d\n", darth[0]);
        wait(NULL);
        close(*(pipeline + (2 * updateValue - 1)));
        read(*(pipeline + (2 * updateValue-2)), &wBoys, sizeof(char));

        printf("--> Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos.\n", getpid(), wBoys);
        sleep(5);
    }


    /*For all other cases n>1*/
    for (; childCount <= updateValue; ++childCount)
    {
            pid = fork();

            if (pid == -1)
            {
              /*Error*/
                printf("Hubo un error al crear el proceso hijo %d.\n", childCount);
                break;
            }
            else if (pid == 0)
            {
              /*Children processes*/
                close(*(pipeline + (2 * childCount - 1)));
                read(*(pipeline + (2 * childCount - 2)), &wBoys, sizeof(char));

                printf("—-> Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos.\n", getpid(), wBoys);

                sleep(5);
                /*Last process*/
                if (childCount == updateValue)
                {
                    close(*(pipeline + (2 * childCount-1)));
                    write(*(pipeline + (2*updateValue-2)), &wBoys, sizeof(char));

                    printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo %c.\n", getpid(), wBoys);
                }
                /*All other process*/
                else
                {
                    close(*(pipeline + (2 * childCount)));
                    write(*(pipeline + (2 * childCount)+1), &wBoys, sizeof(char));

                    printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo %c, el cual tendré por 5 segundos.\n", getpid(), wBoys);
                }
            }
            else
            {

                if (childCount == 1)
                {
                  /*Father process*/
                    close(*(pipeline));
                    write(*(pipeline + 1), &witness, sizeof(char));

                    printf("<-- Soy el proceso con PID %d y acabo de enviar el testigo %c.\n", getpid(), witness);

                    darth = getpid();
                    wait(NULL);
                    close(*(pipeline + (2 * updateValue - 1)));
                    read(*(pipeline + (2 * updateValue-2)), &wBoys, sizeof(char));

                    printf("--> Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos.\n", getpid(), wBoys);
                    sleep(5);
                }
                    break;

            }
    }


    waitpid(pid, NULL, 0);

    free(pipeline);

    return 0;
}

void manual()
{
    printf("Usage: ./a.out <options> <numeric value>\n");
    printf("The possible options are:\n");
    printf("-n\t\tAllows user to input a number in the numeric value field\n-m\t\tAllows the user to view this manual.\n");
}
