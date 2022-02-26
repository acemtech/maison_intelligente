// declaration des differentes bibliothèque
#include
#include  
#include
#include  
#include

//************* Detection d'intrus*************/
const int trigPin= 46;
int screenOffMsg =0;

boolean activated = false; // State of the alarm
boolean isActivated;
boolean activateAlarm = true;
boolean alarmActivated = false;

//******* Comande vocal de l'appartement****//////////

const int chambre=42;
const int salon=43;
const int cuisine=44;
const int toilette=45;
// const int Rx=6;
// const int Tx=7;
int state=0;

// ventillateur et regulation de la temperature****//
const int tempPin = A0; // the output pin of LM35
const int fan = 11; // the pin where fan is
int temp;
int tempMin = 10; // the temperature to start the fan 0%
int tempMax = 40; // the maximum temperature when fan is at 100%
int fanSpeed;
int fanLCD;
LiquidCrystal_I2C Lcd = LiquidCrystal_I2C(0x27, 20, 4);

/************************************************/

// declatation de variables du portail
const byte PINLENGTH = 4;
char pinCode[PINLENGTH+1] = {'0','8','2','5'};
char keyBuffer[PINLENGTH+1] = {'-','-','-','-'};
char pin[] = "0825";
// char keyBuffer[] = "----";
const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
const int greenPin = 12;
const int redPin = 13;
const int servoPin = 24;
const int angleClosed = 30;
const int angleOpen = 120;
const int openDelay = 3000;
Servo myservo;
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
const int L1 = 41; // broche 2 du micro-contrôleur se nomme maintenant : L1nconst 
int BP = 40; // broche 3 du micro-contrôleur se nomme maintenant : BP


 //*********** definition des broches detection de flamme********
int Buzzer = 26 ;// LED
int capt_num = 25; // Interface Capteur Flamme
int capt_ana = A3; //  Interface Capteur Flamme
int val ;
float capteur_flamme; 
const int speakerPin = 10;

//**************** Allumage automatique ***************/////
const int photoresistance=A1;
const int ledPin=50;
const int pir=23;
const int D0=29;
 int seuil=200;
 int valpir; 
void setup() {
  //**************Detection d'intrus ********///
  pinMode(trigPin, INPUT); // Sets the trigPin as an Output
  //************** Commande vocal de l'appartement***///
  pinMode(chambre,OUTPUT);
  pinMode(salon,OUTPUT);
  pinMode(cuisine,OUTPUT);
  pinMode(toilette,OUTPUT);
  
//******************Allumage automatique********/
  pinMode(ledPin,OUTPUT);
  pinMode(photoresistance,INPUT);
  pinMode(pir,INPUT);
  digitalWrite(ledPin,LOW);
  delay(1000);
    /**************************************/
pinMode(fan, OUTPUT);
pinMode(tempPin, INPUT);
Lcd.init();
Lcd.backlight();
Lcd.setCursor(0,1);
Lcd.print("SMART HOUSE FST FES ");
delay(3000);
Lcd.begin(20,4);
delay(4000); // pause de 2 secondes

  // Initialisation et declarations des variable du portail
    Serial.begin(9600);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, LOW);
  myservo.attach(servoPin);
  myservo.write(angleClosed);
  //contenu de l'initialisation
  pinMode(L1, OUTPUT); //L1 est une broche de sortie
  pinMode(BP, INPUT); // BP est une broche d'entree

  //Initialisation et declaration d'allumage automatique*******************************
   pinMode(ledPin,OUTPUT);
  pinMode(photoresistance,INPUT);
  pinMode(pir,INPUT);
  digitalWrite(ledPin,LOW);
  delay(1000);
  // Initialisation des broches detection du flamme-*********
  
  pinMode(Buzzer, OUTPUT);// Définir l'interface de la LED en sortie
  pinMode(capt_num, INPUT);// Définir l'interface du capteur de flamme en entrée
  pinMode(capt_ana, INPUT);// Définir l'interface du capteur de flamme en entrée


