#ifndef SPI_H_
#define SPI_H_

void init_SPI();
void SPI_transmit(unsigned char* data, int size);
void SPI_send_byte(uint8_t byte);
uint8_t SPI_send_byte_receive_byte(uint8_t byte);
uint8_t* SPI_tranceive_burst(uint8_t data);

#endif /* SPI_H_ */
