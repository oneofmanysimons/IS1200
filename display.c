#include <stdint.h>
#include <pic32mx.h>
#include "Orm.h" //Project header file

void *stdout, *stderr, *stdin;

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

// Om någon punkt i arrayen är satt till 1 kommer pixeln med samma position att tändas

uint8_t display[32][128]; // dispaly 32x128 mänskliga läsbara pixlar, position och aktivation

uint8_t oled_display[512]; //Dator läsbara pixlar, position och aktivation

// Denna funktion anroppar det nödvändiga funktionerna för att spelet ska starta
void display_start()
{
    //display_init();
	clear_display(); // Denna funktion rensar skärmen varje gång den loopar (Den rensar den sista framen)

	Spel(); // Anroppar på spel
	translateToImage(); // Översätter
	display_image(0, oled_display); // Och sen visar framen på displayen
}

//(Tagen från labb) Sänder data till OLED displayen
uint8_t spi_send_recv(uint8_t data)
{
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

//(Tagen från labb) Quick sleep timer
void quicksleep(int cyc)
{
	int i;
	for(i = cyc; i > 0; i--);
}

//(Tagen från labb) Display initierar OLED display
void display_init(void)
{
	DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

//(Tagen från Labb) Visar texten på display
void display_string(int line, char *s)
{
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

//(Tagen från Labb) Visar Texten på displayen
void display_update(void)
{
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++)
	{
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++)
		{
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/*(Taken from lab 3) Denna funktion kan printa ut bild på display med hjälp av array inehållandes en karta över
inaktiva och aktiva pixlar */
void display_image(int x, const uint8_t *data)
{
	int i, j;

	for(i = 0; i < 4; i++)
	{
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 128; j++)
			spi_send_recv(~data[i*128 + j]); // invers tecken bakom data för att invertera färgerna (svart och vit) på displayen
	}
}

/* Denna funktion översätter display array till OLED_display array.
Den converterar ett y och x kordinat system till en läsbar array för datorn. */
void translateToImage()
{
  int page, column, row, c, k; //32bits
  uint8_t powerOfTwo = 1;// range 0 - 255 8 bits
  uint8_t oledNumber = 0;// range 0 - 255 8 bits
  int survivalMode = 0; //32 bits

  for(page = 0; page < 4; page++)
	{
    for(column = 0; column < 128; column++)
		{
      powerOfTwo = 1;
      oledNumber = 0;

      for(row = 0; row < 8; row++)
			{
        if(display[8 * page + row][column])
				{
          oledNumber |= powerOfTwo;//copy value of
        }
        powerOfTwo <<= 1;
      }
      oled_display[column + page * 128] = oledNumber;
    }
  }
}

// Denna funktion sätter alla värden på display arrayen och OLED display array till 0:or
void clear_display()
{
  int row, column, i;

  for(row = 0; row < 32; row++)
	{
    for(column = 0; column < 128; column++)
		{
      display[row][column] = 0;
    }
  }

  for (i = 0; i < 512; i++)
	{
    oled_display[i] = 0;
  }
}
