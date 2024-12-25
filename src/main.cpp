#include "ComputerCard.h"
#include "Saw.h"
#include "bernoulligate.h"
#include "delayline.h"
#include "divider.h"
#include "t185_state.h"

#include <algorithm>
#include "../loops/break1.h"
#include "../loops/break2.h"
#include "../loops/break3.h"
#include "../loops/break5.h"
#include "../loops/break6.h"
#include "../loops/break8.h"
#include "../loops/sleigh.h"

uint32_t __not_in_flash_func(rnd12)()
{
	static uint32_t lcg_seed = 1;
	lcg_seed = 1664525 * lcg_seed + 1013904223;
	return lcg_seed >> 20;
}
uint32_t __not_in_flash_func(rnd24)()
{
	static uint32_t lcg_seed = 1;
	lcg_seed = 1664525 * lcg_seed + 1013904223;
	return lcg_seed >> 8;
}

int32_t __not_in_flash_func(rndi32)()
{
	static uint32_t lcg_seed = 1;
	lcg_seed = 1664525 * lcg_seed + 1013904223;
	return lcg_seed;
}



int32_t cabs(int32_t a)
{
	return (a > 0) ? a : -a;
}

void clip(int32_t &a)
{
	if (a < -2047) a = -2047;
	if (a > 2047) a = 2047;
}

int32_t exp4000_vals[170] = { 2048, 2056, 2064, 2073, 2081, 2090, 2098, 2107, 2115, 2124, 2133, 2141, 2150, 2159, 2168, 2177, 2186, 2194, 2203, 2212, 2221, 2231, 2240, 2249, 2258, 2267, 2276, 2286, 2295, 2304, 2314, 2323, 2333, 2342, 2352, 2362, 2371, 2381, 2391, 2400, 2410, 2420, 2430, 2440, 2450, 2460, 2470, 2480, 2490, 2500, 2511, 2521, 2531, 2541, 2552, 2562, 2573, 2583, 2594, 2604, 2615, 2626, 2636, 2647, 2658, 2669, 2680, 2691, 2702, 2713, 2724, 2735, 2746, 2757, 2769, 2780, 2791, 2803, 2814, 2826, 2837, 2849, 2860, 2872, 2884, 2896, 2907, 2919, 2931, 2943, 2955, 2967, 2979, 2992, 3004, 3016, 3028, 3041, 3053, 3066, 3078, 3091, 3103, 3116, 3129, 3142, 3154, 3167, 3180, 3193, 3206, 3219, 3233, 3246, 3259, 3272, 3286, 3299, 3313, 3326, 3340, 3353, 3367, 3381, 3395, 3409, 3422, 3436, 3450, 3465, 3479, 3493, 3507, 3522, 3536, 3550, 3565, 3579, 3594, 3609, 3624, 3638, 3653, 3668, 3683, 3698, 3713, 3728, 3744, 3759, 3774, 3790, 3805, 3821, 3836, 3852, 3868, 3884, 3899, 3915, 3931, 3947, 3964, 3980, 3996, 4012, 4029, 4045, 4062, 4078 };
// Takes integers 0-2047
// Returns exponential values over a ratio ~4000 = 2^12
// from 256 to a bit over 10^6
int32_t Exp4000(int32_t in)
{
	int32_t oct = in / 170; // 0 to 12 inclusive
	int32_t suboct = in % 170;
	if (oct - 3 > 0)
		return exp4000_vals[suboct] << (oct - 3);
	else
		return exp4000_vals[suboct] >> (3 - oct);
}

int32_t voct_vals[341] = { 314964268, 315605144, 316247323, 316890810, 317535606, 318181713, 318829136, 319477876, 320127936, 320779318, 321432026, 322086062, 322741429, 323398129, 324056166, 324715542, 325376259, 326038320, 326701729, 327366488, 328032599, 328700066, 329368890, 330039076, 330710625, 331383541, 332057826, 332733483, 333410515, 334088924, 334768714, 335449887, 336132446, 336816394, 337501733, 338188467, 338876599, 339566130, 340257065, 340949405, 341643155, 342338315, 343034891, 343732883, 344432296, 345133132, 345835394, 346539085, 347244208, 347950766, 348658761, 349368197, 350079076, 350791402, 351505177, 352220405, 352937088, 353655229, 354374831, 355095898, 355818432, 356542436, 357267913, 357994867, 358723299, 359453214, 360184614, 360917502, 361651881, 362387755, 363125126, 363863998, 364604372, 365346254, 366089644, 366834548, 367580967, 368328905, 369078365, 369829350, 370581862, 371335907, 372091485, 372848601, 373607257, 374367457, 375129204, 375892500, 376657350, 377423757, 378191722, 378961250, 379732344, 380505008, 381279243, 382055053, 382832443, 383611414, 384391970, 385174114, 385957850, 386743180, 387530108, 388318638, 389108772, 389900514, 390693867, 391488834, 392285418, 393083624, 393883453, 394684911, 395487998, 396292720, 397099080, 397907080, 398716724, 399528016, 400340958, 401155555, 401971809, 402789724, 403609303, 404430550, 405253468, 406078060, 406904330, 407732282, 408561918, 409393242, 410226258, 411060969, 411897378, 412735489, 413575305, 414416830, 415260068, 416105021, 416951694, 417800089, 418650211, 419502062, 420355647, 421210969, 422068031, 422926837, 423787390, 424649694, 425513753, 426379570, 427247149, 428116493, 428987606, 429860492, 430735153, 431611595, 432489820, 433369831, 434251634, 435135230, 436020625, 436907821, 437796822, 438687632, 439580255, 440474694, 441370953, 442269035, 443168945, 444070686, 444974262, 445879677, 446786934, 447696036, 448606989, 449519795, 450434459, 451350984, 452269373, 453189631, 454111762, 455035769, 455961656, 456889428, 457819087, 458750637, 459684083, 460619429, 461556677, 462495833, 463436900, 464379881, 465324781, 466271604, 467220353, 468171033, 469123648, 470078200, 471034695, 471993136, 472953528, 473915873, 474880177, 475846443, 476814674, 477784876, 478757053, 479731207, 480707343, 481685466, 482665579, 483647686, 484631791, 485617899, 486606014, 487596139, 488588278, 489582437, 490578618, 491576826, 492577066, 493579340, 494583654, 495590012, 496598417, 497608874, 498621387, 499635961, 500652599, 501671305, 502692084, 503714940, 504739878, 505766901, 506796014, 507827220, 508860525, 509895933, 510933447, 511973073, 513014813, 514058674, 515104658, 516152771, 517203017, 518255399, 519309923, 520366592, 521425412, 522486386, 523549519, 524614815, 525682278, 526751914, 527823726, 528897719, 529973898, 531052266, 532132828, 533215589, 534300553, 535387725, 536477109, 537568709, 538662531, 539758579, 540856856, 541957368, 543060120, 544165115, 545272359, 546381856, 547493610, 548607627, 549723910, 550842464, 551963295, 553086406, 554211802, 555339489, 556469470, 557601750, 558736334, 559873227, 561012433, 562153957, 563297803, 564443977, 565592484, 566743327, 567896512, 569052043, 570209926, 571370165, 572532764, 573697729, 574865065, 576034775, 577206866, 578381342, 579558207, 580737467, 581919127, 583103191, 584289664, 585478552, 586669858, 587863589, 589059748, 590258342, 591459374, 592662850, 593868775, 595077154, 596287991, 597501292, 598717062, 599935306, 601156029, 602379235, 603604930, 604833120, 606063808, 607297001, 608532703, 609770919, 611011654, 612254915, 613500705, 614749029, 615999894, 617253304, 618509265, 619767781, 621028858, 622292501, 623558715, 624827505, 626098877, 627372836, 628649388};

// Takes integers 0-4095 from CV in
// Returns exponential values corresponding to volts per octave
// Max frequency about 20kHz, should aim for 2^30 ~ 1 billion, so want max number about 500 million
// About 455 values per octave
// Starts with  314964268 because this  314964268/2^32 * 48kHz = (3512Hz) 3 octaves above A440 
int32_t ExpVoct(int32_t in)
{
	if (in > 4091) in = 4091; // limit to 12 oct;
	int32_t oct = in / 341;
	int32_t suboct = in % 341;

	return voct_vals[suboct] >> (12 - oct);
}

int32_t exp_note[12] = { 250000000, 264865773, 280615512, 297301778, 314980262, 333709963, 353553390, 374576769, 396850262, 420448207, 445449359, 471937156 };

int32_t ExpNote(int32_t in)
{
	if (in > 127) in = 127;
	if (in < 0) in = 0;
	int32_t oct = in / 12;
	int32_t suboct = in % 12;

	return exp_note[suboct] >> (12 - oct);
}


////////////////////////////////////////////////////////////////////////////////
// Looper

