#include <WM8978.h> /* https://github.com/CelliesProjects/wm8978-esp32 */
#include <Audio.h>  /* https://github.com/schreibfaul1/ESP32-audioI2S */

/* M5Stack Node WM8978 I2C pins */
#define I2C_SDA     21
#define I2C_SCL     22

/* M5Stack Node I2S pins */
#define I2S_BCK      5
#define I2S_WS      13
#define I2S_DOUT     2
#define I2S_DIN     34

/* M5Stack WM8978 MCLK gpio number and frequency */
#define I2S_MCLKPIN  0
#define I2S_MFREQ  (24 * 1000 * 1000)

WM8978 dac;
Audio audio;

void setup() {
  /* Setup wm8978 I2C interface */
  if (!dac.begin(I2C_SDA, I2C_SCL)) {
    ESP_LOGE(TAG, "Error setting up dac. System halted");
    while (1) delay(100);
  }

  /* Setup wm8978 MCLK on gpio - for example M5Stack Node needs this clock on gpio 0 */
  double retval = dac.setPinMCLK(I2S_MCLKPIN, I2S_MFREQ);
  if (!retval)
    ESP_LOGE(TAG, "Could not set %.2fMHz clock signal on GPIO %i", I2S_MFREQ / (1000.0 * 1000.0), I2S_MCLKPIN);
  else
    ESP_LOGI(TAG, "Set %.2fMHz clock signal on GPIO %i", retval / (1000.0 * 1000.0), I2S_MCLKPIN);


  /* Setup wm8978 I2S interface */
  audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_DIN);

  WiFi.begin("xxx", "xxx");
  while (!WiFi.isConnected()) {
    delay(10);
  }
  ESP_LOGI(TAG, "Connected");

  ESP_LOGI(TAG, "Starting MP3...\n");
  audio.connecttohost("http://icecast.omroep.nl/3fm-bb-mp3");

  dac.setSPKvol(40); /* max 63 */
  dac.setHPvol(32, 32);
}

void loop() {
  audio.loop();
  //delay(1);
}
