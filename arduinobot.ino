#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <driver/i2s.h>

// Wi-Fi Credentials
const char* ssid = "SLT_FIBRE";
const char* password = "0342221353";

// OpenAI API
const String apiKey = "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";
const String apiUrl = "https://api.openai.com/v1/chat/completions";

// GPIO Pins
#define BUTTON_PIN 32
#define YELLOW_LED_PIN 25
#define BLUE_LED_PIN 26
#define I2S_BCLK GPIO_NUM_27
#define I2S_LRC GPIO_NUM_14
#define I2S_DOUT GPIO_NUM_15
#define MIC_PIN 2

// States
bool isBotActive = false;
bool isListening = false;
bool isResponding = false;
unsigned long lastHeardTime = 0;
const unsigned long silenceTimeout = 4000; // 4 seconds of silence to stop listening

// Function Prototypes
void setupWiFi();
String transcribeAudio();
String getChatGPTResponse(const String& prompt);
void playAudio(const String& text);

void setup() {
  // Pin Configurations
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  // Turn off LEDs initially
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  // I2S Configuration
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false
  };

  i2s_pin_config_t pin_config = {
      .bck_io_num = I2S_BCLK,
      .ws_io_num = I2S_LRC,
      .data_out_num = I2S_DOUT,
      .data_in_num = I2S_PIN_NO_CHANGE // No microphone input in this example
  };

  // Install and start I2S driver
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  // Serial and Wi-Fi Initialization
  Serial.begin(115200);
  setupWiFi();
}

void loop() {
  // Check Button State
  if (digitalRead(BUTTON_PIN) == LOW) {
    isBotActive = true;
    Serial.println("Bot activated. Waiting for wake word...");
    delay(500); // Debounce delay
  }

  // Listening for Wake Word
  if (isBotActive && !isResponding) {
    String voiceInput = transcribeAudio();
    if (voiceInput.equalsIgnoreCase("hello bot")) {
      Serial.println("Wake word detected!");
      isListening = true;
      digitalWrite(YELLOW_LED_PIN, HIGH); // Turn on listening LED
    }
  }

  // Listening for User Input
  if (isListening && !isResponding) {
    String userInput = transcribeAudio();
    if (userInput != "") {
      Serial.println("User input received: " + userInput);
      lastHeardTime = millis(); // Reset silence timer
    } else if (millis() - lastHeardTime > silenceTimeout) {
      isListening = false;
      digitalWrite(YELLOW_LED_PIN, LOW); // Turn off listening LED
      isResponding = true;

      // Get Response from ChatGPT
      String response = getChatGPTResponse("The user said: " + userInput);
      Serial.println("Bot Response: " + response);

      // Play Response Audio
      digitalWrite(BLUE_LED_PIN, HIGH); // Turn on responding LED
      playAudio(response);
      digitalWrite(BLUE_LED_PIN, LOW); // Turn off responding LED

      isResponding = false;
      isBotActive = false; // Reset bot state
      Serial.println("Bot ready for next interaction.");
    }
  }
}

// Function to Setup Wi-Fi
void setupWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
}

// Function to Transcribe Audio (Placeholder for Speech Recognition)
String transcribeAudio() {
  // Simulated transcription for testing
  // Replace this with actual microphone and ML integration for speech recognition
  delay(1000);
  return "hello bot"; // Placeholder transcription
}

// Function to Get ChatGPT Response
String getChatGPTResponse(const String& prompt) {
  HTTPClient http;
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + apiKey);

  // Prepare JSON Payload
  DynamicJsonDocument doc(1024);
  JsonObject systemMessage = doc["messages"].createNestedObject();
  systemMessage["role"] = "system";
  systemMessage["content"] = "You are a helpful assistant.";

  JsonObject userMessage = doc["messages"].createNestedObject();
  userMessage["role"] = "user";
  userMessage["content"] = prompt;

  String payload;
  serializeJson(doc, payload);

  int httpResponseCode = http.POST(payload);
  String response;
  if (httpResponseCode == 200) {
    response = http.getString();
    DynamicJsonDocument responseDoc(2048);
    deserializeJson(responseDoc, response);
    response = responseDoc["choices"][0]["message"]["content"].as<const char*>();
  } else {
    Serial.println("Error: " + String(httpResponseCode));
  }
  http.end();
  return response;
}

// Function to Play Audio (Placeholder)
void playAudio(const String& text) {
  Serial.println("Playing audio: " + text);
  // Use I2S library to convert text-to-speech if supported
}
