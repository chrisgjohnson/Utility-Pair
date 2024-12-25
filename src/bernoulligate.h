#ifndef BERNOULLIGATE_H
#define BERNOULLIGATE_H


class bernoulli_gate
{
public:
    // Discretised to 12-bit
	// Max value 4095*2^20 - 1
	uint32_t __not_in_flash_func(bgrand)()
	{
		static uint32_t lcg_seed = 1;
		lcg_seed = 1664525 * lcg_seed + 1013904223;
		return lcg_seed & 0xFFF00000;
	}


	bernoulli_gate()
	{
		toggle = 0;
		awi = 0;
		state = 0;
	}


	void __not_in_flash_func(set_toggle)(bool t)
	{
		toggle = t;
	}

	void __not_in_flash_func(set_and_with_input)(bool a)
	{
		awi = a;
	}


	bool __not_in_flash_func(step)(int32_t p, bool risingEdge)
	{
		if (!risingEdge) return state;
	
		uint32_t rand = bgrand();
		if (p < 0) p = 0;
		if (p > 4095) p = 4095;
		uint32_t up = p;
		up <<= 20; // 12-bit -> 32-bit conversion

		if (rand <= up)
		{
			if (toggle)
			{
				state = 1 - state;
			}
			else
			{
				state = 1;
			}
		}
		else
		{
			if (toggle)
			{
			}
			else
			{
				state = 0;
			}
		}

		return state;
	}


	bool toggle;
	bool awi;
	bool state;
};




#endif
