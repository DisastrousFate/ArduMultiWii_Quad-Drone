import tkinter as tk
from tkinter import ttk
import serial as serial
from serial.tools import list_ports
import threading as threading
import time

class Controller:
    def __init__(self, port, baud_rate=9600):
        self.port = port
        self.baud_rate = baud_rate
        self.connected = False

        self.mesgDictionary = {
            "Check Connection" : 1,
            "Motor Calibration" : 2,
            "Stop Motors" : 3
        }
        
    def view_config(self, *args):
        print(self.port)
        print(self.baud_rate)

    def connect(self, *args):
        if self.port and self.baud_rate:
            try:
                self.serial_connection = serial.Serial(port=self.port, baudrate=self.baud_rate, timeout=.1)
                self.connected = True
                print(f"Connected to Arduino on port {self.port} with baud rate {self.baud_rate}")
                
                
                self.readThread = threading.Thread(target=self.read)
                self.readThread.start()
                time.sleep(2)
                self.send("Check Connection")
            
            except Exception as e:
                print(f"Error connecting to Arduino: {e}")
                self.connected = False
    
    def disconnect(self, *args):
        if self.connected == True:
            self.serial_connection.close()
            self.connected = False
            self.readThread.join()
            print(f"Disconnected from Arduino on port {self.port}")
        else:
            print("Already Disconnected")
    
    def send(self, mesg, *args):
        translatedMsg = self.mesgDictionary[mesg]

        if self.connected == True:
            try:
                self.serial_connection.write(bytes(str(translatedMsg), 'utf-8'))
                print("Serial sent.")

            except Exception as e:
                print(f"Error sending serial to board: {e}")
    
    def read(self, *args):
        while self.connected == True:
            try:

                data = self.serial_connection.readline().decode().strip()

                if data:
                    print(data)
            except Exception as e:
                print(f"Error reading serial: {e}")



class MainWindow(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)

        # Root
        self.parent = parent 
        self.parent.title("Drone Controller")
        self.parent.geometry("500x500")
        self.arduino = None

        lbl_Title = tk.Label(master=self, text="Drone Controller", bg="black", fg="white", font=("Helvetica bold", 20))
        lbl_Title.grid(row=0, column=0)

        # Serial Port
        lbl_Serialport = tk.Label(master=self, text="Enter Serial Port of Controller: ")
        lbl_Serialport.grid(row=1, column=0)

        self.dropdown_Serialport = ttk.Combobox(self, textvariable = lbl_Serialport)
        self.dropdown_Serialport.grid(row=1, column=1)
        self.dropdown_Serialport.bind("<Button-1>", self.update_serial_ports)
        self.dropdown_Serialport.bind("<<ComboboxSelected>>", self.on_serialdropdown_change)

        # Initialize the serial ports dropdown
        self.update_serial_ports()

        # Baud Rate
        lbl_Baudrate = tk.Label(master=self, text="Enter Baud Rate of Controller: ")
        lbl_Baudrate.grid(row=2, column =0)

        self.dropdown_Baudrate = ttk.Combobox(self, textvariable=lbl_Baudrate, 
            values=["10","300","600","1200","2400","4800","9600","14400","19200","38400","57600","115200","128000", "256000"])

        self.dropdown_Baudrate.grid(row=2, column=1)
        self.dropdown_Baudrate.bind("<<ComboboxSelected>>", self.on_baudrate_change)

        self.btn_connect = tk.Button(master=self, text="Connect to board", bg="green", fg="white")
        self.btn_connect.isBinded = False # Debounce for binding.
        self.btn_connect.grid(row=3, column=0)

        self.btn_disconnect = tk.Button(master=self, text="Disconnect from board", bg="red", fg="black")
        self.btn_disconnect.grid(row=3, column=1)

        self.btn_motorCalibration = tk.Button(master=self, text="Motor Calibration", bg="grey", fg="black")
        self.btn_motorCalibration.grid(row=4, column=0)
        self.btn_motorCalibration.bind("<Button-1>", self.on_motorCalibration)

        self.btn_stopMotors = tk.Button(master=self, text="Stop Motors", bg="red", fg="black")
        self.btn_stopMotors.grid(row=4, column=1)
        self.btn_stopMotors.bind("<Button-1>", self.on_stopMotors)

        # "Y" to check arduino config
        t1 = threading.Thread(target=self.checkSettings)
        t1.start()


    def update_serial_ports(self, *args):
        ports = [port.device for port in list_ports.comports()]
        self.dropdown_Serialport["values"] = ports
    
    def on_serialdropdown_change(self, *args):
        selected_port = self.dropdown_Serialport.get()
        
        if not self.arduino:
            self.arduino = Controller(selected_port)
            self.arduino.port = selected_port
        else:
            self.arduino.port = selected_port

        if self.btn_connect.isBinded == False:    
            self.btn_connect.bind("<Button-1>", self.arduino.connect)
            self.btn_disconnect.bind("<Button-1>", self.arduino.disconnect)
            self.btn_connect.isBinded = True

    def on_baudrate_change(self, *args):
        selected_baudrate = self.dropdown_Baudrate.get()
        if self.arduino:
            self.arduino.baud_rate = selected_baudrate

    def on_motorCalibration(self, *args):
        self.arduino.send("Motor Calibration")
    
    def on_stopMotors(self, *args):
        self.arduino.send("Stop Motors")

    def checkSettings(self, *args):
        while True:
            check = str.lower(input("Check Settings? \n"))
            if check == "y":
                self.arduino.view_config()
        
        
if __name__ == "__main__":
    root = tk.Tk()

    window = MainWindow(root)
    window.pack()

    window.mainloop()