📘 README – Sistema Clasificador de Colores con TCS3200, L298N y Servo
🔧 Descripción del Proyecto
Este proyecto implementa un sistema automático de clasificación de objetos por color, utilizando un sensor TCS3200, un servomotor y un motor DC controlado mediante el driver L298N.
El Arduino detecta el color del objeto (rojo, verde o azul), mueve la compuerta con un servomotor para dirigir el objeto a su contenedor correspondiente y, adicionalmente, lleva un contador de unidades clasificadas por color.

Es ideal para aplicaciones educativas, automatización básica, logística y prototipos industriales.

🎨 Componentes Principales

Arduino UNO

Sensor de color TCS3200

Driver L298N

Motor DC de banda transportadora

Servomotor SG90 o MG90S

Fuente de alimentación 12V para el motor

Cables y protoboard

🧠 Funciones Principales del Código
✔ Lectura del color

El sensor TCS3200 mide la frecuencia asociada a los filtros RGB.
El programa captura estas lecturas y las clasifica según umbrales calibrados.

✔ Clasificación automática

Dependiendo del color detectado:

Rojo → Servo a 90°

Azul → Servo a 160°

Verde → Servo a 30°

✔ Control del motor DC

El usuario puede:

Encender/apagar motor

Ajustar velocidad (niveles 1–4)

Modificar velocidad con + y –

✔ Contador de unidades

El código incluye contadores para:

🔴 Rojo

🟢 Verde

🔵 Azul

📊 Total de objetos clasificados

Los contadores pueden:

Mostrarse en cualquier momento

Reiniciarse con un comando

Registrarse en el monitor serial

✔ Interacción por Monitor Serial

El usuario puede enviar comandos:

Comando	Acción
e	Encender motor
d	Apagar motor
1–4	Cambiar velocidad
+ / -	Aumentar o disminuir velocidad
c	Ver contadores
r	Reiniciar contadores
s	Ver estado del sistema
🔌 Conexiones del Hardware
Sensor TCS3200
Pin Sensor	Arduino
S0	2
S1	3
S2	4
S3	5
OUT	6
Driver L298N
Pin L298N	Arduino
ENA	9 (PWM)
IN1	10
IN2	11

El motor DC se conecta a OUT1–OUT2 del L298N.

Servomotor

Señal → Pin 12

Vcc y GND a 5V (ideal fuente externa si es MG90S)

⚙️ Calibración del Sensor

Los valores umbral se definieron mediante lecturas reales del sensor:

const int ROJO_R = 700;
const int ROJO_G = 730;
const int ROJO_B = 700;
const int AZUL_R = 1000;
const int AZUL_B = 600;
const int VERDE_R = 900;
const int VERDE_G = 680;
const int VERDE_B = 680;


Puedes ajustarlos según las condiciones de luz y tono de los objetos.

📁 Estructura del Código
Funciones principales

leerColor() → Lee valores RGB

clasificarColor() → Determina si es ROJO, VERDE o AZUL

moverServo() → Posiciona el servo según el color

encenderMotor() / detenerMotor() → Control de la banda

contarUnidad() → Suma 1 al color detectado

mostrarContadoresCompletos() → Imprime resumen

procesarComando() → Gestiona comandos del usuario

🛠️ Recomendaciones de Uso

Mantener iluminación estable durante la lectura.

El motor DC puede girar muy rápido incluso con PWM →
se recomienda usar un motor de menos RPM o una reducción mecánica.

No alimentar el servomotor y el motor DC desde el Arduino.

Mantener distancia constante entre sensor y objeto (~2 cm).

Revisar calibración si cambian materiales o colores.

🧪 Resultados Esperados

Detección estable de colores primarios.

Conteo preciso de unidades clasificadas.

Movimiento adecuado de la compuerta según color.

Control manual total del motor vía monitor serial.

📜 Licencia

Este código puede ser usado libremente con fines académicos, de investigación y prototipado.
