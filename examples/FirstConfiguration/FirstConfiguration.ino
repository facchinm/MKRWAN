/*
  First Configuration
  This sketch demonstrates the usage of MKR WAN 1300/1310 LoRa module.
  This example code is in the public domain.

  Modified by Massimo Sacchi, 9 May 2020

  */

LoRaModem modem;

int band       = -1;
int method     = -1;
String appEui  = "";
String appKey  = "";
String devAddr = "";
String nwkSKey = "";
String appSKey = "";

int eol;            // number of chars in the end of line configuration of the terminal

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("=======================================================================");
  Serial.println("                 MKRWAN1300/1310 first configuration ");
  Serial.println("=======================================================================");
  Serial.println("");
  Serial.print("-> Register to your favourite LoRa network and ");
  eol = chk_terminal();
        
  while ((band < 0) || (band > 9)) {
    Serial.println("");
    select_band();
  }
  Serial.print("   initializing on band ");
  Serial.println(band);  
  if (!modem.begin((_lora_band) band)) {
    Serial.println("   failed to start module");
    while (1) {}
  };
  Serial.println("");
  Serial.print("   module version: ");
  Serial.println(modem.version());
  Serial.print("   device EUI: ");
  Serial.println(modem.deviceEUI());

  while (method != 1 && method != 2) {
    Serial.println("-> Are you connecting via OTAA (1) or ABP (2)?");
    method = read_term().toInt();
  }
  Serial.print("   activation method ");
  Serial.println(method);
  Serial.println("");

  if (method == 1) {
    if(appEui.equals("")){
      Serial.print("-> Enter your APP EUI: ");
      appEui = read_term();
      appEui.trim();
    } else {
      Serial.print("   APP EUI: ");
    }
    Serial.println(appEui);
    Serial.println("");

    if (appKey .equals("")) {
      Serial.print("-> Enter your APP KEY: ");
      appKey = read_term();
      appKey.trim();
    } else {
      Serial.print("   APP KEY: ");
    }
    Serial.println(appKey);
    Serial.println("");

  } else if (method == 2) {
    if (devAddr.equals("")) {
      Serial.print("-> Enter your Device Address: ");
      devAddr = read_term();
      devAddr.trim();
    } else {
      Serial.print("   device Address: ");
    }
    Serial.println(devAddr);
    Serial.println("");
    
    if (nwkSKey.equals("")) {
      Serial.print("-> Enter your NWS KEY: ");
      nwkSKey = read_term();
      nwkSKey.trim();
    } else {
      Serial.print("   NWS KEY: ");
    }
    Serial.println(nwkSKey);
    Serial.println("");
    
    if (appSKey.equals("")) {
      Serial.print("-> Enter your APP SKEY: ");
      appSKey = read_term();
      appSKey.trim();
    } else {
      Serial.print("   APP SKEY: ");
    }
    Serial.println(appSKey);
  }
  
  Serial.println("");
  Serial.print(" - Attempting connection.");
  int connected = 0;
  while (!connected) {
    Serial.print(".");
    if (method == 1){
      connected = modem.joinOTAA(appEui, appKey);
    } else if (method == 2){
      connected = modem.joinABP(devAddr, nwkSKey, appSKey);
    }
  }
  Serial.println("connected");
  Serial.println("");
  Serial.print(" - Sending data... ");
  
  delay(5000);
  int err;
  modem.setPort(3);
  modem.beginPacket();
  modem.print("HeLoRA world!");
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("ok");
  } else {
    Serial.println("error");
  }
}

void loop() {
  while (modem.available()) {
    Serial.write(modem.read());
  }
  modem.poll();
}

void select_band(){
  String s;
  
  Serial.println("-> Enter your regional band or ? for help");
  s = read_term();
   
  if (s.equals("?")){ 
      Serial.println("   Select one:");
      Serial.println("\tAS923 (0)");
      Serial.println("\tAU915 (1)");
      Serial.println("\tCN470 (2)");
      Serial.println("\tCN779 (3)");
      Serial.println("\tEU433 (4)");
      Serial.println("\tEU868 (5)");
      Serial.println("\tKR920 (6)");
      Serial.println("\tIN865 (7)");
      Serial.println("\tUS915 (8)");
      Serial.println("\tRU864 (9)");
  } else if (isDigit(s.charAt(0))){
    band = s.toInt();
  }
}

int chk_terminal(){
  Serial.println("press ENTER to continue");
  while (!Serial.available());
  return (Serial.readString().length());
}

String read_term(){
  String str;

  while (!Serial.available());
  str = Serial.readString();
  str = str.substring(0, str.length() - eol);
  return str;
}
