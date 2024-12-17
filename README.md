

# Arduino Talking Bot with OpenAI API

This project demonstrates how to create an interactive Arduino bot that communicates with users using the OpenAI API. The bot listens to the user’s commands, processes the speech input, and responds with synthesized speech. It uses ESP32 and a microphone module for speech recognition, along with OpenAI's API for generating responses.

## Features
- **Wake-up Word Activation**: The bot can be activated with a specific wake-up phrase.
- **Speech Recognition**: The bot listens to the user's speech and converts it into text.
- **Chat with OpenAI**: The bot sends the speech text to OpenAI's API and retrieves a response.
- **Text-to-Speech**: The bot converts the OpenAI response into speech and plays it through a speaker.
- **LED Indicators**: The yellow LED indicates the bot is listening, while the blue LED indicates the bot is speaking.
- **Power Control**: A switch is used to turn the system on and off.

## Components Required
- **ESP32**: Main microcontroller for the project.
- **Microphone Module (e.g., I2S MEMS Microphone)**: For capturing audio input.
- **Speaker**: For playing the bot's responses.
- **LEDs (Yellow and Blue)**: For visual feedback (listening and speaking).
- **Switch**: To turn the bot system on/off.
- **WIFI Network**: Required for API communication.


## Pin Connections


## Circuit Diagram

Here is the circuit diagram of the Arduino bot that shows the connections:

![Circuit Diagram](circuit.png)

### ESP32 Pinout:
- **LRC**: GPIO 14
- **BCLK**: GPIO 27
- **DIN**: GPIO 26
- **Yellow LED**: GPIO 32
- **Blue LED**: GPIO 25
- **Switch**: GPIO 33
- **MIC (Microphone)**:
  - **SD**: GPIO 14
  - **SCK**: GPIO 12
  - **WS**: GPIO 15

## How It Works

1. **Power On**: Press the switch to power on the bot. The bot will greet you with the message:  
   **"Hey, thanks for waking me up! Say 'Hello bot' to start interacting."**
   
2. **Wake-up Word**: Once you say "Hello bot", the bot will start listening to you. The yellow LED will turn on while it is listening.
   
3. **Speech-to-Text**: The bot captures your speech, converts it to text, and waits for a pause (4 seconds of silence) to stop listening.

4. **API Request**: After the user finishes speaking, the bot sends the text to OpenAI's API and fetches the response.

5. **Text-to-Speech**: The bot then converts the API's text response into speech and plays it through the speaker. The blue LED turns on during speaking, and the yellow LED turns off.

6. **Waiting for Wake-up**: Once the response is finished, the bot goes back to waiting for the wake-up word.

7. **Shutdown**: Press the switch again to turn off the system.

## Prerequisites

