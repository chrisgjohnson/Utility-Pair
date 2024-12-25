#ifndef SAW_H
#define SAW_H

// Sawtooth oscillator with reduced aliasing. Uses algorithm of:
// Vesa Välimäki, 'Discrete-Time Synthesis of the Sawtooth Waveform
// With Reduced Aliasing' IEEE Signal Processing Letters 12(3), 2005
class Saw
{
public:
	Saw()
	{
		dphase = 0;
		last_dphase = 0;
		phase_incr = 0;
		invc = 1;
		freq = 440*89478;
	}

	void SetPhase(int32_t dp)
	{
		dphase = dp;
		last_dphase = dp;
	}
	
	void SetFreq(int32_t f)
	{
		freq = f;
		phase_incr = freq; // freq is roughly 89478 per Hz at sr=48kHz
		invc = phase_incr>>15;
	}

	int32_t Tick()
	{
		dphase += phase_incr; // -2147483648 to 2147483647
		int32_t dphase2 = (dphase>>16); // -32768 to 32767
		dphase2 *= dphase2; // 0 to 1073741824
		int32_t retval = dphase2 - last_dphase; // -1073741824 to 1073741824 = ±2^30
		last_dphase = dphase2;
		return retval/invc; 
	}
private:
	int32_t freq;
	int32_t phase_incr, dphase, last_dphase, invc;

};

#endif
