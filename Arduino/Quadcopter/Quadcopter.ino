
#include <Servo.h>
#include <Base64.h>
#include <PID_v1.h>

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
  
  franco.attach(6);
  flavio.attach(7);
  blake.attach(8);
  brad.attach(9);
  
  franco.write(0); 
  flavio.write(0);
  blake.write(0);
  brad.write(0);
  Serial.begin(9600);
  delay(500);
}
void update()
{

  roll.pid->Compute();
  altitude.pid->Compute();
  yaw.pid->Compute();
  pitch.pid->Compute();
}


void loop() {
  fetchcommands();  
  update();
  franco.write(80); //roll.setpoint); 
  flavio.write(80);//altitude.setpoint);
  blake.write(80);//yaw.setpoint);
  brad.write(80);//pitch.setpoint);
  
  
  
}


