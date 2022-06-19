#include<Wire.h>

//Endereço em hexadecimal do sensor MPU 6050
const int ENDERECO_SENSOR=0x68;  

int  acelX;
long moving_average();      //Função para filtro de média móvel
int       original,filtrado;         
#define      n     10       //número de pontos da média móvel 
int       numbers[n];       //vetor com os valores para média móvel 



void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(ENDERECO_SENSOR);
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true);
}
void loop()
{
  //Começa uma transmissão com o sensor
  Wire.beginTransmission(ENDERECO_SENSOR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ENDERECO_SENSOR, 2, true);  

  acelX = Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)  

  original = acelX;

  filtrado = moving_average();

  Serial.print(original);
  Serial.print(" ");
  Serial.println(filtrado);
   
  delay(20);
}



long moving_average()
{

   //desloca os elementos do vetor de média móvel
   for(int i= n-1; i>0; i--) numbers[i] = numbers[i-1];

   numbers[0] = original; //posição inicial do vetor recebe a leitura original

   long acc = 0;          //acumulador para somar os pontos da média móvel

   for(int i=0; i<n; i++) acc = acc + numbers[i]; //faz a somatória do número de pontos


   return acc/n;  //retorna a média móvel

 
}
