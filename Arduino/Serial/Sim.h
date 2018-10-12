
class NonDynamicSystem {
  private:
    float nominalSpeed = 0;
    float start = 0;
    float end = 0;
    float currentSpeed = 0;
    float startTime = 0;
  public:
    NonDynamicSystem(float nomSpeed) {
      nominalSpeed = nomSpeed;
    }
    float evaluate() {
      float t = millis() / 1000.0;
      float pos = start + (t - startTime) * currentSpeed;
      if ((pos > end && start < end) || (pos < end && start > end)) {
          currentSpeed = 0;
          start = pos;
      }
      return pos;
    }
    float goPos(float pos) {
      start = evaluate();
      end = pos;
      currentSpeed = nominalSpeed;
      startTime = millis() / 1000.0;
    }
    float speed(float newSpeed) {
      start = evaluate();
      currentSpeed = newSpeed;
      end = NAN;
      startTime = millis() / 1000.0;
    }
};
