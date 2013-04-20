#define STX		0x02

#define SPIN		0x00
#define STOP		0x01
#define LOAD		0x02

#define MASK		0x80

#include <OctoWS2811.h>
/* LED config */
#define HEIGHT 86
#define WIDTH  180
#define STRIPS 2
#define PAUSE 100

DMAMEM int displayMemory[HEIGHT * 6];
int drawingMemory[HEIGHT * 6];

// Our Strip
const int config =  WS2811_GRB | WS2811_800kHz ;

OctoWS2811 leds(HEIGHT, displayMemory, drawingMemory, config);

#pragma pack(push)
#pragma pack(1)
struct sphcmd_header
{
	unsigned char start;
	unsigned char id;
	unsigned char hi_len;
	unsigned char lo_len;
};

struct sphcmd_spin
{
	struct sphcmd_header hdr;
	unsigned char image_id;
	unsigned char cksum;
};

struct sphcmd_stop
{
	struct sphcmd_header hdr;
	unsigned char cksum;
};

struct sphcmd_ack
{
	struct sphcmd_header hdr;
	unsigned char cksum;
};

struct sphcmd_load
{
	struct sphcmd_header hdr;
	void *image;
	unsigned char *cksum;
};

struct sphcmd_load_ack
{
	struct sphcmd_header hdr;
	unsigned char image_id;
	unsigned char cksum;
};
#pragma pack(pop)


#define MAX_SIZE 8192

enum { START, TYPE, HI_LEN, LO_LEN, DATA, CKSUM };

void setup()
{
	/* 9600 is the baud rate for USB */
	Serial.begin(9600);
	leds.begin();
	leds.show();
}

void loop()
{
	while (Serial.available())
	{
              
                Serial.println("Entering read command...");
		readCommand();
	}
       	int i = 0, j = 0;
	int img[WIDTH][HEIGHT] = {0xFF0087};
        img[0][0] = 0x0000FF;
	/* width is the resolution for one turn */
	for (i = 0; i < WIDTH; i++) 
	{
		for (j = 0; j < HEIGHT; j++) 
		{
              	  leds.setPixel(j, img[i][j]);
             	}
		leds.show();
	}
	/* do other stuff */
}

int sendAck(unsigned char type)
{      
	if (type == LOAD)
	{
		Serial.write(STX);
                Serial.write(LOAD & MASK);
                Serial.write(0x00);
                Serial.write(0x01);
                Serial.write(0x00);
                Serial.write(0x00);
	}
	else if (type == SPIN)
	{
  		Serial.write(STX);
                Serial.write(SPIN & MASK);
                Serial.write(0x00);
                Serial.write(0x00);
                Serial.write(0x00);
	}
	else if (type == STOP)
	{
		Serial.write(STX);
                Serial.write(STOP & MASK);
                Serial.write(0x00);
                Serial.write(0x00);
                Serial.write(0x00);
	}
}

void processCommand(unsigned char type, unsigned char *data, unsigned int data_length)
{

	if (sendAck(type))
		return;

	/* process load (data) or spin or stop */
}

void readCommand()
{
	static unsigned char cmd[MAX_SIZE];
	static unsigned char byte;

	static unsigned int i = START;
	static unsigned int data_length = 0;	
	static unsigned int data_cnt = 0;

	switch (i)
	{
		case START:
			cmd[i] = Serial.read();
                        if (cmd[i] != STX)
                        {
                          Serial.println("Not STX");
                          return;
                        }
			i = TYPE;
		break;
		case TYPE:
			cmd[i] = Serial.read();
			i = HI_LEN;
		break;
		case HI_LEN:
			cmd[i] = Serial.read();
			i = LO_LEN;
		break;
		case LO_LEN:
                        uint8_t hi, lo;
                        
			cmd[i] = Serial.read();
                        hi = cmd[HI_LEN];
                        lo = cmd[LO_LEN];
			data_length = ( hi * 256) + lo;
                                Serial.print("Data length: ");
                                Serial.println(data_length);
			i = DATA;
		break;
		case DATA:
			if (data_length == 0) {
				i = CKSUM;
			} else {
				cmd[DATA + data_cnt] = Serial.read();
				data_cnt++;

				if (data_cnt == data_length) {
					data_cnt = 0;
					i = CKSUM;
				}
			}
		break;
		case CKSUM:
			cmd[DATA + data_length] = Serial.read();
			processCommand(cmd[TYPE], &cmd[DATA], data_length);
			i = START;
		break;
		default: /* Never reached */
			break;
	}
}

void showImage()
{

}
