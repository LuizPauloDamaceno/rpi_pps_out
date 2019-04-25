#include <pthread.h>
#include <pigpio.h>
#include <stdio.h>
//usando isso podemos fazer com que o agendador de io pule para outros processos e depois volte a esse para realizar as operacoes
//compile usando: gcc -o ppsout ppsout.c -lrt -lpthread -lpigpio -lm -fno-pic -march=armv7-a -mfpu=neon-vfpv4 -mhard-float -mfloat-abi=hard -fno-delete-null-pointer-checks -ffast-math -fgcse-lm -fgcse-sm -fsched-spec-load -fforce-addr -fsingle-precision-constant -O3
long double inicial;
long double diferenca;
int runs = 100; //100
struct timespec agora;
struct timespec depois;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int micro_delay (int delay_us)
{
        clock_gettime(CLOCK_REALTIME, &agora);  //primeira chamada da funcao 
        inicial = agora.tv_nsec;        //valor em ns para normalizacao
        for (; ;)
        {
                clock_gettime(CLOCK_REALTIME, &depois);  //pega um timestamp todo inicio do loop
                diferenca = ((depois.tv_nsec - inicial) / (runs));    //delta t
                if ((depois.tv_nsec - inicial) < 0)
                        diferenca += 1000000000;        //(reinicia a cada 1 segundo, registrador estoura a cada 1 segundo)
                if (diferenca > (delay_us * 1000))              //atraso em ns
                        break;
        }
}

int main()
{
        gpioInitialise(); //inicializa GPIOs
        gpioSetMode(4, PI_OUTPUT); //seta GPIO4 como saída
        for (; ;)
        {
                pthread_mutex_lock(&mutex);
                gpioWrite(4, 1);        //pino ligado
                micro_delay(200);       //chamada da funcao com tOn desejado
                gpioWrite(4, 0);        //pino desligado
                micro_delay(499900);    //chamada da funcao com tOff desejado
                pthread_mutex_unlock(&mutex);
        }
}
