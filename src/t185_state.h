#ifndef T185STATE_H
#define T185STATE_H

uint32_t rndu32_lcg_seed;
uint32_t __not_in_flash_func(rndu32)()
{
	rndu32_lcg_seed = 1664525 * rndu32_lcg_seed + 1013904223;
	return rndu32_lcg_seed;
}

// True with probability (prob/1024)
bool rand_bit(int prob)
{
	return (rndu32() >> 22)<prob;
}



typedef struct
{
	uint32_t count, increment;
} clock;

void clock_init(clock *c)
{
	c->count = 0;
	c->increment = 0;
}


uint32_t clock_get_incr_from_hz(clock *c, float f)
{
	return 89478.48533f * f;
}

void clock_set_freq_hz(clock *c, float f)
{
	// tick called at 48kHz
	// wraps at 2^32 = 4,294,967,296
	// increment is linear in Hz, with 89478.485333 per Hz
	c->increment = 89478.48533f * f;
}

void clock_set_freq_incr(clock *c, uint32_t incr)
{
	// tick called at 48kHz
	// wraps at 2^32 = 4,294,967,296
	// increment is linear in Hz, with 89478.485333 per Hz
	c->increment = incr;
}

// Call at 48kHz
// Returns true on both rising and falling edges
bool clock_tick(clock *c)
{
	uint32_t nextCount = c->count + c->increment;

	// Internal clock pulse, both rising and falling edges
	bool edge = (c->count ^ nextCount)&0x80000000;
	c->count = nextCount;
	return edge;
}

bool clock_state(clock *c)
{
	return c->count & 0x80000000;
}



class State
{
public:
	static constexpr uint8_t TYPE_OFF=0;
	static constexpr uint8_t TYPE_FIRST=1;
	static constexpr uint8_t TYPE_EVERY=2;
	static constexpr uint8_t TYPE_SECOND=3;
	static constexpr uint8_t TYPE_THIRD=4;
	static constexpr uint8_t TYPE_FOURTH=5;
	static constexpr uint8_t TYPE_RAND=6;
	static constexpr uint8_t TYPE_SUSTAIN=7;
/*	0000 C
0001 C#
0010 D
0011 Eb
0100 Eb
0101 E
0110 F
0111 F#
1000 G
1001 A
1010 Ab
1011 A
1100 Bb
1101 Bb
1110 B
1111 C
	*/
	static constexpr uint8_t pitchArray[16] = {0,1,2,3,3,4,5,6,7,9,8,9,10,10,11,12};
	/*
  - off
  - first step
  - every step
  - every 2nd step
  - every 3rd step
  - every 4th step
  - random
  - sustain
	*/
	uint8_t steps;
	uint8_t type;
	uint8_t pitch;
	uint8_t pitchmask;
	State()
	{
		steps=0;
		type=0;
		pitch=0;
		pitchmask=0xFF;
	}

	void UpdateProbs(int32_t prob)
	{
		// Function to map 0-1023 to 1023-0, creating
		// larger windows of near-0 and near-1024 probability,
		// and flipping knob to give original TM behaviour
		prob = 1023 - (((prob*prob*3)>>10)- ((prob*prob*prob)>>19));
		
		steps ^= rand_bit(prob);
		steps ^= (rand_bit(prob)<<1);
		steps ^= (rand_bit(prob)<<2);
		
		type ^= rand_bit(prob);
		type ^= (rand_bit(prob)<<1);
		type ^= (rand_bit(prob)<<2);

		pitch ^= rand_bit(prob);
		pitch ^= (rand_bit(prob)<<1);
		pitch ^= (rand_bit(prob)<<2);
		pitch ^= (rand_bit(prob)<<3);
		pitch ^= (rand_bit(prob)<<4);

		// Scatter some root notes into pitch always
//		if (rand_bit(100)) pitch&=0xF0;

	}
	void SetState(uint32_t v)
	{
		steps = v&7;
		v>>=3;
		type = v&7;
		v>>=3;
		pitch = v&0x1F;
	}

	void UpdateMask(int modeknob)
	{
		if (modeknob<500)
		{
			pitchmask = 0x10;
		}
		else if (modeknob<1000)
		{
			pitchmask = 0x18;
		}
		else if (modeknob < 2000)
		{
			pitchmask = 0x1C;
		}
		else if (modeknob < 3000)
		{
			pitchmask = 0x1E;
		}
		else
		{
			pitchmask = 0x1F;
		}
	}
	bool PulseOn(int step)
	{
		switch(type)
		{
		case TYPE_OFF:
			return false;
			break;

		case TYPE_FIRST: case TYPE_SUSTAIN:
			return step == 0;
			break;
			
		case TYPE_EVERY:
			return true;
			break;
			
		case TYPE_SECOND:
			return !(step%2);
			break;
			
		case TYPE_THIRD:
			return !(step%3);
			break;
			
		case TYPE_FOURTH:
			return !(step%4);
			break;
			
		case TYPE_RAND:
			return rand_bit(512);
			break;
		}
		return false;
	}
	uint8_t PitchSemitone()
	{
		uint8_t pitchMasked = pitch & pitchmask;
		return 12*(pitch>>4) + pitchArray[pitchMasked&0x0F];
	}
	
	bool PulseOff(int step)
	{
		return (type != TYPE_SUSTAIN);
	}

	
};
#endif
