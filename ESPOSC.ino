#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//check the serial plotter too
// OLED display configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

// I2C pins for ESP32
#define OLED_SDA 21
#define OLED_SCL 22

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Constants and Variables for Analog Reading
const int SAMPLES = 320;
const int ad_ch0 = 34; // Analog 34 pin for channel 0
const int ad_ch1 = 35; // Analog 35 pin for channel 1
const float VREF = 3.3; // Assuming VREF is 3.3V
const int ADC_RESOLUTION = 4095;
const int Rates[] = {1, 2, 5, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

// Buffers for Storing Data
short data[2][SAMPLES]; // Double buffer to store samples
float CH0, CH1;
int rate_val = 5;  // Default rate (adjust as needed)
short ch0_mode_val = 0;  // Default mode for channel 0
short ch1_mode_val = 2;  // Default mode for channel 1

// Trigger Settings
short trig_mode_val = 0; // Default mode for triggering
int trig_lv_val = 40;    // Default trigger level
short trig_edge_val = 0; // Default trigger edge
short trig_ch_val = 0;   // Default trigger channel

// Function to Read Analog Values
inline float adRead(short ch, short mode) {
    int raw = analogRead(ch);
    float voltage = (raw * VREF) / ADC_RESOLUTION;
    if (mode == 1) {  // MODE_INV
        return VREF - voltage;
    }
    return voltage;
}

// Function to Sample Data and Store
void sampleAndStore() {
    if (trig_mode_val != 2) { // Not in TRIG_SCAN mode
        unsigned long st = millis();
        float oad = (trig_ch_val == 0) ? adRead(ad_ch0, ch0_mode_val) : adRead(ad_ch1, ch1_mode_val);
        for (;;) {
            float ad = (trig_ch_val == 0) ? adRead(ad_ch0, ch0_mode_val) : adRead(ad_ch1, ch1_mode_val);

            if (trig_edge_val == 0) {  // TRIG_E_UP
                if (ad >= trig_lv_val && ad > oad) {
                    break;
                }
            } else {
                if (ad <= trig_lv_val && ad < oad) {
                    break;
                }
            }
            oad = ad;

            if (trig_mode_val == 2 || (trig_mode_val == 0 && (millis() - st) > 100)) { // TRIG_SCAN or TRIG_AUTO
                break;
            }
        }
    }

    for (int i = 0; i < SAMPLES; i++) {
        // Read analog values
        data[0][i] = analogRead(ad_ch0);
        data[1][i] = analogRead(ad_ch1);

        // Convert to voltage
        CH0 = (data[0][i] * VREF) / ADC_RESOLUTION;
        CH1 = (data[1][i] * VREF) / ADC_RESOLUTION;

        // Print to Serial for Serial Plotter
        Serial.print(CH0, 3); // Print CH0 with 3 decimal places
        Serial.print(" ");
        Serial.println(CH1, 3); // Print CH1 with 3 decimal places, followed by a newline

        // Delay for sampling rate
        delay(Rates[rate_val]); // Adjust the delay as per your sampling rate requirements
    }
}

// Setup Function
void setup() {
    Serial.begin(9600);

    // Initialize the OLED display (optional, if you still want to use it)
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

    // Set I2C pins for ESP32
    Wire.begin(OLED_SDA, OLED_SCL);
    
    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
}

// Main Loop Function
void loop() {
    sampleAndStore();
    delay(500); // Delay between samples
}

// Callback Functions for Cloud Variables
void onRateChange()  {
    Serial.print("Rate changed to: ");
    Serial.println(rate_val);
}

void onCh0ModeChange()  {
    Serial.print("CH0 mode changed to: ");
    Serial.println(ch0_mode_val);
}

void onCh1ModeChange()  {
    Serial.print("CH1 mode changed to: ");
    Serial.println(ch1_mode_val);
}

void onCH0Change()  {
    // Add your code here to act upon CH0 change
}

void onCH1Change()  {
    // Add your code here to act upon CH1 change
}