template <int I>
class looper : virtual public ComputerCard
{
public:
	int32_t pulseTimer, changeDivisorTimer;
	int32_t lastBeat, lastDivisorIndex;
	int32_t divisorIndex;
	int32_t phase;
	int beatIndex;
	static constexpr int divisors = 12;
	static constexpr int divisor[divisors] = {1,2,4,8,16,32,64,128,3,6,9,12};
	static constexpr int nBeats = 6;
	int16_t const *breaks[nBeats] = {break1, break2, break3, break8, break5, break6};//, break7};
	int sizes[nBeats];
	int sleighSize;
	int32_t sleighPhase;
	bool lastNextBeat;
	Switch lastSwitchVal;
	looper()
	{
		divisorIndex=0;
		pulseTimer=0;
		changeDivisorTimer=0;
		beatIndex=0;
		lastBeat=0;
		phase=0;
		sleighPhase=-1;
 		sizes[0] = sizeof(break1)/sizeof(break1[0]);
		sizes[1] = sizeof(break2)/sizeof(break2[0]);
		sizes[2] = sizeof(break3)/sizeof(break3[0]);
		sizes[3] = sizeof(break8)/sizeof(break8[0]);
		sizes[4] = sizeof(break5)/sizeof(break5[0]);
		sizes[5] = sizeof(break6)/sizeof(break6[0]);
//		sizes[6] = sizeof(break7)/sizeof(break7[0]);
		sleighSize = sizeof(sleigh)/sizeof(sleigh[0]);
	}
	void __not_in_flash_func(Sample)()
	{
		Switch s = SwitchVal();
		
		int32_t kex = KnobVal(ComputerCard::Knob(Knob::Main));

		bool nextBeat;
		if (I==0)
		{
			nextBeat = (kex > (4080-30*lastNextBeat));
		}
		else
		{
			nextBeat = (s==Switch::Up);
		}
		
		if (nextBeat && !lastNextBeat)
		{
			phase=0;
			beatIndex++;
			if (beatIndex == nBeats)
				beatIndex = 0;
		}

		if (I==0)
		{
			divisorIndex = (kex*(divisors+3))>>12;
			if (divisorIndex <= divisors) divisorIndex=divisorIndex-1;
		}
		else
		{
			if (s==Switch::Down && s!=lastSwitchVal)
			{
				divisorIndex++;
				if (divisorIndex==divisors) divisorIndex=0;
			}
		}
					
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I)) >> 1;
		int32_t dphase = k + CVIn(I);
		dphase -= 1024;
		
		phase += dphase>>1;

		if (phase<0)
		{
			phase+=sizes[beatIndex]<<8;
			pulseTimer=100;
		}
		if (phase>(sizes[beatIndex]<<8))
		{
			phase-=sizes[beatIndex]<<8;
			pulseTimer=100;
		}

		int32_t beat = ((phase>>8)*divisor[divisorIndex])/sizes[beatIndex];

		if (beat != lastBeat && lastDivisorIndex == divisorIndex)
		{
			pulseTimer=100;
		}

		if (divisorIndex != lastDivisorIndex)
		{
			changeDivisorTimer=100;
		}
		
		int32_t r = phase & 0xFF;
		int32_t ind = phase>>8;
		
		int32_t val = breaks[beatIndex][ind]*(265-r)+ breaks[beatIndex][(ind+1)%sizes[beatIndex]]*(r);

		if (sleighPhase>=0)	sleighPhase++;

		if (PulseInRisingEdge(I)) sleighPhase=0;
		if (sleighPhase>sleighSize) sleighPhase=-1;

		val >>= 12;
		val += sleigh[sleighPhase]>>5;
		clip(val);
		  
		AudioOut(I, val);

		if (pulseTimer)
		{
			PulseOut(I, true);
			LedOn(I+4, true);
			pulseTimer--;
			if (pulseTimer==0)
			{
				PulseOut(I, false);
				LedOn(I+4, false);
			}
					
		}

		if (changeDivisorTimer)	LedOn(I,--changeDivisorTimer);
		lastBeat = beat;
		lastDivisorIndex = divisorIndex;
		lastNextBeat = nextBeat;
		lastSwitchVal = s;
	}
};


////////////////////////////////////////////////////////////////////////////////
// Wavefolder
// Using https://ieeexplore.ieee.org/document/7865908 for anti-aliasing
// Uses audio in, audio out, CV in, knob xy
// Free: pulse in/out, 
template <int I>
class wavefolder : virtual public ComputerCard
{
public:
	int32_t blip;
	bool outPulseState;
	int divisors[5]={2048,1366,1024,820,683};
	wavefolder()
	{
		blip = 0;
		outPulseState = false;
	}
	int32_t fold_function(int32_t x)
	{
		int32_t period = 8192;
		x = ((x + 2048) % period + period) % period;

		if (x < 4096)
		{
			return x - 2048;
		}
		else
		{
			return (8191 - x) - 2048;
		}
	}

	int32_t int_function(int32_t x)
	{
		int32_t period = 8192;
		x = ((x+2048) % period + period) % period;
		int32_t x2 = x*2;;
		if (x<4096)
		{
			return ((x2+1)*(x2-8191))>>3;
		}
		else
		{
			return -((x2-8191)*(x2-16383))>>3;
		}
	}

	int32_t aa_wavefolder(int32_t x)
	{
		static int32_t lastval=0, lastx=0;
		int32_t ret;
		if (x==lastx)
		{
			ret = fold_function(x);
		}
		else
		{
			int32_t val = int_function(x);
			int32_t aa = (val - lastval)/(x-lastx);
			ret = aa;
			lastx=x;
			lastval = val;

		}
		clip(ret);
		return ret;
	}
	
	void __not_in_flash_func(Sample)()
	{
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I)) >> 1;
		int32_t mult = k + CVIn(I);
		static Switch lasts = SwitchVal();
		static int32_t kex = 0;
		Switch s = SwitchVal();
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main))/820;
		}
		else
		{
			if (s == Switch::Down && s != lasts)
			{
				kex++;
				if (kex==5)
					kex=0;
			}
		}

		
		if (PulseInRisingEdge(I))
		{
			uint32_t r = rnd12()/divisors[kex];
			CVOut(I, (r*divisors[kex])>>2);
			LedBrightness(I+2, (r*divisors[kex])>>1);
			outPulseState = !outPulseState;
			PulseOut(I, outPulseState);
			LedOn(4+I, outPulseState);
		}

		AudioOut(I, aa_wavefolder((AudioIn(I) * mult) >> 7));

		if (mult>4095) mult=4095;
		LedBrightness(I, mult);

		lasts = s;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Bitcrush

// Audio input/output
// Knob + Cv controls bit depth
// extra control sets mix of wet/dry

template <int I>
class bitcrush : virtual public ComputerCard
{
public:
	int32_t timer;
	bitcrush()
	{
		timer = 0;
	}
	int32_t crush_function(int32_t x, int32_t amt)
	{
		if (amt < 1) amt = 1;
		x += -2047 + (amt >> 1);
		x = x - (((x % amt) + amt) % amt) + 2047;
		if (x < -2047) x = -2047;
		if (x > 2047) x = 2047;
		return x;
	}

	void __not_in_flash_func(Sample)()
	{
		bool doSample = false;
		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			kex = (SwitchVal() == Switch::Middle) ? 1000 : 3000;
		}

		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I)) >> 1;
		int32_t cv = CVIn(I) + k + 300;
		if (cv > 2047)
		{
			cv = 2047;
			doSample = true;
		}
		if (cv < 0) cv = 0;

		timer += Exp4000(cv);
		if (timer > 2000000)
		{
			timer -= 2000000;
			doSample = true;
		}


		if (doSample)
		{
			int32_t audio;

			if (Connected(ComputerCard::Input(Input::Audio1 + I)))
			{
				audio = AudioIn(I);
			}
			else
			{
				audio = rnd12();
			}
			int32_t crushed = crush_function(audio, (kex * kex) >> 12);
			//	int32_t output = (crushed*kex + input*(4095-kex))>>12;
			LedBrightness(I, cabs(audio << 1));
			AudioOut(I, crushed);
		}

		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
// slopesplus

template <int I>
class slopesplus : virtual public ComputerCard
{
public:
	int32_t rndcv, rndaudio;
	bool state;
	slopesplus()
	{
		rndcv = 0;
		rndaudio = 0;
		state = false;
	}
	void __not_in_flash_func(Sample)()
	{
		int32_t cv = Connected(ComputerCard::Input(Input::CV1 + I)) ? CVIn(I) : 2000;
		if (cv > 2000) cv = 2000;
		if (cv < 0) cv = 0;
		int32_t slopeVal = AudioIn(I);

		int32_t kx = KnobVal(ComputerCard::Knob(Knob::X + I)) - 2048;
		int32_t ksx = 1;
		if (kx < 0)
		{
			kx = -kx;
			ksx = -1;
		}
		int32_t km, ksm = 1;
		bool invertCh2Gate = false;
		if (I == 0)
		{
			km = KnobVal(ComputerCard::Knob(Knob::Main)) - 2048;
		}
		else
		{
			km = 1000;
			invertCh2Gate = (SwitchVal() == Switch::Middle);
		}
		if (km < 0)
		{
			km = -km;
			ksm = -1;
		}

		if (slopeVal >= cv && state != false)
		{
			state = false;
			PulseOut(I, invertCh2Gate);
			LedOn(I, invertCh2Gate);
			if (ksx == 1) rndcv = rnd12() >> 1;
			if (ksm == 1) rndaudio = rnd12() >> 1;
		}

		if (slopeVal < 10 && state != true)
		{
			state = true;
			PulseOut(I, !invertCh2Gate);
			LedOn(I, !invertCh2Gate);
			rndcv = rnd12() >> 1;
			rndaudio = rnd12() >> 1;
		}
		CVOut(I, (rndcv * kx) >> 12);
		AudioOut(I, (rndaudio * km) >> 12);
		if (slopeVal < 0) slopeVal = 0;
		LedBrightness(I + 2, slopeVal << 1);
	}
};

////////////////////////////////////////////////////////////////////////////////
// VCA

template <int I>
class vca : virtual public ComputerCard
{
public:
	void __not_in_flash_func(Sample)()
	{
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I)) >> 1;
		int32_t cv = CVIn(I) + k;
		if (cv > 2047) cv = 2047;
		if (cv < 0) cv = 0;
		AudioOut(I, (AudioIn(I) * cv) >> 12);
	}
};


////////////////////////////////////////////////////////////////////////////////
// Turing 185

template <int I>
class turing185 : virtual public ComputerCard
{

