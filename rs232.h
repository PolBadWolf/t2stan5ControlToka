
#ifndef rs232__h
#define rs232__h

void InitUART(void);
char ReadComByte(unsigned char *Byte);
char WriteComByte(unsigned char data);
//----------------------------------------------
// ������ � ����� rs � ���������
void Data2BufferRs( unsigned char data);
// ������� ������ �� rs � �������� �������
void String2Rs( unsigned char *string);


#endif

