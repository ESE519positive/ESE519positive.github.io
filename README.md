####Overview of Project

The purpose of our project is to build an automatic cocktail machine that can produce correct drinks according to the voice command from users. When the machine is available, the LCD screen displays “ready for drink”. At this stage, users can choose which drink to be made by talking to google assistant on the phone, then the machine will start making the drink with the LCD screen displaying the drink information. When the drink is done, the LCD screen goes back to “ready for drink”.

####Project Instructions 

Our project is composed of two microcontrollers: RP2040 and ESP32 Feather, three electric motors, one LCD screen and a 3D printed model. The diagram of the overall operation is shown here. ESP32 receives the voice command from users and send their choice to RP2040 through GPIO. To realize this functionality, the ThingSpeak IoT platform and IFTTT service are used. IFTTT is for receiving the voice command and post the information on the IoT server, then ESP32 will read the drink selection from the server.

ThingSpeak acts as the server. You can simply sign up with your email address for free, and create a channel by. Name one field as “sensor state” and enable it. Since we will read information from channel using ESP32, we need to use the Read API key as shown here to read data.

IFTTT stands for “If This Then That”. It is a free web-based service to create chains of simple conditional statements called applets. With IFTTT, we can trigger an event when a condition is met. Firstly, you need to download IFTTT on your phone and sign up using your email address, then you can start creating applets. They will be similar applets and serving for one purpose: receiving the voice command and uploading the information onto a server. To create an IFTTT applet, you pick a trigger then an action, respectively corresponding to “If This” and “Then That”. In this project we choose Google Assistant as our trigger. Download Google Assistant on your phone and sign in with your gmail. On IFTTT, use Google Assistant as the trigger and set the gradient as the drink name, as shown here. Then, use Webhooks as the action, use the Write API Key provided on ThingSpeak to upload the drink name. The configuration is shown as this. Now test the chain of events by talking to your Google Assistant in the way: “Ok Google, activate [drink name]”. If it’s received successfully, on the ThingSpeak channel page the entry value should increment by 1.

Now we are ready to code for ESP32, which is responsible for reading the data on ThingSpeak, sending the information to RP2040 and modifying the content displaying on the LCD screen. Micropython is used to realize the functionality. For the ESP32 to access IoT server, it needs to connect to WiFi. Once it’s connected to WiFi, it reads the data on ThingSpeak channel and send out the information by enabling and setting the corresponding GPIO pin. Besides, the content of LCD screen is also changed accordingly. The code with comments can be found here.

RP2040 receives the information form ESP32 via GPIO. The operation of RP2040 is written in C. There is a state machine constructed in the code, and RP2040 will enter different states based on the information it receives and actions it has already taken. As shown in the code, there are 3 states: IDLE, DRAW, END. When the drink is decided, the three motors will be activated via GPIO in order, for a specific time set by the code to make the correct drink.

The three motors we used are … motor bought from Amazon. They require 12V DC and produce around 12W power. 

An LCD screen is used and connected to ESP32. The commands are transferred via SPI.

The assembly details are stated here. Firstly, the SPI pins of ESP32 are connected to the SPI pins of LCD screen. Three GPIO pins of ESP32 are directly connected three GPIO pins of RP2040 for sending the decision of drink. The power wires of motors are directly connected to the 12V power supply, and the ground wires are connected to the emitter of BJT. The collector of BJT is connected to ground, and the base is connected to the corresponding GPIO pin of RP2040 that is set when a specific motor is expected to operate. 

Narrative Overview

We started our project with developing RP2040, ESP32 and motors individually and simultaneously. 

Troubleshooting

For mechanical devices, the biggest problem we met is that if we used one motor to control the angle of rotation to control which drink cup the pipe enters, the pipe cannot be pulled down successfully. After we tried two times, we decided to use three pumps to achieve our purpose.

Satisfying details

The most satisfying part of the project was the wifi control module. We were very successful in implementing voice input commands, which were transmitted to the RP2040 via ESP32, and then the motor and pump started working. We achieved this process accurately and clearly. Users can observe the progress of making drinks through the LCD display.

Team overview

Zhijing Yao :

https://github.com/ZhijingY

Siyun Wang :

https://www.linkedin.com/in/siyun-wang-8442a6253/

https://github.com/Phoebe-www

Wenxi Wei : 

Demo

https://youtu.be/BPBAF2OwBS8