- **OpenAI API Key**: You must have an OpenAI API key. [Get it here](https://platform.openai.com/signup).
- **Wi-Fi Credentials**: Replace with your own Wi-Fi network name and password.

## Setup Instructions

### 1. Install Libraries
Ensure you have the necessary libraries installed in your Arduino IDE:
- `ESP32` by Espressif Systems
- `WiFi.h`
- `HTTPClient.h`
- `ArduinoJson.h`
- `SpeechRecognition.h` (for capturing speech)
- `TTS (Text-to-Speech) Library` (for generating audio from text)

### 2. Configuration
- **Wi-Fi Settings**: Replace `your_SSID` and `your_PASSWORD` with your actual Wi-Fi network credentials.
- **OpenAI API**: Replace `your_openai_api_key` with your actual API key obtained from OpenAI.

### 3. Code Customization

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SpeechRecognition.h>  // Adjust according to your setup
#include <TTS.h>  // Replace with your TTS library

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// OpenAI API key
const String apiKey = "your_openai_api_key";

// GPIO Pin Assignments
#define SWITCH_PIN 33
#define YELLOW_LED_PIN 32
#define BLUE_LED_PIN 25
#define MIC_PIN 34

void setup() {
  // Initialize Serial, WiFi, and LEDs
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Wait for switch press to turn on the bot
  while(digitalRead(SWITCH_PIN) == LOW) {
    delay(100);
  }

  // Initial greeting
  speak("Hey, thanks for waking me up! Say 'Hello bot' to start interacting.");
}

void loop() {
  // Listen for wake-up word
  if (isWakeUpWordDetected()) {
    digitalWrite(YELLOW_LED_PIN, HIGH); // Turn on listening LED
    speak("Hello bot, I'm ready to listen.");
    String userSpeech = listenForSpeech(); // Function to capture and convert speech to text
    if (userSpeech.length() > 0) {
      String response = getResponseFromAPI(userSpeech);
      speak(response);  // Play the response through text-to-speech
    }
    digitalWrite(YELLOW_LED_PIN, LOW); // Turn off listening LED
  }

  // Check switch status for shutdown
  if (digitalRead(SWITCH_PIN) == HIGH) {
    turnOffBot();
  }
}

bool isWakeUpWordDetected() {
  // Replace this with actual wake-up word detection logic
  return true;  // Placeholder for wake-up word detection
}

String listenForSpeech() {
  // Placeholder for capturing and processing speech input
  String speech = "Sample user input";  // Replace with actual logic to process audio
  return speech;
}

String getResponseFromAPI(String userSpeech) {
  HTTPClient http;
  http.begin("https://api.openai.com/v1/chat/completions");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + apiKey);

  // Build the request body
  String requestBody = "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + userSpeech + "\"}]}";

  // Send the request
  int httpCode = http.POST(requestBody);

  String response = "";
  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    response = doc["choices"][0]["message"]["content"].as<String>();
  } else {
    response = "Error with the API request.";
  }

  http.end();
  return response;
}

void speak(String message) {
  // Placeholder for text-to-speech function
  Serial.println("Bot says: " + message);
  digitalWrite(BLUE_LED_PIN, HIGH); // Turn on speaking LED
  delay(2000); // Simulate speech delay
  digitalWrite(BLUE_LED_PIN, LOW); // Turn off speaking LED
}

