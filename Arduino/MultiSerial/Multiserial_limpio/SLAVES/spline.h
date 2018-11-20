<<<<<<< Updated upstream
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
=======
#ifndef SPLINE_H_
#define SPLINE_H_

class Spline {
public:

bool active = false;
void loadSpline(void) {
  n = Serial1.parseInt();
  free(a); a = malloc(sizeof(float) * n);
  free(b); b = malloc(sizeof(float) * n);
  free(c); c = malloc(sizeof(float) * n);
  free(d); d = malloc(sizeof(float) * n);
  free(t); t = malloc(sizeof(float) * (n + 1));
  for (int i = 0; i < n; i++)
    d[i] = Serial1.parseFloat();
  for (int i = 0; i < n; i++)
    c[i] = Serial1.parseFloat();
  for (int i = 0; i < n; i++)
    b[i] = Serial1.parseFloat();
  for (int i = 0; i < n; i++)
    a[i] = Serial1.parseFloat();
  for (int i = 1; i < n + 1; i++)
    t[i] = Serial1.parseFloat();
  t[0] = 0;
    Serial1.parseFloat();

//  Serial1.println(String(n) + " " + String(a[0]) +" " + String(b[0]) + " " +String(c[0]) + " " + String(d[0]) + " " + String(t[0]) + " " + String(t[1]));

  start = millis();
  interval = 0;
  active = true;
 }

float evaluate(float time) {

  // if (time > t[interval + 1])
   // interval++;
    
  for (interval=0;interval<n;interval++){
    if (time < t[interval]) break;        
    }
    
  interval--;
  
  return a[interval] +
         b[interval] * (time - t[interval]) +
         c[interval] * (time - t[interval]) * (time - t[interval]) +
         d[interval] * (time - t[interval]) * (time - t[interval]) * (time - t[interval]);


}

bool stop(float time) {
  return ((time-start/1000.0 > t[n]) || t==NULL);
}

unsigned long int getStart(){
  return start;
  }

  
private:
float * a=NULL, * b=NULL, * c=NULL, * d=NULL, * t=NULL;
int n=0; unsigned long int start;
int interval;
} spline;

#endif
>>>>>>> Stashed changes
