int bientroX = A0 ; 
int bientroY = A1 ; 
int button = 2;      
void setup ()
{
     pinMode(2, INPUT);
     pinMode(A0, INPUT);
     pinMode(A1, INPUT);
     Serial.begin(9600);
}
void loop ()
{
     int x = analogRead(bientroX);  // doc gia tri cua truc X
     int y = analogRead(bientroY);  // doc gia tri cua truc Y
     int KEY = digitalRead(button);  // doc gia tri cua nut nhan
              // Xuat ra cong Serial
              Serial.print("X="); Serial.println(x);
              Serial.print("Y="); Serial.println(y);
              Serial.print("KEY="); Serial.print(KEY); 
              Serial.println();
              Serial.println();
              delay(200);//delay để ổn định hơn
      
}
