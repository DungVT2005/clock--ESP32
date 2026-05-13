#include "MaxSensor.h"
#include "Global.h"
#include "heartRate.h"

void MaxSensor::begin() {
    if (xSemaphoreTake(i2cMutex, portMAX_DELAY)) {
        // Bắt đầu giao tiếp I2C với tốc độ 100kHz
        if (sensor.begin(Wire, 100000)) {
            // Cú pháp: setup(powerLevel, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange)
            sensor.setup(0x1F, 4, 2, 400, 411, 4096); 
            sensor.setPulseAmplitudeRed(0x1F); // BẬT ĐÈN ĐỎ
            sensor.setPulseAmplitudeIR(0x1F);  // BẬT ĐÈN HỒNG NGOẠI
            
            isDetected = true;
            
        } else {
            isDetected = false;
            Serial.println("[LOI] Khong tim thay MAX30102!");
        }
        xSemaphoreGive(i2cMutex);
    }
}
void MaxSensor::read() {
    if (!isDetected) {
        currentBPM=0;
        // Nếu không thấy cảm biến, đợi 3 giây rồi mới thử tìm lại tránh treo ESP32
        if (millis() - lastRetry > 3000) {
            lastRetry = millis();
            begin(); // Gọi lại hàm begin 
        }
        return; // Thoát ra ngay để không làm kẹt I2C
    } 
    // Nếu đã phát hiện cảm biến, đọc dữ liệu bình thường
    // Chỉ đợi Mutex 10ms không đợi lâu để tránh kẹt Task
    if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        long ir = sensor.getIR();
        if (ir <= 0) { 
            isDetected = false; // Đánh dấu là đã mất kết nối để vòng lặp sau tự gọi begin()
            currentBPM = 0;
            Serial.println(" Loi sensor!");
            xSemaphoreGive(i2cMutex);
            return;
        }
        if (ir > 25000) { 
            //  LỌC DC 
            float current_v = ir + 0.99 * dcFilter_v_prev;
            float signal_AC = current_v - dcFilter_v_prev;
            dcFilter_v_prev = current_v;

            //  LỌC THÔNG THẤP EMA  chỉ lấy 15% tín hiệu mới
            lpf_prev = (0.15 * signal_AC) + (0.85 * lpf_prev);
            float signal_filtered = lpf_prev;

            //  TÌM ĐỈNH NHỊP TIM 
            // Giảm cực chậm (0.99) và đặt mức SÀN CỨNG LÀ 100 để né nhiễu nền
            dynamicThreshold = dynamicThreshold * 0.99; 
            if (dynamicThreshold < 100.0) dynamicThreshold = 100.0;

            // Kéo dài thời gian khóa lên 350ms (Chặn mọi nhịp > 170 BPM)
            if (signal_filtered > dynamicThreshold && (millis() - lastBeat > 350)) {
                dynamicThreshold = signal_filtered; // Đẩy ngưỡng lên bằng đỉnh

                long delta = millis() - lastBeat;
                lastBeat = millis();
                float bpm = 60000.0 / delta; 

                // Siết lại dải đo thực tế của con người: từ 50 đến 150 BPM
                if (bpm > 50 && bpm < 150) {
                    rates[rateSpot++] = (byte)bpm;
                    rateSpot %= RATE_SIZE;

                    float beatAvg = 0;
                    for (byte x = 0; x < RATE_SIZE; x++) {
                        beatAvg += rates[x];
                    }
                    currentBPM = beatAvg / RATE_SIZE; 
                }
            }
        } 
        else {
            // NẾU RÚT TAY RA
            currentBPM = 0;
            dcFilter_v_prev = 0;
            lpf_prev = 0;
            dynamicThreshold = 100.0; // Không reset về 0 nữa mà cho về mức sàn 100
        }

        // CƠ CHẾ TIMEOUT: Quá 5 giây không có nhịp (do để tay không chuẩn), reset về 0
        if (millis() - lastBeat > 5000) {
            currentBPM = 0;
        }
        xSemaphoreGive(i2cMutex);
    }
}
String MaxSensor::getAlerts() {
    if (currentBPM > 100) return "(!) Nhip tim cao";
    if (currentBPM > 0 && currentBPM < 60) return "(!) Nhip tim thap";
    return "";
}