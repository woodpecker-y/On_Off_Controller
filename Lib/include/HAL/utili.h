#ifndef _UTILI_H
#define _UTILI_H

#define ORD_INTEL   0x01   //
#define ORD_MOTOR   0x00   //

unsigned char*	dec_2_bcd_type(unsigned char *dst, int len, unsigned long dec, unsigned char type);
unsigned long	bcd_2_dec_type(const unsigned char *iData, int iLen, unsigned char type);

unsigned char* dec_2_hex_type(unsigned char* pData, int iCovLen, unsigned long iValue, int iType);
unsigned long  hex_2_dec_type(const unsigned char* pData, int iCovLen, int iType);

unsigned char	check_sum(unsigned char *dat, unsigned char len);
unsigned char	*bytes_reverse(unsigned char *dat, unsigned char len);
char			*bytes_2_ascii(char *str, unsigned char str_len, const unsigned char *bytes, unsigned char bytes_len);
unsigned short	crc_16_modbus(const unsigned char *data, unsigned short len);
int ascii_2_bytes(unsigned char *o_hex, unsigned int hex_size, const char *i_ascii, unsigned int ascii_size);

int is_valid_time(int year, int month, int day, int hour, int min, int sec);

void str_split(char *substr1, char *substr2, const char *str, char sep);

#endif
