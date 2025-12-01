# 전기차 배터리 스마트 공랭식 냉각 시스템 (EV Battery Smart Air-Cooling System)

## 프로젝트 개요 (Project Overview)
본 프로젝트는 **캡스톤 디자인 졸업작품**으로 진행되었으며, 전기차 배터리의 효율과 안전을 저하시키는 발열 문제를 해결하기 위한 **스마트 공랭식 냉각 장치**입니다.
아두이노와 각종 센서를 통해 실시간으로 배터리 온도와 전압을 모니터링하며, **모바일 앱(App Inventor)**을 통해 원격으로 제어하거나 자동 냉각 모드를 수행할 수 있습니다.

## 시스템 구성 및 기능 (Features)

### 1. 실시간 모니터링
- **온도 측정:** DS18B20 온도 센서를 이용하여 배터리 셀의 온도를 정밀 측정.
- **전압 측정:** 전압 분배 회로(Voltage Divider)를 통해 배터리 잔량(%) 및 전압(V) 측정.
- **데이터 시각화:** 측정된 데이터를 LCD 디스플레이 및 스마트폰 앱에 실시간 출력.

### 2. 듀얼 냉각 제어 모드 (Control Modes)
사용자 편의와 에너지 효율을 위해 두 가지 운전 모드를 지원합니다.

| 모드 (Mode) | 설명 (Description) | 동작 로직 |
| :--- | :--- | :--- |
| **수동 모드 (Manual)** | 사용자가 앱 스위치를 통해 직접 팬을 ON/OFF | **ON:** 최대 풍량 가동 <br> **OFF:** 가동 중지 |
| **자동 모드 (Auto)** | 온도 변화에 따라 2단계로 팬 속도 자동 조절 (PWM 제어) | **28°C 이상:** 강풍 (PWM 255) <br> **26°C 이상:** 약풍 (PWM 100) <br> **26°C 미만:** 정지 |

### 3. 모바일 앱 연동 (App Inventor)
- **Bluetooth 통신:** HC-06 모듈을 사용하여 스마트폰과 아두이노 간 양방향 통신.
- **UI 구성:**
    - 현재 온도 및 배터리 잔량 표시 대시보드.
    - Cooling ON/OFF 스위치 (수동 제어).
    - AUTO MODE 버튼 (자동 제어 전환).
    - 시스템 상태 메시지 출력.

## 하드웨어 핀 맵 (Pinout)

| 컴포넌트 (Component) | 아두이노 핀 (Pin) | 비고 |
| :--- | :--- | :--- |
| **Bluetooth (HC-06)** | 2 (RX), 3 (TX) | SoftwareSerial 통신 |
| **LCD (I2C)** | SDA, SCL | 16x2 디스플레이 |
| **온도 센서 (DS18B20)** | 5 | OneWire 프로토콜 |
| **전압 센서** | A0 | 전압 분배 (R1=30k, R2=7.5k) |
| **Cooling Fan 1** | 9 | PWM 제어 가능 |
| **Cooling Fan 2** | 10 | 보조 팬 |
| **MOSFET** | 12 | 팬 전원 제어 스위칭 |

## 소프트웨어 로직 (Code Logic)

### 아두이노 (C++)
- **데이터 처리:** `loop()` 내에서 주기적으로 온도와 전압을 읽어 LCD와 시리얼 모니터에 출력.
- **블루투스 수신:** 앱에서 보내는 명령어(`1`, `0`, `A`)를 파싱하여 모드 전환.
    - `'1'`: 수동 ON
    - `'0'`: 수동 OFF
    - `'A'`: 자동 모드 활성화
- **자동 제어 알고리즘:**
```cpp
if (autoMode) {
    if (tem >= 28) {
        analogWrite(FAN_1, 255); // 강하게 회전
        digitalWrite(MOS, HIGH);
    }
    else if(tem >= 26){
        analogWrite(FAN_1, 100); // 약하게 회전
        digitalWrite(MOS, HIGH);
    }
    else {
        digitalWrite(FAN_1, LOW); // 정지
        digitalWrite(MOS, LOW);
    }
}
