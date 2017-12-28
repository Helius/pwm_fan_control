I have to control 4-wire PWM fan to make my headless NAS be quiet. I use arduino pro mini (16MHz 5V) and usb-usart adapter to connect controller to motherboard USB pin. So i can easy update firmware via arduino bootloader and communicate with controller, for example i can set fan speed via terminal just put '0'-'9' char to /dev/ttyUSB0.

[Build]
```
cd pwm_fan_control
mkdir build
cd build
cmake ../
make
make upload_fancontroller #will flash your arduino via avrdude
```
