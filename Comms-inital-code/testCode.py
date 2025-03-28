import serial
import time

class LaptopCommunication:
    def __init__(self, port, baud_rate=115200):
        self.port = port
        self.baud_rate = baud_rate
        self.serial_connection = serial.Serial(port, baud_rate, timeout=2)
        self.lastSentID = 0 

    def calculate_checksum(self, message):
        checksum = 0
        for char in message:
            checksum ^= ord(char)
        return checksum

    def send_message(self, data):
        #format <ID:DATA:CHECKSUM>.
        self.lastSentID += 1
        message_body = f"{self.lastSentID}:{data}"
        checksum = self.calculate_checksum(message_body)
        message = f"<{message_body}:{checksum}>\n"
        self.serial_connection.write(message.encode())
        print(f"sent: {message.strip()}")

    def handle_missing_ids(self, requested_id):
        #print(f"handling missing IDs starting from:{requested_id}")
        for id in range(requested_id, self.lastSentID + 1):
            data = id * 10 #regenerate data for the missingID
            message_body = f"{id}:{data}"
            checksum = self.calculate_checksum(message_body)
            message = f"<{message_body}:{checksum}>\n"
            self.serial_connection.write(message.encode())
            print(f"Resent: {message.strip()}")

    #receive and parse response or missing ID request fromESP
    def receive_response(self):
        if self.serial_connection.in_waiting > 0:
            response = self.serial_connection.readline().decode().strip()
            print(f"received: {response}")
            #handle missing ID requests
            if response.startswith("<REQ:"):
                requested_id = int(response[5:-1])#extract the requested ID
                self.handle_missing_ids(requested_id)
            return response
        return None

    def close(self):
        self.serial_connection.close()
        print("serial connection closed.")


if __name__ == "__main__":
    port_name = "COM4"  
    laptop = LaptopCommunication(port_name)
    try:
        time.sleep(3)#Modify later
        while laptop.serial_connection.in_waiting:
            laptop.serial_connection.readline()
        laptop.send_message(10) 
        laptop.send_message(20)  
        #skip ID3
        laptop.lastSentID += 1  
        laptop.send_message(40)
        #modify later 
        time.sleep(1)  
        while True:
            response = laptop.receive_response()
            if response is None:
                break
    finally:
        laptop.close()
