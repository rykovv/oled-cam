#include <Arduino.h>
#include "esp_camera.h"

#include "SSD1306.h"

#include "OneButton.h"

#define BAUDRATE        115200  ///< Serial Baudrate

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

#define SSD1306_ADDRESS 0x3c    ///< SSD1306 I2C address

#define IMAGE_DEPTH     3       ///< Image depth in pixels

SSD1306 oled(SSD1306_ADDRESS, I2C_SDA, I2C_SCL);

OneButton button(BUTTON_1, false, false);
static void button_func();

static uint8_t inverted = 0;

void setup() {
    Serial.begin(BAUDRATE);
    oled.init();
    oled.setFont(ArialMT_Plain_16);
    oled.setTextAlignment(TEXT_ALIGN_CENTER);
    delay(50);

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
    if (IMAGE_DEPTH == 1) {
        config.frame_size = FRAMESIZE_QQVGA;
    } else if (IMAGE_DEPTH == 3) {
        config.frame_size = FRAMESIZE_HQVGA;
    } 
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
    s->set_vflip(s, 1);
    /* Black and white special effect set */
    s->set_special_effect(s, 2);

    button.attachClick(button_func);
}

void loop() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
    }
    size_t offset;
    
    if (IMAGE_DEPTH == 1) {
        for (uint8_t i = 0; i < oled.getHeight(); i++) {
            offset = (((fb->height-oled.getHeight())/2)+i)*fb->width*3 + ((fb->width-oled.getWidth())/2)*3;
            for (uint8_t j = 0; j < oled.getWidth(); j++) {
                if (fb->buf[ offset+j*3 ] > 127) oled.setPixel(j, i);
            }
        }
    } else if (IMAGE_DEPTH == 3) {
        for (uint8_t i = 0; i < oled.getHeight(); i++) {
            offset = (((fb->height-oled.getHeight())/2)+i)*fb->width*3 + ((fb->width-oled.getWidth()/3)/2)*3;
            for (uint16_t j = 0; j < oled.getWidth(); j+=3) {
                if (fb->buf[ offset+j*3 ] > 192) {
                    oled.setPixel(j, i);
                    oled.setPixel(j+1, i);
                    oled.setPixel(j+2, i);
                } else if (fb->buf[ offset+j*3 ] > 128) {
                    oled.setPixel(j, i);
                    oled.setPixel(j+1, i);
                } else if (fb->buf[ offset+j*3 ] > 64) {
                    oled.setPixel(j, i);
                }
            }
        }
    }
    esp_camera_fb_return(fb);
    oled.display();
    oled.clear();
    button.tick();
}

/* Invert display color. Roughly works */
static void button_func() {
    if (inverted) {
        oled.normalDisplay();
        inverted = 0;
    } else {
        oled.invertDisplay();
        inverted = 1;
    }
}