	static constexpr int nStates = 6;

	
	clock clk;
	State states[nStates];
	int stepInState=0, currentState=0;
	int barStepIndex=0;
	int activeLed;
	int pulseCount=0;
public:
	turing185()
	{
		activeLed=I;
		clock_init(&clk);
		clock_set_freq_hz(&clk, 10);

		// Do some randomisation of initial state
		
		rndu32_lcg_seed = UniqueCardID();
		rndu32_lcg_seed ^= Knob(Knob::X);
		rndu32_lcg_seed ^= Knob(Knob::Y);
		rndu32_lcg_seed ^= Knob(Knob::Main);
		rndu32();
		for (int i=0; i<nStates; i++)
		{
			states[i].SetState(rndu32());
		}
	}
	void __not_in_flash_func(Sample)()
	{
		bool pulsein = Connected(ComputerCard::Input(Input::Pulse1 + I));

		if (((!pulsein) && clock_tick(&clk))
			|| (pulsein && (PulseInRisingEdge(I) || PulseInFallingEdge(I))))
		{
			int32_t km = KnobVal(ComputerCard::Knob(Knob::X + I));
			int32_t kex;
			
			if (I==0) kex = KnobVal(Knob::Main);
			else
			{
				Switch s = SwitchVal();
				if (s == Switch::Up) kex = 4080;
				else if (s == Switch::Middle) kex = 2001;
				else if (s == Switch::Down) kex = 1;
			}
			
			for (int i=0; i<nStates; i++)
				states[i].UpdateMask(kex);
			
			bool risingEdge = (!pulsein && clock_state(&clk))
				|| (pulsein && PulseInRisingEdge(I));


			if (risingEdge)
			{
				// Advance step in state, and state itself
				stepInState++;
				barStepIndex++;
				if (stepInState >= states[currentState].steps)
				{
					// New state:
					stepInState = 0;
					currentState++;
					if (currentState >= nStates)
					{
						currentState=0;
					}
					states[currentState].UpdateProbs(km>>2);
					
					if (states[currentState].type != State::TYPE_OFF)
					{
						LedOn(activeLed, false);
						activeLed+=2;
						if (activeLed>=6) activeLed-=6;
					}
				}

				if (barStepIndex >= 16)
				{
					// New state:
				
   					barStepIndex=0;
					stepInState = 0;
					currentState=0;
					states[currentState].UpdateProbs(km>>2);
				}

				// Turn on gate if needs be
				if (states[currentState].PulseOn(stepInState))
				{
					PulseOut(I, true);

					if (states[currentState].type != State::TYPE_SUSTAIN)
						pulseCount = 100;
					LedOn(activeLed, true);
					CVOutMIDINote(I, states[currentState].PitchSemitone()+60);
				}

			}
			else
			{
				if (states[currentState].PulseOff(stepInState))
				{
					LedOn(activeLed, false);
					PulseOut(I, false);
				}
			}
		}
		if (pulseCount)
		{
			pulseCount--;
			if (pulseCount == 0)
				PulseOut(I, false);
		}
	}
};


////////////////////////////////////////////////////////////////////////////////
// lpg

template <int I>
class lpg : virtual public ComputerCard
{
public:
	constexpr static int nSeqEntries = 5;
	int64_t sa1,ff;
	int64_t cvsm;
	int32_t blipTimer, resetTimer;
	int seqPos=0;
	int32_t pitches[nSeqEntries];
	lpg()
	{
		blipTimer=0;
		resetTimer=0;
		sa1=0;
		ff=0;
		
		seqPos=0;
		uint64_t id = UniqueCardID();

		for (int i=0; i<nSeqEntries; i++)
		{
			pitches[i] = id & 0xF;
			id >>= 4;
		}
	}
	void __not_in_flash_func(Sample)()
	{
		static Switch lasts = SwitchVal();
		Switch s = SwitchVal();
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));

		static int32_t kex = 2000;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			if (s == Switch::Down && s != lasts)
			{
				kex += 1000;
				if (kex==5000)
					kex=0;
			}
		}


		if (PulseInRisingEdge(I))
		{
			blipTimer = 500;
			seqPos++;
			if (seqPos==nSeqEntries)
			{
				seqPos=0;
				resetTimer = 100;
			}
			CVOut(I, pitches[seqPos]<<6);
		}


		// If pulse in is connected, main knob/switch attenuates pulse blip
		// else main knob / switch attenuates CV.
		int32_t cv = k;
		if (Connected(ComputerCard::Input(Input::Pulse1 + I)))
		{
			cv += CVIn(I);
		}
		else
		{
			cv += (CVIn(I)*kex)>>11;
		}

		
		if (blipTimer > 0)
		{
			blipTimer--;
			cv += kex;
		}

		if (resetTimer > 0)
		{
			resetTimer--;
		}
		PulseOut(I, resetTimer>0);
		
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;
		
		int32_t in = AudioIn(I);
		int32_t amp=512;
		if (cv<512)
		{
			cv=(cv*cv)>>9;
		}
		int32_t cv8 = (cv<<8);
		if (cv8>cvsm) cvsm = (31*cvsm + cv8)>>5;
		if (cv8<cvsm) cvsm = (4095*cvsm + cv8)>>12;

		int32_t cvsm8 = cvsm>>8;
		if (cvsm8 < 512)
		{
			amp=cvsm8;
		}
		ff = ExpVoct(cvsm>>8)>>1;
//		ff= 1000*4096;
		sa1 = ((in<<12)*ff + sa1*(268435456-ff))>>28;
		
		AudioOut(I, ((sa1 >> 12)*amp)>>9);

	    lasts = s;
	}
};


////////////////////////////////////////////////////////////////////////////////
// Sample and hold

// Trigger on pulse in, or on knob+cv-controlled tempo if no pulse in jack
// Sample audio in, if no audio in jack
template <int I>
class sandh : virtual public ComputerCard
{
public:
	int32_t shtarget, shout;
	uint32_t timer;
	int32_t sampleTimer=0;
	sandh()
	{
		shtarget=0;
		shout=0;
		timer = 0;
		sampleTimer=0;
	}
	void __not_in_flash_func(Sample)()
	{
		
		int32_t kex;
	   
		bool pulseInputConnected = Connected(ComputerCard::Input(Input::Pulse1 + I));
		
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			switch(SwitchVal())
			{
			case Switch::Up: kex=1000; break;
			case Switch::Middle: kex=2700; break;
			case Switch::Down: kex=0; break;
			}
		}

		if (pulseInputConnected)
		{
			kex += CVIn(I);
			if (kex>4095) kex=4095;
			if (kex<0) kex=0;
		}
		
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I)) >> 1;
		bool doSample = false;
		if (pulseInputConnected)
		{
			if (PulseInRisingEdge(I))
				doSample = true;
		}
		else
		{
			int32_t cv = CVIn(I) + k;
			if (cv > 2047) cv = 2047;
			if (cv < 0) cv = 0;

			timer += Exp4000(cv);
			if (timer > 50000000)
			{
				timer -= 50000000;
				doSample = true;
			}
		}

		if (doSample)
		{
			if (Connected(ComputerCard::Input(Input::Audio1 + I)))
			{
				shtarget = AudioIn(I)<<16;
			}
			else
			{
				int32_t new_random;
				do
				{
					new_random = (rnd12()-2048)<<16;
				} while (new_random == shtarget);
				shtarget = new_random;
			}
			sampleTimer=100;
			LedOn(I, true);
		}

		if (sampleTimer)
		{
			sampleTimer--;
			if (!sampleTimer)
				LedOn(I, false);
		}
		
		int32_t slewspeed = Exp4000(170+(kex>>1));
		if (shout<shtarget)
		{
			shout += std::min(slewspeed, (shtarget-shout));
			PulseOut(I, false);
			LedOn(I+2, false);
		}
		else if (shout>shtarget)
		{
			shout -= std::min(slewspeed, (shout-shtarget));
			PulseOut(I, false);
			LedOn(I+2, false);
		}
		else
		{
			PulseOut(I, true);
			LedOn(I+2, true);
		}

		LedBrightness(I+4, 2048+(shtarget>>16));
		AudioOut(I, shtarget>>16);
		CVOut(I, shout>>16);
	}
};



////////////////////////////////////////////////////////////////////////////////
// Max / Absolute value
//
// TODO: use X/Y knob to control gain of CV channel, attenuverted


template <int I>
class maxrect : virtual public ComputerCard
{
public:
	int32_t last_diff;

	maxrect()
	{

		last_diff = 0;
	}
	int32_t intfn(int32_t n)
	{
		if (n > 0)
			return (n * (n + 1)) >> 1;
		else
			return 0;
	}
	int32_t fn(int32_t n)
	{
		if (n > 0)
			return n;
		else
			return 0;
	}
	void __not_in_flash_func(Sample)()
	{
		int32_t audio = AudioIn(I);
		int32_t cv;
		if (Connected(ComputerCard::Input(Input::CV1 + I)))
		{
			cv = CVIn(I);
		}
		else
		{
			cv = -audio;
		}

		cv = (cv * (KnobVal(ComputerCard::Knob(Knob::X + I)) - 2048)) >> 11;
		if (audio < 0)
		{
			PulseOut(I, true);
		}
		else
		{
			PulseOut(I, false);
		}

		int32_t audio_out;
		int32_t diff = cv - audio;
		int32_t func_diff;
		if (diff != last_diff)
			func_diff = (intfn(diff) - intfn(last_diff)) / (diff - last_diff);
		else
			func_diff = fn(diff);

		audio_out = audio + func_diff;
		clip(audio_out);
		AudioOut(I, audio_out);
		last_diff = diff;
	}
};



////////////////////////////////////////////////////////////////////////////////
// Window comparator
//

template <int I>
class windowcomp : virtual public ComputerCard
{
public:
	void __not_in_flash_func(Sample)()
	{
		int32_t audio = AudioIn(I);
		int32_t cv = CVIn(I);
		int32_t kxy = KnobVal(ComputerCard::Knob(Knob::X + I)) >> 1;
		bool pi = PulseIn(I);

		bool above = (audio > cv + kxy) ^ pi,
		     below = (audio < cv - kxy) ^ pi,
		     mid = (!(above || below)) ^ pi;

		AudioOut(I, above ? 2000 : 0);
		LedOn(I, above);

		CVOut(I, mid ? 2000 : 0);
		LedOn(I + 2, mid);

		PulseOut(I, below);
		LedOn(I + 4, below);
	}
};



////////////////////////////////////////////////////////////////////////////////
// Cross switch

// TODO - could add slew rate on switching using knob

template <int I>
class cross : virtual public ComputerCard
{
public:
	void __not_in_flash_func(Sample)()
	{
		int32_t audio = AudioIn(I);
		int32_t cv = CVIn(I);

		if (PulseIn(I) || (I == 1 && SwitchVal() == Switch::Down))
		{
			AudioOut(I, cv);
			CVOut(I, audio);
		}
		else
		{
			AudioOut(I, audio);
			CVOut(I, cv);
		}
	}
};



