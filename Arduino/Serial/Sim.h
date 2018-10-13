
class NonDynamicSystem {
  private:
    float nominalSpeed = 0;
    float start = 0;
    float end = 0;
    float currentSpeed = 0;
    float startTime = 0;
    float pos = 0;
  public:
    NonDynamicSystem(float nomSpeed) {
      nominalSpeed = nomSpeed;
    }
    void update() {
      float t = millis() / 1000.0;
      if (pos < end && currentSpeed > 0) {
         pos = start + (t - startTime) * currentSpeed;
      } else if  (pos > end && currentSpeed < 0) {
         pos = start + (t - startTime) * currentSpeed;
      } else {
          pos = end;
      }
      
    }
    float evaluate() {
      return pos;
    }
    float goPos(float obj) {
      start = evaluate();
      end = obj;
      if (start < end)
       currentSpeed = nominalSpeed;
      else if (start > end)
       currentSpeed = -nominalSpeed;
      startTime = millis() / 1000.0;
    }
    float speed(float newSpeed) {
      start = evaluate();
      currentSpeed = newSpeed;
      end = NAN;
      startTime = millis() / 1000.0;
    }
};



