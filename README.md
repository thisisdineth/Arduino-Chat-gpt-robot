

# ESP32 ChatGPT-Powered Voice Assistant Robot

An Arduino-based project using the ESP32 microcontroller to create a voice-enabled assistant powered by ChatGPT. This assistant can process user commands via voice, fetch responses from OpenAI's GPT model, and play responses via text-to-speech.

---

## 🚀 **Features**

- **Voice Recognition:** Detects wake words and user input through a connected microphone.
- **Wi-Fi Connectivity:** Communicates with OpenAI's API over Wi-Fi.
- **OpenAI Integration:** Uses OpenAI GPT to generate intelligent responses.
- **LED Indicators:**
  - **Yellow LED:** Indicates the bot is listening.
  - **Blue LED:** Indicates the bot is responding.
- **Text-to-Speech Playback:** Converts ChatGPT responses into speech.

---

## 🛠️ **Hardware Requirements**

1. **ESP32 Development Board** (e.g., ESP32 Dev Module)
2. **Microphone** (I2S compatible)
3. **LEDs**:
   - Yellow LED (Listening Indicator)
   - Blue LED (Responding Indicator)
4. **Push Button** (To activate the assistant)
5. Resistors, wires, and a breadboard for prototyping.

---

## 🧰 **Software Requirements**

1. **Arduino IDE** (v1.8.19 or higher recommended)
2. **ESP32 Board Core**:
   - Install from **Boards Manager** in Arduino IDE.  
     **Steps**:  
     Go to **File > Preferences**, and add the following URL to the "Additional Board Manager URLs":  
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
3. **Required Libraries**:
   - `WiFi.h` (ESP32's built-in library)
   - `HTTPClient.h` (ESP32's built-in library)
   - `ArduinoJson` (Install from Library Manager)
   - `driver/i2s.h` (Included in ESP32 Core)

---

## 📋 **Circuit Diagram**

Here’s how to wire up your components:

| **Component**   | **ESP32 Pin** |
|------------------|---------------|
| Button           | GPIO 32       |
| Yellow LED       | GPIO 25       |
| Blue LED         | GPIO 26       |
| I2S BCLK         | GPIO 27       |
| I2S LRC          | GPIO 14       |
| I2S DOUT         | GPIO 15       |
| Microphone Data  | GPIO 2        |

Connect all grounds to the ESP32 GND pin.

---

## 🖥️ **Code Setup**

1. Clone this repository:
   ```bash
   git clone https://github.com/thisidineth/arduniorobot.git
   cd arduniorobot
   ```
2. Open the `arduinobot.ino` file in the Arduino IDE.
3. Update the following variables with your details:
   ```cpp
   const char* ssid = "Your_SSID";           // Your Wi-Fi SSID
   const char* password = "Your_PASSWORD";   // Your Wi-Fi Password
   const String apiKey = "Your_OpenAI_API_Key";  // Your OpenAI API Key
   ```
4. Select your ESP32 board from **Tools > Board > ESP32 Arduino > ESP32 Dev Module**.
5. Select the appropriate COM port under **Tools > Port**.
6. Upload the code to your ESP32 board.

---

## 📖 **How It Works**

1. **Initialization**:
   - Connect to Wi-Fi.
   - Initialize I2S microphone and LEDs.

2. **Voice Activation**:
   - Press the button to activate the bot.
   - Speak the wake word, **"hello bot"**.

3. **Conversation**:
   - Speak your query after activation.
   - The bot sends your input to ChatGPT and retrieves a response.
   - The response is played back as audio.

4. **LED Indicators**:
   - **Yellow LED ON:** Bot is listening for user input.
   - **Blue LED ON:** Bot is processing or responding.

---

## 🧪 **Testing and Debugging**

- Use the **Serial Monitor** in the Arduino IDE to debug:
  - View connection status.
  - Check transcription and API responses.
- Ensure the correct `WiFi.h` library is installed (use the one provided by ESP32).

---

## 📌 **Future Enhancements**

- Add real-time speech recognition with Google Speech-to-Text or TensorFlow Lite.
- Improve text-to-speech functionality using a dedicated TTS engine.
- Expand hardware support to include external DACs for better audio quality.
- Integrate additional wake words and multi-language support.

---

## 📜 **License**

This project is licensed under the MIT License. Feel free to use, modify, and distribute the code.

---

## 🤝 **Contributing**

Contributions are welcome! Please fork the repository and submit a pull request with your improvements.

---

## 📞 **Contact**

For any questions or feedback, feel free to open an issue on this repository or reach out to:
- **Email**: chat@dinethdilshan.com
- **GitHub**: [your_username](https://github.com/thisisineth)

---
