"""
IoT Door Greeting - Serial to WebSocket Bridge
Reads Arduino serial output and broadcasts greeting triggers to the HTML page.

Requirements:
    pip install pyserial websockets
"""

import asyncio
import serial
import websockets

SERIAL_PORT = "COM3"
BAUD_RATE = 9600
WS_PORT = 8765

connected_clients = set()

async def broadcast(message):
    if connected_clients:
        await asyncio.gather(*[client.send(message) for client in connected_clients])

async def ws_handler(websocket):
    connected_clients.add(websocket)
    print(f"[WS] Client connected. Total: {len(connected_clients)}")
    try:
        await websocket.wait_closed()
    finally:
        connected_clients.discard(websocket)
        print(f"[WS] Client disconnected. Total: {len(connected_clients)}")

async def serial_reader():
    loop = asyncio.get_event_loop()
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"[Serial] Listening on {SERIAL_PORT} at {BAUD_RATE} baud...")

    while True:
        line = await loop.run_in_executor(None, ser.readline)
        line = line.decode("utf-8", errors="ignore").strip()
        if not line:
            continue
        print(f"[Serial] {line}")
        if "Person detected" in line:
            await broadcast("GREET")

async def main():
    print(f"[WS] WebSocket server starting on ws://localhost:{WS_PORT}")
    async with websockets.serve(ws_handler, "localhost", WS_PORT):
        await serial_reader()

if __name__ == "__main__":
    asyncio.run(main())
