#ifndef SPI_H_
#define SPI_H_

void init_SPI();
void SPI_Transmit(unsigned char* data, int size);
void SPI_Send_Byte(uint8_t byte);

#endif /* SPI_H_ */
