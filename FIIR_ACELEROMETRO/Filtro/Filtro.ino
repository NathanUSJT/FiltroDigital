
// Teste dos Paramêtros do Filtro Butterworth (Frequência de Corte = 3.7Hz , Frequência de amostragem = 1000Hz , Amáx Pass_Band = 0.7dB , Amin Reject_Band = 5dB em 7Hz)

float x[] = {0,0,0}; //vetores do sinal puro
float y[] = {0,0,0};//vetores do sinal filtrado
int k = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Test signal
  float t = micros()/1.0e6;
  x[0] = sin(2*PI*3*t) + 0.5*sin(2*PI*20*t); //Sinal puro de 5Hz com interferência de 50Hz

  
  float b[] = {0.00073978, 0.00147956, 0.00073978};//coeficientes do numerador da função de transferência
  float a[] = { 1.90018891, -0.90314802};//coeficientes do denominador da função de transferência
  
  y[0] = a[0]*y[1] + a[1]*y[2] +
               b[0]*x[0] + b[1]*x[1] + b[2]*x[2];//Sinal Filtrado

  
  if(k % 3 ==0)
  {

/* Essa condicional serve para reduzir o numero de vezes que o dado é transmitido na porta serial, 
   Enviar dados pela porta serial atrapalha a frequência de amostragem */
    
    //Monitor
    Serial.print(2*x[0]);
    Serial.print(" ");
    Serial.println(2*y[0]);
  }

  delay(1);
  for(int i = 1; i >= 0; i--){
    x[i+1] = x[i]; // guarda os valores obtidos nas amostras passadas de xi
    y[i+1] = y[i]; // guarda os valores obtidos nas amostras passadas de yi 
  }
  
  k = k+1;
}
