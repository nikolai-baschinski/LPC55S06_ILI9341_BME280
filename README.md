BME280 data (temperature, air pressure and humidity) displayed on the Waveshare ILI9341 LCD by NXP LPC55S06-EVK, bare metal (no SDK functions), using the MCUXpresso IDE.

![](/Doc/Setup.jpg)

| LCD      | Connector | Function     |
|----------|---------- |------------- |
| VCC      | J10.11    | Vcc (3.3V)   |
| GND      | J10.17    | GND          |
| DIN      | J12.1     | SPI3_MOSI    |
| CLK      | J10.16    | SPI3_CLK     |
| CS       | J10.14    | Chip Select  |
| DC       | J10.4     | Data/Command |
| RST      | J10.5     | Reset        |
| BL       | -         | Backlight    |


| BME280   | Connector | Function     |
|----------|-----------|--------------|
| VCC      | J10.11    | Vcc (3.3V)   |
| GND      | J10.17    | GND          |
| SCK      | J10.16    | SPI3_CLK     |
| MOSI     | J12.1     | SPI3_MOSI    |
| MISO     | J12.2     | SPI3_MISO    |
| CS       | J9.7      | Chip Select  |

![](/Doc/SPI_burst.bmp)

BME280 SPI raw data burst

![](/Doc/block_diagram_small.png)

Hardware

BME280: https://seengreat.com/product/207/bme280-environmental-sensor?srsltid=AfmBOorvlymsT9w0Ea-JBnftBbgADYcXMKpadnPHUyHl7X1wOO5TTgUa

LCD: https://www.waveshare.com/wiki/2.4inch_LCD_Module?srsltid=AfmBOoqtv3bq-mZfPtsi2BxiewwQnIkomXrloIzpVwGw_HnrOcmvQZar

LPC55S06-EVK: https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/lpcxpresso-boards/lpcxpresso-development-board-for-lpc55s0x-0x-family-of-mcus:LPC55S06-EVK

Video: https://youtube.com/shorts/Pad_Dj1ar_0?feature=share
