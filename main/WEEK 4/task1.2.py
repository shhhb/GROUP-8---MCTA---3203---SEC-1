import serial
import time

# -- Replace with your Arduino's serial port
SERIAL_PORT = 'COM4'
BAUD_RATE = 9600


def is_motion(prev_x, prev_y, curr_x, curr_y):
    """
    Detects general movement (not rotation)
    """
    dx = abs(curr_x - prev_x)
    dy = abs(curr_y - prev_y)

    # Total movement
    movement = dx + dy

    # Sensitivity threshold (adjust if needed)
    MOTION_THRESHOLD = 800

    return movement > MOTION_THRESHOLD


def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)

    print("System ready. Move MPU6050 to turn LED ON...")

    led_is_on = False

    prev_x = None
    prev_y = None

    try:
        while True:
            raw = ser.readline().decode(errors='ignore').strip()

            if raw:
                parts = raw.split(',')

                if len(parts) >= 2:
                    try:
                        ax = int(parts[0])
                        ay = int(parts[1])

                        if prev_x is not None and prev_y is not None:
                            if is_motion(prev_x, prev_y, ax, ay):
                                if not led_is_on:
                                    print("Motion Detected! LED ON")
                                    ser.write(b'O')
                                    led_is_on = True
                            else:
                                if led_is_on:
                                    print("No Motion. LED OFF")
                                    ser.write(b'F')
                                    led_is_on = False

                        prev_x = ax
                        prev_y = ay

                    except ValueError:
                        pass

    except KeyboardInterrupt:
        print("\nProgram terminated.")

    finally:
        ser.write(b'F')
        ser.close()
        print("Serial closed.")


if __name__ == "__main__":
    main()