# M5_LoRa_Geotag
Geotagging LoRa reception spots on the M5Stack

Starting a new project that's probably going to be prominently in the "Chronophagos" category.

0. Test whether we have LoRa reception. If not, bugger off.
1. Acquire GPS location.
2. Acquire a static map from Google Maps and display on-screen with a marker for the location, numbered.
3. Save coords to SD card (and/or online).
4. Go home and display a Google map with all spots.

For now, I have solved point #2. M5_Get_Static_Map.ino does just that.
