/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;
int prime = 1234567;
    volatile int* portE = (volatile int*) 0xbf886110;//initilise memory address

/* Interrupt Service Routine */
;void user_isr( void )
{

    if(IFS(0) & 0x80000){
        (*portE)++;
        IFSCLR(0) = 0x80000;
    }
    //if(IFS(0)&0x100){
            if(timeoutcount == 10){
                 time2string( textstring, mytime );
                display_string( 3, textstring );
                display_update();
                tick( &mytime );
                timeoutcount = 0;
            }
            IFSCLR(0) = 0x100;
            timeoutcount++;
    //}
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    volatile int* trisE = (volatile int*) 0xbf886100;//initilise memory address
    *trisE = *trisE & 0x00;//set bit 7 to 0 to 0
    volatile int* portE = (volatile int*) 0xbf886110;//initilise memory address
    *portE = 0;//clear portE
    volatile int* trisD = (volatile int*) 0xbf8860c0;
    *trisD = *trisD | 0x0fe0;

    /* Initialization of Timer2 */
    T2CON = 0;//set all bits in Timer2 to 0
    TMR2 = 0;		//set timer value to 0 before start
    PR2 = ((80000000/256)/10); //set period time to 31250
    T2CONSET = 0x8070;//start Timer2 and set prescale value to 256

    /* Enabling interrupts */

    IEC(0) = (1 << 19);
    IPCSET(4) = (3 << 28);
    enable_interrupt();

    IEC(0) = (1 << 8); //Skriver en 1 till T2IE, interupt enable
    IPC(2) = 4; //Sätter prioritet,
    enable_interrupt();
    return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
 prime = nextprime( prime );
 display_string( 0, itoaconv( prime ) );
 display_update();
}
