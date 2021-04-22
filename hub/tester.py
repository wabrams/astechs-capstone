import serial

port = '/dev/ttyS5'
s = serial.Serial(port)
s.baudrate = 115200
s.bytesize = serial.EIGHTBITS
s.parity = serial.PARITY_NONE
s.stopbits = serial.STOPBITS_ONE
printLog('Serial Port open on', port.upper(), 'for EFR32MG12')