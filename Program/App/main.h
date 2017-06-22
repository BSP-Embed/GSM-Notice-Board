#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#ifdef TESTING
#define PhNum1		"9980237552"
#define PhNum2		"9980237552"
#else
#define PhNum1		"8951129286"
#define PhNum2		"8722172886"
#endif

#define MSG_WAIT_MSG			1
#define MSG_PH_NUM				2
#define MSG_COLL_MSG			3
#define MSG_RCV_MSG				4

#define LINE_FEED				0x0A

struct  {
	volatile int8u msg:1;
}AppFlags;


//DEFINE MACROS
#define StartTmr()				TCCR0  	|= _BV(CS01)
#define StopTmr()				TCCR0  	&= ~_BV(CS01)

#define EnUARTInt()					UCSRB |= _BV(RXCIE); UCSRA |= _BV(RXC)
#define DisUARTInt()				UCSRB &= ~_BV(RXCIE); UCSRA &= ~_BV(RXC)

//FUNCTION PROTOTYPES
static	void	init		(void);
static	void	dispMsg		(void);
static	void 	tmr1init	(void);
static	int8u	checkmsg	(void);
static	void	Flagsinit	(void);
static	void	ReadMsg		(void);
static	void	WriteMsg	(void);
static	void	scroll		(void);

#endif
