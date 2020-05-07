#docker run .. --env snmphost=<IP> --env snmpcommunity=<snmp community> --env mqtthost=<IP> --env mqttport=<port if diff than 1883>  

FROM python:3.8-slim-buster 
ADD python-code/bandwidth.py /
#RUN apt-get install .build-deps gcc libsmi-dev py3-cffi musl-dev libsmi libffi-dev 
RUN apt-get update && apt-get install gcc -y
RUN apt-get install libsmi-dev -y
RUN echo "deb http://ftp.us.debian.org/debian stretch main non-free" >> /etc/apt/sources.list
RUN apt-get update && apt-get install snmp-mibs-downloader -y
RUN pip install --upgrade pip
RUN pip install snimpy
RUN pip install paho-mqtt 
CMD [ "python", "./bandwidth.py" ]

