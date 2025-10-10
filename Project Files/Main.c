#include "C:\Users\nvaru\OneDrive\Documents\lpc213x.h"
#include <string.h>

#define I2C_SLAVE_ADDR 0x70
#define AA 2
#define SI 3
#define STO 4
#define STA 5
#define I2EN 6

unsigned char write[] = {0xf7, 0xfd, 0xfb, 0xfe};

unsigned char combination[] = {0x77, 0xb7, 0xd7, 0xe7, 0x7b, 0xbb, 0xdb, 0xeb, 0x7d, 0xbd, 0xdd, 0xed, 0x7e, 0xbe, 0xde, 0xee}; 

char num[] = "0000032106540987"; 																																				
 
unsigned char seg_values[] = {0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x4F, 0x5B, 0x06, 0x3F, 0x7D, 0x6D, 0x66, 0x3F, 0x6F, 0x7F, 0x07};

void wait(int count){
	while(count--);
}

const char *NumToMorseCode[10] = {
    "-----", ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----."
};

void I2C_Init(void){
	VPBDIV = 0x02;
	PINSEL0 = 0x30C00050;
	I2C1SCLH = 150;
	I2C1SCLL = 150;
	I2C1CONSET = (1<<I2EN);
	I2C0SCLH = 150;
	I2C0SCLL = 150;
	I2C0CONSET = (1<<I2EN);
}

int I2C1_Start(){
	I2C1CONCLR = (1<<STO);
	I2C1CONCLR = (1<<SI);
	I2C1CONSET = (1<<STA);
	I2C0CONCLR = (1<<STO);
	I2C0CONCLR = (1<<SI);
	I2C0CONSET = (1<<STA);
	return 0;
}

// Hard Coded Delay
void delay_ms(int count){
	for(int i=0; i<count; i++){
		for(int j=0; j<35; j++);
	}
	
}

// Timer Intialized for delay
/*void timer0_init_us(void){
	T0PR = 29;
	T0TCR = 0X02;
	T0TCR = 0X01;
}

void delay_us(unsigned int us){
	T0TCR = 0x02;
	T0TCR = 0x01;
	while(T0TC < us);
	T0TCR = 0x00;
}

void delay_ms(int count){
	while(count--){
		delay_us(1000);
	}
}*/
	
void DataSend_1(char data){
	while(!(I2C1CONSET & 0x08));
	I2C1DAT = data;
	I2C1CONCLR = 1<<SI;
	delay_ms(200);
}

void DataSend_0(char data){
	while(!(I2C0CONSET & 0x08));
	I2C0DAT = data;
	I2C0CONCLR = 1<<SI;
	delay_ms(200);
}


void CharSend_1(char data){
	DataSend_1(0x50 | (data >> 4));
	delay_ms(50);
	DataSend_1(0x40 | (data >>4));
	delay_ms(50);
	DataSend_1(0x50 | (data & 0x0f));
	delay_ms(50);
	DataSend_1(0x40 | (data & 0x0f));
	delay_ms(100);
}

void CharSend_0(char data){
	DataSend_0(0x50 | (data >> 4));
	delay_ms(50);
	DataSend_0(0x40 | (data >> 4));
	delay_ms(50);
	DataSend_0(0x50 | (data & 0x0f));
	delay_ms(50);
	DataSend_0(0x40 | (data & 0x0f));
	delay_ms(100);
}