Lcd.clear(); // // efface écran et met le curseur en haut à gauche
delay(10);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  maison_intelligente();

}
void maison_intelligente()
{
  allumage_auto();
  portail();
  ventilateur();
  detect_flamme();
  bouton_poussoir();  
  Commande_vocal();
  detect_intrus();
  
  }

  void Commande_vocal()
  {
    if(Serial.available()>0){
    state=Serial.read();
  }
  if(state=='1'){
    digitalWrite(cuisine,HIGH);
  }
   else if(state=='6'){
    digitalWrite(chambre,HIGH);
  }
  else if(state=='2'){
    digitalWrite(cuisine,LOW);
    
  }
  else if(state=='3'){
    digitalWrite(salon,HIGH);
  }
  else if(state=='4'){
    digitalWrite(salon,LOW);
  }
  else if(state=='5'){
    digitalWrite(chambre,LOW);
  }
 
  else if(state=='7'){
    digitalWrite(toilette,HIGH);
  }
  else if(state=='8'){
    digitalWrite(toilette,LOW);
  }
  else if(state=='9'){
    digitalWrite(cuisine,HIGH);
    digitalWrite(chambre,HIGH);
    digitalWrite(salon,HIGH);
    digitalWrite(toilette,HIGH);
    
  }
  else if(state=='*'){
    digitalWrite(cuisine,LOW);
    digitalWrite(salon,LOW);
    digitalWrite(chambre,LOW);
    digitalWrite(toilette,LOW);
  }
  }
  void allumage_auto()
  {
    int valeur = analogRead(photoresistance);
  valpir=digitalRead(pir);
  Serial.print("Luminosite=");
  Serial.println(valeur);
  if(valeur   {
    
             digitalWrite(ledPin,HIGH);
            Serial.println("LED allumée");
            }

      else{
        digitalWrite(ledPin,LOW);
            Serial.println("LED Eteint");
      }
  }
void detect_flamme()
{
  capteur_flamme = analogRead(capt_ana);
  
  Serial.println(capteur_flamme);  
   
  val = digitalRead (capt_num) ;
    if (capteur_flamme<58) // Quand le capteur détecte une flamme, la led s'allume
  {   
    //tone(Buzzer, 1000);
    Serial.println("Flamme"); 
    digitalWrite (Buzzer, HIGH); 
  }

  else
  {
    digitalWrite (Buzzer, LOW);
    //Serial.println("Pas de flamme");
  }
 // delay(2000);
  }

///////////////// les fonctions du portail////////////////

void addToKeyBuffer(char inkey) { 
  Serial.print(inkey); 
  Serial.print(" > ");
  
  for (int i=1; i     keyBuffer[i-1] = keyBuffer[i];
  }
  
  keyBuffer[PINLENGTH-1] = inkey;
  //Serial.println(keyBuffer);
  Lcd.setCursor(12,1);
  Lcd.print(keyBuffer);
}
 
void checkKey() {
  
  if (strcmp(keyBuffer, pinCode) == 0) {
    Serial.println("CORRECT");
    
    pinCorrect();
  }
  else {
    Serial.println("WRONG!");
    
    pinWrong();
  }
 
  
  for (int i=0; i < PINLENGTH; i++) {
    keyBuffer[i] = '-'; 
  }
}
 

void pinCorrect() {
  myservo.write(angleOpen);
  digitalWrite(greenPin, HIGH);
 // delay(openDelay);
  //myservo.write(angleClosed);
  //digitalWrite(greenPin, LOW);
  
}
 

void pinWrong() {
  for (int i=0; i<3; i++) {
    digitalWrite(redPin, HIGH);
    delay(50);
    digitalWrite(redPin, LOW);  
    delay(20);
  }
}
void portail()
{
   char customKey = customKeypad.getKey();
 
  if (customKey) {
    
    if ((int(customKey) >= 48) && (int(customKey) <= 57)){ 
      addToKeyBuffer(customKey); 
    }

    else if (customKey == '#') { 
      checkKey(); 
    } 
  }
} 

int readTemp() { // get the temperature and convert it to celsius
temp = analogRead(tempPin);
return temp * 0.48828125;
}

void ventilateur()
{ 
  temp = readTemp(); // get the temperature
//Serial.print( temp );
if(temp < tempMin) // if temp is lower than minimum temp
{
fanSpeed = 0; // fan is not spinning
analogWrite(fan, fanSpeed);
fanLCD=0;
digitalWrite(fan, LOW);
}
if((temp >= tempMin) && (temp <= tempMax)) // if temperature is higher than minimum temp
{
fanSpeed = temp;//map(temp, tempMin, tempMax, 0, 100); // the actual speed of fan//map(temp, tempMin, tempMax, 32, 255);
fanSpeed=1500*fanSpeed;
fanLCD = map(temp, tempMin, tempMax, 0, 100); // speed of fan to display on LCD100
analogWrite(fan, fanSpeed); // spin the fan at the fanSpeed speed
}
 
if(temp > tempMax) // if temp is higher than tempMax
{
//digitalWrite(Led, HIGH); // turn on led
}
else // else turn of led
{
//digitalWrite(Led, LOW);
}
 
Lcd.print("TEMP: ");
Lcd.print(temp); // display the temperature
Lcd.print("C ");
Lcd.setCursor(0,1); // move cursor to next line
Lcd.print("FANS: ");
Lcd.print(fanLCD); // display the fan speed
Lcd.print("%");
delay(200);
Lcd.clear();
}
void bouton_poussoir()
{
  //contenu du programme
int test = digitalRead(BP); // Lecture de l'entree BP et sockage du résultats dans test
if(test==LOW) // Si test est à l'état bas
{
digitalWrite(L1, HIGH); // Allumer L1
 myservo.write(angleClosed);
 digitalWrite(greenPin, LOW);
}
else // Sinon
{
digitalWrite(L1, LOW); // Eteindre L1
}
}
void detect_intrus()
{
  int valeur;
  if (activateAlarm) {
    Lcd.clear();
    Lcd.setCursor(0,0);
    Lcd.setCursor(0,1);
    Lcd.print("Alarm will be");
    Lcd.print("activated in");
   
    int countdown = 9; // 9 seconds count down before activating the alarm
    while (countdown != 0) {
      Lcd.setCursor(13,1);
      Lcd.print(countdown);
      countdown--;
      tone(speakerPin, 700, 100);
      delay(1000);
    }
    Lcd.clear();
    Lcd.setCursor(0,0);
    Lcd.print("Alarm Activated!");
//    initialDistance = getDistance();
    activateAlarm = false;
    alarmActivated = true;
  }

  if (alarmActivated == true){
      
      valeur=digitalRead(trigPin);
      if ( valeur == HIGH) {
        Serial.println(valeur);
        tone(speakerPin, 1000); // Send 1KHz sound signal 
        Lcd.clear();
              }
              else { noTone(speakerPin);
              
    }

  
      }
}
