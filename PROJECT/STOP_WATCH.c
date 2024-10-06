/*
 * STOP_WATCH.c
 *
 *  Created on: Sep 8, 2024
 *      Author: elouf
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
/* ************************************************* */
void TIMER1_CTC_ENABLE(void);
void show_times(void);
void count_up_timer(void);
void RESET(void);
void PAUSE(void);
void RESUME(void);
void INT0_ENABLE(void);
void INT1_ENABLE(void);
void INT2_ENABLE(void);
void COUNT_DOWN_TIMER(void);
void HOURS_INCREASED(void);
void HOURS_DECRESED(void);
void MIN_INCREASED(void);
void MIN_DECRESED(void);
void SEC_INCREASED(void);
void SEC_DECRESED(void);





/* ************************************************* */

#define EHOUR10 PA0
#define EHOUR1 PA1
#define EMIN10 PA2
#define EMIN1 PA3
#define ESEC10 PA4
#define ESEC1 PA5
#define RESET_button PD2
#define PAUSE_BUTTON PD3
#define RESUME_BUTTON PB2
#define TOOGLE_MODE PB7
#define HOURES_INC PB1
#define HOURES_DEC PB0
#define MIN_INC PB4
#define MIN_DEC PB3
#define SECOND_INC PB6
#define SECOND_DEC PB5
#define RED_COUNT_UP PD4
#define YELLOW_COUNT_DOWN PD5
#define BUZZER_END_COUNT_DOWN PD0




/* ************************************************* */
unsigned char second=0;
unsigned char min=0;
unsigned char ff=0;
unsigned char hours=0;
unsigned char toogle_flag=0;
unsigned char flag_timer=0;
unsigned char FLAG_HOURS_INCREASED=0;
unsigned char FLAG_HOURS_DECREASED=0;
unsigned char FLAG_MIN_INCREASED=0;
unsigned char FLAG_MIN_DECREASED=0;
unsigned char FLAG_SEC_INCREASED=0;
unsigned char FLAG_SEC_DECREASED=0;

unsigned char FLAG_ADJUSTMENT=0;

/* ************************************************* */



