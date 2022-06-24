//Filtro Butterworth (Frequência de Corte = 3.7Hz , Frequência de amostragem = 1000Hz , Amáx Pass_Band = 0.7dB , Amin Reject_Band = 5dB em 7Hz)

#include<Wire.h>
const int ENDERECO_SENSOR=0x68;  
int  acelX,original,filtrado,mediaMovel;
long moving_average();
int k = 0;
#define      n     50       //número de pontos da média móvel 
int       numbers[n];

float x[] = {0,0,0}; //vetores do sinal puro
float y[] = {0,0,0};//vetores do sinal filtrado
int i=1;

void setup()
{
  Serial.begin(9600);

  //Inicializa a biblioteca Wire
  Wire.begin();
  Wire.beginTransmission(ENDERECO_SENSOR);
  Wire.write(0x6B); 
   
  //Inicializa o sensor
  Wire.write(0); 
  Wire.endTransmission(true);
}



void loop()
{
  //Começa uma transmissão com o sensor
  Wire.beginTransmission(ENDERECO_SENSOR);
  Wire.write(0x3B);  
 /*Finaliza e transmite os dados para o sensor. O false fará com que seja enviado uma mensagem 
   de restart e o barramento não será liberado*/
  Wire.endTransmission(false);
  Wire.requestFrom(ENDERECO_SENSOR, 2, true);//Socilita 2 primeiros bytes do sensor   
  acelX = Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)  
  original = acelX;
  mediaMovel = moving_average();
  x[0] = acelX;//Aloca valores do sensor como entrada da Função de transferência
  
  //Frequencia de Amostragem de 50Hz
  //float b[] = {0.04902202, 0.09804405, 0.04902202};//coeficientes do numerador da função de transferência
  //float a[] = {  1.16211361, -0.3582017};//coeficientes do denominador da função de transferência

  //Frequência de Amostragem de 500Hz
  float b[] = {0.00073978, 0.00147956, 0.00073978};//coeficientes do numerador da função de transferência
  float a[] = { 1.90018891, -0.90314802};//coeficientes do denominador da função de transferência


    
   y[0] = a[0]*y[1] + a[1]*y[2] + b[0]*x[0] + b[1]*x[1] + b[2]*x[2];//Sinal Filtrado

  delay(1);
  
  for(int i = 1; i >= 0; i--)
  {
    x[i+1] = x[i]; // guarda os valores obtidos nas amostras passadas de xi
    y[i+1] = y[i]; // guarda os valores obtidos nas amostras passadas de yi 
  }
  
  delay(10);

  Serial.print("Acelerômetro X = "); 
  Serial.print(acelX);//Sinal Acelerômetro Puro
  Serial.print(",");
  Serial.print(y[0]);
  Serial.print(",");
  Serial.print(mediaMovel);//Sinal Filtrado
  Serial.println();
 
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
