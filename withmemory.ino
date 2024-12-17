#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include "driver/i2s.h"

// WiFi Credentials
#define WIFI_SSID "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_WiFi_Password"

// OpenAI API Key
#define OPENAI_API_KEY "Your_OpenAI_API_Key"

// Pin Definitions
#define SWITCH_PIN 33
#define YELLOW_LED_PIN 32
#define BLUE_LED_PIN 25

// I2S Pin Definitions
#define I2S_BCLK 27
#define I2S_LRC 14
#define I2S_DOUT 26
#define MIC_SD_PIN 14
#define MIC_SCK_PIN 12
#define MIC_WS_PIN 15

// SD Card Pin Definitions
#define SD_CS_PIN 5

bool systemOn = false;
bool isListening = false;

// File pointers for conversation and memory
File conversationFile;
File memoryFile;

// Function Prototypes
void initializeI2S();
void initializeSD();
void loadMemory();
void saveToMemory(String key, String value);
String recallFromMemory(String key);
void logConversation(String role, String message);
String getResponseFromChatGPT(String input);
void textToSpeech(String text);

void setup() {
  Serial.begin(115200);

  // Initialize GPIO Pins
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  // Turn off LEDs initially
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  // Initialize I2S
  initializeI2S();

  // Initialize SD Card
  initializeSD();

  // Load Memory from SD Card
  loadMemory();

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (digitalRead(SWITCH_PIN) == LOW) { // Check if the button is pressed
    delay(200); // Debounce delay
    systemOn = !systemOn;

    if (systemOn) {
      Serial.println("System turned ON");
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(BLUE_LED_PIN, LOW);
    } else {
      Serial.println("System turned OFF");
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(BLUE_LED_PIN, LOW);
    }
  }

  if (systemOn) {
    Serial.println("Say the wakeup word...");
    isListening = true;
    digitalWrite(YELLOW_LED_PIN, HIGH); // Yellow LED ON when listening

    String command = ""; // Simulate audio to text recognition
    delay(5000); // Wait for user to speak
    command = "hellow bot"; // Simulate wakeup word recognition

    if (command == "hellow bot") {
      Serial.println("Wakeup word detected. Listening to the user...");
      digitalWrite(YELLOW_LED_PIN, HIGH); // Listening mode ON

      // Simulate user speech to text
      command = "My name is John"; // Simulate user speech recognition
      delay(4000); // Simulate 4 seconds of silence after speaking
      digitalWrite(YELLOW_LED_PIN, LOW); // Stop listening

      // Process command
      logConversation("user", command); // Log user input
      String response = getResponseFromChatGPT(command); // Generate response
      logConversation("bot", response); // Log bot response
      textToSpeech(response); // Convert response to speech
    }
  }
}

// Function to initialize I2S
void initializeI2S() {
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false,
      .tx_desc_auto_clear = true,
      .fixed_mclk = 0};
  i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_BCLK,
      .ws_io_num = I2S_LRC,
      .data_out_num = I2S_DOUT,
      .data_in_num = I2S_PIN_NO_CHANGE};

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM_0);
}

// Function to initialize SD card
void initializeSD() {
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed!");
    while (1);
  }
  Serial.println("SD Card initialized.");
}

// Load memory from SD card
void loadMemory() {
  if (SD.exists("/memory.txt")) {
    memoryFile = SD.open("/memory.txt", FILE_READ);
    Serial.println("Loading memory...");
    while (memoryFile.available()) {
      String line = memoryFile.readStringUntil('\n');
      Serial.println("Memory: " + line);
    }
    memoryFile.close();
  } else {
    Serial.println("No memory file found.");
  }
}

// Save key-value pair to memory
void saveToMemory(String key, String value) {
  memoryFile = SD.open("/memory.txt", FILE_APPEND);
  if (memoryFile) {
    memoryFile.println(key + ":" + value);
    memoryFile.close();
  } else {
    Serial.println("Failed to open memory file for writing.");
  }
}

// Recall value by key from memory
String recallFromMemory(String key) {
  if (SD.exists("/memory.txt")) {
    memoryFile = SD.open("/memory.txt", FILE_READ);
    while (memoryFile.available()) {
      String line = memoryFile.readStringUntil('\n');
      if (line.startsWith(key + ":")) {
        memoryFile.close();
        return line.substring(key.length() + 1);
      }
    }
    memoryFile.close();
  }
  return "";
}

// Log conversations to SD card
void logConversation(String role, String message) {
  conversationFile = SD.open("/conversation.txt", FILE_APPEND);
  if (conversationFile) {
    conversationFile.println(role + ": " + message);
    conversationFile.close();
  } else {
    Serial.println("Failed to open conversation file for writing.");
  }
}

// Simulated function to process command with ChatGPT API
String getResponseFromChatGPT(String input) {
  HTTPClient http;
  http.begin("https://api.openai.com/v1/chat/completions");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Bearer ") + OPENAI_API_KEY);

  // Create JSON payload
  StaticJsonDocument<512> doc;
  doc["model"] = "gpt-3.5-turbo";
  JsonArray messages = doc.createNestedArray("messages");
  JsonObject message = messages.createNestedObject();
  message["role"] = "user";
  message["content"] = input;

  String payload;
  serializeJson(doc, payload);

  // Send HTTP POST
  int httpResponseCode = http.POST(payload);

  String response = "";
  if (httpResponseCode == 200) {
    response = http.getString();
    Serial.println("Response from ChatGPT: " + response);
  } else {
    Serial.println("Error in ChatGPT API request");
  }
  http.end();

  return response;
}

// Simulated function to convert text to speech
void textToSpeech(String text) {
  digitalWrite(BLUE_LED_PIN, HIGH); // Blue LED ON when speaking
  Serial.println("Speaking: " + text);
  delay(5000); // Simulate TTS duration
  digitalWrite(BLUE_LED_PIN, LOW); // Blue LED OFF after speaking
}