int main(void)
{
	DDRD|=(1<<YELLOW_COUNT_DOWN); // YELLPW COUNT DOWN
	PORTD&=~(1<<YELLOW_COUNT_DOWN);// OFF
	DDRD|=(1<<RED_COUNT_UP);// RED COUNT UP O/P
	PORTD|=(1<<RED_COUNT_UP);// RED=ON;
	DDRC|=0X0F; // SEVEN SEGMENT O/P
	//PORTC&=0XF0; // INTIALIZE 0
	DDRA|=0X3F; // ENABLE TRANS O/P
	PORTA|=0X3F;// ALL TRANS ON (6 7SEG ON)
	SREG|=(1<<7);//ENABLE I BIT
	DDRB&=~(1<<TOOGLE_MODE);//  TOOGLE_MODE ARE I/P
	PORTB|=(1<<TOOGLE_MODE);//INTERNAL PULL UP
	DDRB&=0X84; // ALL ADJUSTMENT I/P
	PORTB|=0X7B; // INTERNAL PULL UP;
	DDRD|=(1<<BUZZER_END_COUNT_DOWN);// BUZZER AS O/P
	PORTD&=~(1<<BUZZER_END_COUNT_DOWN); //BUZZER OFF



	TIMER1_CTC_ENABLE(); // TIMER ON
	INT0_ENABLE();// ENABLE INT0
	INT1_ENABLE();// ENABLE INT1
	INT2_ENABLE();

	while(1)
	{
		 show_times();

			 //AFTER PAUSE BEFORE RESUME
			 HOURS_INCREASED();
			 			HOURS_DECRESED(); // AFTER PAUSE BEFORE RESUME
			 			 MIN_INCREASED(); // WE ENABLE ADJUSTMENT
			 			MIN_DECRESED();
			 			 SEC_INCREASED();
			 			 SEC_DECRESED();


		 if(!(PINB&(1<<TOOGLE_MODE))) // WE ENABLE TOGGLE MODE
		 {
			 if(ff==0)
			 {
				 ff=1;
				  if (toogle_flag == 0) {
					  toogle_flag= 1; // Set to count down
				    } else {
				        toogle_flag = 0; // Set to count up
				    }


			 }
		 }
		 else
		 {
			 ff=0;
		 }



		if(toogle_flag) // pressed on toggle //count down
		{
			PORTD&=~(1<<RED_COUNT_UP); // RED OFF
			PORTD|=(1<<YELLOW_COUNT_DOWN);// YELLOW ON;




		}
		else // COUNT UP
		{
			PORTD&=~(1<<YELLOW_COUNT_DOWN); // YELLOW OFF
			PORTD|=(1<<RED_COUNT_UP); // RED ON


		}
		if(second!=0||min!=0||hours!=0)
		{
			PORTD&=~(1<<BUZZER_END_COUNT_DOWN); //BUZZER OFF
		}






	}
}
void show_times(void)
{
	PORTA=(PORTA&0XC0)|(0X20); // ESEC1=1 AND OTHER=0
		PORTC=(PORTC&0XF0)|((second%10)&0X0F); // SEC1=SECOND%10 الاحاد
		_delay_ms(2);
		PORTA=(PORTA&0XC0)|(0X10); // ESEC10=1 AND OTHER=0
		PORTC=(PORTC&0XF0)|((second/10)&0X0F); // SEC10=SECOND%10
		_delay_ms(2);
		PORTA=(PORTA&0XC0)|(0X08); // Emin1=1 AND OTHER=0
		PORTC=(PORTC&0XF0)|((min%10)&0X0F); // min1=min%10
		_delay_ms(2);
		PORTA=(PORTA&0XC0)|(0X04); // Emin10=1 AND OTHER=0
		PORTC=(PORTC&0XF0)|((min/10)&0X0F); // min10=min%10
		_delay_ms(2);
		PORTA=(PORTA&0XC0)|(0X02); // Emin10=1 AND OTHER=0
		PORTC=(PORTC&0XF0)|((hours%10)&0X0F); // min10=min%10
		_delay_ms(2);
		PORTA=(PORTA&0XC0)|(0X01); // Emin10=1 AND OTHER=0
		PORTC=(PORTC&0XF0)|((hours/10)&0X0F); // min10=min%10
		_delay_ms(2);

}
void TIMER1_CTC_ENABLE(void)
{
	TCCR1A=(1<<FOC1A)|(1<<FOC1B);
	TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10); // TIMER WITH CTC N=1024 TCLK=62.5 US
	TCNT1=0;
	OCR1A=16000;
	TIMSK=(1<<OCIE1A); //ENABLE BIT=1

}
void count_up_timer(void)
{
	if(second==59 &&min==59&&hours==99 )
	{
		second=0;
		min=0;
		hours=0;

	}
	if(second<59)
	{
		second++;
	}
	else if(min<59)
	{
		min++;
		second=0;
	}
	else if(hours<99)
	{
		hours++;
		second=0;
		min=0;

	}

	/*
	second++;
	if(second==60)
	{
		second=0;
		min++;
	}
	if(min>=60)
	{
		min=0;
		hours++;

	}
	if(hours==99)
	{
		second=0;
		min=0;
		hours=99;
	}
*/
}
void RESET(void)
{
	second=0;
	min=0;
	hours=0;
}
void PAUSE(void)
{
	TCCR1B&=0XF8; // TIMER CLOCK DISABLED
}
void RESUME(void)
{
	TCCR1B|=0X05; // TIMER CLOCK ENABLED
}
void COUNT_DOWN_TIMER(void)
{
    if (second == 0 && min == 0 && hours == 0)
    {
        // The timer has reached zero, so no need to continue.
    	PORTD|=(1<<BUZZER_END_COUNT_DOWN); // BUZZER ON
        return;
    }
    if(second>0)
    {
    	second--;
    }
    else if(min>0) // sec<=0
    {
    	min--;
    	second=59;
    }
    else if(hours>0) // sec<=0 && min<=0
    {
    	hours--;
    	min=59;
    	second=59;

    }

    // Decrement the seconds

}

