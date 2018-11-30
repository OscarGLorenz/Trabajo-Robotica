#ifndef SPLINE_H_
#define SPLINE_H_

class Spline {
public:

bool active = false;
void loadSpline(char * str) {
  char * ptr = strtok(str," ");

  sscanf(ptr,"%d",&n);

  free(a); a = malloc(sizeof(float) * n);
  free(b); b = malloc(sizeof(float) * n);
  free(c); c = malloc(sizeof(float) * n);
  free(d); d = malloc(sizeof(float) * n);
  free(t); t = malloc(sizeof(float) * (n + 1));
  
  char s[30];
  ptr = strtok (NULL," ");
  
  for (int i = 0; i < n; i++) {
    sscanf(ptr,"%s",s);
    d[i] = atof(s);
    ptr = strtok (NULL," ");
  }
  
  for (int i = 0; i < n; i++) {
    sscanf(ptr,"%s",s);
    c[i] = atof(s);
    ptr = strtok (NULL," ");
  }

  for (int i = 0; i < n; i++) {
    sscanf(ptr,"%s",s);
    b[i] = atof(s);
    ptr = strtok (NULL," ");
  }

  for (int i = 0; i < n; i++) {
    sscanf(ptr,"%s",s);
    a[i] = atof(s);
    ptr = strtok (NULL," ");
  }

  for (int i = 1; i < n + 1; i++) {
    sscanf(ptr,"%s",s);
    t[i] = atof(s);
    ptr = strtok (NULL," ");
  }
  t[0] = 0;
 
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
