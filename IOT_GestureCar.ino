  #include <ESP8266WiFi.h>
const char* ssid = "SamsungA14"; // wifi router name
const char* password = "Amith@1212  "; // Wifi router password

WiFiServer server(80); // create server object at port 80 ,declare server object and specify port, 
                       // 80 is the standard port number used for web access.

int x=0,y=0,z=0;
#define LMforward D0
#define LMreverse D1
#define RMforward D2
#define RMreverse D3

void setup() 
{
  Serial.begin(115200);
  pinMode(LMforward, OUTPUT);
  pinMode(LMreverse, OUTPUT);
  pinMode(RMforward, OUTPUT);
  pinMode(RMreverse, OUTPUT);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // begin wifi class
  while (WiFi.status() != WL_CONNECTED) // print '.' until WiFi is connected
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  server.begin(); // server started (start listening for clients) at port 80
  Serial.println("Server started"); 
  Serial.println(WiFi.localIP()); // print IP address alloted by Wifi router to the NodeMCU

}

void loop() 
{
  
  // new client object is created
  WiFiClient client = server.available(); // web browser act as a client// now server will start to lestening on port 80 

  if (!client)  // if no client, then it returns
    return;

 // Serial.println("new client");
  while(!client.available())
    delay(1);
  
  String rxd_str = client.readStringUntil('\n'); //read a string from the client (web browser),Read the first line of the request
  Serial.print("\nString received from client(web browser) : \n\n\t");
  Serial.println(rxd_str);
  client.flush();

  if(rxd_str.indexOf("/ACCELEROMETER:") != -1)
  {
      // message format :: http://192.168.1.8/ACCELEROMETER:X-1,Y-0,Z-185/end
      x = ( rxd_str.substring(rxd_str.indexOf(":X-")+3,rxd_str.indexOf(",Y-"))).toInt();
      y = ( rxd_str.substring(rxd_str.indexOf(",Y-")+3,rxd_str.indexOf(",Z-"))).toInt();
      z = ( rxd_str.substring(rxd_str.indexOf(",Z-")+3,rxd_str.indexOf("/end"))).toInt();
      printf("X - %d\tY - %d\tZ - %d\n",x,y,z);
      

   if ( (x >= -1 && x <= 1) && (y >4))  //forward
  {
    digitalWrite(LMforward, HIGH);
    digitalWrite(LMreverse, LOW);
    digitalWrite(RMforward, HIGH);
    digitalWrite(RMreverse, LOW);
    Serial.println("Forward");
  }
  else if (x >= -1 && x <= 1 && y <-2) //reverse
  {
    digitalWrite(LMforward, LOW);
    digitalWrite(LMreverse, HIGH);
    digitalWrite(RMforward, LOW);
    digitalWrite(RMreverse, HIGH);
    Serial.println("Back");
  }
  else if (x>5 && z <= 7)          //left
  {
    digitalWrite(LMforward, LOW);
    digitalWrite(LMreverse, HIGH);
    digitalWrite(RMforward, HIGH);
    digitalWrite(RMreverse, LOW);
    Serial.println("Left");
  }
  else if (x < -5 && z <= 7)         //right
  {
    digitalWrite(LMforward, HIGH);
    digitalWrite(LMreverse, LOW);
    digitalWrite(RMforward, LOW);
    digitalWrite(RMreverse, HIGH);
    Serial.println("Right");
  }
  else                                //stop
  {
    digitalWrite(LMforward, LOW);
    digitalWrite(LMreverse, LOW);
    digitalWrite(RMforward, LOW);
    digitalWrite(RMreverse, LOW);
    Serial.println("STOP");
  }
   
  } 

  client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  client.flush();
}
