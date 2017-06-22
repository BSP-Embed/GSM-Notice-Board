#ifndef GSM_H
#define GSM_H

//INCLUDE HEADER FILES
#include "includes.h"

//DEFINE MACROS
#define GSMEn()				do {									\
								GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 	\
								dlyms(1000);						\
							} while(0)

#define GPSEn()				do {									\
								GSM_GPS_PORT |= _BV(GSM_GPS_PIN);		\
								dlyms(1000);						\
							} while(0)

//FUNCTION PROTOTYPES	
void 	GSMsndmsg		(const char *PhNum, const char *GSMMsg);
int8u 	GSMinit			(void);
int8u 	gsm_call_gen	(const char *PhNum);
void	GSMRxCall		(void);		
void	GSMDisCall		(void);	

#endif
