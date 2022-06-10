#include <Wire.h>
#include <math.h>

#define TSAMPLE 50000

const int MPU = 0x68; // Endereço I2C do sensor
int AcX,AcY,AcZ, Tmp, GyX, GyY, GyZ;// Dados lidos pelos sensores
unsigned long tempoAnterior = 0, tempoAtual = 0; //Variaveis de tempo
float ang_accel;// Ângulo calculado a partir do acelerômetro

void deslocaVetor(float *vetorAddr, int tam, float valor)
{
    for(int k = tam-1; k > 0; k --)
    {
        *(vetorAddr+k) = *(vetorAddr+k-1);
    }
    *vetorAddr = valor;
}

float filtroPB(float x)
{
    static float y_pass[2] = {0,0}, x_pass[2] = {0,0};
    const float a = 0.65, b = 0.7;
    float y = (a+b)* y_pass[1] + (1-a-b+a*b)* x_pass[1];
    deslocaVetor(y_pass,2,y);
    deslocaVetor(x_pass,2,x);
    return y;
}

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true); 
}

void loop()
{
    tempoAtual = micros();
    if (tempoAtual - tempoAnterior >= TSAMPLE)
    {
        //Adquire dados do sensor
        Wire.beginTransmission(MPU);
        Wire.write(0x3B); //Regitrador 0x3B(ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU,14,true);
        AcX = Wire.read() <<8 | Wire.read();//0x3B(ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        AcY = Wire.read() <<8 | Wire.read(); //0x3D(ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)

    // CALCULA OS ˆANGULOS
    ang_accel = ATAN2(-AcY,AcX)*180 / PI;
    