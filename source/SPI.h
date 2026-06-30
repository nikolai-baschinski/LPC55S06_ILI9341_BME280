#ifndef SPI_H_
#define SPI_H_

void init_SPI();
void SPI_Transmit(unsigned char* data, int size);
void SPI_Send_Byte(uint8_t byte);
uint8_t SPI_Send_Byte_Receive_Byte(uint8_t byte);
uint8_t* SPI_Tranceive_Burst(uint8_t data);

#endif /* SPI_H_ */
