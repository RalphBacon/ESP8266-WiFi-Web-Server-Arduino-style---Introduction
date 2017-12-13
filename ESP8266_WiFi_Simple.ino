#include <ESP8266WiFi.h>
const char* ssid = "YOUR WIFI SSID GOES HERE";
const char* password = "YOUR WIFI PASSWORD GOES HERE";

WiFiServer server(80);

void setup() {
  // initialize BUILTIN_LED as an output.
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  Serial.begin(9600);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    //digitalWrite(ledPin, HIGH);
    //digitalWrite(BUILTIN_LED, HIGH);
    value = HIGH;
    digitalWrite(BUILTIN_LED, !value);
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    //digitalWrite(ledPin, LOW);
    //digitalWrite(BUILTIN_LED, HIGH);
    value = LOW;
    digitalWrite(BUILTIN_LED, !value);
  }

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);


  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 WiFi Testing</title>");
  client.println("<style type=text/css>");
  client.println(".status {font-weight:bold;color:red;}");
  client.println("</style>");
  client.println("</head>");
  client.print("<div class=status>Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("</div><br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
