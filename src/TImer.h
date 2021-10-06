
class Timer {
  public:
    static void getTime();
    static void setTime(int t);
    static void getSerial();
  private:
    int refreshed;
    int t;
};

void Timer::getTime() {
  Serial.println("Hi");
  Serial.println("there");
};

void Timer::setTime(int t) {

}

void Timer::getSerial() {
  while (Serial.available() > 0) {

  }
}