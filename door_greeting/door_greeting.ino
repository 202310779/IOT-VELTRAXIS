/**
 * IoT Door Greeting Device
 * GDAP Roadshow - Computer Laboratory Entrance
 *
 * Hardware:
 *   - Arduino Uno with Robot Expansion Shield
 *   - HC-SR04 Ultrasonic Sensor (Trig: Pin 12, Echo: Pin 13)
 *   - DFPlayer Mini (placeholder - connect to Serial1 or SoftwareSerial)
 *
 * Logic:
 *   - Continuously measures distance at the door entrance
 *   - Triggers a greeting when an object is detected within 100cm
 *   - 5-second cooldown between greetings to prevent spamming
 */

// ─── Pin Definitions ───────────────────────────────────────────────────────
#define TRIG_PIN       12
#define ECHO_PIN       13

// ─── Configuration ─────────────────────────────────────────────────────────
#define DETECT_DISTANCE_CM  100     // Maximum trigger distance in centimeters
#define MIN_DISTANCE_CM     10      // Ignore readings closer than this (filters false positives)
#define GREETING_COOLDOWN   5000    // Cooldown in milliseconds (5 seconds)

// ─── State ─────────────────────────────────────────────────────────────────
unsigned long lastGreetingTime = 0;
float lastDistance = -1;

#define MOTION_THRESHOLD_CM 50  // Minimum change in distance to count as motion

// ───────────────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("========================================");
  Serial.println("  IoT Door Greeting Device - Ready");
  Serial.println("  Welcome to the Computer Laboratory!");
  Serial.println("========================================");
}

void loop() {
  float distance = measureDistance();

  if (distance > MIN_DISTANCE_CM && distance <= DETECT_DISTANCE_CM) {
    bool motionDetected = (lastDistance < 0) ? false : abs(distance - lastDistance) >= MOTION_THRESHOLD_CM;

    unsigned long now = millis();
    if (motionDetected && (now - lastGreetingTime >= GREETING_COOLDOWN)) {
      triggerGreeting();
      lastGreetingTime = now;
    }
  }

  if (distance > 0) lastDistance = distance;
}

/**
 * Measures distance using the HC-SR04 ultrasonic sensor.
 * Returns distance in centimeters, or -1 if the reading is invalid.
 */
float measureDistance() {
  // Send a 10µs pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pulse duration (timeout after 30ms ~ 510cm)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return -1; // No echo received

  // Convert duration to centimeters (speed of sound: 343 m/s)
  float distanceCm = (duration * 0.0343) / 2.0;
  return distanceCm;
}

/**
 * Triggers the greeting sequence:
 *   - Logs the greeting to Serial monitor
 *   - Calls the audio greeting placeholder
 */
void triggerGreeting() {
  Serial.println("----------------------------------------");
  Serial.println("  Person detected at the entrance!");
  Serial.println("  Welcome to the Computer Laboratory!");
  Serial.println("----------------------------------------");

  playAudioGreeting();
}

/**
 * Placeholder for audio greeting via DFPlayer Mini.
 * Replace this body with actual DFPlayer Mini commands
 * once the module is wired up (e.g., using DFRobotDFPlayerMini library).
 *
 * Example with DFPlayer:
 *   myDFPlayer.play(1);  // Play track 0001.mp3 from SD card
 */
void playAudioGreeting() {
  Serial.println("  [Audio] Playing greeting audio...");
  // TODO: Uncomment and configure once DFPlayer Mini is connected
  // myDFPlayer.play(1);
}

