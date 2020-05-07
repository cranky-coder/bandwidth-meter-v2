General: 
This is version 2.   The code for version one was lost during the great data apocalypse (see my blog for that disaster).   This contains 2 pieces.  
Some python code and some arduino code.   The python code queries my router on a regular basis via snmp and gets my up/down bandwidth usage.
It the passes the current bandwidth usage to mqtt nodes /bandwidth/up and /bandwidth/down.

The second piece (the arduino code) connects via the network and subscribes to those same nodes.  Updating the analog meters every time new data is received.

Setup:

I have configured the python code to run on a linux box (may work on windows).  I run the script in rc.local to run in background.  
python /scripts/bandwidth.py &



Requirements:
You do need to install Snimpy (pip install snimpy)
MQTT server 


# bandwidth-meter-docker
