#include "access-point.h"

// Set web server port number to 80
WiFiServer server(80);

void sendBadRequestResponse(WiFiClient client) {
  // HTTP requests always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 400 Bad Request");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");            
  client.println("<body><h1>Bad request. Expecting query string of type: /?ssid=mySSID&password=mySSIDPassword&access_token=sensorAccessToken</h1>");
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line
  client.println();
}

void sendSuccessResponse(WiFiClient client) {
  // HTTP requests always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");            
  client.println("<body><h1>Successfully saved config</h1>");
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line
  client.println();
}

bool setupAP() {
  boolean result = WiFi.softAP(CFG_ONBOARD_WIFI_SSID, CFG_ONBOARD_WIFI_PASSWORD);
  if(!result) {
    ardprintf("AP: Soft-AP setup failed");
    return false;
  }
  
  IPAddress IP = WiFi.softAPIP();
  ardprintf("AP: IP address: %s", IP.toString().c_str());

  server.begin();

  return true;
}

void listenForConfig() {
  bool credentialsSaved = false;
  // Variable to store the HTTP request
  char request[700] = "";
  char currentLine[200] = "";                // make a String to hold incoming data from the client

  while(!credentialsSaved) {
    WiFiClient client = server.available();   // Listen for incoming clients

    if (!client) continue;
    
    strcpy(request, "");
    strcpy(currentLine, "");
    // If we get to here, a new client connected
    ardprintf("AP: New client connection");          
    while (client.connected()) { 
      // if there's no bytes to read from the client 
      if (!client.available()) continue;        

      char c = client.read();          
      append(request, c);

      if (c == '\n') { // if the byte is a newline character
        // if the current line has zero characters, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (strlen(currentLine) == 0) {
          if (
            strstr(request, "GET /?") == NULL
            || strstr(request, "ssid") == NULL
            || strstr(request, "password") == NULL
            || strstr(request, "access_token") == NULL
          ) {
            sendBadRequestResponse(client);
            break;
          }

          // request in the form of /?ssid=mySSID&password=mySSIDPassword&access_token=sensorAccessToken
          strtok(request, "?"); 
          char *ssidName = strtok(NULL, "="), *ssidVal = strtok(NULL, "&");
          char *passwordName = strtok(NULL, "="), *passwordVal = strtok(NULL, "&");
          char *accessTokenName = strtok(NULL, "="), *accessTokenVal = strtok(NULL, " ");

          if (strcmp("ssid", ssidName) != 0 || strcmp("password", passwordName) != 0 || strcmp("access_token", accessTokenName) != 0) {
            sendBadRequestResponse(client);
            break;
          }

          credentialsSaved = saveConfig(ssidVal, passwordVal, accessTokenVal);
          sendSuccessResponse(client);
          break;
        }
        
        // if you got a newline, then clear currentLine
        strcpy(currentLine, "");
      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        // add it to the end of the currentLine
        append(currentLine, c);
      }
    }
    // Clear the request variable
    strcpy(request, "");
    // Close the connection
    client.stop();
    ardprintf("AP: Client disconnected.");
  }
}

bool cleanupAP() {
  server.stop();
  return true;
}
