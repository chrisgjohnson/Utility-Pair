#ifndef DIVIDER_H
#define DIVIDER_H

////////////////////////////////////////
// Clock divider


class Divider
{
public:
	Divider()
	{
		divisor = 1;
		counter = 0;
	}
	void Set(uint8_t d)
	{
		divisor = d;
		if (counter >= divisor)
			counter = divisor - 1;
	}
	void SetResetPhase(uint8_t d)
	{
		counter=0;
		divisor = d;
		if (counter >= divisor)
			counter = divisor - 1;
	}
	bool Step(bool risingEdge)
	{
		if (risingEdge)
		{
			counter++;
			if (counter == divisor)
			{
				counter = 0;
				return true;
			}
			else
				return false;
		}
		else
		{
			if (divisor == 1)
				return false;
			else
				return counter == 0;
		}
	}
	uint8_t counter;
	uint8_t divisor;
};

#endif
