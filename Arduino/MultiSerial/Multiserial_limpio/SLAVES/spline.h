#ifndef SPLINE_H_
#define SPLINE_H_

class Spline {
public:

bool active = false;
void loadSpline(void) {
  n = Serial.parseInt();
  free(a); a = malloc(sizeof(float) * n);
  free(b); b = malloc(sizeof(float) * n);
  free(c); c = malloc(sizeof(float) * n);
  free(d); d = malloc(sizeof(float) * n);
  free(t); t = malloc(sizeof(float) * (n + 1));
  for (int i = 0; i < n; i++)
    d[i] = Serial.parseFloat();
  for (int i = 0; i < n; i++)
    c[i] = Serial.parseFloat();
  for (int i = 0; i < n; i++)
    b[i] = Serial.parseFloat();
  for (int i = 0; i < n; i++)
    a[i] = Serial.parseFloat();
  for (int i = 1; i < n + 1; i++)
    t[i] = Serial.parseFloat();
  t[0] = 0;
  Serial.parseFloat();
  start = millis();
  interval = 0;
  active = true;
 }

float evaluate(void) {
  if (millis() / 1000.0 > start / 1000.0 + t[interval + 1])
    interval++;

  float time = (millis() - start) / 1000.0;
  if (time > t[n])
    active = true;
    return a[interval] +
           b[interval] * (time - t[interval]) +
           c[interval] * (time - t[interval]) * (time - t[interval]) +
           d[interval] * (time - t[interval]) * (time - t[interval]) * (time - t[interval]);
}

private:
float * a, * b, * c, * d, * t;
int n; unsigned long int start;
int interval;
} spline;

#endif
