from machine import RTC, Pin, PWM, Timer
from machine import I2C, sleep
import machine
import network
import esp32
import socket
import json
import gc
import time
from mp_i2c_lcd1602 import LCD1602
from time import sleep_ms
import urequests

red_led = Pin(33, Pin.OUT)
green_led = Pin(15, Pin.OUT)
yellow_led = Pin(32, Pin.OUT)

red_led.value(0)
green_led.value(0)
yellow_led.value(0)

#ssid = "Maxi"
#password = "zhoucaihong"
ssid = "weijie"
password = "0401wei?"
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
if wlan.isconnected():
    wlan.disconnect()
    while wlan.isconnected():
        pass
    
if not wlan.isconnected():
    print('connecting to network...')
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        pass
print('Connected to', ssid)
print('IP address:', wlan.ifconfig())

i2c = I2C(1, scl=Pin(22), sda = Pin(23))

LCD = LCD1602(i2c, addr=0x3f)

motion = False
timerFlag = 0
detectFlag = 0
timer0 = machine.Timer(0)
timer1 = machine.Timer(1)
order = 0;

def interruptHandler(timer0):
    global timerFlag
    timerFlag = 1
    
def detectHandler(timer1):
    global detectFlag
    detectFlag = 1
    

timer0.init(mode = Timer.PERIODIC, period = 5000, callback = interruptHandler)
timer1.init(mode = Timer.PERIODIC, period = 6000, callback = detectHandler)

# while loop 0.3
while(True):
    if(timerFlag == 1):
        state = machine.disable_irq()
        timerFlag = 0
        machine.enable_irq(state)
        
        if wlan.isconnected():
            gc.collect()
            status = urequests.get(url = 'https://api.thingspeak.com/channels/1721581/feeds/last.json').json()
            status = status['field1'].strip()
            order = order + 1
    
            if(status == 'Positive'):
                if(order == 1) :
                    green_led.value(1)
                    red_led.value(0)
                    yellow_led.value(0)
                    LCD.clear()
                    LCD.puts("Current status:")
                    LCD.puts("  Positive", 0, 1)
                    sleep_ms(4000)
                    LCD.clear()
                    LCD.puts("Preparing drink")
                    LCD.puts("  Positive", 0, 1)
                else :
                    urequests.get(url = 'https://api.thingspeak.com/update?api_key=6M9XOWZ408MD81G3&field1=check')
                
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
                    
            elif(status == 'Hello World'):
                if(order == 1) :
                    green_led.value(0)
                    red_led.value(0)
                    yellow_led.value(1)
                    LCD.clear()
                    LCD.puts("Current status:")
                    LCD.puts("  Hello World", 0, 1)
                    sleep_ms(4000)
                    LCD.clear()
                    LCD.puts("Preparing drink")
                    LCD.puts("  Hello World", 0, 1)
                else :
                    urequests.get(url = 'https://api.thingspeak.com/update?api_key=6M9XOWZ408MD81G3&field1=check')
                
            else :
                green_led.value(0)
                red_led.value(0)
                yellow_led.value(0)
                order = 0
                LCD.clear()
                LCD.puts("    Ready for ")
                LCD.puts("    next order", 0, 1)
