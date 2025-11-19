#include <Servo.h>

// Pines para el sensor TCS3200
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define OUT 6

// Pines para el driver L298N
#define ENA 9
#define IN1 10
#define IN2 11

// Pin para el servomotor
#define SERVO_PIN 12

// Variables para colores
int valorR = 0;
int valorG = 0;
int valorB = 0;

// Umbrales calibrados
const int ROJO_R = 700;
const int ROJO_G = 730;
const int ROJO_B = 700;
const int AZUL_R = 1000;
const int AZUL_B = 600;
const int VERDE_R = 900;
const int VERDE_G = 680;
const int VERDE_B = 680;

// Crear objeto servo
Servo miServo;

// Variables de control
bool motorEncendido = false;
String colorActual = "";
String colorAnterior = "";
int velocidadMotor = 150;
const int VELOCIDAD_MINIMA = 100;
const int VELOCIDAD_MAXIMA = 255;

// 🆕 CONTADORES DE UNIDADES POR COLOR
int contadorRojo = 0;
int contadorVerde = 0;
int contadorAzul = 0;
int contadorTotal = 0;

void setup() {
  // Configurar motor
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  // Configurar sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);
  
  // Configurar servo
  miServo.attach(SERVO_PIN);
  
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  Serial.begin(9600);
  
  // Iniciar con motor apagado
  detenerMotor();
  miServo.write(90);
  
  Serial.println("🤖 SISTEMA CLASIFICADOR CON CONTADOR");
  Serial.println("==================================================");
  Serial.println("🎯 CONTADORES INICIALES:");
  Serial.println("   🔴 ROJO: 0 unidades");
  Serial.println("   🟢 VERDE: 0 unidades");
  Serial.println("   🔵 AZUL: 0 unidades");
  Serial.println("   📊 TOTAL: 0 unidades");
  Serial.println("==================================================");
  Serial.println("🎮 COMANDOS:");
  Serial.println("   e - ENCENDER motor");
  Serial.println("   d - APAGAR motor");
  Serial.println("   1-4 - Control de velocidad");
  Serial.println("   c - Mostrar CONTADORES actuales");
  Serial.println("   r - RESETEAR contadores");
  Serial.println("   s - Estado del sistema");
  Serial.println("==================================================");
}

void loop() {
  // 1. Verificar comandos del usuario
  if (Serial.available()) {
    char comando = Serial.read();
    procesarComando(comando);
  }
  
  // 2. Leer sensor
  static unsigned long ultimaLectura = 0;
  if (millis() - ultimaLectura >= 500) {
    leerColor();
    
    Serial.print("📊 R:");
    Serial.print(valorR);
    Serial.print(" G:");
    Serial.print(valorG);
    Serial.print(" B:");
    Serial.print(valorB);
    
    String colorDetectado = clasificarColor();
    Serial.print(" -> 🎨 ");
    Serial.println(colorDetectado);
    
    // 🆕 DETECTAR NUEVO COLOR Y CONTAR
    if (colorDetectado != "DESCONOCIDO" && colorDetectado != colorAnterior) {
      moverServo(colorDetectado);
      contarUnidad(colorDetectado);  // 🆕 CONTAR LA UNIDAD
      colorAnterior = colorDetectado;
      colorActual = colorDetectado;
    }
    
    ultimaLectura = millis();
  }
  
  delay(100);
}

// 🆕 FUNCIÓN PARA CONTAR UNIDADES
void contarUnidad(String color) {
  if (color == "ROJO") {
    contadorRojo++;
    contadorTotal++;
    Serial.println("🔴 +1 UNIDAD ROJA");
    mostrarResumenParcial();
  }
  else if (color == "VERDE") {
    contadorVerde++;
    contadorTotal++;
    Serial.println("🟢 +1 UNIDAD VERDE");
    mostrarResumenParcial();
  }
  else if (color == "AZUL") {
    contadorAzul++;
    contadorTotal++;
    Serial.println("🔵 +1 UNIDAD AZUL");
    mostrarResumenParcial();
  }
}

// 🆕 FUNCIÓN PARA MOSTRAR RESUMEN PARCIAL
void mostrarResumenParcial() {
  Serial.print("   📊 ACUMULADO: ");
  Serial.print(contadorRojo);
  Serial.print(" rojas, ");
  Serial.print(contadorVerde);
  Serial.print(" verdes, ");
  Serial.print(contadorAzul);
  Serial.print(" azules");
  Serial.print(" → TOTAL: ");
  Serial.println(contadorTotal);
}

