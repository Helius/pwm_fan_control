add_definitions("-DUART_BAUDRATE=115200")
add_definitions("-DUART_DOUBLE_SPEED")

add_avr_library(
   dslib
	 ds18S20.c 
	 ds18S20.h
)
