import serial
import csv
import time

arduino_port = "COM5" # NOTE: GET PORT BEFORE RUNNING
baud = 9600

ser = serial.Serial(arduino_port, baud)

# will run for 1 hour if left alone
for i in range(60):
    data = [ser.readline().decode('utf-8')[0:-2]]
    print(data)

    with open("co2_data.csv", 'a', encoding='UTF8', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(data)
    
    time.sleep(60)