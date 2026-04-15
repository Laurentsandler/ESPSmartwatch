# ESPSmartwatch

A small modern smartwatch style device built on ESP32.

## Description


This compact device is a custom built smartwatch featuring an ESP32 microcontroller.

The device runs a custom firmware which is inluded in the repo under the firmware folder.

## Motivation

My school recently banned smartwatches, so I decided to make my own, that is "dumb" enough that it would be allowed.


## Hardware Design

### Initial Sketch


<img width="348" height="272" alt="image" src="https://github.com/user-attachments/assets/6fb3882b-43ee-46c2-be0d-6d05694fd80b" />
<img width="348" height="272" alt="image" src="https://github.com/user-attachments/assets/26f6f140-6870-481b-9fe4-ea4edc0ab84a" />




### Case Design

The case was designed in onshape, it fits a small battery and the esp32 module itself

Onshape project link: https://cad.onshape.com/documents/3428df8c7d261986fb7cca95/w/0c7e97bd99419912bdb89cec/e/683d611628950277cc38041f?renderMode=0&uiState=69df789f65c4cbf28e853c96

### Case:
<img width="688" height="694" alt="image" src="https://github.com/user-attachments/assets/8e5b0126-7a99-4084-a6f6-6251948ee715" />


### Case Render:

<img width="666" height="567" alt="image" src="https://github.com/user-attachments/assets/1e0886e8-38e0-46ca-909e-7c64373bd852" />





**Assembly Requirements:**
Case is a snap on fit, with holes for M2 bolts for secure it.
The watch band fits in, and secures in place with a retractable pin.

### 3D Models

The CAD folder contains:
- `Case.stl` - Main case body
- `Lid.stl` - Button component (if applicable)
With step files as well.





## Wiring Diagram


Esp32 module battery pins <--- battery




## Pins

<img width="960" height="629" alt="image" src="https://github.com/user-attachments/assets/008cb1f8-f401-4c25-abe7-dc19f6b483f1" />




## Software Dependencies

Install these libraries via Arduino IDE Library Manager or PlatformIO:

- #include <WiFi.h> 
- #include <time.h>
- #include <HTTPClient.h>
- #include <ArduinoJson.h>
- #include <TFT_eSPI.h>
- #include <lvgl.h>
- #include "ui.h"



## Configuration

Before uploading, edit the following in `Firmware/main.cpp`:

```cpp
// --- Configuration ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// NTP Server Settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;           
const int   daylightOffset_sec = 3600;   

// Go to https://open-meteo.com/ to find coordinates for your exact city
const String latitude = "51.5085";  
const String longitude = "-0.1257"; 
```



## Bill of Materials (BOM)

| Component | Quantity | Price | Link |
|-----------|----------|-------|------|
| [ESP32-S3 LCD Development Board] | 1 | ~$[17] | [[Link](https://fr.aliexpress.com/item/1005009281283730.html?spm=a2g0o.productlist.main.9.35842wgl2wglVw&algo_pvid=b4c70c90-0498-4b42-aeff-ed7fcbff31ab&algo_exp_id=b4c70c90-0498-4b42-aeff-ed7fcbff31ab-8&pdp_ext_f=%7B%22order%22%3A%2249%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%2135.34%2114.49%21%21%21275.45%21112.94%21%40211b6a7a17759113307988777e225f%2112000048597466064%21sea%21FR%217032395364%21X%211%210%21n_tag%3A-29919%3Bd%3Adbb9d545%3Bm03_new_user%3A-29895&curPageLogUid=SW4mfDLCXmv9&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005009281283730%7C_p_origin_prod%3A)] |
| [BATTERY] | 1 | ~$[6] | [[Link](https://fr.aliexpress.com/item/1005006584143607.html?mp=1&pdp_npi=6@dis!EUR!EUR%207.52!EUR%204.59!!EUR%204.59!!!@211b80e117762550188798675e117a!12000037729349745!ct!FR!7032395364!!1!0!&pdp_ext_f=%7B%22cart2PdpParams%22:%7B%22pdpBusinessMode%22:%22retail%22%7D%7D&gatewayAdapt=glo2fra)] |
| [Fabric Nylon Watch Band] | [1] | ~$[9.17] | [[Link](https://amzn.eu/d/0azpKPLg)] |






<!-- TODO: Add links to all relevant datasheets and documentation -->
