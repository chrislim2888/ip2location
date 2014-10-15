===========================
IP2Location 7.0.0
===========================

IP2Location is a tool allowing user to get IP address information such as country, region, city, 
latitude, longitude, zip code, time zone, ISP, domain name, connection type, area code, weather, 
mobile network, elevation, usage type by IP address (IPv4 or IPv6) from IP2Location BIN database. 

For more details, please visit:  
http://www.ip2location.com/free/applications  
  
  
Installation
------------
[ Unix/Linux ]  
./autogen.sh  
./configure  
make  
make install  
  
  
Usage
-----
Query an IP address and display the result  
ip2location --datafile [IP2LOCATION BIN DATA PATH] --ip [IP ADDRESS]  
  
Query all IP addresses from an input file and display the result  
ip2location --datafile [IP2LOCATION BIN DATA PATH] --inputfile [INPUT FILE PATH]  
  
Query all IP addresses from an input file and display the result in XML format  
ip2location --datafile [IP2LOCATION BIN DATA PATH] --inputfile [INPUT FILE PATH] --format XML  
  
  
Download More Sample Databases  
------------------------------  
wget http://www.ip2location.com/downloads/sample.bin.db24.zip  
unzip samples-db24.zip  
  
  
Support 
------- 
Email: support@ip2location.com  
URL: http://www.ip2location.com  