////////////////////////////////////////////////////////////////////////////////
// Dual attenuverter

// Separate audio and CV attenuverters. No mixing. LEDs show output level. Inputs normalised to ~+6V

template <int I>
class attenuvert : virtual public ComputerCard
{
public:
	int32_t muteState;
	attenuvert()
	{
		muteState = 128;
	}
	void __not_in_flash_func(Sample)()
	{
		bool audioConnected = Connected(ComputerCard::Input(Input::Audio1 + I));
		bool cvConnected = Connected(ComputerCard::Input(Input::CV1 + I));
		int32_t audio = audioConnected ? AudioIn(I) : 2000;
		int32_t cv = cvConnected ? CVIn(I) : audio;

		int32_t kxy = KnobVal(ComputerCard::Knob(Knob::X + I)) - 2048;


		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main)) - 2048;
		}
		else
		{
			kex = (SwitchVal() == Switch::Middle) ? 2000 : -2000;
		}

		if (PulseIn(I) && muteState > 0)
		{
			muteState--;
		}
		if (!PulseIn(I) && muteState < 128)
		{
			muteState++;
		}

		int32_t out = (((audio * kxy) >> 11) * muteState) >> 7;
		AudioOut(I, out);
		LedBrightness(I, cabs(out << 1));

		out = (((cv * kex) >> 11) * muteState) >> 7;
		CVOut(I, out);
		LedBrightness(I + 2, cabs(out << 1));

		int32_t b = (128 - muteState) << 5;
		if (b >= 4096) b = 4095;
		LedBrightness(I + 4, b);

		PulseOut(I, PulseIn(I));
	}
};



////////////////////////////////////////////////////////////////////////////////
// CV mixer

// Inputs A and B from audio and CV
// Outputs Ax + By and Ax - By on audio and CV
// LEDs show output brightness
// Inputs normalised to ~6V.
template <int I>
class cvmix : virtual public ComputerCard
{
public:
	void __not_in_flash_func(Sample)()
	{
		bool audioConnected = Connected(ComputerCard::Input(Input::Audio1 + I));
		bool cvConnected = Connected(ComputerCard::Input(Input::CV1 + I));
		int32_t audio = audioConnected ? AudioIn(I) : 2000;
		int32_t cv = cvConnected ? CVIn(I) : 2000;

		int32_t kxy = KnobVal(ComputerCard::Knob(Knob::X + I)) - 2048;


		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main)) - 2048;
		}
		else
		{
			kex = (SwitchVal() == Switch::Middle) ? 2000 : -2000;
		}

		int32_t audioout = ((audio * kxy) >> 12) + ((cv * kex) >> 12);
		int32_t cvout = ((audio * kxy) >> 12) - ((cv * kex) >> 12);
		clip(audioout);
		clip(cvout);
		AudioOut(I, audioout);
		CVOut(I, cvout);
		LedBrightness(I, cabs(audioout << 1));
		LedBrightness(I + 2, cabs(cvout << 1));
		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}
	}
};



////////////////////////////////////////////////////////////////////////////////
// Supersaw

template <int I>
class supersaw : virtual public ComputerCard
{
public:
	constexpr static int nSaws = 8;
	int freqs[nSaws];
	Saw saws[nSaws];
	supersaw()
	{
		for (int i = 0; i < nSaws; i++)
		{
			saws[i].SetPhase(rndi32());
			freqs[i] = ExpVoct(2048 + i) << 1;
			saws[i].SetFreq(freqs[i]);
		}
	}

	void __not_in_flash_func(Sample)()
	{
		// Output 1 is sawtooth stack
		static int count = 0;
		int32_t tot = 0;
		for (int i = 0; i < nSaws; i++)
		{
			tot += saws[i].Tick() >> 7;
		}
		AudioOut(I, tot);

		// Set saw frequencies according to knobs
		count++;
		if (count == nSaws) count = 0;

		int k = KnobVal(ComputerCard::Knob(Knob::X + I));
		k += CVIn(I);
		if (k < 0) k = 0;
		if (k > 4095) k = 4095;
		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			kex = (SwitchVal() == Switch::Middle) ? 1000 : 3000;
		}
		int32_t offs = ((count - 4) * kex) >> 10;
		freqs[count] = ExpVoct(k + offs) << 1;
		saws[count].SetFreq(freqs[count]);

		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}
	}
};



////////////////////////////////////////////////////////////////////////////////
// Chords

template <int I>
class chords : virtual public ComputerCard
{
public:
	int32_t cvInput;
	int octaveOffs, last_baseNote;
	bool p;
	constexpr static int nSaws = 8;
	int freqs[nSaws];
	Saw saws[nSaws];
	bool active[nSaws];
	int chordtype;
	Switch lasts;
	constexpr static int nChords[8] = { 12, 12, 12, 12, 12, 8, 12, 12 };
	constexpr static int notes[8][12][4] = {
		{ { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 }, { 0, 3, 7, -255 } }, // minor triad,
		{ { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 }, { 0, 4, 7, -255 } }, // major triad,
		{ { 0, 11, 16, 19 }, { -1, 10, 15, 18 }, { 0, 10, 15, 19 }, { -1, 9, 14, 18 }, { 0, 10, 15, 19 }, { -1, 9, 14, 18 }, { -1, 10, 15, 18 }, { 0, 10, 16, 19 }, { -1, 9, 15, 18 }, { 0, 10, 15, 19 }, { -1, 9, 14, 18 }, { 0, 10, 15, 19 } }, // diatonic  7ths
		{ { 0, 4, 7, 11 }, { 0, 3, 7, 10 }, { 0, 2, 4, 11 }, { 0, 3, 7, 10 }, { 0, 4, 7, 11 }, { 0, 3, 7, 10 }, { 0, 2, 4, 11 }, { 0, 3, 7, 10 }, { 0, 4, 7, 11 }, { 0, 3, 7, 10 }, { 0, 2, 4, 11 }, { 0, 3, 7, 10 } }, // slightly odd chords, but OK sounding
		{ { -12, 4, 7, -1 }, { -12, 3, 7, -2 }, { -12, 2, 4, -1 }, { -12, 3, 7, -2 }, { -12, 4, 7, -1 }, { -12, 3, 7, -2 }, { -12, 2, 4, -1 }, { -12, 3, 7, -2 }, { -12, 4, 7, -1 }, { -12, 3, 7, -2 }, { -12, 2, 4, -1 }, { -12, 3, 7, -2 } }, // inversion of the same chords, pretty good
		{ { 0, 5, 9, 11 }, { 1, 5, 10, 11 }, { 1, 6, 10, 12 }, { 2, 6, 11, 12 }, { 2, 7, 11, 13 }, { 3, 7, 12, 13 }, { 3, 8, 12, 14 }, { 4, 8, 13, 14 }, { -255, -255, -255, -255 }, { -255, -255, -255, -255 }, { -255, -255, -255, -255 }, { -255, -255, -255, -255 } }, // Messiaen 2nd mode
		{ { 0, 3, 6, 8 }, { 1, 3, 6, 9 }, { 1, 4, 6, 9 }, { 3, 4, 7, 9 }, { 0, 3, 6, 8 }, { 1, 3, 6, 9 }, { 1, 4, 6, 9 }, { 3, 4, 7, 9 }, { 0, 3, 6, 8 }, { 1, 3, 6, 9 }, { 1, 4, 6, 9 }, { 3, 4, 7, 9 } }, // messaein mode 4, but wrong
		{ { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 }, { -12, 0, 12, 24 } }, // octaves,
	};

	chords()
	{
		cvInput = 0;
		octaveOffs=0; last_baseNote=0;
		p=0;
		for (int i = 0; i < nSaws; i++)
		{
			freqs[i] = 1000;
			saws[i].SetPhase(rndi32());
			saws[i].SetFreq(freqs[i]);
			active[i]=false;
		}
		p = false;
	}


	void __not_in_flash_func(Sample)()
	{
		Switch s = SwitchVal();
		static int count = 0;
		int32_t tot = 0;
		for (int i = 0; i < nSaws; i += 2)
		{

			if (active[i >>1]) tot += (saws[i].Tick() >> 7) - (saws[i + 1].Tick() >> 7);
		}
		AudioOut(I, tot);

		// Set saw frequencies according to knobs
		count++;
		if (count == nSaws) count = 0;


		int k = KnobVal(ComputerCard::Knob(Knob::X + I));
		if (Disconnected(ComputerCard::Input(Input::Pulse1 + I)) || PulseInRisingEdge(I))
		{
			cvInput = CVIn(I);
		}
		k += cvInput;
		if (k < 0) k = 0;
		if (k > 4095) k = 4095;
			int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
			chordtype = kex >> 9;
		}
		else
		{
			if (s == Switch::Down && s != lasts)
			{
				chordtype = (chordtype+1)&0x07;
			}
		}

		int32_t sawind = count >>1;

		 
		int32_t baseNote = 30 + (k / 70); // Freq = ExpVoct(k+offs+(((count%(nSaws/2))*455)))<<1;
		int32_t baseNoteOffs = (baseNote/nChords[chordtype])*(12-nChords[chordtype]);
		int32_t chordNote = notes[chordtype][(baseNote % nChords[chordtype])][sawind];

		active[sawind] = (chordNote != -255);
		if (active[sawind]) saws[count].SetFreq(10 * ExpNote(baseNote + baseNoteOffs + chordNote));

		int minnote = 1000;
		for (int i = 0; i < 4; i++)
		{
			if (notes[chordtype][(baseNote % nChords[chordtype])][i] < minnote) minnote = notes[chordtype][(baseNote % nChords[chordtype])][i];
		}
		CVOutMIDINote(I, baseNote + baseNoteOffs + minnote);

		
		LedOn(I, chordtype & 0x4);
		LedOn(I + 2, chordtype & 0x2);
		LedOn(I + 4, chordtype & 0x1);
		lasts = s;
		
		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}
	}
};


