
int colorpin[3];
int colorval[3];
enum enu_mode {COLOR = 0, PATTERN = 1};
enu_mode m;
char c;
int delval;  // Delay Value

//Pattern variables
int patterndir; //Patter Direction, 0 = color 1 rise, 1 = color 2 rise, 2 = color 1 drop, 3 = color 2 drop
double patternc1; // color 1 value
double patternc2; // color 2 value
int patternmap1; // color 1 mapping
int patternmap2; // color 2 mapping
double patternmult; // Multiplying factor

//
void colorset();
void patternset();


void setup()
{
  colorpin[0] = 9;  // RED
  colorpin[1] = 10; // BLU
  colorpin[2] = 11; // GRN
  colorval[0] = 0;  // RED
  colorval[1] = 0;  // BLU
  colorval[2] = 0;  // GRN
  delval = 200;
  m = 0;
  patternmult = (double) 17/10;
  
  
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(colorpin[0], OUTPUT);
    pinMode(colorpin[2], OUTPUT);
    pinMode(colorpin[1], OUTPUT);

}


void loop()
{
  delay(delval);

  // if color sync available from serial port, update color
  Serial.print(Serial.available());
  Serial.println();
  if(Serial.available() >=  10) {
    c = Serial.read(); 
    
    if( c == 'S'  ) {
      colorval[0] = 0;
      colorval[1] = 0;
      colorval[2] = 0;
      colorset();
      delval = 200;
      m = COLOR;      
    } else if (c == 'P') {
      patternset();
      m = PATTERN;
      colorval[0] = 0;
      colorval[1] = 0;
      colorval[2] = 0;
    }
  }
  while(Serial.available() <  10 && Serial.available() > 0) {
    c = Serial.read();
  }
  
  
  if (m == COLOR) {
  
  } else if (m == PATTERN) {
    pattern();
  }
}

void pattern() {
    if (patterndir == 0) {     
      patternc2=patternc2 * patternmult;
      }
    
    else if (patterndir == 1) {  
      patternc1=patternc1 / patternmult;
      }
    
    else if (patterndir == 2) {     
      patternc2=patternc2 / patternmult;
      } 
      
    else if (patterndir == 3) {  
      patternc1=patternc1 * patternmult;
      } 
      
      
  if (patterndir==0 && patternc2 >= 250) {
    patterndir = 1;
    patternc1 = 255;
    patternc2 = 255;
    
  } else if (patterndir==1 && patternc1 <= 5) {
    patterndir = 2;
    patternc1 = 0;
    patternc2 = 255;
    
  } else if (patterndir==2 && patternc2 <= 5) {
    patterndir = 3;
    patternc1 = 5;
    patternc2 = 0;
    
  } else if (patterndir==3 && patternc1 >= 250) {
    patterndir = 0;
    patternc1 = 255;
    patternc2 = 5;
    
  }

  
  colorval[patternmap1] = (int) patternc1;
  colorval[patternmap2] = (int) patternc2;
  
  setcolor(0);
  setcolor(1);
  setcolor(2);
}

void colorset() {
  int i = 0;
  
      while(i <3) {
        c = Serial.read();
        if( c >= '0' && c <= '9'){ 
            colorval[0] = (10 * colorval[0]) + (c - '0');
          }
        i++;
      }
      
      i = 0;
      while(i <3) {
        c = Serial.read();
        if( c >= '0' && c <= '9'){ 
          colorval[1] = (10 * colorval[1]) + (c - '0');
        }
        i++;
      }
      
      i = 0;
      while(i <3) {
        c = Serial.read();
        if( c >= '0' && c <= '9'){ 
          colorval[2] = (10 * colorval[2]) + (c - '0');
        }
        i++;
      }
      Serial.println();
      setcolor(0);
      setcolor(1);
      setcolor(2);
  
}

void patternset() {
  int i = 0;
  
  c = Serial.read();
  if( c >= '0' && c <= '2') {
    patternmap1 = (c - '0');
  } else {
    Serial.print("ERROR 1");
  }
  
  c = Serial.read();
  if( c >= '0' && c <= '2') {
    patternmap2 = (c - '0');
  } else {
    Serial.print("ERROR 1");
  }
  c = Serial.read();
  
  delval = 0;
  while(i <3) {
    c = Serial.read();
    if( c >= '0' && c <= '9'){ 
      delval = (10 * delval) + (c - '0');
    }
    i++;
  }

  i = 0;
  patternmult = 0;
  while(i <3) {
    c = Serial.read();
    if( c >= '0' && c <= '9'){ 
      patternmult = (10 * patternmult) + (c - '0');
    }
    i++;
  }
  patternmult  = patternmult / 100;
  


  patterndir = 3;
  patternc1 = 5;
  patternc2 = 0;
  Serial.print("Delay = " + (String) delval);
  Serial.println();
  Serial.print("PM = " + (String) patternmult);
  Serial.println();
}

void setcolor(int i) {
  if(colorval[i] == 888){
    
  }
  else {
    if(colorval[i] > 255) {
      colorval[i] = 255;
      Serial.print("Color " + (String) i + "set to 255");
      Serial.println();
    }
    
    if(colorval[i] < 0) {
      colorval[i] = 0;
      Serial.print("Color " + (String) i + "set to 0");
      Serial.println();
    }
    

    analogWrite(colorpin[i],colorval[i]);

  
    
    Serial.print(i);
    Serial.print(":");
    Serial.print(colorval[i]);
    Serial.print(", ");
    Serial.println();
    
    
  }
}

