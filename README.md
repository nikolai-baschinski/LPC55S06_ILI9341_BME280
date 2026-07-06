BME280 sensor data (temperature, air pressure, and humidity) displayed on a Waveshare ILI9341 LCD using the NXP LPC55S06-EVK. Bare-metal implementation (no SDK functions), developed with the MCUXpresso IDE. SPI and I²C drivers are available on a separate branch.

![](/Doc/Setup.jpg)

| LCD      | Connector | Function     | Port | Peripheral |
|----------|---------- |------------- |------|------------|
| VCC      | J10.11    | Vcc (3.3V)   |      |            |
| GND      | J10.17    | GND          |      |            |
| DIN      | J12.1     | SPI3_MOSI    | P0_3 | SPI3       |
| CLK      | J10.16    | SPI3_CLK     | P0_6 | SPI3       |
| CS       | J10.14    | Chip Select  | P0_4 | GPIO       |
| DC       | J10.4     | Data/Command | P0_7 | GPIO       |
| RST      | J10.5     | Reset        | P0_8 | GPIO       |
| BL       | -         | Backlight    |      |            |


| BME280   | Connector | Function     | Port  | Peripheral |
|----------|-----------|--------------|-------|------------|
| VCC      | J10.11    | Vcc (3.3V)   |       |            |
| GND      | J10.17    | GND          |       |            |
| SCL      | J13.12    | Clock        | P0_14 | I2C        |
| SDA      | J13.10    | Data         | P0_13 | I2C        |
| ADDR     | J12.2     | Vcc (3.3V)   |       |            |
| CS       | J10.11    | Vcc (3.3V)   |       |            |


![](/Doc/I2C_burst.bmp)

BME280 I2C raw data burst

Hardware

BME280: https://seengreat.com/product/207/bme280-environmental-sensor?srsltid=AfmBOorvlymsT9w0Ea-JBnftBbgADYcXMKpadnPHUyHl7X1wOO5TTgUa

LCD: https://www.waveshare.com/wiki/2.4inch_LCD_Module?srsltid=AfmBOoqtv3bq-mZfPtsi2BxiewwQnIkomXrloIzpVwGw_HnrOcmvQZar

LPC55S06-EVK: https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/lpcxpresso-boards/lpcxpresso-development-board-for-lpc55s0x-0x-family-of-mcus:LPC55S06-EVK

Video: https://youtube.com/shorts/Pad_Dj1ar_0?feature=share
