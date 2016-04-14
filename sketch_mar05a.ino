int led = 12;
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, HIGH);
  counter += 1;
  Serial.println(Serial.readString());
  Serial.print(counter);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}
