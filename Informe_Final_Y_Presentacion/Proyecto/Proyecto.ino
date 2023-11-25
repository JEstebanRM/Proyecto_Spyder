#include <Keypad.h>
#include <LiquidCrystal.h>
const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;  
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long tiempo_x=0;
long tiempo_y=0;
long distancia_x=0;
long distancia_y=0;
int pinTrigger_x=46; 
int pinEcho_x=47;
int pinTrigger_y=48; 
int pinEcho_y=49;
int pasos_x=37;
int dir_x=38;
int pasos_y=39;
int dir_y=40;
int pasos_z=41;
int dir_z=42;
int pasos_z2=43;
int dir_z2=44;
int vialFinal;
String vial="";
String vial1="";
String volumen = "";
String volumen1 = "";
int vol;
char pulsacion = 0;
int i=0;
int pos_x[2]={39,130}; // disntancia en pasos hasta los viales ubicados en las columnas del semi plano negativo y positivo. 391
int pos_y[3]={65,917,1178}; //disntancia en pasos hasta los viales ubicados en las filas 1,2 y 3, respectivamente. 656
int vol_pasos_j1[20]={226,452,678,904,1030,1356,1582,1808,2034,2261,2487,2713,2939,3165,3391,3617,3843,4069,4295,4521}; // volumen en pasos para la jeringa 1
int vol_pasos_j2[20]={121,241,362,482,603,723,844,964,1085,1205,1326,1447,1567,1688,1808,1929,2049,2170,2290,2411}; // volumen en pasos para la jeringa 2
int q; // pos_x
int w; // pos_y
int e; // vol_j1, vol_j2
byte a; // semiplanos, si a=1 entonces son los viales en el semiplano derecho, si a=0 entonces son los viales del semiplano izquierdo.
const byte Filas = 4;
const byte Cols = 4;
char Teclas[ Filas ][ Cols ] =
{
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};
byte Pins_Filas[Filas] = {28,29,30,31};
byte Pins_Cols[Cols] = {32,33,34,35};
Keypad Teclado = Keypad(makeKeymap(Teclas), Pins_Filas, Pins_Cols, Filas, Cols);

//Hasta este punto es definicion de variables necesarias
//creacion del teclado, definicion de la pantalla LCD
//definiciones para el uso de los motores

