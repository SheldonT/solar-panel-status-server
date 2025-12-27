import socketio
import serial
import asyncio
from aiohttp import web

# Serial setup
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

# Socket.IO server setup
sio = socketio.AsyncServer(async_mode='aiohttp', cors_allowed_origins='*')
app = web.Application()
sio.attach(app)

@sio.event
async def send_command(sid, data):
    print(f"Received from client {sid}: {data}")

    command_prefix = data[0]

    ser.write((data + "\n").encode())

    response = ser.readline().decode().strip()
    print(f"Arduino responded: {response}")
    
    if command_prefix == 'L':
        await sio.emit("led", response)
    elif command_prefix == 'V':
        await sio.emit("voltage", response)
    elif command_prefix == 'C':
        await sio.emit("current", response)
    elif command_prefix == 'B':
        await sio.emit("battery_voltage", response)
    elif command_prefix == 'S':
        await sio.emit("battery_current", response)
    else:
        print("Unknown command prefix")
        await sio.emit("error", "Unknown command prefix")
# Start server
if __name__ == '__main__':
    web.run_app(app, host='192.168.1.11', port=5000)
