#include <Arduino.h>
#include "esp_camera.h"

#include "SSD1306.h"
// #include "OLEDDisplayUi.h"

#include "OneButton.h"

#define PWDN_GPIO_NUM   26      ///< PWD camera pin
#define RESET_GPIO_NUM  -1      ///< RST camera pin
#define XCLK_GPIO_NUM   32      ///< XCLK camera pin
#define SIOD_GPIO_NUM   13      ///< SIOD camera pin
#define SIOC_GPIO_NUM   12      ///< SIOC camera pin

#define Y9_GPIO_NUM     39      ///< Y9 camera pin
#define Y8_GPIO_NUM     36      ///< Y8 camera pin
#define Y7_GPIO_NUM     23      ///< Y7 camera pin
#define Y6_GPIO_NUM     18      ///< Y6 camera pin
#define Y5_GPIO_NUM     15      ///< Y5 camera pin
#define Y4_GPIO_NUM     4       ///< Y4 camera pin
#define Y3_GPIO_NUM     14      ///< Y3 camera pin
#define Y2_GPIO_NUM     5       ///< Y2 camera pin
#define VSYNC_GPIO_NUM  27      ///< VSNC camera pin
#define HREF_GPIO_NUM   25      ///< HREF camera pin
#define PCLK_GPIO_NUM   19      ///< PCLK camera pin

#define I2C_SDA         21      ///< SDA I2C board pin
#define I2C_SCL         22      ///< SCL I2C board pin

#define AS312_PIN       33      ///< PIR Sensor pin
#define BUTTON_1        34      ///< Board button

#define SSD1306_ADDRESS 0x3c  ///< SSD1306 I2C address

SSD1306 oled(SSD1306_ADDRESS, I2C_SDA, I2C_SCL);
// OLEDDisplayUi ui(&oled);

OneButton button(BUTTON_1, true);


void setup() {
    oled.init();
    oled.setFont(ArialMT_Plain_16);
    oled.setTextAlignment(TEXT_ALIGN_CENTER);
    delay(50);

    oled.setPixel(10, 10);
    oled.display();

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_RGB888;
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;

    /* camera sensor init */
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init Fail");
        oled.clear();
        oled.drawString(oled.getWidth() / 2, oled.getHeight() / 2, "Camera init Fail");
        oled.display();

        while (1);
    }

    sensor_t *s = esp_camera_sensor_get();
    /* Flip image initially if necessary */
    // s->set_vflip(s, 1);
    /* Black and white special effect set */
    s->set_special_effect(s, 2);

    button.attachClick(button_func);
}

void loop() {
  // put your main code here, to run repeatedly:
}

/* Invert display color */
void button_func() {
    oled.setColor(INVERSE);
}