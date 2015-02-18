// You MUST enable SERIALCOMMAND_HARDWAREONLY
// SerialCommand should be modified to use \n as term

#include <SerialCommand.h>

#define PI      3.14159
#define PERIOD  6000

SerialCommand SCmd;

typedef enum led_mode_t {
    MODE_NONE = 0,
    MODE_CYCLE,
    MODE_END
};

led_mode_t mode;

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

    mode = MODE_CYCLE;

    Serial.println("READY");
}

void loop() {
    SCmd.readSerial();
    if (mode < MODE_END && mode != MODE_NONE) {
        // Update mode
        switch(mode) {
            case MODE_CYCLE:
                setColor(
                    int(128+127*sin(2*PI/PERIOD*millis())),
                    int(128+127*sin(2*PI/PERIOD*millis() + PERIOD/3)),
                    int(128+127*sin(2*PI/PERIOD*millis() + 2*PERIOD/3)));
                break;
            default:
                Serial.println("This should not happen!");
                break;
        }
    }
}

void setColor(int color[3]) {
    setColor(color[0], color[1], color[2]);
}

void setColor(int r, int g, int b) {
    Serial.print(r);
    Serial.print(" ");
    Serial.print(g);
    Serial.print(" ");
    Serial.print(b);
    Serial.println();
    analogWrite(RED_LED, constrain(r, 0, 255));
    analogWrite(GREEN_LED, constrain(g, 0, 255));
    analogWrite(BLUE_LED, constrain(b, 0, 255));
}

void doesnotexist() {
    Serial.println("Does not exist!");
}

void setLED() {
    char *arg;
    int color[3];

    for (int i=0; i < 3; i++) {
        arg = SCmd.next();

        if (arg == NULL)
            return;

        color[i] = atoi(arg);
    }

    mode = MODE_NONE;
    setColor(color);

    Serial.print("Set LEDs: ");
    Serial.print(color[0]);
    Serial.print(" ");
    Serial.print(color[1]);
    Serial.print(" ");
    Serial.print(color[2]);
    Serial.println();
}

void setMode() {
    char *arg;

    arg = SCmd.next();
    if (arg == NULL)
        return;

    mode = led_mode_t(atoi(arg));

    Serial.print("Set Mode: ");
    Serial.print(arg);
    Serial.println();
}
