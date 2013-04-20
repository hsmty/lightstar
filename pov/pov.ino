#define STX		0x02

#define SPIN		0x00
#define STOP		0x01
#define LOAD		0x02

#define MASK		0x80

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

#define MAX_SIZE 65535

enum { START, TYPE, HI_LEN, LO_LEN, DATA, CKSUM };

void setup()
{
	 Serial.begin(9600);
}

void loop()
{
	while (Serial.available())
	{
		readCommand();
	}

	/* do other stuff */
}

int sendAck(unsigned char type)
{
	if (type == LOAD)
	{
		struct sphcmd_load_ack ack;
		ack.hdr.start = STX;
		ack.hdr.id = LOAD & MASK;
		ack.hdr.hi_len = 0x00;
		ack.hdr.lo_len = 0x01;
		ack.image_id = 0x01;
		ack.cksum = 0x00;
		Serial.write(&ack, sizeof(struct sphcmd_load_ack));
	}
	else if (type == SPIN)
	{
		struct sphcmd_ack ack;
		ack.hdr.start = STX;
		ack.hdr.id = SPIN & MASK;
		ack.hdr.hi_len = 0x00;
		ack.hdr.lo_len = 0x00;
		ack.cksum = 0x00;

		Serial.write(&ack, sizeof(struct sphcmd_ack));
	}
	else if (type == STOP)
	{
		struct sphcmd_ack ack;
		ack.hdr.start = STX;
		ack.hdr.id = STOP & MASK;
		ack.hdr.hi_len = 0x00;
		ack.hdr.lo_len = 0x00;
		ack.cksum = 0x00;
		Serial.write(&ack, sizeof(struct sphcmd_ack));
	}
}

void processCommand(unsigned char type, unsigned char *data, unsigned int data_length)
{
	if (sendAck(type))
		return;

	/* process load (data) or spin or stop */
}

int readCommand()
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
			cmd[i] = Serial.read();
			data_length = (cmd[HI_LEN] * 256) + cmd[LO_LEN];
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
