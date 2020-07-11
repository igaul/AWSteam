IoT design for Portland park population and cell phone usage

For the IoT portion of our design flow I used a an Arduino script on the ESP8266 which allows the device to communicate with AWS IoT Core over MQTT on Port 8883.  This is a newer feature is now available with the updated core from Espressf as of August 2019.  The IoT JSON data package is issued every cycle for each monitored park and reports th current population, cell phone signal usage.  As a prototyping platefore th ESP8266 offers only raw functionality and sensor integration.  A production level design is likely to utilize a Amazon Deep Lens Camera which allows both more programibility and built in functionality.  

The incoming IoT Payload comes in as JSON in this format:

{
  "Object_Id": 298,
  "Density": 2
}
Object Id is the Portland ID code for each Park in city limits
Density is the population / Park Area


From here we dispatch it via two  methods for repository(data lake)  and database storage.

Dynamo Database:

1.  It is sent directly from AWS IoT Core to a DynamoDB table to be held for historic query's and scans.

S3 Data Lake

2.  Data from IoT Core is sent to lambda then places in a premade S3 bucket and folder. Each payload is designated by an Epoch timestamp and held as a data lake for data ingestion.