// 🆕 FUNCIÓN PARA MOSTRAR CONTADORES COMPLETOS
void mostrarContadoresCompletos() {
  Serial.println("==================================================");
  Serial.println("📊 CONTADOR FINAL DE UNIDADES");
  Serial.println("==================================================");
  Serial.print("   🔴 ROJO:    ");
  Serial.print(contadorRojo);
  Serial.println(" unidades");
  
  Serial.print("   🟢 VERDE:   ");
  Serial.print(contadorVerde);
  Serial.println(" unidades");
  
  Serial.print("   🔵 AZUL:    ");
  Serial.print(contadorAzul);
  Serial.println(" unidades");
  
  Serial.println("   ─────────────────────────");
  Serial.print("   📊 TOTAL:   ");
  Serial.print(contadorTotal);
  Serial.println(" unidades");
  Serial.println("==================================================");
}

void procesarComando(char comando) {
  switch(comando) {
    case '1': 
      velocidadMotor = 100;
      Serial.println("🚗 Velocidad 100");
      if (motorEncendido) encenderMotor();
      break;
      
    case '2': 
      velocidadMotor = 150;
      Serial.println("🚀 Velocidad 150");
      if (motorEncendido) encenderMotor();
      break;
      
    case '3': 
      velocidadMotor = 200;
      Serial.println("⚡ Velocidad 200");
      if (motorEncendido) encenderMotor();
      break;
      
    case '4': 
      velocidadMotor = 255;
      Serial.println("🔥 Velocidad 255");
      if (motorEncendido) encenderMotor();
      break;
      
    case '+': 
      velocidadMotor = min(255, velocidadMotor + 20);
      Serial.print("📈 Velocidad: ");
      Serial.println(velocidadMotor);
      if (motorEncendido) encenderMotor();
      break;
      
    case '-': 
      velocidadMotor = max(100, velocidadMotor - 20);
      Serial.print("📉 Velocidad: ");
      Serial.println(velocidadMotor);
      if (motorEncendido) encenderMotor();
      break;
      
    case 'e': case 'E':  // ENCENDER MOTOR
      if (!motorEncendido) {
        encenderMotor();
        motorEncendido = true;
        Serial.print("🚀 MOTOR ENCENDIDO - Velocidad: ");
        Serial.println(velocidadMotor);
      } else {
        Serial.println("⚠️  Motor YA encendido");
      }
      break;
      
    case 'd': case 'D':  // APAGAR MOTOR
      if (motorEncendido) {
        detenerMotor();
        motorEncendido = false;
        Serial.println("🛑 MOTOR APAGADO");
      } else {
        Serial.println("⚠️  Motor YA apagado");
      }
      break;
      
    case 'c': case 'C':  // 🆕 MOSTRAR CONTADORES COMPLETOS
      mostrarContadoresCompletos();
      break;
      
    case 'r': case 'R':  // 🆕 RESETEAR CONTADORES
      contadorRojo = 0;
      contadorVerde = 0;
      contadorAzul = 0;
      contadorTotal = 0;
      Serial.println("🔄 CONTADORES REINICIADOS A CERO");
      Serial.println("   🔴 ROJO: 0 unidades");
      Serial.println("   🟢 VERDE: 0 unidades");
      Serial.println("   🔵 AZUL: 0 unidades");
      Serial.println("   📊 TOTAL: 0 unidades");
      break;
      
    case 's': case 'S':  // ESTADO
      Serial.println("📊 ESTADO ACTUAL DEL SISTEMA:");
      Serial.print("   Motor: ");
      Serial.println(motorEncendido ? "🟢 ENCENDIDO" : "🔴 APAGADO");
      Serial.print("   Velocidad: ");
      Serial.println(velocidadMotor);
      Serial.print("   Último color detectado: ");
      Serial.println(colorActual != "" ? colorActual : "Ninguno");
      Serial.print("   Unidades clasificadas: ");
      Serial.println(contadorTotal);
      break;
      
    default:
      Serial.println("❌ Comando no válido");
      Serial.println("💡 Comandos: e=Encender, d=Apagar, 1-4=Velocidad");
      Serial.println("             c=Contadores, r=Reset, s=Estado");
      break;
  }
}

void moverServo(String color) {
  Serial.print("📍 Servo movido a: ");
  if (color == "ROJO") {
    miServo.write(90);
    Serial.println("90° (ROJO)");
  } 
  else if (color == "AZUL") {
    miServo.write(160);
    Serial.println("160° (AZUL)");
  }
  else if (color == "VERDE") {
    miServo.write(30);
    Serial.println("30° (VERDE)");
  }
}

void encenderMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, velocidadMotor);
}

void detenerMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

void leerColor() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  valorR = pulseIn(OUT, LOW);
  delay(10);
  
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  valorG = pulseIn(OUT, LOW);
  delay(10);
  
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  valorB = pulseIn(OUT, LOW);
  delay(10);
}

String clasificarColor() {
  if (valorR < ROJO_R && valorG > ROJO_G && valorB > ROJO_B) {
    return "ROJO";
  }
  else if (valorR > AZUL_R && valorB < AZUL_B) {
    return "AZUL";
  }
  else if (valorR > VERDE_R && valorG < VERDE_G && valorB > VERDE_B) {
    return "VERDE";
  }
  else {
    return "DESCONOCIDO";
  }
}