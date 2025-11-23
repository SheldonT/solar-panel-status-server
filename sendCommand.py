import serial
from time import sleep

import socketio

class Arduino:
    def __init__(self, url):
        self.sio = socketio.Client()

        self.sio.connect(url)

        self.sio.on("distance", self.get_response)
        self.sio.on("voltage", self.get_response)
        self.sio.on("current", self.get_response)

    def changeLED(self, status=0):
        command = f"L,{status};\n"
        self.sio.emit('send_command', command)

    def send_measure_request(self):
        command = "D,1;"
        self.sio.emit('send_command', command)

    def send_voltage_request(self):
        command = "V,1;"
        self.sio.emit('send_command', command)

    def send_current_request(self):
        command = "C,1;"
        self.sio.emit('send_command', command)

    def get_response(self, data):
        print(data)
        
if __name__ == "__main__":
    arduino = Arduino(url="http://192.168.2.250:5000")

    quit = False

    while not quit:
        print("1. Activate LED")
        print("2. Deactivate LED")
        print("3. Take a measurement")
        print("4. Get Source Voltage")
        print("5. Take Source Current")
        print("q: quit")
        print()

        command = input("What do you want to do?: ")

        if command == '1':
            arduino.changeLED(1)
        elif command == '2':
            arduino.changeLED(0)
        elif command == '3':
            arduino.send_measure_request()
            sleep(1)
        elif command == '4':
            arduino.send_voltage_request()
            sleep(1)
        elif command == '5':
            arduino.send_current_request()
            sleep(1)
        elif command == 'q':
            quit = True
        else:
            print("Invalid Input")

    print("Goodbye!")