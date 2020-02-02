# Ambient Arduino Library for Sony Spresense LTE-M 
This library is an Arduino library for Spresensen LTE-M board to connect to the Ambient service.

![Sony Spresense LTE-M](https://github.com/TE-YoshinoriOota/Ambient_SpresenseLTEM/blob/master/resources/sDSC01719.jpg)

## What is Sony Spresense LTE-M?
Sony Spresense is a single board computer integrated with a positioning functionality that has GPS, GLONASS and QZSS (みちびき) that enables to get a precise position. Sony released Spresense LTE-M extension board having an LTE-M module that is made by Altair of a Sony group company. LTE-M is a super low power communication method (LPWA: Low Power Wide Area) using LTE licensed band. It would be suitable for IoT PoC driven by batteries.  For more information, please refer to the [Sony support site](https://developer.sony.com/develop/spresense/docs/hw_docs_lte_ja.html).

![Spresense main and LTE-M](https://github.com/TE-YoshinoriOota/Ambient_SpresenseLTEM/blob/master/resources/SpresenseMain_LTE-MExtensionBoard.jpg)

## What is Ambient?
["Ambient"](https://ambidata.io/) is an IoT cloud service in Japan to make IoT data to be visualized. And it is able to put the data on maps easily when you add the position information. Ambient also has a powerful graphing function that is very easy to see your collected data. Using the service is also very easy. Sign in your account with your email, then you can use the basic service free of charge. 

![Ambient Graph function](https://ambidata.io/wp/wp-content/uploads/2016/08/chart2-1024x712.jpg)


## Example 
The example sketches in this library will give you a hint to make an application of collecting and sending your position using Sony Spresense. This library supports only the sending (write) function.

![Sony Spresense with Ambient](https://github.com/TE-YoshinoriOota/Ambient_SpresenseLTEM/blob/master/resources/ambient.png)

This library defines its instance named "theAmbient". It means you can use it directly when  "Ambient_SprensenseLTEM.h" is included. The below is the description of the outline of the library.

* __theAmbient.begin("apn_name", "apn_usrname", "apn_passwd")__ <br/> 
__apn_name:__ _an APN name given by operators of your SIM card_ <br/>
__apn_usrname:__ _an APN user name given by operators of your SIM card_ <br/>
__apn_passwd:__ _an APN password given by operators of your SIM card_ <br/>

* __theAmbient.setupChannel(channel_id, "writeKey")__ <br/>
__channel_id:__ _a channelId given by Ambient service_ <br/>
__writeKey:__ _a write key given by Ambient service_ <br/>

* __theAmbient.set(field, "data")__ <br/>
__field:__ _specifing a field number to put your data (The field numbers of "9" and "10" are reserved for latitudes and longitudes)_ <br/>
__data:__ _data to put on Ambient_ <br/>

* __theAmbient.send()__ <br/>
_Send data set by theAmbient.set() the Ambient service_ <br/>

* __theAmbinet.clear(field)__ <br/>
_Clear data of a specified field on the channel_ <br/>

* __theAmbinet.clearAll()__ <br/>
_Clear all data on the channel_ <br/>

* __theAmbient.end()__ <br/>
_Close the channel_ <br/>



