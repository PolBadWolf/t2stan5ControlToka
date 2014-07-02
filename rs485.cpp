
#include "rs485.h"
#include "main.h"
/* UART2.c - Interrupt Driver */
/* includes */

/* UART Buffer Defines */
#define UART2_RX_BUFFER_SIZE 64 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART2_RX_BUFFER_MASK ( UART2_RX_BUFFER_SIZE - 1 )
#define UART2_TX_BUFFER_SIZE 64 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART2_TX_BUFFER_MASK ( UART2_TX_BUFFER_SIZE - 1 )

#if (UART2_RX_BUFFER_SIZE & UART2_RX_BUFFER_MASK)
#error RX buffer size is not a power of 2
#endif

/* Static Variables */
static unsigned char UART2_RxBuf[UART2_RX_BUFFER_SIZE];
static unsigned char UART2_RxHead;
static unsigned char UART2_RxTail;
static unsigned char UART2_TxBuf[UART2_TX_BUFFER_SIZE];
static unsigned char UART2_TxHead;
static unsigned char UART2_TxTail;
unsigned char fl_485rx=0;

/* Prototypes */
void InitUART2(void);
char ReadCom2Byte(unsigned char *Byte);
char WriteCom2Byte(unsigned char data);
//-------------------------------------------
#define K1             0x02  /*   */
#define RS485_R  PORTD &= ~K1 /* прием */
#define RS485_T  PORTD |= K1  /* передача */


void InitUART2(void)
{
 unsigned char x;
// For mega128
 UBRR1L = 51;//скорость работы интерфейса 19200
//enable UART receiver and transmitter,transmitte and receive interrupt
 UCSR1B = ((1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1)|(1<<TXCIE1));
// UCSR1C = 0x24; //19200,7,E,1
 
// DDRD = 0x02; 
 x = 0; /* flush receive buffer */
 UART2_RxTail = x;
 UART2_RxHead = x;
 UART2_TxTail = x;
 UART2_TxHead = x;
}

/* interrupt handlers */
#pragma vector = USART1_RXC_vect /* Прерывание приема от RS485 */
__interrupt void UART2_RX_interrupt(void)
{
 unsigned char data;
 unsigned char tmphead;
 data = (UDR1 & 0x7F); /* read the received data */
 /* calculate buffer index */
 tmphead = (UART2_RxHead + 1) & UART2_RX_BUFFER_MASK;
 UART2_RxHead = tmphead; /* store new index */
 if(tmphead == UART2_RxTail)
 {
  /* ERROR! Receive buffer overflow */
 }
 UART2_RxBuf[tmphead] = data; /* store received data in buffer */
}

#pragma vector = USART1_UDRE_vect /* Прерывание передачи от RS485 */
__interrupt void UART2_TX_interrupt(void)
{
 unsigned char tmptail;
 /* check if all data is transmitted */
 if(UART2_TxHead != UART2_TxTail)
 {
  /* calculate buffer index */
  tmptail = (UART2_TxTail + 1) & UART2_TX_BUFFER_MASK;
  UART2_TxTail = tmptail; /* store new index */
  UDR1 = UART2_TxBuf[tmptail]; /* start transmition */
 }
 else
 {
  UCSR1B &= ~(1<<UDRE1); /* disable UDRE interrupt */
  fl_485rx=1;
 }
}

#pragma vector = USART1_TXC_vect /* Прерывание передачи от RS485 */
__interrupt void UART2_TX_END(void)
{
 if(fl_485rx ==1)
 {
   RS485_R; // переключение на прием
   fl_485rx=0;
 }
}



/* Read and write Com functions */
char ReadCom2Byte(unsigned char *Byte)
{
 unsigned char tmptail;

if (UART2_RxHead == UART2_RxTail) return 0; /* wait for incomming data */
 else
 {
  tmptail = (UART2_RxTail + 1) & UART2_RX_BUFFER_MASK;/* calculate buffer index */
  UART2_RxTail = tmptail; /* store new index */
  *Byte = UART2_RxBuf[tmptail];/* return data */
  return 1;
 }  
}

char WriteCom2Byte(unsigned char data)
{
 unsigned char tmphead;          /* calculate buffer index */
 RS485_T; // send
 tmphead = (UART2_TxHead + 1) & UART2_TX_BUFFER_MASK; 
 if (tmphead == UART2_TxTail) return 0;/* There is no free place in the buffer */
 else
    {      
      UART2_TxBuf[tmphead] = data | 0x80; /* store data in buffer */
      UART2_TxHead = tmphead; /* store new index */
      UCSR1B |= (1<<UDRE1); /* enable UDRE interrupt */
      return 1;
    }
}
 