////////////////////////////////////////////////////////////////////////////////
// Glitch


template <int I>
class glitch : virtual public ComputerCard
{
public:
	static constexpr uint32_t bufSize = 48000;
	int16_t buffer[bufSize];
	unsigned writeInd, offs, newoffs;
	int32_t xfadeInd=0;
	uint32_t frame;
	uint32_t pulseFrames[16];
	int pulseFramesInd;
	bool rev, loop;
	uint32_t loopoffs;
	int loopcnt;
	static constexpr int xfadeLen=25;
	glitch()
	{
		for (int i = 0; i < bufSize; i++) buffer[i] = 0;
		writeInd = 0;
		offs = 0;
		frame=0;
		pulseFramesInd=0;
		rev=false;
		loop=false;
	}
	void __not_in_flash_func(Sample)()
	{
		static int t=0;
		int32_t in = AudioIn(I);
		buffer[writeInd] = in;
		writeInd++;
		if (writeInd>=bufSize) writeInd=0;

		int kex;
		if (I==0)
		{
			kex = KnobVal(Knob::Main);
		}
		else
		{
			Switch s = SwitchVal();
			if (s == Switch::Up) kex = 2100;
			else if (s == Switch::Middle) kex = 0;
			else if (s == Switch::Down) kex = 4095;
		}
		kex += CVIn(I);
		if (kex>4095) kex=4095;
		if (kex<0)kex=0;

		int k = KnobVal(ComputerCard::Knob(Knob::X + I));
		k += CVIn(I);
		if (k>4095) k=4095;
		if (k<0) k=0;
		t++;

		bool pulseInputConnected = Connected(ComputerCard::Input(Input::Pulse1 + I));
		bool timerTrigger=false;
		if (t>=4500-k)
		{
			t=0;
			timerTrigger = true;
		}
		if ((!pulseInputConnected && timerTrigger))
		{
			int32_t r = rnd24();
			if (r<(k*k) && newoffs==0)
			{
				newoffs = r>>9;
				xfadeInd = xfadeLen;
			}
			
		}
		else if (PulseInRisingEdge(I))
		{

			int nValidPulseFrames=0;
			for (int i=1; i<16; i++)
			{
				uint32_t pf = pulseFrames[(pulseFramesInd + 32 - i)%16];
				if ((frame-pf)<(k<<3))
					nValidPulseFrames++;
				else
					break;
					
			}
			
			int32_t r = (rnd24()*(nValidPulseFrames+1))>>24;
			if (r>0)
			{
				newoffs = frame - pulseFrames[(pulseFramesInd + 16 - r)%16];
			}
			else
			{
				newoffs = 1;
			}
			loopoffs=newoffs;
			xfadeInd = xfadeLen;

			pulseFrames[pulseFramesInd]=frame;
			pulseFramesInd++;
			if (pulseFramesInd==16)
				pulseFramesInd=0;

			rev = rnd12()<(kex>>1);
			loop = int32_t(rnd12())<(kex-3000);
			loopcnt=2500;
		}

		if (loopoffs<bufSize-10) loopoffs++;
		
		if (rev)
		{
			if (offs<bufSize-10) offs+=2;
			if (newoffs!=0 && newoffs < bufSize-10) newoffs+=2;
		}

		if (loop)
		{
			loopcnt--;
			if (loopcnt==0)
			{
				loopcnt=2500;
				if (newoffs>0) newoffs=loopoffs;
				else offs=loopoffs;
			}
			
		}
		if (newoffs != 0)
		{
			unsigned readInd = (writeInd + (bufSize << 2) - (offs)-1) % bufSize;
			unsigned newReadInd = (writeInd + (bufSize << 2) - (newoffs)-1) % bufSize;
			int32_t xfaded = (buffer[readInd]*xfadeInd + buffer[newReadInd]*(xfadeLen-xfadeInd))/xfadeLen;
			AudioOut(I,xfaded);
			xfadeInd--;
			if (xfadeInd==0)
			{
				offs=newoffs;
				newoffs=0;
			}
			
		}
		else
		{
			unsigned readInd = (writeInd + (bufSize << 2) - (offs)-1) % bufSize;
			AudioOut(I,buffer[readInd]);
		}
		frame++;
	}
};


////////////////////////////////////////////////////////////////////////////////
// Delay


int32_t __not_in_flash_func(highpass_process)(int32_t *out, int32_t b, int32_t in)
{
	*out += (((in - *out) * b) >> 16);
	return in - *out;
}

template <int I>
class delay : virtual public ComputerCard
{
public:
	static constexpr uint32_t bufSize = 48000;
	int16_t delaybuf[bufSize];
	unsigned writeInd, readInd, cvs;
	int32_t ledtimer = 0;
	int32_t hpf = 0;
	delay()
	{
		for (int i = 0; i < bufSize; i++) delaybuf[i] = 0;
		writeInd = 0;
		readInd = 0;
		cvs = 0;
	}
	void __not_in_flash_func(Sample)()
	{
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));
		int32_t cv = CVIn(I) + k;
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;

		int cvtarg = cv * cv / 178;
		cvs = (cvs * 255 + (cvtarg << 5)) >> 8;

		int cvs1 = cvs >> 7;
		int r = cvs & 0x7F;

		readInd = (writeInd + (bufSize << 1) - (cvs1)-1) % bufSize;
		int32_t fromBuffer1 = delaybuf[readInd];
		int readInd2 = (writeInd + (bufSize << 1) - (cvs1)-2) % bufSize;
		int32_t fromBuffer2 = delaybuf[readInd2];

		int32_t fromBuffer = (fromBuffer2 * r + fromBuffer1 * (128 - r)) >> 7;
		int32_t in = AudioIn(I);


		int32_t k2;
		if (I == 0)
		{
			k2 = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			if (SwitchVal() == Switch::Up)
				k2 = 800;
			else if (SwitchVal() == Switch::Middle)
				k2 = 2500;
			else
				k2 = 4095; // switch down = infinite loop
		}

		int32_t out = (((4095 - ((k2 * k2) >> 12)) * in) >> 12) + (((4095 - (((4095 - k2) * (4095 - k2)) >> 12)) * fromBuffer) >> 12);

		int32_t buf_write = highpass_process(&hpf, 200, out);

		delaybuf[writeInd] = buf_write;
		clip(out);
		AudioOut(I, out);

		writeInd++;
		if (writeInd >= bufSize) writeInd = 0;

		ledtimer--;
		if (ledtimer <= 0) ledtimer = cvs1;
		LedOn(I + 4, ledtimer < 100);
		LedBrightness(I, cabs(in << 1));
		LedBrightness(I + 2, cabs(out << 1));

		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}

	}
};

////////////////////////////////////////////////////////////////////////////////
// Flanger

template <int I>
class flanger : virtual public ComputerCard
{
public:
	DelayLine<4096> del;
	int32_t phase;

	flanger()
	{
		phase = 0;
	}

	void __not_in_flash_func(Sample)()
	{
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));
		int32_t cv = CVIn(I) + k;
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;

		phase += (cv >> 4);

		if (phase > 2097152) phase -= 4194304;
		int32_t lfoval;
		int32_t p2 = phase >> 8;
		if (phase > 0)
			lfoval = p2 * (8192 - p2);
		else
			lfoval = p2 * (8192 + p2);

		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			kex = (SwitchVal() == Switch::Middle) ? 100 : 3000;
		}


		int32_t in = AudioIn(I);

		int32_t lfov = ((lfoval >> 12) * kex) >> 12;
		int32_t out = del.ReadInterp(12800 + lfov);

		del.Write((in + out) >> 1);
		AudioOut(I, (out + del.ReadInterp(12800 - lfov)) >> 1);
	}
};

////////////////////////////////////////////////////////////////////////////////
// Slow LFO

/*
looking for
sin(omega1 t)
and 
sin(omega2 t)

such that at t=T, second sine has done one more phase than first.

That is,

omega1 T + 2pi = omega2 T
omega2 = omega1 + 2pi/T

Now, index here is 21-bit ~= 2 million
Sample rate 48kHz
So incrementing one per sample gives 2^21/48000Hz = 44s loop
Let's just increment a 32-bit unsigned, let it wrap

 */

#include <cmath>
template <int I>
class slowlfo : virtual public ComputerCard
{
public:
	static constexpr int npts = 8192;
	int32_t sinevals[npts];

	uint32_t phase1, phase2;
	slowlfo()
	{
		for (int i=0; i<npts; i++)
		{
			// just shy of 2^22 * sin
			sinevals[i] = 2048*2040*sin(2*i*M_PI/double(npts));
		}

		phase1=0;
		phase2=0;
	}
	// Given 21-bit index x, return 2^22 * sin(y) where y = x/2^21
	int32_t sinval(int32_t x)
	{
		x &= 0x001FFFFF; // wrap at 21 bits = 13+8 bits
		int32_t r = x&0xFF; //
		x >>= 8; // x now 13-bit number, 0-8191
		int32_t s1 = sinevals[x];
		int32_t s2 = sinevals[(x+1)&0x1FFF];
		return (s2*r+s1*(256-r))>>8;
	}
	void SetAudio(int32_t cv_val)
	{
		static int32_t error=0;
		cv_val+=524288;
		uint32_t truncated_cv_val = (cv_val-error) & 0xFFFFFF00;
		error += truncated_cv_val - cv_val;
		AudioOut(I, int32_t(truncated_cv_val>>8)-2048);
	}
	void SetCV(int32_t cv_val)
	{
		static int32_t error=0;
		cv_val+=524288;
		uint32_t truncated_cv_val = (cv_val-error) & 0xFFFFFF00;
		error += truncated_cv_val - cv_val;
		CVOut(I, int32_t(truncated_cv_val>>8)-2048);
	}
	void __not_in_flash_func(Sample)()
	{
		int32_t omega = ExpVoct(KnobVal(ComputerCard::Knob(Knob::X + I)))>>14;
		int32_t diff;
		if (I==0)
		{
			diff = (ExpVoct(KnobVal(Knob::Main))>>15);
		}
		else
		{
			diff = (SwitchVal() == Switch::Middle) ? 12 : 149;
			if (SwitchVal() == Switch::Down) {phase1=0; phase2=0;}
		}
		if (diff>omega) diff=omega;
		phase1 += omega+diff;
		phase2 += omega - diff;

		if (PulseInRisingEdge(I))
		{
			phase1=0;
			phase2=0;
		}
		
		SetCV(sinval(phase1>>11)>>3);
		SetAudio(sinval(phase2>>11)>>3);
	}
};
////////////////////////////////////////////////////////////////////////////////
// KS