void turnOffBot() {
  // Logic to power off the system (e.g., entering deep sleep mode)
  Serial.println("Turning off the bot.");
  // Add power off or deep sleep logic here
}
```

### 4. Upload Code
Upload the code to your ESP32 using the Arduino IDE.

### 5. Power On
Press the switch to power on the bot, and it will guide you through the interaction process.

## Troubleshooting

- **No Response**: Ensure the microphone is properly connected and placed close to the user.
- **API Errors**: Double-check your OpenAI API key and ensure the bot is connected to the internet.
- **Wi-Fi Issues**: Make sure the ESP32 is within range of your Wi-Fi network.

## Contributing
Feel free to fork this repository, open issues, or submit pull requests if you have suggestions for improvements or fixes.

___________________
---
Here’s a step-by-step guide to set up the Arduino IDE to work with your **ESP32** and this project:

---

### **Step 1: Install Arduino IDE**
1. Download and install the Arduino IDE from the official website:  
   [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software).
   
   - Use version 1.8.x or the newer **Arduino IDE 2.x**.

---

### **Step 2: Add ESP32 Support to Arduino IDE**

1. **Open Arduino IDE**:
   - Launch the Arduino IDE.

2. **Go to Preferences**:
   - In the menu bar, click **File > Preferences** (or **Arduino > Preferences** on macOS).

3. **Add ESP32 Board URL**:
   - In the **Additional Board Manager URLs** field, add the following URL:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - If there are already URLs in this field, separate them with a comma or a new line.

4. **Open the Board Manager**:
   - Go to **Tools > Board > Boards Manager**.

5. **Install the ESP32 Board Package**:
   - Search for `ESP32` in the Boards Manager.
   - Find the package from **Espressif Systems** and click **Install**.

6. **Select Your ESP32 Board**:
   - After installation, go to **Tools > Board** and select the appropriate ESP32 board for your setup (e.g., `ESP32 Dev Module`).

---

### **Step 3: Install Required Libraries**
1. **Open the Library Manager**:
   - Go to **Tools > Manage Libraries**.

2. **Search and Install These Libraries**:
   - **ArduinoJson**:
     - Used for handling JSON data.
     - Search for `ArduinoJson` by Benoit Blanchon and install the latest version.
   - **SD**:
     - Built into the IDE but ensure it’s installed and updated.
     - If missing, search for `SD` and install it.
   - **HTTPClient**:
     - Comes with the ESP32 core. No additional installation is required.
   - **driver/i2s.h**:
     - This is included with the ESP32 board package. No separate installation is required.

---

### **Step 4: Configure Your ESP32**
1. **Connect Your ESP32 Board**:
   - Use a USB cable to connect your ESP32 to your computer.

2. **Select the Correct Port**:
   - Go to **Tools > Port** and select the port to which your ESP32 is connected (e.g., `COM3` on Windows or `/dev/ttyUSB0` on Linux/macOS).

3. **Set Upload Speed**:
   - Go to **Tools > Upload Speed** and set it to **115200**.

4. **Set Partition Scheme**:
   - Go to **Tools > Partition Scheme** and choose **Default 4MB with spiffs** (or similar).

---

### **Step 5: Test ESP32 with a Simple Sketch**
1. **Upload the Blink Example**:
   - Go to **File > Examples > Basics > Blink**.
   - Change the built-in LED pin to 2 (for most ESP32 boards):
     ```cpp
     #define LED_BUILTIN 2
     void setup() {
       pinMode(LED_BUILTIN, OUTPUT);
     }
     void loop() {
       digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
       delay(1000); // Wait for a second
       digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
       delay(1000); // Wait for a second
     }
     ```
   - Upload the sketch to your ESP32 and verify the onboard LED blinks.

---

### **Step 6: Upload Your Project**
1. Open your project code in the Arduino IDE.
2. Make sure the following are properly configured:
   - **Board**: Set to `ESP32 Dev Module` (or your specific ESP32 board).
   - **Port**: Ensure the correct port is selected.
3. Click **Upload** to flash the code onto the ESP32.

---

### **Step 7: Debugging with Serial Monitor**
1. Open the Serial Monitor:
   - Go to **Tools > Serial Monitor** (or press `Ctrl+Shift+M`).
2. Set the baud rate to **115200**.
3. Watch the output to debug issues with WiFi, SD card initialization, or API requests.

---

### **Step 8: Verify SD Card Functionality**
1. Insert the formatted SD card into your SD card module.
2. Run the **SD card initialization test code**:
   ```cpp
   #include <SD.h>
   #define SD_CS_PIN 5

   void setup() {
     Serial.begin(115200);
     if (!SD.begin(SD_CS_PIN)) {
       Serial.println("SD Card initialization failed!");
       while (1);
     }
     Serial.println("SD Card initialized successfully!");
   }

   void loop() {}
   ```
3. Open the Serial Monitor to verify the SD card initialization.

---

### **Step 9: Test OpenAI API**
1. Replace the placeholder `Your_OpenAI_API_Key` with a valid OpenAI API key.
2. Use a simple test to ensure the API responds correctly:
   ```cpp
   #include <WiFi.h>
   #include <HTTPClient.h>

   #define WIFI_SSID "Your_WiFi_SSID"
   #define WIFI_PASSWORD "Your_WiFi_Password"

   #define OPENAI_API_KEY "Your_OpenAI_API_Key"

   void setup() {
     Serial.begin(115200);
     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     while (WiFi.status() != WL_CONNECTED) {
       delay(1000);
       Serial.println("Connecting to WiFi...");
     }
     Serial.println("Connected to WiFi");

     HTTPClient http;
     http.begin("https://api.openai.com/v1/models");
     http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

     int httpResponseCode = http.GET();
     if (httpResponseCode > 0) {
       String response = http.getString();
       Serial.println("API Response:");
       Serial.println(response);
     } else {
       Serial.println("Error on HTTP request");
     }
     http.end();
   }

   void loop() {}
   ```
3. Verify that the API responds with a list of available models.

---

### **Troubleshooting**
1. **WiFi Issues**:
   - Ensure the WiFi credentials are correct.
   - Verify the ESP32 is in range of the router.

2. **SD Card Errors**:
   - Format the SD card to **FAT32**.
   - Check the connections (CS pin, power, GND).

3. **Serial Communication**:
   - Ensure the baud rate in the Serial Monitor matches the code (115200).

