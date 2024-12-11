This circuit diagram of Arduino bot that can talk with users using open AI API 

This work like this 
with ESP 

32LRC- GPIO 14
BCLK - GPIO 27
DIN - GPIO 26 

Yellow LED - GPIO 32
Blue LED - GPIO 25
Switch  - GPIO 33MIC,

SD- GPIO 14SCK - GPIO12
WS- GPIO 15



If click press switch it turn on the whole system 
Then bot says “  Hey , thanks for wakeup me, Say hellow bot” 
 then user have to say wakeup word “hellow bot” then bot start listening to the user while listening the yellow light turns on. After user says something it converts users speach into text ..after 4 second of silence it stop listening and send the chat gpt through api. Then responses fetch from API  and it converts text to speach then bot reads it users can hear it through speaker, while bot talking the blue light on yellow light off .. after reading all reponse it again looking for wakeup word..

If user says wake up word again it agin work like that .. 
If user press switch button again it turns off the power of system  


Replace Your API key with actual API of open ai https://openai.com/index/openai-api/

REPLACE YOUR WIFI name and password with actual connected wifi and password