int32_t __not_in_flash_func(lowpass_process)(int32_t *out, int32_t b, int32_t in)
{
	*out += (((in - *out) * b) >> 16);
	return *out;
}

template <int I>
class karplusstrong : virtual public ComputerCard
{
public:
	DelayLine<1000> d[2];
	int32_t mults[512][2];
	int32_t hpf0, hpf1, hpfi, lpf0, lpf1;
	Switch lasts;
	int frame = 0;
	int32_t mv;
	int inhit;
	karplusstrong()
	{
		inhit = 0;
		hpf0 = 0;
		hpf1 = 0;
		hpfi = 0;
		lpf0 = 0;
		lpf1 = 0;
		lasts=Switch::Middle;
		frame = 0;
		mv = 100;
		for (int i = 0; i < 512; i++)
		{
			float ang = 3.14159265359f * float(i) / 256;
			mults[i][0] = cos(ang) * 32768.0f;//131072.0f;
			mults[i][1] = sin(ang) * 32768.0f;//131072.0f;
		}
	}

	void __not_in_flash_func(Sample)()
	{
		Switch s = SwitchVal();

		int32_t k = 4095 - KnobVal(ComputerCard::Knob(Knob::X + I));
		k += CVIn(I);
		if (k < 0) k = 0;
		if (k > 4095) k = 4095;
		int v = ExpVoct(k >> 1);
	   	int32_t r0 = d[0].ReadInterp(v >> 6);
		int32_t r1 = d[1].ReadInterp(v >> 6);
//		int32_t r0 = d[0].ReadRaw(v >> 14);
//		int32_t r1 = d[1].ReadRaw(v >> 14);

		bool pulsein = PulseIn(I);
		PulseOut(I, pulsein);

		if (I == 0)
		{
			mv = KnobVal(Knob::Main) >> 4;
		}
		else
		{
			if (s == Switch::Down && lasts != s)
			{
				mv = rnd12() >> 4;
			}
			else if (s == Switch::Up)
			{
				if (frame == 0) mv++;
				if (mv == 512) mv = 0;
			}
		}
		int32_t m0, m1;
		if (I == 1 && s == Switch::Up)
		{
			m0 = (mults[mv][0]*(1024-frame) + mults[(mv+1)&0x1FF][0]*frame)>>10;
			m1 = (mults[mv][1]*(1024-frame) + mults[(mv+1)&0x1FF][1]*frame)>>10;
		}
		else
		{
			m0 = mults[mv][0];
			m1 = mults[mv][1];
		}
		int32_t w0 = (r0 * m0 - r1 * m1) >> 15;
		int32_t w1 = (r0 * m1 + r1 * m0) >> 15;
		
		w0 = (w0 * (65536 - (k >> 3))) >> 16;
		w1 = (w1 * (65536 - (k >> 3))) >> 16;


		w0 = highpass_process(&hpf0, 200, w0);
		w1 = highpass_process(&hpf1, 200, w1);
		
		w0 = lowpass_process(&lpf0, 55000, w0);
		w1 = lowpass_process(&lpf1, 55000, w1);


		clip(w0);
		clip(w1);
		int32_t in = AudioIn(I) >> 1;
		in += rnd12() >> 11;

		if (PulseInRisingEdge(I))
		{
			inhit = (rnd12() - 2048) << 1;
		}
		inhit = (inhit * 3) >> 2;
		in += inhit;
		in = highpass_process(&hpfi, 200, in);
		d[0].Write(in + w0);
		d[1].Write(w1);
		AudioOut(I, w0);
		lasts = s;
		frame++;
		if (frame == 1024) frame = 0;

		LedBrightness(I, cabs(in << 1));
		LedBrightness(I+4, cabs(w0 << 1));

	}
};

////////////////////////////////////////////////////////////////////////////////
// Chorus

template <int I>
class chorus : virtual public ComputerCard
{
public:
	DelayLine<4096> delf, del;
	int32_t phase[5];
	int32_t phasef;
/*	DelayLine<480,int32_t>ap1,ap1b;
	DelayLine<240,int32_t>ap2,ap2b;
	DelayLine<160,int32_t>ap3,ap3b;
	DelayLine<120,int32_t>ap4,ap4b;
*/
	chorus()
	{
		for (int i = 0; i < 5; i++)
			phase[i] = -2096151 + ((4194304 * i) / 5);
	}

	// Allpass filter
/*	template <int DL>
	int32_t Allpass(DelayLine<DL,int32_t> &db, uint16_t t, int32_t gain, int32_t in)
	{
		// give 4 more bits of headroom in multiply than elsewhere
		// since gain is not a critical parameter (we can cope with 12-bit accuracy)
		// and overflow was fairly frequent without this
		gain >>= 4;

		int32_t delayed = db.ReadRaw(t);
		int32_t output = -in+delayed;
		//	clip(output);
		db.Write(in+(output>>4));
		return output;
		}*/

	
	void __not_in_flash_func(Sample)()
	{

		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));
		int32_t cv = CVIn(I) + k + 40;
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;

		
		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			kex = (SwitchVal() == Switch::Middle) ? 100 : 3000;
		}

		int32_t in = AudioIn(I);
		
		int flangerSpeed = cv>>7, flangerAmount=1600, chorusAmpl = 2000, chorusSpeed=cv>>1;
		phasef += flangerSpeed;
		if (phasef > 2097152) phasef -= 4194304;
		int32_t lfoval;
		int32_t p2 = phasef >> 8;
		if (phasef > 0)
			lfoval = p2 * (8192 - p2);
		else
			lfoval = p2 * (8192 + p2);



		int32_t lfov = ((lfoval >> 12) * flangerAmount ) >> 12;
		int32_t out = delf.ReadInterp(12800 + lfov);

		delf.Write((in + 7*out) >> 3);

		int32_t flanger_out = (out + delf.ReadInterp(13000 - lfov)) ;

		
		int32_t chorus_out = 0;

		for (int i = 0; i < 5; i++)
		{
			phase[i] += (chorusSpeed >> 4);// + (i << 2);

			if (phase[i] > 2097152) phase[i] -= 4194304;

			int32_t lfoval;
			int32_t p2 = phase[i] >> 8;
			if (phase[i] > 0)
				lfoval = p2 * (8192 - p2);
			else
				lfoval = p2 * (8192 + p2);

			int32_t lfov = ((lfoval >> 8) * chorusAmpl) >> 14;
			chorus_out += del.ReadInterp(25600 + lfov);
		}

		del.Write(in);
		chorus_out >>= 3;

		
		AudioOut(I, (flanger_out*kex + chorus_out*(4095-kex))>>12);

		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}
/*
		static int32_t lfo[8]={0,0,0,0,0,0,0,0}, lfos[8]={1,1,1,1,1,1,1,1}, lfoct[8]={5,6,7,8,9,10,11,12};

		for (int i=0; i<8; i++)
		{
			if (lfoct[i]==0)
			{
				if (lfos[i]) lfo[i]++; else lfo[i]--;
				lfoct[i]=5+i;
			}
			else lfoct[i]--;
			if (lfo[i]>10000) lfos[i]=0;
			if (lfo[i]<1) lfos[i]=1;
		}

		int32_t out = Allpass<480>(ap1, 470-(lfo[0]>>6), 65536, in>>1);
	   	out = Allpass<480>(ap1b, 470-(lfo[0]>>7), 65536, out>>1);
		out = Allpass<240>(ap2, 230-(lfo[0]>>6), 65536, out>>1);
		out = Allpass<240>(ap2b, 230-(lfo[0]>>7), 65536, out>>1);
		out = Allpass<160>(ap3, 150-(lfo[0]>>7), 65536, out);
		out = Allpass<160>(ap3b, 150-(lfo[0]>>7), 65536, out);
		out = Allpass<120>(ap4, 110-(lfo[0]>>8), 65536, out);
		out = Allpass<120>(ap4b, 110-(lfo[0]>>7), 65536, out);
		clip(out);
		if (SwitchVal()!=Switch::Down) AudioOut(I,(out+in)>>1);
		else AudioOut(I, in);
*/
	}


};

////////////////////////////////////////////////////////////////////////////////
// Clock divider

// TODO: - move 2/4/8  3/5/7 to main knob/switch
//       - use x/y knob as speed of default clock
//       - if CV input only plugged, CV controls default clocks speed.
//       - if pulse input plugged in, default clock is turned off, CV acts as part of the 'or'
//       - SOME KIND OF CONTROL OVER TRIGGER/GATE?
//       - document all this

template <int I>
class clockdiv : virtual public ComputerCard
{
public:
	Divider d[3];
	volatile int32_t pulseCounter[3];
	bool last_inpulse = false;
	int32_t clock;
	Switch lasts;
	int savedk;
	bool triggerMode;
	bool lastAudioInHigh;
	clockdiv()
	{
		for (int i = 0; i < 3; i++) pulseCounter[i] = 0;
		clock = 0;
		lasts = SwitchVal();
		savedk = 3000;
		triggerMode = true;
		lastAudioInHigh = false;
	}

