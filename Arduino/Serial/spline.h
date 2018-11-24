#ifndef SPLINE_H_
#define SPLINE_H_

class Spline {
public:

bool active = false;
void loadSpline(String string) {
  char str[200];
  string.toCharArray(str, 200);

  char * pch;
  pch = strtok (str," ");
  sscanf(str,"%d",&n);

  free(a); a = malloc(sizeof(float) * n);
  free(b); b = malloc(sizeof(float) * n);
  free(c); c = malloc(sizeof(float) * n);
  free(d); d = malloc(sizeof(float) * n);
  free(t); t = malloc(sizeof(float) * (n + 1));
  
  for (int i = 0; i < n; i++) {
    pch = strtok (NULL," ");
    sscanf(pch,"%f",&d[i]);
  }
  for (int i = 0; i < n; i++) {
    pch = strtok (NULL," ");
    sscanf(pch,"%f",&c[i]);
  }
  for (int i = 0; i < n; i++) {
    pch = strtok (NULL," ");
    sscanf(pch,"%f",&b[i]);
  }
  for (int i = 0; i < n; i++) {
    pch = strtok (NULL," ");
    sscanf(pch,"%f",&a[i]);
  }
  for (int i = 1; i < n + 1; i++) {
    pch = strtok (NULL," ");
    sscanf(pch,"%f",&t[i]);
  }
  t[0] = 0;

  Serial.println(String(n) + " " + String(a[0]) +" " + String(b[0]) + " " +String(c[0]) + " " + String(d[0]) + " " + String(t[0]) + " " + String(t[1]));

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

bool stop() {
  //Serial.println( (millis()-start)/1000.0);
  return ( ((millis()-start)/1000.0 > t[n]) || t==NULL);
}

unsigned long int getStart(){
  return start;
  }

  
private:
float * a=NULL, * b=NULL, * c=NULL, * d=NULL, * t=NULL;
int n=0; unsigned long int start;
int interval;
};



#endif
