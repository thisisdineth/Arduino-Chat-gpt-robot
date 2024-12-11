New Additions
Hardware Changes:
Add an SD card module.
Connect the SD card module to the ESP32 using the SPI interface:
MISO → GPIO 19
MOSI → GPIO 23
SCK → GPIO 18
CS → GPIO 5 (Chip Select)
Software Features:
Use the SD library to handle file operations.
Implement persistent memory using the SD card to:
Store user and bot details.
Log conversations.
Load memory on startup and use it to improve responses.
Updated Code:
Integrate SD card functionality for reading and writing files.
Implement memory loading and conversation logging.