void LCD_init(){
	char code = I2C_SLAVE_ADDR;
	I2C_Init();
	I2C1_Start();
	wait(4000);
	
	while(!(I2C1CONSET & 0x08));
	
	IO1SET = (1 << 21);
	I2C1CONCLR = 1 << STO;
	I2C1CONCLR = 1 << STA;
	I2C1CONSET = 1 << AA;
	
	I2C1DAT = code;
	
	I2C1CONCLR = 1 << SI;
	
	while(!(I2C0CONSET & 0x08));
	
	IO0SET = (1 << 21);
	I2C0CONCLR = 1 << STO;
	I2C0CONCLR = 1 << STA;
	I2C0CONSET = 1 << AA;
	I2C0DAT = code; 
	I2C0CONCLR = 1 << SI;
	
	while(!(I2C1CONSET & 0x08));
	if(I2C1STAT == 0x18){									
		IO1SET = (1 << 23);
    I2C1CONSET = 1 << AA;
    I2C1DAT = 0x00;
    I2C1CONCLR = 1 << SI;
    while (!(I2C1CONSET & 0x08));
		
		for (int i = 0; i < 2000; i++){
			wait(800);
		}
		if(I2C1STAT == 0x28){
			DataSend_1(0x10);
			DataSend_1(0x00);
			DataSend_1(0x12);
			DataSend_1(0x02);
			DataSend_1(0x12); 
      DataSend_1(0x02);
      DataSend_1(0x18); 
      DataSend_1(0x08);
      DataSend_1(0x10);
      DataSend_1(0x00);
      DataSend_1(0x1e); 
      DataSend_1(0x0e);
      DataSend_1(0x10); 
      DataSend_1(0x00);
      DataSend_1(0x16); 
      DataSend_1(0x06);
      DataSend_1(0x10); 
      DataSend_1(0x00);
      DataSend_1(0x11); 
      DataSend_1(0x01);
      DataSend_1(0x18); 
      DataSend_1(0x08);
      DataSend_1(0x10);
      DataSend_1(0x00);
		}
	}
	
	while(!(I2C0CONSET & 0x08));
	if(I2C0STAT == 0x18){
		IO0SET = (1 << 23);
		I2C0CONSET = 1 << AA;
		I2C0DAT = 0x00;
		I2C0CONCLR = 1 << SI;
		
		while(!(I2C0CONSET & 0x08));
		
		for(int i=0; i<2000; i++){
			wait(800);
		}
		
		if(I2C0STAT == 0x28){
						DataSend_0(0x10);
            DataSend_0(0x00);
            DataSend_0(0x12);
            DataSend_0(0x02);
            DataSend_0(0x12);
            DataSend_0(0x02);
            DataSend_0(0x18);
            DataSend_0(0x08);
            DataSend_0(0x10);
            DataSend_0(0x00);
            DataSend_0(0x1e);
            DataSend_0(0x0e);
            DataSend_0(0x10);
            DataSend_0(0x00);
            DataSend_0(0x16);
            DataSend_0(0x06);
            DataSend_0(0x10);
            DataSend_0(0x00);
            DataSend_0(0x11);
            DataSend_0(0x01);
            DataSend_0(0x18);
            DataSend_0(0x08);
            DataSend_0(0x10);
            DataSend_0(0x00);
		}
	}
}

void init(){
	VPBDIV = 0x02;
	PINSEL1 = 0x00;
	PINSEL2 = 0x00;
	IODIR0 = 0xFF;
  IODIR1 = 0x000F0000; 
}


char RecKey(){
	int row = 0; 
	unsigned char w, w_final;
	int ind;
	int temp;
	
	while(1){
		IO1CLR = 0xffffffff;
		w = write[row];
		IO1SET |= (w << 16);
		delay_ms(1000);
		temp = IO1PIN;
		w_final = (temp >> 16) & 0xFF;
		
		if(w_final != w){
			break;
		}
		row++;
		if(row >= 4){
			row = 0;
		}
	}
	
	for(int i=0; i<16; i++){
		if(combination[i] == w_final){
			ind = i;
		}
	}
	
	return num[ind];
}


const char *GetMorseCode(char digit){
	switch (digit){
		case '0':
        return "-----";
    case '1':
        return ".----";
    case '2':
        return "..---";
    case '3':
        return "...--";
    case '4':
        return "....-";
    case '5':
        return ".....";
    case '6':
        return "-....";
    case '7':
        return "--...";
    case '8':
        return "---..";
    case '9':
        return "----.";
		default:
        return "Invalid Digit"; 
	}
}


void delay_ms_1(int j){
	for(int i=0; i<j; i++){
		for(int x=0; x < 6000; x++);
	}
}



void led(char input){
	int led_pin = 0;
	int dot = 100;
	int dash = 250;
	
	
	switch (input)
    {

    case '0': 
        IOSET0 |= 1 << led_pin;
        delay_ms_1(dash);
        IOCLR0 |= 1 << led_pin;
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '1': 
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '2': 
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '3':
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dash);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dash);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '4': 
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '5':
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '6': 
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '7':
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '8':
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;

    case '9':
        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dash);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(dot);

        IOSET0 |= (1 << led_pin);
        delay_ms_1(dot);
        IOCLR0 |= (1 << led_pin);
        delay_ms_1(500);

        break;
    }
}

int main(){
	char x; 
	int count = 100;
	const char *morseCode;
	LCD_init();
	init();
	
	
	while(count>0){
		x = RecKey();							
		CharSend_0(x);						
		morseCode = GetMorseCode(x); 		
		
		for(int i=0; morseCode[i] != '\0'; i++){
			CharSend_1(morseCode[i]);								
		}
		CharSend_1(' ');
		led(x);						
		delay_ms(35000);
		count--;
	}
	
	DataSend_1(0x10);
	DataSend_1(0x0C);
	DataSend_0(0x10);
	DataSend_0(0x0C);
	
	return 0;
}