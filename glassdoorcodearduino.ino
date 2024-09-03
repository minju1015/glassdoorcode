// 핀 설정
const int sensor1Pin = 2;     // 첫 번째 적외선 센서 핀
const int sensor2Pin = 3;     // 두 번째 적외선 센서 핀
const int ledPin = 5;         // LED 핀 설정
const int playPin = 4;        // ISD1820 P-L 핀
const int triggerPin = 9;     // 초음파 센서의 트리거 핀
const int echoPin = 10;       // 초음파 센서의 에코 핀

// 센서 상태 변수
int sensor1State = HIGH;
int sensor2State = HIGH;

// 적외선 센서 시간 변수
unsigned long sensor1Time = 0;
unsigned long sensor2Time = 0;
unsigned long elapsedTime = 0;
const unsigned long speedLimitTime = 5000;  // 5초

// 적외선 센서 감지 여부
bool sensor1Detected = false;

// 초음파 센서 거리 변수
const int distanceThreshold = 10; // 거리 임계값 (cm)

void setup() {
  pinMode(sensor1Pin, INPUT_PULLUP);  // 적외선 센서 핀 설정 (풀업 저항 사용)
  pinMode(sensor2Pin, INPUT_PULLUP);  // 적외선 센서 핀 설정 (풀업 저항 사용)
  pinMode(ledPin, OUTPUT);   // LED 핀 설정
  pinMode(playPin, OUTPUT);  // ISD1820 P-L 핀 설정

  pinMode(triggerPin, OUTPUT);  // 초음파 센서의 트리거 핀 설정
  pinMode(echoPin, INPUT);      // 초음파 센서의 에코 핀 설정

  Serial.begin(9600);  // 시리얼 모니터용
}

void loop() {
  // 첫 번째 센서 상태 읽기
  sensor1State = digitalRead(sensor1Pin);
 
  // 첫 번째 센서 감지 시
  if (sensor1State == LOW && !sensor1Detected) {
    sensor1Time = millis(); // 첫 번째 센서 감지 시간 저장
    sensor1Detected = true; // 첫 번째 센서 감지 여부 설정
    Serial.println("센서 1 감지됨");
  }

  // 두 번째 센서 상태 읽기
  sensor2State = digitalRead(sensor2Pin);
 
  // 두 번째 센서 감지 시
  if (sensor2State == LOW && sensor1Detected) {
    sensor2Time = millis(); // 두 번째 센서 감지 시간 저장
    elapsedTime = sensor2Time - sensor1Time; // 시간 차이 계산
    Serial.print("경과 시간: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");

    // 시간 차이가 5초 이내이면 LED와 소리 실행
    if (elapsedTime < speedLimitTime) {
      Serial.println("속도 초과 감지! LED 점등 및 소리 재생...");
      lightLEDAndPlaySound();  // LED 점등 및 소리 재생
    }

    // 센서 감지 상태 리셋
    sensor1Detected = false;
  }

  // 초음파 센서 거리 측정
  long distance = readDistance();
  Serial.print("초음파 센서 거리: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 거리 감지 시 LED와 소리 실행
  if (distance < distanceThreshold) {  // 임계 거리 이내일 때
    Serial.println("거리 초과 감지! LED 점등 및 소리 재생...");
    lightLEDAndPlaySound();  // LED 점등 및 소리 재생
  }

  delay(100);  // 주기적 데이터 읽기를 위한 대기
}

// 초음파 센서 거리 측정 함수
long readDistance() {
  digitalWrite(triggerPin, LOW);  // 트리거 핀을 낮은 상태로 유지
  delayMicroseconds(2);           // 2ms 대기

  digitalWrite(triggerPin, HIGH); // 트리거 핀을 높은 상태로 유지
  delayMicroseconds(10);          // 10ms 대기
  digitalWrite(triggerPin, LOW);  // 트리거 핀을 다시 낮은 상태로 유지

  long duration = pulseIn(echoPin, HIGH); // 에코 핀에서 신호 지속 시간 읽기
  long distance = (duration / 2) / 29.1;  // 거리 계산 (cm)

  return distance;
}

// LED 점등 및 소리 재생 함수
void lightLEDAndPlaySound() {
  digitalWrite(ledPin, HIGH);   // LED 점등
  digitalWrite(playPin, HIGH);  // ISD1820 소리 재생 시작
  delay(4500);  // 음성 재생이 완료될 때까지 충분히 대기 (예: 4.5초)
  digitalWrite(ledPin, LOW);    // LED 소등
  digitalWrite(playPin, LOW);   // ISD1820 소리 재생 종료
  delay(500);  // 소리 종료 후 대기
}