void INT0_ENABLE(void)
{
	DDRD&=~(1<<RESET_button);// RESET BUTTON I/P
	PORTD|=(1<<RESET_button);// ENABLE INTERNAL PULL UP RESISTOR
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);// INT0 WITH FALLING EDGE
	GICR|=(1<<INT0); // MODULE INTERRUPT ENABLE
}
void INT1_ENABLE(void)
{
	DDRD&=~(1<<PAUSE_BUTTON);// PAUSE BUTTON I/P
	MCUCR|=(1<<ISC10)|(1<<ISC11); // INT1 WITH RAISING EDGE
	GICR|=(1<<INT1); // MODULE INTERRUPT ENABLE
}
void INT2_ENABLE(void)
{
	DDRB&=(1<<RESUME_BUTTON); // RESUME BUTTON I/P
	PORTB|=(1<<RESUME_BUTTON); // ENABLE INTERNAL PULL UP RESISTOR
	MCUCSR&=~(1<<ISC2); // INT2 WITH FALLING EDGE
	GICR|=(1<<INT2);  // MODULE INTERRUPT ENABLE
}
void HOURS_INCREASED(void) {

	if (!(PINB & (1 << HOURES_INC))) {
		_delay_ms(30);
		if (!(PINB & (1 << HOURES_INC))) {
			if (FLAG_HOURS_INCREASED == 0) {
				FLAG_HOURS_INCREASED=1;
				hours++;
				if (hours == 100) {
					hours = 99;
				}
			}

		}
	}
	else
	{
		FLAG_HOURS_INCREASED=0;
	}
}
void HOURS_DECRESED(void)
 {

	if (!(PINB & (1 << HOURES_DEC))) {
		_delay_ms(30);

		if (!(PINB & (1 << HOURES_DEC))) {
			if (FLAG_HOURS_DECREASED == 0) {
				FLAG_HOURS_DECREASED = 1;
				if (hours == 0) {
					hours = 0;
				} else {
					hours--;
				}
			}

		}
	} else {
		FLAG_HOURS_DECREASED = 0;
	}
}
void MIN_DECRESED(void) {
	if (!(PINB & (1 << MIN_DEC))) {

		_delay_ms(30);
		if (!(PINB & (1 << MIN_DEC))) // IF PRESSED ON HOURS INCREASED
		{
			if (FLAG_MIN_DECREASED == 0) {
				FLAG_MIN_DECREASED = 1;
				if (min == 0) {
					min = 0;

				} else {
					min--;
				}

			}
		}

	} else {
		FLAG_MIN_DECREASED = 0;
	}
}
void MIN_INCREASED(void) {

	if (!(PINB & (1 << MIN_INC))) {
		_delay_ms(30);
		if (!(PINB & (1 << MIN_INC))) // IF PRESSED ON HOURS INCREASED
		{
			if (FLAG_MIN_INCREASED == 0) {
				FLAG_MIN_INCREASED = 1;
				min++;
				if (min == 60) {
					min = 59;

				}
			}

		}
	} else {
		FLAG_MIN_INCREASED = 0;
	}

}
void SEC_INCREASED(void)
{


	if (!(PINB & (1 << SECOND_INC))) {
			_delay_ms(30);
			if (!(PINB & (1 << SECOND_INC))) // IF PRESSED ON HOURS INCREASED
			{
				if (FLAG_SEC_INCREASED == 0) {
					FLAG_SEC_INCREASED = 1;
					second++;
					if (second == 60) {
						second = 59;

					}
				}

			}
		} else {
			FLAG_SEC_INCREASED = 0;
		}



}
void SEC_DECRESED(void) {

	if (!(PINB & (1 << SECOND_DEC))) // IF PRESSED ON HOURS INCREASED
	{
		_delay_ms(30);
		{
			if (!(PINB & (1 << SECOND_DEC))) // IF PRESSED ON HOURS INCREASED
			{
				if (FLAG_SEC_DECREASED == 0) {
					FLAG_SEC_DECREASED = 1;

					if (second == 0) {
						second = 0;
					} else {
						second--;
					}
				}

			}
		}
	} else {
		FLAG_SEC_DECREASED = 0;
	}

}




ISR(INT0_vect)
{
	RESET();
}
ISR(INT1_vect)
{
	PAUSE();

}
ISR(INT2_vect)
{
	RESUME();

}

ISR(TIMER1_COMPA_vect)
{

	if(toogle_flag==0)
	count_up_timer();
	else if(toogle_flag==1)
	{
	COUNT_DOWN_TIMER();
	}
	// after one second
}
