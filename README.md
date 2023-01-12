# Overview of Project

The purpose of our project is to build an automatic cocktail machine that can produce correct drinks according to the voice command from users. When the machine is available, the LCD screen displays “ready for drink”. At this stage, users can choose which drink to be made by talking to google assistant on the phone, then the machine will start making the drink with the LCD screen displaying the drink information. When the drink is done, the LCD screen goes back to “ready for drink”.

# Project Instructions 

Our project is composed of two microcontrollers: RP2040 and ESP32 Feather, three electric pumps, one LCD screen and a 3D printed model. The diagram of the overall operation is shown here. ESP32 receives the voice command from users and send their choice to RP2040 through GPIO. To realize this functionality, the ThingSpeak IoT platform and IFTTT service are used. IFTTT is for receiving the voice command and post the information on the IoT server, then ESP32 will read the drink selection from the server.

ThingSpeak acts as the server. You can simply sign up with your email address for free, and create a channel by. Name one field as “sensor state” and enable it. Example of result is shown below:


<p>
    <img src="/Instruction/ThingSpeak1.png" width="500" height="400"/>
</p>

Since we will read information from channel using ESP32, we need to use the Read API key as shown here to read data.

IFTTT stands for “If This Then That”. It is a free web-based service to create chains of simple conditional statements called applets. With IFTTT, we can trigger an event when a condition is met. Firstly, you need to download IFTTT on your phone and sign up using your email address, then you can start creating applets. They will be similar applets and serving for one purpose: receiving the voice command and uploading the information onto a server. To create an IFTTT applet, you pick a trigger then an action, respectively corresponding to “If This” and “Then That”. In this project we choose Google Assistant as our trigger. Download Google Assistant on your phone and sign in with your gmail. On IFTTT, use Google Assistant as the trigger and set the gradient as the drink name, [as shown here](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Instruction/pic2.png). Then, use Webhooks as the action, use the Write API Key provided on ThingSpeak to upload the drink name. The configuration is [shown as this](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/Instruction/pic1.png).

Now test the chain of events by talking to your Google Assistant in the way: “Ok Google, activate [drink name]”. If it’s received successfully, on the ThingSpeak channel page the entry value should increment by 1.