	void __not_in_flash_func(Sample)()
	{
		static int32_t last_k = -1;
		int32_t k;
		Switch s = SwitchVal();
		if (I == 0)
		{
			k = KnobVal(ComputerCard::Knob(Knob::Main));
			triggerMode = (k > 1000 && k < 3000);
		}
		else
		{
			if (s == Switch::Up && lasts != Switch::Up)
			{
				savedk = 4000 - savedk;
			}
			if (s == Switch::Down && lasts != Switch::Down)
			{
				triggerMode = !triggerMode;
			}
			k = savedk;
		}

		if (last_k == -1 || k < 2048 && !(last_k < 2048))
		{
			d[0].SetResetPhase(2);
			d[1].SetResetPhase(4);
			d[2].SetResetPhase(8);
		}
		if (!(k < 2048) && (last_k < 2048))
		{
			d[0].SetResetPhase(3);
			d[1].SetResetPhase(5);
			d[2].SetResetPhase(7);
		}


		int32_t pk = KnobVal(ComputerCard::Knob(Knob::X + I));
		pk = (pk >> 1) + (pk >> 2);
		pk += CVIn(I) >> 1;
		if (pk > 3071) pk = 3071;
		if (pk < 0) pk = 0;

		bool resetClock = false;
		clock -= (ExpVoct(pk) >> 8);
		if (clock <= 0)
		{
			clock += 40000000;
			resetClock = true;
		}

		bool inputConnection = Connected(ComputerCard::Input(Input::Pulse1 + I));

		bool inpulse;
		bool audioInHigh = AudioIn(I) > 800;
		if (inputConnection)
			inpulse = PulseIn(I) | (AudioIn(I) > 800) | (CVIn(I) > 800);
		else
			inpulse = resetClock | (audioInHigh && (!lastAudioInHigh));

		// Rising edge
		if (inpulse && !last_inpulse)
		{
			for (int i = 0; i < 3; i++)
			{
				if (d[i].Step(true))
				{
					pulseCounter[i] = 100;
					if (i == 0)
						AudioOut(I, 2000);
					else if (i == 1)
						CVOut(I, 2000);
					else
						PulseOut(I, true);
					LedOn((i * 2) + I, true);
				}
				else
				{
					if (i == 0)
						AudioOut(I, 0);
					else if (i == 1)
						CVOut(I, 0);
					else
						PulseOut(I, false);
					LedOn((i * 2) + I, false);
				}
			}
		}

		// Falling edge
		if (!inpulse && last_inpulse)
		{
			for (int i = 0; i < 3; i++)
			{
				bool fe = d[i].Step(false);
				/*	if (!triggerMode && !fe)
				{

				    if (i==0) AudioOut(I, 0);
				    else if (i==1) CVOut(I, 0);
				    else PulseOut(I, false);
				    LedOn((i*2)+I, false);
				    }*/
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (pulseCounter[i] > 0)
			{
				pulseCounter[i]--;
				if (pulseCounter[i] == 0 && triggerMode)
				{
					if (i == 0)
						AudioOut(I, 0);
					else if (i == 1)
						CVOut(I, 0);
					else
						PulseOut(I, false);
					LedOn((i * 2) + I, false);
				}
			}
		}

		last_inpulse = inpulse;
		last_k = k;
		lasts = s;
		lastAudioInHigh = audioInHigh;
	}
};



////////////////////////////////////////////////////////////////////////////////
// Bernoulli Gate
template <int I>
class bernoulli : virtual public ComputerCard
{
public:
	bernoulli_gate bg;
	int kex;
	Switch lastSwitch;
	bool out;
	bernoulli()
	{
		kex = 0;
		out = false;
	}
	void __not_in_flash_func(Sample)()
	{
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));
		int32_t cv = CVIn(I) + k;
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;
		CVOut(I, CVIn(I));

		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main)) >> 10;
		}
		else
		{
			Switch sv = SwitchVal();
			if (sv != lastSwitch)
			{
				lastSwitch = sv;
				if (sv == Switch::Down)
				{
					kex++;
					if (kex == 4) kex = 0;
				}
			}
		}

		bg.set_toggle(kex & 1);
		bg.set_and_with_input(kex & 2);

		if (PulseInRisingEdge(I))
		{
			out = bg.step(cv, true);
		}
		if (PulseInFallingEdge(I))
		{
			out = bg.step(cv, false);
			if (bg.awi) out = false;
		}

		PulseOut(I, out);
		LedOn(4 + I, out);
		AudioOut(I, out ? 0 : 2000);
		LedOn(I, kex & 1);
		LedOn(I + 2, kex & 2);
	}
};



////////////////////////////////////////////////////////////////////////////////
// Pulse generator

// Input on pulse in
// Output on pulse out
// CV/Audio in set speed and number of pulses
template <int I>
class pulsegen : virtual public ComputerCard
{
public:
	volatile int32_t time, pulseCounter, pulseCounter2, pulseCount, pulsePeriod, pulsePeriod2, time2;
	pulsegen()
	{
		pulseCounter = 0;
		pulseCounter2 = 0;
		pulsePeriod = 0;
		pulsePeriod2 = 0;
		time = 0;
		time2 = 0;
	}

	void __not_in_flash_func(Sample)()
	{
		bool connected = Connected(ComputerCard::Input(Input::Pulse1 + I));
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));
		int32_t cv = CVIn(I) + k;
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;

		int32_t kex;
		if (I == 0)
		{
			kex = KnobVal(ComputerCard::Knob(Knob::Main));
		}
		else
		{
			if (SwitchVal() == Switch::Up)
				kex = 3000;
			else if (SwitchVal() == Switch::Middle)
				kex = 1200;
			else
				kex = 0;
		}
		kex += AudioIn(I);
		if (kex > 4095) kex = 4095;
		if (kex < 0) kex = 0;

		if (connected)
		{
			pulsePeriod = (cv << 2) + 1000;
			if (PulseInRisingEdge(I))
			{
				time = 0;
				pulseCount = (kex >> 9);
				LedOn(I, true); // input on top led
			}


			if (time == 0 && pulseCount > 0)
			{
				PulseOut(I, true);
				LedOn(4 + I, true); // output on bottom led
				pulseCounter = 100;
				time = pulsePeriod;
				pulseCount--;
			}
		}
		else
		{

			pulsePeriod = ((cv * cv) >> 8) + 200;
			pulsePeriod2 = ((kex * kex) >> 8) + 200;
			if (time == 0)
			{
				PulseOut(I, true);
				LedOn(4 + I, true); // output on bottom led
				pulseCounter = 100;
				time = pulsePeriod;
			}
			if (time2 == 0)
			{
				CVOut(I, 2000);
				LedOn(2 + I, true); // output on bottom led
				pulseCounter2 = 100;
				time2 = pulsePeriod2;
			}
		}

		if (time > 0) time--;
		if (time2 > 0) time2--;

		if (pulseCounter > 0)
		{
			pulseCounter--;
			if (pulseCounter == 0)
			{
				PulseOut(I, false);
				LedOn(4 + I, false);
				LedOn(I, false);
			}
		}
		if (pulseCounter2 > 0)
		{
			pulseCounter2--;
			if (pulseCounter2 == 0)
			{
				CVOut(I, 0);
				LedOn(2 + I, false);
			}
		}
	}
};


////////////////////////////////////////////////////////////////////////////////
// Euclidean rhythms

// http://web.archive.org/web/20190218190346/http://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained
//
// Num steps = X/Y knob + CV in
// Num pulses = Main/switch + audio in
// Pulse input triggers
// Audio out gives trigger per bar
// CV out gives Euclidean gate output
// Pulse out gives Euclidean trigger output
template <int I>
class euclidean : virtual public ComputerCard
{
public:
	volatile int32_t pulseCounter, step, bucket, inputCounter, nPulses, metronome;
	Switch lastSwitch;
	euclidean()
	{
		lastSwitch = Switch::Down;
		step = 0;
		bucket = 0;
		pulseCounter = 0;
		inputCounter = 0;
		nPulses = 1;
		metronome = 1;
	}

	void __not_in_flash_func(Sample)()
	{
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I));
		k += CVIn(I);
		if (k > 4095) k = 4095;
		if (k < 0) k = 0;
		int32_t nSteps = (k >> 8) + 1; // 1 to 16

		if (I == 0)
		{
			nPulses = (KnobVal(ComputerCard::Knob(Knob::Main)) >> 8) + 1;
		}
		else
		{

			Switch sv = SwitchVal();
			if (sv != lastSwitch)
			{
				lastSwitch = sv;
				if (sv == Switch::Down)
				{
					nPulses++;
					if (nPulses > nSteps) nPulses = 1;
				}
			}
		}

		int nPulsesWithCV = nPulses;

		nPulsesWithCV += AudioIn(I) >> 7;
		if (nPulses > nSteps) nPulses = nSteps;

		if (PulseInRisingEdge(I) || (Disconnected(ComputerCard::Input(Input::Pulse1 + I)) && metronome == 0))
		{
			step++;
			if (step >= nSteps)
			{
				step = 0;
				bucket = nSteps - nPulsesWithCV;
				AudioOut(I, 2000);
				inputCounter = 100;
			}

			LedBrightness(I, 4095 - ((step << 12) / nSteps)); // led brightness shows step
			bucket += nPulsesWithCV;
			if (bucket >= nSteps)
			{
				bucket -= nSteps;
				pulseCounter = 100;
				PulseOut(I, true);
				CVOut(I, 2000);
				LedOn(4 + I, true); // output on bottom led
			}
		}

		if (PulseInFallingEdge(I))
		{
			CVOut(I, 0);
		}
		if (pulseCounter > 0)
		{
			pulseCounter--;
			if (pulseCounter == 0)
			{
				PulseOut(I, false);
				LedOn(4 + I, false);
			}
		}


		if (inputCounter > 0)
		{
			inputCounter--;
			if (inputCounter == 0)
			{
				AudioOut(I, 0);
			}
		}

		if (metronome <= 0)
		{
			metronome = 6000; // = 480 semiquavers per minute = 120bpm
		}
		metronome--;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Pitch quantiser

template <int I>
class quantiser : virtual public ComputerCard
{
public:
	int note_in = 69, pulseCounter = 0;
	constexpr static uint8_t scales[8][12] = {
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, // chromatic
		{ 0, 0, 2, 2, 4, 4, 5, 7, 7, 9, 9, 11 }, // major
		{ 0, 0, 2, 2, 3, 3, 5, 7, 7, 8, 8, 11 }, // minor
		{ 0, 0, 0, 2, 2, 5, 5, 7, 7, 7, 9, 9 }, // pentatonic
		{ 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10 }, // whole toen
		{ 0, 1, 3, 3, 4, 6, 6, 7, 9, 9, 10, 12 }, // Octatonic / Messiaen 2nd mode
		{ 0, 0, 0, 0, 4, 4, 4, 4, 7, 7, 7, 7 }, // Major triad
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	}; // Octaves

