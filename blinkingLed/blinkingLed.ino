#define LED_TOGGLE 'L'
#define DISTANCE 'D'
#define VOLTAGE 'V'
#define CURRENT 'C'
#define BATTERY_VOLTAGE 'B'
#define SHUNT_VOLTAGE 'S'

const int LED = 13;
const int MEASURE_ECHO = 12;
const int MEASURE_TRIGGER= 11;
const int VOLTAGE_INPUT = A0; //analog pin 0
const int CURRENT_INPUT = A1; //analog pin 1
const int BATTERY_VOLTAGE_INPUT = A2; //analog pin 1
const int SHUNT_VOLTAGE_INPUT = A3;


const char commandSeparator = ';';
const float shuntResistance = 0.0003313333;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}


void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();  // remove whitespace

        int sepIndex = 0;
        int nextSep = command.indexOf(';', sepIndex);

        while (sepIndex < command.length()) {
            String currentCommand;
            if (nextSep == -1) {
                currentCommand = command.substring(sepIndex);
            } else {
                currentCommand = command.substring(sepIndex, nextSep);
            }

            currentCommand.trim();
            if (currentCommand.length() > 0) {
                int comma = currentCommand.indexOf(',');
                if (comma > 0) {
                    char metric = currentCommand[comma - 1];
                    int value = currentCommand.substring(comma + 1).toInt();


                        if (metric == LED_TOGGLE){
                            digitalWrite(LED, value);
                        } else if (metric == VOLTAGE) {
                            int input = analogRead(VOLTAGE_INPUT);
                            float v = (input / 1024.0) * 5.0 * 5.0; // divider_ratio
                            Serial.println(v, 2);
                        } else if (metric == CURRENT){
                            
                            float vDiff = 0;
                            float voltageOffset = 2.5; //this is what the sensor outputs at 0A
                            float mVARatio = 0.066;

                            int raw = analogRead(A1);
                        
                            float analogVoltage = (raw / 1024.0) * 5;

                            if (analogVoltage > voltageOffset){
                                vDiff = analogVoltage - voltageOffset;
                            }
                            
                            float c = vDiff / mVARatio;

                            Serial.println(c, 2);
                        }
                        else if (metric == BATTERY_VOLTAGE) {
                            int input = analogRead(BATTERY_VOLTAGE_INPUT);
                            float v = (input / 1024.0) * 5.0 * 5.0; // divider_ratio
                            Serial.println(v, 2);
                        }
                        else if (metric == SHUNT_VOLTAGE) {

                            //I found at 0 v, the analog output actually reads about 16.
                            //for the shunt voltage, I'm going to take that away.
                            
                            float input = 0;

                            for (int i = 0; i < 10; i++) {

                                int analog = analogRead(SHUNT_VOLTAGE_INPUT);

                                if (analog < 0) analog = 0;

                                input += analog;

                                delay(10);
                            }
                            
                            input = input / 10;

                            float v = (input / 1024.0) * 5.0 * 5.0;
                            float shuntCurrent = v / shuntResistance;

                            Serial.println(shuntCurrent ,6);
                        } else {
                            Serial.println(-2);
                        }
                }
            }

            if (nextSep == -1) break;
            sepIndex = nextSep + 1;
            nextSep = command.indexOf(';', sepIndex);
        }
    }
    delay(100);
}