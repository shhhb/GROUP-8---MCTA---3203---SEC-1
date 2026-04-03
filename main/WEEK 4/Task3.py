import serial
import time

# --- CONFIG ---
SERIAL_PORT = "COM4"   # Change if needed (e.g., COM3)
BAUD_RATE = 9600
AUTHORIZED_ID = "0009522418"
# ---------------

def main():
    try:
        rduino = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        time.sleep(2)  # Wait for Arduino reset
    except:
        print("Error: Cannot connect to Arduino!")
        return

    print("System Ready. Waiting for RFID...")

    try:
        while True:
            card = input("\nTap card: ").strip()

            if card == AUTHORIZED_ID:
                print("Card Accepted - Move sensor now!")

                # Send command to Arduino
                rduino.write(b'1')

                # Read Arduino feedback
                time.sleep(1)
                while rduino.in_waiting > 0:
                    response = rduino.readline().decode().strip()
                    print("Arduino:", response)

            else:
                print("WRONG CARD -> Access Denied")
                rduino.write(b'X')

    except KeyboardInterrupt:
        print("\nExiting program...")

    finally:
        rduino.write(b'0')  # Ensure system locked
        rduino.close()


if __name__ == "__main__":
    main()