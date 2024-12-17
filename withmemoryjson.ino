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

// SD Card Pin Definitions
#define SD_CS_PIN 5

bool systemOn = false;
bool isListening = false;

// Memory JSON object
StaticJsonDocument<2048> memory; // Adjust size based on memory needs

// Function Prototypes
void initializeI2S();
void initializeSD();
void loadMemory();
void saveMemory();
void saveConversationToMemory(String role, String message);
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
    command = "hello bot"; // Simulate wakeup word recognition

    if (command == "hello bot") {
      Serial.println("Wakeup word detected. Listening to the user...");
      digitalWrite(YELLOW_LED_PIN, HIGH); // Listening mode ON

      // Simulate user speech to text
      command = "My name is John"; // Simulate user speech recognition
      delay(4000); // Simulate 4 seconds of silence after speaking
      digitalWrite(YELLOW_LED_PIN, LOW); // Stop listening

      // Process command
      saveConversationToMemory("user", command); // Log user input
      String response = getResponseFromChatGPT(command); // Generate response
      saveConversationToMemory("bot", response); // Log bot response
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
  if (SD.exists("/memory.json")) {
    File memoryFile = SD.open("/memory.json", FILE_READ);
    if (deserializeJson(memory, memoryFile)) {
      Serial.println("Memory loaded successfully.");
    } else {
      Serial.println("Failed to parse memory JSON.");
    }
    memoryFile.close();
  } else {
    Serial.println("No memory file found. Starting fresh.");
  }
}

// Save memory to SD card
void saveMemory() {
  File memoryFile = SD.open("/memory.json", FILE_WRITE);
  if (memoryFile) {
    serializeJson(memory, memoryFile);
    memoryFile.close();
    Serial.println("Memory saved successfully.");
  } else {
    Serial.println("Failed to save memory JSON.");
  }
}

// Save a conversation to memory
void saveConversationToMemory(String role, String message) {
  JsonArray conversation = memory["conversation"].to<JsonArray>();
  JsonObject entry = conversation.createNestedObject();
  entry["role"] = role;
  entry["message"] = message;

  saveMemory();
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