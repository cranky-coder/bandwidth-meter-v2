from snimpy.manager import Manager as M
from snimpy.manager import load
import time
import paho.mqtt.client as mqtt
import os

snmphost = os.getenv('snmphost','127.0.0.1')
snmpcommunity = os.getenv('snmpcommunity')
mqtthost = os.getenv('mqtthost','127.0.0.1')
mqttport = os.getenv('mqttport')
print snmphost




while True:

 load("IF-MIB")
 m = M(host=snmphost, community=snmpcommunity,version = 1)
 ifspeed= m.ifSpeed[2]
 OutOctet1 = m.ifInOctets[2]
 InOctet1 = m.ifOutOctets[2]
 time.sleep(1)
 m = M(host=snmphost, community=snmpcommunity,version = 1)
 OutOctet2 = m.ifInOctets[2]
 InOctet2 = m.ifOutOctets[2]
 upload= float(float(OutOctet2) - float(OutOctet1))*8 
 download=float(float(InOctet2) - float(InOctet1))*8 
 mqttc = mqtt.Client("python_pub")
 print upload
 print download
 mqttc.connect(mqtthost,mqttport)
 mqttc.publish("bandwidth/up", upload)
 mqttc.publish("bandwidth/down",download)
 mqttc.loop(2) 
 time.sleep(1)



