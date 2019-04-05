#include <pigpio.h>
//usando isso podemos fazer com que o agendador de io pule para outros processos e depois volte a esse para realizar as operacoes
//compile usando: gcc -o ppsout ppsout.c -lrt -lpthread -lpigpio
void micro_delay (int delay_us)
{
        //vars:
        long int inicial;
        long int diferenca;
        struct timespec agora;

        clock_gettime(CLOCK_REALTIME, &agora);  //primeira chamada da funcao 
        inicial = agora.tv_nsec;        //valor em ns para normalizacao
        for (; ;)
        {
                clock_gettime(CLOCK_REALTIME, &agora);  //pega um timestamp todo inicio do loop
                diferenca = agora.tv_nsec - inicial;    //delta t
                if (diferenca < 0)
                        diferenca += 1000000000;        //(reinicia a cada 1 segundo)
                if (diferenca > (delay_us * 1000))              //atraso em ns
                        break;
        }
}

void main()
{
        gpioInitialise(); //inicializa GPIOs
        gpioSetMode(4, PI_OUTPUT); //seta GPIO4 como saída
        for (; ;)
        {
                gpioWrite(4, 1);        //pino ligado
                micro_delay(200);       //chamada da funcao com tOn desejado
                gpioWrite(4, 0);        //pino desligado
                micro_delay(999800);    //chamada da funcao com tOff desejado
        }
}
