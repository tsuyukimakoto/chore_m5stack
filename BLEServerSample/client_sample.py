import time
import uuid

import Adafruit_BluefruitLE


# UUID1
SERVICE_UUID = uuid.UUID('3f7b3ec2-0e6d-11ea-82fc-f40f242254e3')
CHAR_UUID = uuid.UUID('4067918c-0e6d-11ea-82fc-f40f242254e3')


# UUID4
# SERVICE_UUID = uuid.UUID('20759c32-8c07-494f-bc01-bc802e843dac')
# CHAR_UUID = uuid.UUID('31e87af5-3d86-4371-9fd1-2bb4fa956743')


ble = Adafruit_BluefruitLE.get_provider()


def main():
    ble.clear_cached_data()

    adapter = ble.get_default_adapter()
    adapter.power_on()
    print('Using adapter: {0}'.format(adapter.name))

    print('Disconnecting any connected devices...')
    ble.disconnect_devices([SERVICE_UUID])

    try:
        adapter.start_scan()

        device = ble.find_device(name="M5Stack")
        if device is None:
            raise RuntimeError('Failed to find device!')
    finally:
        adapter.stop_scan()

    print('Connecting to device...')
    device.connect()
    print('Connected.')

    try:
        print('Discovering services...')
        device.discover([SERVICE_UUID], [CHAR_UUID])

        # Find the service and its characteristics.
        uart = device.find_service(SERVICE_UUID)
        characteristic = uart.find_characteristic(CHAR_UUID)

        v = characteristic.read_value()
        print(v)

        # Write a string to the characteristic.
        print('Sending message to device...')
        characteristic.write_value(b'Message from python client!')

        def received(data):
            print('Received: {0}'.format(data))

        # Turn on notification of characteristics using the callback above.
        print('Subscribing to characteristic changes...')
        characteristic.start_notify(received)

        # Wait for 10 seconds to receive data.
        print('Waiting 10 seconds to receive data from the device...')
        time.sleep(10)
    finally:
        device.disconnect()


if __name__ == '__main__':
    ble.initialize()
    ble.run_mainloop_with(main)
