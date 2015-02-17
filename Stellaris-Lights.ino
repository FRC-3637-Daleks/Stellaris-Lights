// You MUST enable SERIALCOMMAND_HARDWAREONLY
// SerialCommand should be modified to use \n as term

#include <SerialCommand.h>

SerialCommand SCmd;

void setup() {
    // initialize serial:
    Serial.begin(9600);

    // make the pins outputs:
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    SCmd.addCommand("L", setLED);
    SCmd.addCommand("M", setMode);
    SCmd.addDefaultHandler(doesnotexist);
    Serial.println("READY");
}

void loop() {
    SCmd.readSerial();
}

void doesnotexist() {
    Serial.println("Does not exist!");
}

void setLED() {
    int color[3];
    char *arg;

    for (int i=0; i < 3; i++) {
        arg = SCmd.next();
        if (arg != NULL) {
            color[i] = atoi(arg);
        }
    }
    analogWrite(RED_LED, color[0]);
    analogWrite(GREEN_LED, color[1]);
    analogWrite(BLUE_LED, color[2]);

    Serial.print("Set: ");
    Serial.print(color[0]);
    Serial.print(" ");
    Serial.print(color[1]);
    Serial.print(" ");
    Serial.print(color[2]);
    Serial.println();
}

void setMode() {
}
