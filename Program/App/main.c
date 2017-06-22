#include "main.h"

int8u phnum[15];
int8u sbuf[160];
int8u Msg[160];

int8u Mcnt = 0;

int8u EEMEM MsgAddr[160];

int main(void)
{
	
	init();
	
	while (TRUE) {
		
		if (AppFlags.msg) {
			DisUARTInt();
			AppFlags.msg = 0;
			beep(1,50);	
			
			if (checkmsg()) {
				beep(1,250);
				WriteMsg();
				ReadMsg();
				Mcnt = 0;
			} else 
				beep(1,750);
			
			EnUARTInt();
		}
	}
	return 0;
}
static void init(void)
{
	buzinit();
	ledinit();
	beep(2,100);
	lcdinit();
	uartinit();
	tmr1init();
	GSMinit();
	Flagsinit();
	ReadMsg();
	
	lcdscrolinit();
	
	sei();
	
	EnUARTInt();

}
static void Flagsinit(void)
{
	AppFlags.msg = FALSE;
}

static void tmr1init(void)
{
	TCNT1H   = 0xD3;
	TCNT1L   = 0x00;
	TIMSK   |= _BV(TOIE1);			//ENABLE OVERFLOW INTERRUPT
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS10) | _BV(CS11); /* PRESCALAR BY 16 */
}

/* overflows at every 100msec */
ISR(TIMER1_OVF_vect) 
{ 
	static int8u i, j, l;

	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	
	if (++i >= 50) 
		 i = 0;
			
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	} 
	
	if (++j >= 3) {
		j = 0;
		if (Msg[Mcnt] != '\0') {
			lcdwd(Msg[Mcnt++]);
		} else {
			if (++l <= 16)
				lcdwd(' ');
			else {
				l = 0;
				Mcnt = 0;
				lcdwc(0x01);
				lcdwc(0x8f);
			}
		}
	}
}

ISR (USART_RXC_vect)
{
	static int8u i;
	static int8u msgcnt,phcnt;
	static int8u state = MSG_WAIT_MSG;

	switch (state) {
		case MSG_WAIT_MSG:
		if ( UDR == '\"') state = MSG_PH_NUM;
		break;
		case MSG_PH_NUM:
		if (phcnt++ < 13)
		phnum[phcnt-1] = UDR;
		else
		state = MSG_COLL_MSG;
		break;
		case MSG_COLL_MSG:
		if (UDR == LINE_FEED)
		state = MSG_RCV_MSG;
		break;
		case MSG_RCV_MSG:
		if ((sbuf[msgcnt++] = UDR) == LINE_FEED) {
			sbuf[msgcnt-2] = '\0';
			for (i = 0 ; i < 10; i++)	/* eliminate +91 */
			phnum[i] = phnum[i+3];
			phnum[i] = '\0';
			state = MSG_WAIT_MSG;
			msgcnt = 0;
			phcnt = 0;
			AppFlags.msg = 1;
			DisUARTInt();
		}
	}
}
static int8u checkmsg(void)
{
	if ((!strcmp(phnum, PhNum1)) || (!strcmp(phnum, PhNum2)))
		return 1;
	else
		return 0;
}

static void WriteMsg(void)
{
	eeprom_update_block ((const void *)sbuf, (void *)MsgAddr, 160);
}

static void ReadMsg(void)
{
	eeprom_read_block ((void *)Msg , (const void *)MsgAddr, 160) ;
	
}
