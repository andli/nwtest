/*
  The circuit:
   LCD RS pin to digital pin 7
   LCD Enable pin to digital pin 6
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

   SCK - Pin 13
   SO  - Pin 12
   SI  - Pin 11
   CS  - Pin 10 # Selectable with the ether.begin() function
*/

#include <LiquidCrystal.h>
#include <enc28j60.h>
#include <EtherCard.h>
#include <net.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };
byte Ethernet::buffer[700];
const char website[] PROGMEM = "google.com";
bool connected = false;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("== nwscout v1 ==");
  lcd.setCursor(0, 1);
  lcd.print("=== by andli ===");
  delay(3000);
  lcd.clear();
  
  lcd.print("Init Ethernet...");
  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0) {
    lcd.setCursor(0, 1);
    lcd.print("HW failure!");
  }
  else {
    lcd.clear();
    lcd.print("DHCP init...");
    if (!ether.dhcpSetup()) {
      lcd.setCursor(0, 1);
      lcd.print("DHCP failed.");
    }
    else {
      connected = true;
    }
  }

}

void loop() {
  if (connected) {
    testNetwork();
  }
}

void printAddressToLcd(String label, byte address[]) {
  lcd.clear();
  lcd.print(label);
  
  char strAddress[20];
  sprintf(strAddress, "%d.%d.%d.%d", address[0], address[1], address[2], address[3]);

  lcd.setCursor(0, 1);
  lcd.print(strAddress);
  delay(3000);
}

void testNetwork() {
  printAddressToLcd("IP:", ether.myip);
  printAddressToLcd("Netmask:", ether.netmask);
  printAddressToLcd("GW IP:", ether.gwip);
  printAddressToLcd("DNS IP:", ether.dnsip);

  lcd.clear();
  lcd.print("DNS Lookup...");
  lcd.setCursor(0, 1);
  lcd.print("=> google.com");
  delay(2000);

  //ether.staticSetup(ether.myip, ether.gwip);
  if(!ether.dnsLookup(website))
  {
      lcd.clear();
      lcd.print("No internet");
      delay(3000);
  }
  else {
    printAddressToLcd("Lookup result: ", ether.hisip);
  }
}
