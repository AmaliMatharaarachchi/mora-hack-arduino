String ssid = "CharithBB";
String password = "GhostWarrior";
String data;
String server = "192.168.1.3";
String uri = "/saggitarius/phpfiles/add.php";
String user = "X";
int tempPin = 1;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial.begin(115200);
  reset();
  connectWifi();
}

void reset(){
  Serial1.println("AT+RST");
  delay(1000);
  if(Serial1.find("OK")){
    Serial.print("Module reset");
  }
}

void connectWifi() {
  String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
  Serial1.println(cmd);
  delay(4000);
  if(Serial1.find("OK")) {
    Serial.println("Connected!");
  }else {
    connectWifi();
    Serial.println("Cannot connect to wifi"); 
  }
}

void loop() {
  int val = analogRead(tempPin);
  float cel = (val/1024.0)*5000;
  String temp = String(cel/10);
  data = "temp=" + temp + "&id="+user;// data sent must be under this form //name1=value1&name2=value2.
  //Serial.println(data);
  httppost();
  delay(1000); 
}

void httppost () {
  Serial1.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");
  if( Serial1.find("OK")) {
    Serial.println("TCP connection ready");
  }
  delay(1000);
  
  String postRequest =
  "POST " + uri + " HTTP/1.0\r\n" +
  "Host: " + server + "\r\n" +
  "Accept: *" + "/" + "*\r\n" +
  "Content-Length: " + data.length() + "\r\n" +
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  "\r\n" + data;

  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  Serial1.print(sendCmd);
  Serial1.println(postRequest.length() );
  delay(500);
  if(Serial1.find(">")) {
    Serial.println("Sending..");
    Serial1.print(postRequest);
  }
  if( Serial1.find("SEND OK")) {
    Serial.println("Packet sent");
  }
  while (Serial1.available()) {
    String tmpResp = Serial1.readString();
    Serial.println(tmpResp);
  }
  Serial1.println("AT+CIPCLOSE");
}
