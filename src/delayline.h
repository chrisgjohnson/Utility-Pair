#ifndef DELAYLINE_H
#define DELAYLINE_H

template <uint32_t bufSize, typename storage=int16_t>
class DelayLine
{
public:
	unsigned writeInd;
	DelayLine()
	{
		writeInd=0;
		for (unsigned i=0; i<bufSize; i++)
		{
			delaybuf[i] = 0;
		}
	} 

	// delay is in 128ths of a sample
	storage ReadInterp(int32_t delay)
	{

		int32_t r = delay & 0x7F;
		
		int32_t readInd1 = (writeInd + (bufSize<<1) - (delay>>7) - 1)%bufSize;
		int32_t fromBuffer1 =  delaybuf[readInd1];
		int readInd2 = (writeInd + (bufSize<<1) - (delay>>7) - 2)%bufSize;
		int32_t fromBuffer2 =  delaybuf[readInd2];
		return (fromBuffer2*r + fromBuffer1*(128-r))>>7;
	}
	// delay is in samples
	storage ReadRaw(int32_t delay)
	{
		int32_t readInd1 = (writeInd + (bufSize<<1) - delay - 1)%bufSize;
		return  delaybuf[readInd1];
	}
	void Write(int16_t value)
	{
		delaybuf[writeInd] = value;

		writeInd++;
		if (writeInd>=bufSize) writeInd=0;
	}
private:
	storage delaybuf[bufSize];
};


#endif
