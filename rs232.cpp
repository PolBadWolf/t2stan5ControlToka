
#include "main.h"

/* UART Buffer Defines */
#define UART_RX_BUFFER_SIZE 16 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )
#define UART_TX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )

#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
#error RX buffer size is not a power of 2
#endif

/* Static Variables */
static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static  unsigned char UART_RxHead;
static  unsigned char UART_RxTail;
static unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static  unsigned char UART_TxHead;
static  unsigned char UART_TxTail;

/* Prototypes */
void InitUART(void);
char ReadComByte(unsigned char *Byte);
char WriteComByte(unsigned char data);


void InitUART(void)
{
  unsigned char x;
// For mega128
 UBRR0L = 103;// set the baud rate 9600
//enable UART receiver and transmitter,transmitte and receive interrupt
 UCSR0B = ((1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1)|(1<<TXCIE1));
// DDRD = 0x02; 
 x = 0; /* flush receive buffer */
 UART_RxTail = x;
 UART_RxHead = x;
 UART_TxTail = x;
 UART_TxHead = x;
}

/* interrupt handlers */
#pragma vector = USART0_RXC_vect  /* Прерывание приема от RS232 */
__interrupt void UART_RX_interrupt(void)
{
 unsigned char data;
 unsigned char tmphead;
 data = UDR0;  /* read the received data */
 /* calculate buffer index */
 tmphead = (UART_RxHead + 1) & UART_RX_BUFFER_MASK;
 UART_RxHead = tmphead; /* store new index */
 if(tmphead == UART_RxTail)
 {
  /* ERROR! Receive buffer overflow */
 }
 UART_RxBuf[tmphead] = data; /* store received data in buffer */
}

#pragma vector = USART0_UDRE_vect /* ?????????? ???????? ?? RS232 */
__interrupt void UART_TX_interrupt(void)
{
 unsigned char tmptail;
 /* check if all data is transmitted */
 if(UART_TxHead != UART_TxTail)
 {
  /* calculate buffer index */
  tmptail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
  UART_TxTail = tmptail; /* store new index */
  UDR0 = UART_TxBuf[tmptail]; /* start transmition */
 }
 else UCSR0B &= ~(1<<UDRE0); /* disable UDRE interrupt */
}



/* Read and write Com functions */
char ReadComByte(unsigned char *Byte)
{
 unsigned char tmptail;
 if (UART_RxHead == UART_RxTail)return 0; /* wait for incomming data */
 else
 {
  tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;/* calculate buffer index */
  UART_RxTail = tmptail; /* store new index */
  *Byte = UART_RxBuf[tmptail];/* return data */
  return 1;
 }  
}

char WriteComByte(unsigned char data)
{
 unsigned char tmphead;                             /* calculate buffer index */
 tmphead = (UART_TxHead + 1) & UART_TX_BUFFER_MASK; 
 if (tmphead == UART_TxTail) return 0;/* There is no free place in the buffer */
 else
    {      
      UART_TxBuf[tmphead] = data; /* store data in buffer */
      UART_TxHead = tmphead; /* store new index */
      UCSR0B |= (1<<UDRE0); /* enable UDRE interrupt */
      return 1;
    }
}
 
//----------------------------------------------
// выдача в буфер rs с ожиданием
void Data2BufferRs( unsigned char data)
{
  for (;WriteComByte(data)==0;) __no_operation();
}
//----------------------------------------------
// Вывести строку на rs с заданной позиции
void String2Rs( unsigned char *string)
{
 unsigned char tmp;
  for(tmp=*string++;tmp>0;)
  {
    Data2BufferRs( tmp );
    tmp = *string++;
  }
}
