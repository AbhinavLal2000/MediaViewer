1. To enable SD card use FATFS from cubeide
2. Use 9 MBits/s speed
3. check jumper wires before use
4. sd card adapter miso to stm32 miso, i.e. straight connections
5. use 5V in sd card adapter as it has 3v3 regulator
6. correctly configure CS pin in sd card FATFS api