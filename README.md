## Overview of Project

The purpose of our project is to build an automatic cocktail machine that can produce correct drinks according to the voice command from users. When the machine is available, the LCD screen displays “ready for drink”. At this stage, users can choose which drink to be made by talking to google assistant on the phone, then the machine will start making the drink with the LCD screen displaying the drink information. When the drink is done, the LCD screen goes back to “ready for drink”.

## Project Instructions 

Our project is composed of two microcontrollers: RP2040 and ESP32 Feather, three electric pumps, one LCD screen and a 3D printed model. The diagram of the overall operation is shown here. ESP32 receives the voice command from users and send their choice to RP2040 through GPIO. To realize this functionality, the ThingSpeak IoT platform and IFTTT service are used. IFTTT is for receiving the voice command and post the information on the IoT server, then ESP32 will read the drink selection from the server.

ThingSpeak acts as the server. You can simply sign up with your email address for free, and create a channel by. Name one field as “sensor state” and enable it. Since we will read information from channel using ESP32, we need to use the Read API key as shown here to read data.

IFTTT stands for “If This Then That”. It is a free web-based service to create chains of simple conditional statements called applets. With IFTTT, we can trigger an event when a condition is met. Firstly, you need to download IFTTT on your phone and sign up using your email address, then you can start creating applets. They will be similar applets and serving for one purpose: receiving the voice command and uploading the information onto a server. To create an IFTTT applet, you pick a trigger then an action, respectively corresponding to “If This” and “Then That”. In this project we choose Google Assistant as our trigger. Download Google Assistant on your phone and sign in with your gmail. On IFTTT, use Google Assistant as the trigger and set the gradient as the drink name, as shown here. Then, use Webhooks as the action, use the Write API Key provided on ThingSpeak to upload the drink name. The configuration is [shown as this](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Instruction/pic1.png).

Now test the chain of events by talking to your Google Assistant in the way: “Ok Google, activate [drink name]”. If it’s received successfully, on the ThingSpeak channel page the entry value should increment by 1.

Now we are ready to code for ESP32, which is responsible for reading the data on ThingSpeak, sending the information to RP2040 and modifying the content displaying on the LCD screen. Micropython is used to realize the functionality. For the ESP32 to access IoT server, it needs to connect to WiFi. Once it’s connected to WiFi, it reads the data on ThingSpeak channel and send out the information by enabling and setting the corresponding GPIO pin. Besides, the content of LCD screen is also changed accordingly. The code with comments can be found here.

RP2040 receives the information form ESP32 via GPIO. The operation of RP2040 is written in C. There is a state machine constructed in the code, and RP2040 will enter different states based on the information it receives and actions it has already taken. As shown in the code, there are 3 states: IDLE, DRAW, END. When the drink is decided, the three pumps will be activated via GPIO in order, for a specific time set by the code to make the correct drink.

The three pumps we used are peristaltic pumps [bought from Amazon](https://www.amazon.com/gp/product/B09MVPJXFJ/ref=ppx_yo_dt_b_asin_image_o02_s00?ie=UTF8&psc=1). They require 12V DC and produce around 12W power. 

An LCD screen is used and connected to ESP32. The commands are transferred via SPI.

The assembly details are stated here. Firstly, the SPI pins of ESP32 are connected to the SPI pins of LCD screen. Three GPIO pins of ESP32 are directly connected three GPIO pins of RP2040 for sending the decision of drink. The power wires of pumps are directly connected to the 12V power supply, and the ground wires are connected to the emitter of BJT. The collector of BJT is connected to ground, and the base is connected to the corresponding GPIO pin of RP2040 that is set when a specific pump is expected to operate. 

## Narrative Overview

We started our project with developing RP2040, ESP32 and pumps individually and simultaneously. 

## Troubleshooting

For mechanical devices, the biggest problem we met is that if we used one motor to control the angle of rotation to control which drink cup the pipe enters, the pipe cannot be pulled down successfully. After we tried two times, we decided to use three pumps to achieve our purpose.

The feasibility of the mechanical part is one of the major challenges we face. We tried three different designs. In the beginning, we used RP2040 to control two motors shown in the following figures.

![image](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Troubleshooting/Picture1.png)

In an ideal state, we can put down the pipe and pull up the pipe through the rotation of motor I, and control the direction of the water pipe through the rotation of motor II. In the original idea, if we need to mix three kinds of wine, the trigger sequence of each device is shown in the figure below.

![image](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Troubleshooting/Picture2.png)

However, when we installed the simple mechanics and tested the code, we found that the water pipe did not drop smoothly. Many methods were used but could not alleviate the situation that the water pipe would bend (as shown in the yellow boxed area in the figure, the water pipe was bent so it could not be lowered).

After discussing with the mechanical students, they suggested adding limits and forces when lowering and pulling up the pipe. In this case, our second design was using friction wheels to replace Motor I. The design structure shows in the following figure.

![image](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Troubleshooting/Picture3.png)

The pipe was clamped between two wheels. We used L298N to control Motor A and Motor B. The pipe was put down when Motor A rotates clockwise and Motor B rotates counterclockwise. The pipe was pulled up when Motor A rotates counterclockwise and Motor B rotates clockwise. Motor II was still used to control the pipe direction. In this design, wheels give the limit and the force to pull up or put down the pipe. Compared to the first design, this design has improved a lot, but there is still the case that the water pipe will bend in the red box area sometimes.

To ensure its stability, we finally adopted the design of three pumps to divide the wine suction instead of a complex mechanical structure. We used RP2040 to control 3 pumps. The circuit diagram shows in the following figure. RP2040 can control the pump by sending the signal to the gate electrode of 2N222a. The details will introduce in the next section.

![image](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Troubleshooting/Picture4.png)

## Reflections

The praiseworthy part  is the simple and efficient pump control part. We take use of the characteristics of the N-type transistor and use the RP2040's GPIO to its gate input signal. When the GPIO outputs a high voltage, the transistor conducts and the circuit where the pump is located is the pathway. 

## Satisfying details

The most satisfying part of the project was the wifi control module. We were very successful in implementing voice input commands, which were transmitted to the RP2040 via ESP32, and then the motor and pump started working. We achieved this process accurately and clearly. Users can observe the progress of making drinks through the LCD display.

Triggering the water pump in this way is very simple and easy to operate, and the success of the final demonstration proves the stability of this method. But it also has disadvantages, for example, we found that when running the circuit for a long time. The transistor will get hot. In future improvements, we consider using different transistors to test their heat level.

## Team overview

Zhijing Yao :

https://github.com/ZhijingY

Siyun Wang :

https://www.linkedin.com/in/siyun-wang-8442a6253/

https://github.com/Phoebe-www

Wenxi Wei : 

## Demo

https://youtu.be/BPBAF2OwBS8