void setup() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Setup
{
  lcd.begin(16, 2);
  pinMode(37,OUTPUT);
  pinMode(38,OUTPUT);
  pinMode(39,OUTPUT);
  pinMode(40,OUTPUT);
  pinMode(41,OUTPUT);
  pinMode(42,OUTPUT);
  pinMode(43,OUTPUT);
  pinMode(44,OUTPUT);
  pinMode(pinEcho_x,INPUT);
  pinMode(pinTrigger_x,OUTPUT);
  pinMode(pinEcho_y,INPUT);
  pinMode(pinTrigger_y,OUTPUT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Calibracion en X

void calibracion_x()
{
  digitalWrite(pinTrigger_x,LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrigger_x,HIGH);                  // Valor inicial de distancia para el sensor del eje x
  delayMicroseconds(10); 
  digitalWrite(pinTrigger_x,LOW);
  tiempo_x=pulseIn(pinEcho_x,HIGH);
  distancia_x=(tiempo_x*0.0343)/2;


while(distancia_x >5)
{
  digitalWrite(pinTrigger_x,LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrigger_x,HIGH); 
  delayMicroseconds(10);                              // Actualiza el valor de la distancia y mueva el motor un paso a la vez hasta que este en la distancia minima.
  digitalWrite(pinTrigger_x,LOW);
  tiempo_x=pulseIn(pinEcho_x,HIGH);
  distancia_x=(tiempo_x*0.0343)/2;
  digitalWrite(dir_x,LOW);
  for(int x=0; x<1; x++)
  {
    digitalWrite(pasos_x,HIGH);
    delay(50);
    digitalWrite(pasos_x,LOW);
    delay(50);
  }
} 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Calibracion en Y

void calibracion_y()
{
  digitalWrite(pinTrigger_y,LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrigger_y,HIGH);                     // Valor inicial de distancia para el sensor del eje y
  delayMicroseconds(10); 
  digitalWrite(pinTrigger_y,LOW);
  tiempo_y=pulseIn(pinEcho_y,HIGH);
  distancia_y=(tiempo_y*0.0343)/2;


while(distancia_y >5)
{
  digitalWrite(pinTrigger_y,LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrigger_y,HIGH); 
  delayMicroseconds(10);                                   // Actualiza el valor de la distancia y mueva el motor un paso a la vez hasta que este en la distancia minima.
  digitalWrite(pinTrigger_y,LOW);
  tiempo_y=pulseIn(pinEcho_y,HIGH);
  distancia_y=(tiempo_y*0.0343)/2;
  digitalWrite(dir_y,LOW);
  for(int x=0; x<1; x++)
  {
    digitalWrite(pasos_y,HIGH);
    delay(50);
    digitalWrite(pasos_y,LOW);
    delay(50);
  }
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Movimiento Beaker

void movimiento_beaker()
{
  digitalWrite(dir_x,HIGH);
  for(int x=0; x<=14; x++)//1432 pasos reales.
  {
    digitalWrite(pasos_x,HIGH);
    delay(50);
    digitalWrite(pasos_x,LOW);
    delay(50);                                                        // Pasos necesarios para mover el cabezal desde la posicion de ref hasta el beaker.
  }

  digitalWrite(dir_y,LOW);
  for(int x=0; x<=14; x++)//1406 pasos reales.
  {
    digitalWrite(pasos_y,HIGH);
    delay(50);
    digitalWrite(pasos_y,LOW);
    delay(50);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Movimiento Jeringa 1
////////////////////////////////////////////////////// delay de 1.5 para que funcione a 35RPM ///////////////////////////////////////////////////////////////////
void mov_j1()
{
  lcd.setCursor(2,0);                                  
  lcd.print("Ejecutando");                         
  digitalWrite(dir_z2,LOW); //sube
  for(int z = 0; z <=vol_pasos_j1[e]; z++)
  {                                                                         //Llena la jerinfa con el volumen almacenado en la variable e.
    digitalWrite(pasos_z2,HIGH);                                            //Todos los delay tienen un valor de 50 esto es porque es el valor minimo para que la simulacion funcione
    delay(50);                                                              //sin embargo, para su implementacion es necesario usar los valores puestos al inicio de la funcion.
    digitalWrite(pasos_z2,LOW);
    delay(50);
  }

  digitalWrite(dir_x,a);
  for(int x = 0; x <= pos_x[q]; x++)
  {
    digitalWrite(pasos_x,HIGH);
    delay(50);                                                              //Mueve el cabezal en la direccion de x, la cantidad de pasos y el sentido de giro es dado por las variables
    digitalWrite(pasos_x,LOW);                                              //a y q, donde a controla el sentido de giro y q el valor en el vector pos_x.
    delay(50);
  }
  
  digitalWrite(dir_y,HIGH); //positivo
  for(int y = 0; y <=pos_y[w]; y++)
  {
    digitalWrite(pasos_y,HIGH);
    delay(50);                                                             //Mueve el cabezal en la direccion de y, la cantidad de pasos es dado por la variable
    digitalWrite(pasos_y,LOW);                                             //w, donde w es el valor en el vector pos_y.
    delay(50);
  }
  
  digitalWrite(dir_z2,HIGH); //Baja                                       //Desde aqui hasta el final de funcion, se realiza el proceso inverso al mencionado anteriormente.
  for(int z = 0; z <=vol_pasos_j1[e]; z++)
  {
    digitalWrite(pasos_z2,HIGH);
    delay(50);
    digitalWrite(pasos_z2,LOW);
    delay(50);
  }                                                                   

  delay(1000);

  digitalWrite(dir_x,!a);
  for(int x = 0; x <= pos_x[q]; x++)
  {
    digitalWrite(pasos_x,HIGH);
    delay(50);
    digitalWrite(pasos_x,LOW);
    delay(50);
  }

  digitalWrite(dir_y,LOW); //negativo
  for(int y = 0; y <=pos_y[w]; y++)
  {
    digitalWrite(pasos_y,HIGH);
    delay(50);
    digitalWrite(pasos_y,LOW);
    delay(50);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Moviento jeringa 2
//////////////////////////////////////////////////////////// delay de 8 para que funcione a 19RPM/////////////////////////////////////////////////////////////
void mov_j2()
{
  lcd.setCursor(2,0);                                  
  lcd.print("Ejecutando");
  digitalWrite(dir_z2,LOW); //sube
  for(int z = 0; z <=vol_pasos_j2[e]; z++)
  {                                                                         //Este funcion es igual a la anterior con la unica diferencia que se usa el vecto de volumen vol_pasos_j2.
    digitalWrite(pasos_z2,HIGH);                                            //esto ultimo porque se tiene activa la jeringa 2.
    delay(50);
    digitalWrite(pasos_z2,LOW);
    delay(50);
  }

  digitalWrite(dir_x,a);
  for(int x = 0; x <= pos_x[q]; x++)
  {
    digitalWrite(pasos_x,HIGH);
    delay(50);
    digitalWrite(pasos_x,LOW);
    delay(50);
  }
  
  digitalWrite(dir_y,HIGH); //positivo
  for(int y = 0; y <=pos_y[w]; y++)
  {
    digitalWrite(pasos_y,HIGH);
    delay(50);
    digitalWrite(pasos_y,LOW);
    delay(50);
  }
  
  digitalWrite(dir_z2,HIGH); //Baja
  for(int z = 0; z <=vol_pasos_j2[e]; z++)
  {
    digitalWrite(pasos_z2,HIGH);
    delay(50);
    digitalWrite(pasos_z2,LOW);
    delay(50);
  }

  delay(1000);

  digitalWrite(dir_x,!a); //derecha
  for(int x = 0; x <= pos_x[q]; x++)
  {
    digitalWrite(pasos_x,HIGH);
    delay(50);
    digitalWrite(pasos_x,LOW);
    delay(50);
  }

  digitalWrite(dir_y,LOW); //negativo
  for(int y = 0; y <=pos_y[w]; y++)
  {
    digitalWrite(pasos_y,HIGH);
    delay(50);
    digitalWrite(pasos_y,LOW);
    delay(50);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Loop
void loop ()
{ 
  switch (i)
  {

    case 0:
      lcd.setCursor(3,0);
      lcd.print("Calibrando");
      calibracion_x();
      calibracion_y();
      movimiento_beaker();
      i=1;
      break;

    case 1:
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Bienvenido");
      delay(1000);
      lcd.clear();
      
   while(pulsacion != 'B' && pulsacion != 'A' )               //Inicialmente se tiene una maquina de estados finitos creada con el comando Switch
   {                                                          //dicha maquina inicia con el caso 0 en donde se realiza la calibracion.
      lcd.setCursor(0,0);                                    
      lcd.print("Modo demo=> A");                            
      lcd.setCursor(0,1);                                     //Luego, en el caso 1, se da un mensaje de bienvenida y se abre el menu principal
      lcd.print("Modo normal=> B");                           //en donde se puede escoger dos opciones, modo demo o modo normal, en cada caso, se irá a un caso distinto.
      pulsacion = Teclado.getKey();
      
      if(pulsacion=='A')
      {
        lcd.clear();
        i=2;
      }
      if(pulsacion=='B')
      {
        lcd.clear();
        i=3;
      }
      
   }
      break;

  case 2:  
          while(pulsacion != '1' && pulsacion != '2' )             
           {                                                     
              lcd.setCursor(0,0);                                  
              lcd.print("Jeringa 1=> 1");                                 //Si se escoje el modo demo, este caso se activa, preguntando que jeringa se va a usar
              lcd.setCursor(0,1);                                         //Luego, se reescriben algunas variables para que las funciones de movimiento se configuren y operen
              lcd.print("Jeringa 2=> 2");
              pulsacion = Teclado.getKey();
              
              if(pulsacion=='1')
              {
                lcd.clear();
                lcd.setCursor(2,0);                                  
                lcd.print("Ejecutando");  
                a=0;
                q=0;
                w=0;
                e=0;
                mov_j1();
                i=1;
                break;
              }
              if(pulsacion=='2')
              {
                lcd.clear();
                lcd.setCursor(2,0);                                  
                lcd.print("Ejecutando");                                // el modo demo esta configurado para cargar 0.1 mL de liquido y depositarlo en el vial numero 1.
                a=0;
                q=0;
                w=0;
                q=0;
                e=0;
                mov_j2();
                i=1;
                break;
              }
              
           }
           break;
           
 case 3: pulsacion = 0;                                             
         vial = "";
                                                                      // Si se selecciona el modo normal, se activa este caso en donde se requiere introducir el vial que se desea usar.
    while(pulsacion == 0 )
    {
      pulsacion = Teclado.getKey();
      lcd.setCursor(0,0);
      lcd.print("Seleccione Vial");
      lcd.setCursor(0,1);
      lcd.print("entre 01 y 12.");

    }
      lcd.clear();
      vial += pulsacion;
      lcd.setCursor(0,0);
      lcd.print("Vial:");
      lcd.setCursor(0,2);
      lcd.print(vial.toInt());

      pulsacion = 0;
      vial1 = "";
      vial1 = vial1.toInt(); 
      while(pulsacion == 0 ){
      pulsacion = Teclado.getKey();}
      vial1 += pulsacion;
      lcd.setCursor(1,2);
      lcd.print(vial1.toInt());
      delay(2000);
      lcd.clear();
      vialFinal= (int)(vial.toInt()*10) + (int)(vial1.toInt());                                           //es el valor en numero entero que representa la posicion en el vector de pasos tanto en x como en y.
      if(vialFinal==1 ||vialFinal==2 || vialFinal==5 || vialFinal==6 || vialFinal==9 || vialFinal==10)   //Viales a la izquierda del Beaker.
      {
        a=0;
        if(vialFinal==1|| vialFinal==5 || vialFinal==9)
        {
          q=1;
        }
        if(vialFinal==2|| vialFinal==6 || vialFinal==10)
        {
          q=0;
        }
        if(vialFinal==1|| vialFinal==2)
        {
          w=0;
        }
        if(vialFinal==5|| vialFinal==6)
        {
          w=1;
        }
         if(vialFinal==9|| vialFinal==10)
        {
          w=3;
        }
      }
      if(vialFinal==3 ||vialFinal==4 || vialFinal==7 || vialFinal==8 || vialFinal==11 || vialFinal==12)   //Viales a la derecha del Beaker.
      {
        a=1;
        if(vialFinal==4|| vialFinal==8 || vialFinal==12)
        {
          q=1;
        }
        if(vialFinal==3|| vialFinal==7 || vialFinal==11)
        {
          q=0;
        }
        if(vialFinal==3|| vialFinal==4)
        {
          w=0;
        }
        if(vialFinal==7|| vialFinal==8)
        {
          w=1;
        }
         if(vialFinal==11|| vialFinal==12)
        {
          w=3;
        }
      }
      i=4;
      break;


  case 4:  pulsacion = 0;
           volumen = "";                            //luego de seleccionar el vial, se le pide al usuario ingresar el valor de volumen que desea depositar en el vial.

    while(pulsacion == 0 )
    {
      pulsacion = Teclado.getKey();
      lcd.setCursor(0,0);
      lcd.print("Seleccione volumen");
      lcd.setCursor(0,1);
      lcd.print("entre 0.1 y 2.0 ml.");

    }
      lcd.clear();
      volumen += pulsacion;
      lcd.setCursor(0,0);
      lcd.print("Volumen:");
      lcd.setCursor(0,2);
      lcd.print(volumen.toInt());
      lcd.setCursor(1,2);
      lcd.print(".");

      pulsacion = 0;
      volumen1 = "";
      volumen1 = volumen1.toInt(); 
      while(pulsacion == 0 ){
      pulsacion = Teclado.getKey();}
      volumen1 += pulsacion;
      lcd.setCursor(2,2);
      lcd.print(volumen1.toInt());
      lcd.setCursor(4,2);
      lcd.print("ml");
      delay(2000);
      lcd.clear();
      vol= (int)(volumen.toInt()*10) + (int)volumen1.toInt();        //valor en numero entero de la cantidad de volumen seleccionado, dicho valor representa una posicion en el vector 
      e=vol-1;                                                       //Vol_pasos j1 o j2, ya que las posiciones estan ordenadas de menor cantidad de volumen a mayor.
      i=5;
      break;
      
  case 5:   pulsacion=0;
            while(pulsacion != '1' && pulsacion != '2' )             //Le pide al usuario la seleccion de la jeringa que esta usando, dependiendo de cada una se va a un caso disntinto.
            {                                                        // solo se admiten jeringas de 5 mL==>j1 y de 10 mL==>j2
              lcd.setCursor(0,0);                                  
              lcd.print("Jeringa 1=> 1");                           
              lcd.setCursor(0,1);                                    
              lcd.print("Jeringa 2=> 2");
              pulsacion = Teclado.getKey();
              if(pulsacion=='1')
              {
                lcd.clear();
                i=6;
                break;
              }
              if(pulsacion=='2')
              {
                lcd.clear();
                i=7;
                break;
              }
           }
            break;
 case 6:   mov_j1();
           while(pulsacion != 'C' && pulsacion != 'D' )             //Si se escoje la jeringa uno, se hace uso de la funcion mov_j1 junto con los parametros definidos en casos anteriores
            {                                                       //es decir, a,q,w,e
              lcd.setCursor(0,0);                                  
              lcd.print("Reiniciar=> C");                           
              lcd.setCursor(0,1);                                    
              lcd.print("Apagar=> D");
              pulsacion = Teclado.getKey();
              if(pulsacion=='C')
              {
                lcd.clear();
                i=1;
                break;                                             // y, ademas, se le pregunta al usuario si desea reiniciar el proceso o apagar la planta
              }
              if(pulsacion=='D')                                  // si se reiniciar, el proceso vuelve al caso 1, pues siempre vuelve a la posicion del beaker entonces no es necesario
              {                                                   // calibrar nuevamente.
                lcd.clear();
                i=8;
                break;
              }
            }
            break;
 case 7:   mov_j2();                                         //Si se escoje la jeringa uno, se hace uso de la funcion mov_j2 junto con los parametros definidos en casos anteriores
           while(pulsacion != 'C' && pulsacion != 'D' )     //es decir, a,q,w,e        
            {                                                     
              lcd.setCursor(0,0);                                  
              lcd.print("Reiniciar=> C");                           
              lcd.setCursor(0,1);                                    
              lcd.print("Apagar=> D");
              pulsacion = Teclado.getKey();
              if(pulsacion=='C')
              {
                lcd.clear();
                pulsacion=0;
                i=1;
                break;
              }                                                // y, ademas, se le pregunta al usuario si desea reiniciar el proceso o apagar la planta
              if(pulsacion=='D')                               // si se reiniciar, el proceso vuelve al caso 1, pues siempre vuelve a la posicion del beaker entonces no es necesario
              {                                                // calibrar nuevamente.
                lcd.clear();                                    
                pulsacion=0;
                i=8;
                break;
              }
            }
            break;

 case 8: lcd.clear();
         lcd.setCursor(2,0);                                  //Si seleccion apagar, este caso se activa y nunca sale de el a menos que se reinicie el arduino, es decir, prender de nuevo
         lcd.print("Apague todo");                            //el sistema.
}
}