	void __not_in_flash_func(Sample)()
	{
		static uint32_t kex;
		static int lastQuantisedNote = 0;
		static Switch lastSwitch = SwitchVal();


		if (Disconnected(ComputerCard::Input(Input::Pulse1 + I)) || PulseInRisingEdge(I))
		{
			int32_t k = (KnobVal(ComputerCard::Knob(Knob::X + I)) - 2048)>>1;
			if (k<-200) k+=200;
			else if (k>200) k-=200;
			else k=0;
			int32_t note_in_cont = (CVIn(I) + k + 2048) << 8;
			if (note_in_cont<0) note_in_cont=0;
			int32_t note_in_up = (note_in_cont - 1000) / 7282;
			int32_t note_in_down = (note_in_cont + 1000) / 7282;

			if (note_in_up > note_in)
			{
				note_in = note_in_up;
			}
			else if (note_in_down < note_in)
			{
				note_in = note_in_down;
			}
		}

		if (I == 0)
		{
			kex = KnobVal(Knob::Main) >> 9;
		}
		else
		{
			Switch sv = SwitchVal();
			if (sv != lastSwitch)
			{
				lastSwitch = sv;
				if (sv == Switch::Down)
				{
					kex++;
					if (kex == 8) kex = 0;
				}
			}
		}

		int octave = note_in / 12;
		int noteval = note_in % 12;

		int quantisedNote = 12 * octave + scales[kex][noteval];
		if (lastQuantisedNote != quantisedNote || PulseInRisingEdge(I))
		{
			pulseCounter = 100;
			PulseOut(I, true);
		}

		CVOutMIDINote(I, quantisedNote);

		LedOn(I, kex & 0x4);
		LedOn(I + 2, kex & 0x2);
		LedOn(I + 4, kex & 0x1);


		if (pulseCounter > 0)
		{
			pulseCounter--;
			if (pulseCounter == 0)
			{
				PulseOut(I, false);
			}
		}

		lastQuantisedNote = quantisedNote;
	}
};

////////////////////////////////////////////////////////////////////////////////
// VCO

template <int I>
class vco : virtual public ComputerCard
{
public:
	int frame;
	bool quantised;
	
	void __not_in_flash_func(Sample)()
	{
		static Switch lasts = SwitchVal();
		static int switchMode=0;
		Switch s = SwitchVal();
		static int mode=0, submode=0;

		if (frame<=10000)
		{
			frame++;
			if (s==Switch::Down)
			{
				quantised=true;
			}
			else
			{
				quantised=false;
			}
		}
		
		if (I==0)
		{
			mode = (3*KnobVal(Knob::Main))>>12;
		}
		else
		{
			if (s == Switch::Down && s != lasts)
			{
				switchMode++;
				if (switchMode==9)
					switchMode=0;
				
				mode = switchMode/3;
				submode = switchMode%3;
			}

		}
		
		int32_t k = KnobVal(ComputerCard::Knob(Knob::X + I))/3 ;
		int32_t cv = 1900+CVIn(I) + k;
		if (cv > 4095) cv = 4095;
		if (cv < 0) cv = 0;

		int32_t kex;
		if (I==0)
		{
			kex = ((3*(KnobVal(Knob::Main)))&0xFFF) + AudioIn(I);
			if (kex > 4095) kex = 4095;
			if (kex < 0) kex = 0;
		}
		else
		{
			kex = submode * 2040;
			if (submode==1) kex+=1020;
		}

		if (quantised)
		{
			int32_t cvoct = cv/341;
			int32_t cvpitch_in_oct = cv%341;
			cvpitch_in_oct = (cvpitch_in_oct/28);
			if (cvpitch_in_oct==12) cvpitch_in_oct=11;
			cvpitch_in_oct*=28;
			cv = (cvoct*341)+cvpitch_in_oct;
		}
		
		int32_t out;

		if (mode==0)
		{
			phase_offs = kex << 19;
			SquareSetFreq(ExpVoct(cv));
			out = SquareTick();
		}
		else if (mode == 1)
		{
			phase_offs = kex << 19;
			SquareSetFreq(ExpVoct(cv));
			out = SawTriTick();
		}
		else if (mode == 2)
		{
			int32_t noise = (rnd12()-2048);
			noise *= (4095-kex);
			noise >>=14;

			// Fitted tuning curve for filter, at max resonance
			int32_t cv6 = cv>>6;
			int32_t cv_warped = -((161*cv6*cv6*cv6)>>16)+((70647*cv)>>16) + ((106086*cv6*cv6)>>20) - 731;
			if (cv_warped<0) cv_warped=0;
			if (cv_warped>4095) cv_warped=4095;
			ff = ExpVoct(cv_warped);//cv<<14;//1000;
			resonance = kex<<1;
			out = Filt64Tick(noise);
		}
		else
		{
			out = 0;
		}

		clip(out);
		AudioOut(I, out);
		lasts = s;
		{ // Gate to trigger
			static int gateToTrigCounter=0;
			if (PulseInRisingEdge(I)) gateToTrigCounter = 100;
			if (gateToTrigCounter) PulseOut(I, --gateToTrigCounter);
		}
	}

	
	vco()
	{
		frame=0;
		quantised=false;
		dphase = 0;
		last_dphase = 0;
		phase_incr = 0;
		invc = 1;
		SquareSetFreq(440*89478);

		
		sa1 = 0;
		sa2 = 0;
		sa3 = 0;
		sa4 = 0;
		ff = 1000;
		resonance = 0;
	}

	
// Filter tick
	int32_t Filt64Tick( int32_t s)
	{
		int64_t ff64 = ff;
		int64_t res64 = (resonance*6)>>3;
		// Subtract feedback
		int64_t sa = (s<<12)*(int64_t)ff - ((100000+(ff64>>3))*(res64>>4)*(sa4>>3)); // 

		sa1 = (sa + sa1*(268435456-ff))>>28;
		sa2 = (sa1*ff + sa2*(268435456-ff))>>28;
		sa3 = (sa2*ff + sa3*(268435456-ff))>>28;
		sa4 = (sa3*ff + sa4*(268435456-ff))>>28;
		sa4 = (8388608*sa4)/(8388608+std::abs(sa4));
		return (sa2-sa1)>>10;
	}


	void SquareSetFreq(int32_t f)
	{
		freq = f;
		phase_incr = freq; // freq is roughly 89478 per Hz at sr=48kHz
		invc = phase_incr>>15;
	}

	int32_t SquareTick()
	{
		int32_t po = (2147483648 - phase_offs)>>1;
		if (po<21474836) po = 21474836;
		// Calculate values for current phase offeset but previous phase
		int64_t dphase2 =  ((dphase-po)>>8);
		dphase2 *= dphase2; // 0 to 1073741824
		last_dphase = dphase2;


		dphase2 = ((dphase+po)>>8); // -32768 to 32767
		dphase2 *= dphase2; // 0 to 1073741824
		last_dphase_osc2 = dphase2;

		// Now advance phase, calculate for current phase offset
		dphase += phase_incr; // -2147483648 to 2147483647
		
		dphase2 = ((dphase-po)>>8);
		dphase2 *= dphase2; // 0 to 1073741824
		int32_t retval = (dphase2 - last_dphase)>>16; // -1073741824 to 1073741824 = ±2^30


		dphase2 = ((dphase+po)>>8); // -32768 to 32767
		dphase2 *= dphase2; // 0 to 1073741824
	  	retval -= (dphase2 - last_dphase_osc2)>>16; // -1073741824 to 1073741824 = ±2^30
		
		return (retval/invc)>>5; 
	}

	
	int32_t SawTriTick()
	{	   
		// Calculate previous oscillator phase value, using current phase offset
		// Doing this doubles workload, but allows us to have audio rate modulation
		// of tri-sine parameter, without so many horrible artefacts
		int64_t dphase2 = (dphase>>12);
		int32_t p = phase_offs>>12;
		if (dphase < phase_offs)
		{
			dphase2 = ((dphase2+524288)*(dphase2-p));
			dphase2 <<=11;
			dphase2 /= (524288+p);
		}
		else
		{
			dphase2 = ((524288-dphase2)*(dphase2-p));
			dphase2 <<=11;
			dphase2 /= (524288-p);
		}
		last_dphase = dphase2;

		// Advance phase
		dphase += phase_incr; // -2147483648 to 2147483647
		
		// Recalculate everything using updated phase offset
		dphase2 = (dphase>>12);
		if (dphase < phase_offs)
		{
			dphase2 = ((dphase2+524288)*(dphase2-p));
			dphase2 <<=11;
			dphase2 /= (524288+p);
		}
		else
		{
			dphase2 = ((524288-dphase2)*(dphase2-p));
			dphase2 <<=11;
			dphase2 /= (524288-p);
		}

		// Now calculate difference between previous and current offsets
		int32_t retval = dphase2 - last_dphase;
		last_dphase = dphase2;
		retval = retval/invc;
		return (retval>>4)+(retval>>5);
	}

	
private:
	int32_t freq;
	int32_t phase_incr, dphase, invc;
	int64_t  last_dphase, last_dphase_osc2;
	int32_t dphase_osc2, phase_offs;


	// filter
	int64_t sa1, sa2, sa3, sa4;
	int32_t ff, resonance;
	
};

////////////////////////////////////////////////////////////////////////////////

class UtilityPair : public UTILITY1<0>, UTILITY2<1>
{
public:
	void ProcessSample()
	{
		UTILITY1<0>::Sample();
		UTILITY2<1>::Sample();
	}
};

int main()
{

	set_sys_clock_khz(160000, true);
	UtilityPair up;
	up.EnableNormalisationProbe();
	up.Run();
}
