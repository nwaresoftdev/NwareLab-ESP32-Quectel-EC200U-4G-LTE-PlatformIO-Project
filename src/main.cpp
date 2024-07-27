#include <Arduino.h>


// Initialize the software serial port, connect the EC200U module's TX to pin 10 and RX to pin 11 of the Arduino
HardwareSerial ec200u(1); //use UART1 (pins GPIO16 and GPIO17)



void sendATCommand(String command, String response, int timeout) {
  String receivedData;
  ec200u.println(command); // Send the AT command
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (ec200u.available()) {
      char c = ec200u.read(); // Read the response from the module
      receivedData += c;
    }
  }
  if (receivedData.indexOf(response) != -1) {
    Serial.println("Received: " + receivedData); // If the response is as expected
  } else {
    Serial.println("Error");
  }
}

void sendSMS(String number, String text) {
  sendATCommand("AT+CMGS=\"" + number + "\"\r\n", ">", 2000); // Command to send SMS
  ec200u.print(text); // Text of the message
  delay(100);
  ec200u.write(26); // CTRL+Z to send the message
}

void makeCall(String number) {
  sendATCommand("ATD" + number + ";\r\n", "OK", 20000); // Command to dial a number
}


void setup() {
  // Open the serial communications and wait for the port to open
  Serial.begin(115200);



  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Set the data rate for the SoftwareSerial port
  //ec200u.begin(115200);
  ec200u.begin(115200, SERIAL_8N1, 16, 17); //RX, TX
  delay(1000);

  // Set module to SMS text mode
  sendATCommand("AT+CMGF=1\r\n", "OK", 2000);
}

void loop() {
  if (ec200u.available()) {
    Serial.write(ec200u.read());
  }

  if (Serial.available()) {
    char command = Serial.read();  // Read the character sent from the Serial Monitor

    if (command == 's') {
      // Sending an SMS to your number with a custom message
      sendSMS("+918860836382", "Hello from Arduino and Quectel EC200U!");
    } else if (command == 'c') {
      // Making a call to your number
      makeCall("+918472864382");
    } else if (command == 'r') {
      // Enable receiving SMS in text mode
      sendATCommand("AT+CNMI=2,2,0,0,0\r\n", "OK", 2000);
    }
  }
}

