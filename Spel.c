#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>
#include "Orm.h" //Project header file

int Hastighet = 2500; //Höggre värde ger långsammare hastighet
int Knappar;

int applen_y[] = { 10, 29, 20, 9, 15, 3, 21, 27, 11, 17, 8, 12, 31, 21, 19, 30, 24, 16, 8 };
int applen_x[] = { 12, 39, 28, 95, 46, 107, 13, 69, 65, 11, 28, 82, 96, 74, 5, 66, 13, 74, 33 };

//getKnappar funktionen
int getKnappar(void)
{
    int btn = (PORTD >> 5) & 0x0007;
    btn = (btn << 1);
    /* Eftersom knapparna är satta mellan bitarna 11 till 5 shiftar 5 till höger
    och maskar ut det tre lsb bitarna och kollar sedan om knappa 2,3 eller 4 är aktiva */

    // Den första knappen finns i PORTF registret

    btn = ((PORTF >> 1) & 0x1) | btn;
    return btn; // Returnera det aktiva bitarna (1:or) som integers.
}

void Spel()
{
    orm_movements(); //Anroppa på orm_movements funktionen
    //Om vi koliderar med ett äpple
    if(orm.y[0] == applen_y[poang] && orm.x[0] == applen_x[poang])
    {
        eat_applen(); //Anroppa på eat_applen funktionen
    }
        if(orm.y[0] == applen_y[poang]+1 && orm.x[0] == applen_x[poang])
    {
        eat_applen(); //Anroppa på eat_applen funktionen
    }
        if(orm.y[0] == applen_y[poang]-1 && orm.x[0] == applen_x[poang])
    {
        eat_applen(); //Anroppa på eat_applen funktionen
    }
        if(orm.y[0] == applen_y[poang] && orm.x[0] == applen_x[poang]-1)
    {
        eat_applen(); //Anroppa på eat_applen funktionen
    }
        if(orm.y[0] == applen_y[poang] && orm.x[0] == applen_x[poang]+1)
    {
        eat_applen(); //Anroppa på eat_applen funktionen
    }
    //Om vi koliderar med väggen
    if((orm.y[0] == 32 || orm.y[0] == 0) || (orm.x[0] == 128 || orm.x[0] == 0))
    {
        kollision(); //Anroppar på kollision funktionen
    }
    skapa_orm(); //Efter att ormens movements är gjorda skapar vi ormen

    int h;
    // Denna loop kollar om huvudet kommer att kolidera med några kordinater av kroppen
    for(h = orm.langd - 1; h > 0; h--)
    {
        if(orm.x[0] == orm.x[h] && orm.y[0] == orm.y[h])
            kollision();
    }

    create_applen(); //För varje frame skapar vi ett äpple som baseras på poängen som ändrar äpplets kordinater
    delay(Hastighet);
}

//Här kommer vi printa ut ormen
void skapa_orm()
{
    int i,j,h,x,y;
    for(h = 0; h < orm.langd; h++)
    {
        for(i = 0; i < 1; i++)
        {  //Denna forloop kan ändra storleken i X-led
            for(j = 0; j < 1; j++)
            {  //Denna forloop kan ändra storleken i Y-led
                x = orm.x[h] + i;
                y = orm.y[h] + j;

                display[y][x] = 1;
            }
        }
    }
}

//Movement för ormen
void orm_movements()
{
    Knappar = getKnappar(); //Tilldellar Knappar getKnappar funktionen

    int i;
    // Denna forloop kommer att ändra kordinaterna för varje del av ormen så att kroppen följer efter huvudet
    for(i = orm.langd - 1; i > 0; i--)
    {
        orm.x[i] = orm.x[i - 1];
        orm.y[i] = orm.y[i - 1];
    }
    //Ormen åker HÖGER (RIGHT)
    if(orm.riktning == 'R')
    {
        orm.x[0] += 1;
    }
    //Ormen åker NED (DOWN)
    if(orm.riktning == 'D')
    {
        orm.y[0] += 1;
    }
    //Ormen åker UPP (UP)
    if(orm.riktning == 'U')
    {
        orm.y[0] -= 1;
    }
    //Ormen åker VÄNSTER (LEFT)
    if(orm.riktning == 'L')
    {
        orm.x[0] -= 1;
    }

    //Denna kod gör så att Ormen kommer ihåg åt vilket håll den ska åka utan att trycka ned knappen
    if(orm.riktning != 'L')
    {
      //HÖGER (RIGHT)
        if(Knappar == 0b0001)
        {
            orm.riktning = 'R';
        }
    }
    if(orm.riktning != 'U')
    {
      //NED (DOWN)
        if(Knappar == 0b0010)
        {
            orm.riktning = 'D';
        }
    }
    if(orm.riktning != 'D')
    {
      //UPP (UP)
        if(Knappar == 0b0100)
        {
            orm.riktning = 'U';
        }
    }
    if(orm.riktning != 'R')
    {
      //VÄNSTER (LEFT)
        if(Knappar == 0b1000)
        {
            orm.riktning = 'L';
        }
    }
}

void create_applen()
{
    display[applen_y[poang]][applen_x[poang]] = 1; //Visar äpplet på displayen
}

void eat_applen()
{
    orm.langd++; //Ökar längden på ormen
    poang++; //Öka poängen vilket också ger äppler nya kordinater
    if(poang == 15){
        kollision();
    }
}

kollision()
{
    char poang2[] = "X"; //Deklarerar en char array
    poang2[0] = poang + '0'; //Här gör vi en integer till en sträng
    if(poang > 9){
        poang2[0] = poang + 'A' - 10;
    }

    clear_display();
    // Medans knapp 4 inte är pressad ska displayen visa IF HE DIES HE DIES och poängen
    while(Knappar != 0b1000)
    {
        display_string(0, "Better luck");
	display_string(1, "next time!");
	display_string(2, "SCORE:");
	//time2string( "SCORE:", poang );
	display_string(3, poang2);
        display_update();
        Knappar = getKnappar();
    }
    main(); //Om knapp 4:a är pressad ska vi hoppa tillbaka till main och spelet ska fortsätta
}
