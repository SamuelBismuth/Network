import socket
import random
from threading import Thread
from errno import ECONNREFUSED

# The ip adress of the target.
hostName = '127.0.0.1'

# This function send a socket to a specific port, and try to connect.
# If the connection is a success, then print the port, because that's mean that the port is open, else do nothing. 
def sendSocket(port):
    try:
        socket.socket().connect((hostName, port))
        print("\nOPEN " + str(port))
        return port
    except socket.error as err:
        if err.errno == ECONNREFUSED:
            return False
        raise

# This method define a thread which call the function sendSocket with all the possible numbers of ports (from 1 to 65536).
def portScanning():
    listOfPorts = list(range(1, 65536))
    random.shuffle(listOfPorts)
    count = 0
    while(count < 65534):
        thread = Thread(target=sendSocket, args=(listOfPorts[count],))
        count += 1
        thread.start()
     
portScanning()