Now we are ready to code for ESP32, which is responsible for reading the data on ThingSpeak, sending the information to RP2040 and modifying the content displaying on the LCD screen. Micropython is used to realize the functionality. For the ESP32 to access IoT server, it needs to connect to WiFi. Once it’s connected to WiFi, it reads the data on ThingSpeak channel and send out the information by enabling and setting the corresponding GPIO pin. Besides, the content of LCD screen is also changed accordingly. The code with comments can be found [here](https://github.com/ESE519positive/ESE519positive.github.io/blob/main/code/ESP32/main.py).

RP2040 receives the information form ESP32 via GPIO. The operation of RP2040 is written in C. There is a state machine constructed in the code, and RP2040 will enter different states based on the information it receives and actions it has already taken. As shown in the code, there are 3 states: IDLE, DRAW, END. When the drink is decided, the three pumps will be activated via GPIO in order, for a specific time set by the code to make the correct drink.

The three pumps we used are peristaltic pumps [bought from Amazon](https://www.amazon.com/gp/product/B09MVPJXFJ/ref=ppx_yo_dt_b_asin_image_o02_s00?ie=UTF8&psc=1). They require 12V DC and produce around 12W power. 

An LCD screen is used and connected to ESP32. The commands are transferred via I2C.

The assembly details are stated here. Firstly, the I2C pins of ESP32 are connected to the I2C pins of LCD screen. Three GPIO pins of ESP32 are directly connected three GPIO pins of RP2040 for sending the decision of drink. The power wires of pumps are directly connected to the 12V power supply, and the ground wires are connected to the emitter of BJT. The collector of BJT is connected to ground, and the base is connected to the corresponding GPIO pin of RP2040 that is set when a specific pump is expected to operate. 

# Narrative Overview

We started our project with developing RP2040, ESP32 and pumps individually and simultaneously. For RP2040, at the beginning we started from constructing the frame of state machine for controlling the actions of pumps. The original code looked almost the same as the final version, but without the specific sleep time of the program for waiting the pumps to finish operating.

For ESP32, the completion of code started from WiFi connection. After verifying the functionality of WiFi, we added the feature of reading from the IoT platform, and adjusted the sleep time to make it compatible with real-time operation. One instance of this adjustment is shown as below:

            elif(status == 'Phoebe'):
                if(order == 1) :
                    green_led.value(0)
                    red_led.value(1)
                    yellow_led.value(0)
                    LCD.clear()
                    LCD.puts("Current status:")
                    LCD.puts("    Phoebe", 0, 1)
                    sleep_ms(4000)
                    LCD.clear()
                    LCD.puts("Preparing drink")
                    LCD.puts("    Phoebe", 0, 1)
                else :
                    urequests.get(url = 'https://api.thingspeak.com/update?api_key=6M9XOWZ408MD81G3&field1=check')
                    
The `sleep_ms(4000)` here is to ensure that the system has enough time to process the drink request without calling for the same action for multiple times. After verifying the functionality of WiFi and IoT interaction, we finally added the logic of sending commands via GPIO signals.

For the pump part, we tried three different designs. Our first design uses two motors and a pump to achieve the mixing of different types of wine drawn from different bottles. The physical picture is shown in the next section (Troubleshooting part).

<p>
    <img src="/Troubleshooting/pic11.png"/>
</p>
 
The first solution did not achieve the function we wanted perfectly, and after optimization, we replaced the servo motor with two dc motors. Two friction wheels were installed on top of the dc motors. The function is achieved by the structure of two friction wheels, one servo motor, and one pump. (The physical drawing is shown in part Troubleshooting).

<p>
    <img src="/Troubleshooting/pic22.png"/>
</p>

After the physical installation, we found that the complex mechanical structure did not achieve the desired effect. So we used three pumps instead of the original mechanical part. This time the design is efficient and stable.

<p>
    <img src="/Troubleshooting/pic33.png"/>
</p>

# Troubleshooting

## Software Issue

At beginning, we were trying to program on ESP32 in C with Arduino IDE. However, adding the functionality of interrupt while trying to connect to WiFi resulted in corruption of program. The code is shown as below:

            #include "WiFi.h"

            const char* ssid = "...";
            const char* password =  "...";

            volatile int interruptCounter = 0;
            int totalInterruptCounter = 0;

            hw_timer_t * timer = NULL;
            portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

            void IRAM_ATTR onTimer() {
            portENTER_CRITICAL_ISR(&timerMux);
            interruptCounter++;
            Serial.println("Interrupt done");
            portEXIT_CRITICAL_ISR(&timerMux);
 
            }

            void connectToNetwork() {
             WiFi.begin(ssid, password);
 
            while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Establishing connection to WiFi..");
            }
 
            Serial.println("Connected to network");
 
            }

            void setup() {
            Serial.begin(115200);
            // Set WiFi to station mode and disconnect from an AP if it was previously connected
            if (WiFi.status() != WL_CONNECTED) {
            WiFi.mode(WIFI_STA);
            WiFi.disconnect();
            delay(100);
            connectToNetwork();
            Serial.println(WiFi.localIP());
            Serial.println("Setup done");
            }

            timer = timerBegin(0, 80, true);
            timerAttachInterrupt(timer, &onTimer, true);
            timerAlarmWrite(timer, 5000000, true);
            timerAlarmEnable(timer);
            }

            void loop() {
            if (interruptCounter > 0) {
 
            portENTER_CRITICAL(&timerMux);
            interruptCounter--;
            portEXIT_CRITICAL(&timerMux);
 
            totalInterruptCounter++;
 
            Serial.print("An interrupt as occurred. Total number: ");
            Serial.println(totalInterruptCounter);
            }
            }
            
Running the WiFi portion individually would not result in any problems, but with back-end interruption the system will restart randomly, along with reconnection of WiFi and non-stoping warnings. We tried to fix the error but eventually we gave up and chose to implement the functionality in Python, as that was a more efficient and reliable approach. We met no problem with Python.

## Mechanical Issue

For mechanical devices, the biggest problem we met is that if we used one motor to control the angle of rotation to control which drink cup the pipe enters, the pipe cannot be pulled down successfully. After we tried two times, we decided to use three pumps to achieve our purpose.

The feasibility of the mechanical part is one of the major challenges we face. We tried three different designs. In the beginning, we used RP2040 to control two motors shown in the following figures.


<p>
    <img src="/Troubleshooting/Picture1.png"/>
</p>

In an ideal state, we can put down the pipe and pull up the pipe through the rotation of motor I, and control the direction of the water pipe through the rotation of motor II. In the original idea, if we need to mix three kinds of wine, the trigger sequence of each device is shown in the figure below.

<p>
    <img src="/Troubleshooting/Picture2.png"/>
</p>

However, when we installed the simple mechanics and tested the code, we found that the water pipe did not drop smoothly. Many methods were used but could not alleviate the situation that the water pipe would bend (as shown in the yellow boxed area in the figure, the water pipe was bent so it could not be lowered).

After discussing with the mechanical students, they suggested adding limits and forces when lowering and pulling up the pipe. In this case, our second design was using friction wheels to replace Motor I. The design structure shows in the following figure.

<p>
    <img src="/Troubleshooting/Picture3.png"/>
</p>

The pipe was clamped between two wheels. We used L298N to control Motor A and Motor B. The pipe was put down when Motor A rotates clockwise and Motor B rotates counterclockwise. The pipe was pulled up when Motor A rotates counterclockwise and Motor B rotates clockwise. Motor II was still used to control the pipe direction. In this design, wheels give the limit and the force to pull up or put down the pipe. Compared to the first design, this design has improved a lot, but there is still the case that the water pipe will bend in the red box area sometimes.

To ensure its stability, we finally adopted the design of three pumps to divide the wine suction instead of a complex mechanical structure. We used RP2040 to control 3 pumps. The circuit diagram shows in the following figure. RP2040 can control the pump by sending the signal to the gate electrode of 2N222a. The details will introduce in the next section.

<p>
    <img src="/Troubleshooting/Picture4.png"/>
</p>

# Reflections

The praiseworthy part is the simple and efficient pump control part. We take use of the characteristics of the N-type transistor and use the RP2040's GPIO to its gate input signal. When the GPIO outputs a high voltage, the transistor conducts and the circuit where the pump is located is the pathway. 

# Explanation of PIO

In our code on RP2040, we used PIO functioning as GPIO. We were planning to utilize PIO to realize I2C, but soon we figured that it would be better to let MicroPython handle the I2C communication between the board and LCD screen. As the main functionalities of our project are primarily achieved via GPIO, we decided to apply PIO for toggling the pins. We used PIO state machine to change the status of the GPIO pins for controlling the motors.

# Satisfying details

The most satisfying part of the project was the wifi control module. We were very successful in implementing voice input commands, which were transmitted to the RP2040 via ESP32, and then the motor and pump started working. We achieved this process accurately and clearly. Users can observe the progress of making drinks through the LCD display.

Triggering the water pump in this way is very simple and easy to operate, and the success of the final demonstration proves the stability of this method. But it also has disadvantages, for example, we found that when running the circuit for a long time. The transistor will get hot. In future improvements, we consider using different transistors to test their heat level.

# Team overview

Zhijing Yao :

https://github.com/ZhijingY

Siyun Wang :

https://www.linkedin.com/in/siyun-wang-8442a6253/

https://github.com/Phoebe-www

Wenxi Wei : 

https://github.com/wenxiwei00/

# Demo

https://youtu.be/BPBAF2OwBS8
