#include <Math.h>

#include <Wire.h>

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


#include <Servo.h>
#include <Base64.h>
#include <PID_v1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
bool firstrun = true;

#define maxcommandlen 12

Servo franco,flavio,brad,blake;


class Control
{
public:
    double setpoint = 0;
    double input = 0;
    double output = 0;
    PID* pid;



};
Control roll,pitch,yaw,altitude;

void clear(char* c, int len)
{
    for(int i = 0;i < len;i++)
    {
        c[i] = 0;

    }
  
}


int devicectrl = 0;
int propctrl = 0;
char currentcmd[12];
int currentpos = 0;
void fetchcommands()
{
	while(Serial.available()>0)
	{

		char c = Serial.read();


		if(c>0)
		    {

		        if(c==17)
		        {
		        	devicectrl = 1;
		            clear(currentcmd,maxcommandlen);
		            currentpos = 0;
		            return;
		        }
		        if(c==18)
		        {
		            devicectrl = 2;
		            clear(currentcmd,maxcommandlen);
		            currentpos = 0;
		            return;
		        }
		        if(c==19)
		        {
		            devicectrl = 3;
		            clear(currentcmd,maxcommandlen);
		            currentpos = 0;
		            return;
		        }
		        if(c==20)
		        {
		            devicectrl = 4;
		            clear(currentcmd,maxcommandlen);
		            currentpos = 0;
		            return;
		        }
		        if(c==36)
		        {
		            propctrl = 1;
		            return;
		        }
		        if(c==37)
		        {
		            propctrl = 2;
		            return;

		        }
		        if(c==38)
		        {
		            propctrl = 3;
		            return;
		        }
		        if(c==39)
		        {
		            propctrl = 0;
		            return;
		        }


		        if(devicectrl!=0&&currentpos<=maxcommandlen)
		        {
		            if(c==25)
		            {
		                Control* sp;
		                bool ptrset = false;
		                if(base64_dec_len(currentcmd,currentpos+1)>8){ return;}
		                if(devicectrl==1){ sp = &roll; ptrset = true;}
		                if(devicectrl==2){ sp = &pitch; ptrset = true;}
		                if(devicectrl==3){ sp = &yaw; ptrset = true;}
		                if(devicectrl==4){ sp = &altitude; ptrset = true;}

		                if(ptrset)
		                {
		                    int decoded = 0;
		                    base64_decode((char*)&decoded,currentcmd,currentpos+1);


		                    if(propctrl==1)
		                    {
		                        sp->pid->SetTunings(decoded /1000.0,sp->pid->GetKi(),sp->pid->GetKd());
		                    }
		                    else if(propctrl ==2)
		                    {
		                       sp->pid->SetTunings(sp->pid->GetKp(),decoded /1000.0,sp->pid->GetKd());
		                    }
		                    else if(propctrl ==3)
		                    {
		                       sp->pid->SetTunings(sp->pid->GetKp(),sp->pid->GetKi(),decoded /1000.0);
		                    }
		                    else
		                    {
		                        sp->setpoint = decoded;
		                    }


		                }
		                clear(currentcmd,maxcommandlen);
		                currentpos = 0;



		            }
		            else
		            {
		            currentcmd[currentpos] = c;
		            currentpos++;
		            }

		        }
		    }

    }
}


void setup() {
  
  roll.pid = new PID(&roll.input,&roll.output,&roll.setpoint,0,0,0,DIRECT);
  yaw.pid = new PID(&yaw.input,&yaw.output,&yaw.setpoint,0,0,0,DIRECT);
  pitch.pid = new PID(&pitch.input,&pitch.output,&pitch.setpoint,0,0,0,DIRECT);
  altitude.pid = new PID(&altitude.input,&altitude.output,&altitude.setpoint,0,0,0,DIRECT);
  
  
  roll.pid->SetMode(AUTOMATIC);
  yaw.pid->SetMode(AUTOMATIC);
  pitch.pid->SetMode(AUTOMATIC);
  altitude.pid->SetMode(AUTOMATIC);
  
  franco.attach(3);
  flavio.attach(2);
  blake.attach(5);
  brad.attach(4);
  
  franco.write(0); 
  flavio.write(0);
  blake.write(0);
  brad.write(0);
  Serial.begin(9600);
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
    if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }
  delay(5000);
  franco.write(0); 
  flavio.write(0);
  blake.write(0);
  brad.write(0);
}
void update()
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  accel.getEvent(&accel_event);
  sensors_vec_t   orientation;
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    roll.input = orientation.roll;
    pitch.input = orientation.pitch;
  }
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    yaw.input = orientation.heading-180;
  }
    bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    float temperature;
    bmp.getTemperature(&temperature);
    if(firstrun)
    {
      seaLevelPressure = bmp_event.pressure;
      firstrun = false;
      
      
    }
    else
    {
    
    altitude.input = bmp.pressureToAltitude(seaLevelPressure,
                                        bmp_event.pressure,
                                       temperature); 
    }
 
  }

  altitude.input = 0;

  roll.pid->Compute();
  roll.pid->SetOutputLimits(-100,179);
  altitude.pid->Compute();
  altitude.pid->SetOutputLimits(-100,179);
  yaw.pid->Compute();
  yaw.pid->SetOutputLimits(-100,179);
  pitch.pid->Compute();
  pitch.pid->SetOutputLimits(-100,179);
  

}


void loop() {
  fetchcommands();  
  update();
  
  
  franco.write(0); 
  flavio.write(0);
  blake.write(0);
  brad.write(0);
  
  int M1 = altitude.output-roll.output+pitch.output-yaw.output;
  int M2 = altitude.output+roll.output+pitch.output+yaw.output;
  int M3 = altitude.output+roll.output-pitch.output-yaw.output;  
  int M4 = altitude.output-roll.output-pitch.output+yaw.output;  
  
  Serial.print("\nAltitude\n");
  Serial.print(altitude.output);
  Serial.print("\nRoll\n");
  Serial.print(roll.output);
  Serial.print("\nPitch\n");
  Serial.print(pitch.output);
  Serial.print("\nYaw\n");
  Serial.print(yaw.output);  
  
  
  Serial.print("\nM1\n");
  Serial.print(altitude.output-roll.output+pitch.output-yaw.output);
  Serial.print("\nM2\n");
  Serial.print(altitude.output+roll.output+pitch.output+yaw.output);
  Serial.print("\nM3\n");
  Serial.print(altitude.output+roll.output-pitch.output-yaw.output);
  Serial.print("\nM4\n");
  Serial.print(altitude.output-roll.output-pitch.output+yaw.output);  
  
  if(M1>179)
    M1 = 179;
  else if (M1<0)
    M1 = 0;  


  if(M2>179)
    M2 = 179;
  else if (M2<0)
    M2 = 0;  
  

  if(M3>179)
    M3 = 179;
  else if (M3<0)
    M3 = 0;  


  if(M4>179)
    M4 = 179;
  else if (M4<0)
    M4 = 0;  
    
  franco.write(M1); 
  flavio.write(M2);
  blake.write(M3);
  brad.write(M4);
  
  

  
  
}


