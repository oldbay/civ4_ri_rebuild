#pragma once

#include "CvStructs.h"


// MOD - START - Population Metrics
template <typename TKey>
inline int TurnValueMap<TKey>::getTurnValue(TKey eKey, int iTurn) const
{
    typename data::const_iterator it = aValues.find(std::make_pair(eKey, iTurn));
	if (it!=aValues.end())
	{
		return it->second;
	}
	return 0;
}

template <typename TKey>
inline void TurnValueMap<TKey>::setTurnValue(TKey eKey, int iTurn, int iValue)
{
	if (iValue != 0)
	{
        std::pair<typename data::iterator, bool> it = aValues.insert(std::make_pair(std::make_pair(eKey, iTurn), iValue));

		if (it.second)
		{
			// New entry
			updateGlobalStats(1, iValue);
			updateKeyedStats(eKey, 1, iValue);
		}
		else
		{
			// Updated entry
			int iOldValue = it.first->second;

			it.first->second = iValue;

			updateGlobalStats(0, iOldValue, iValue);
			updateKeyedStats(eKey, 0, iOldValue, iValue);
		}
	}
	else
	{
        typename data::iterator it = aValues.find(std::make_pair(eKey, iTurn));
		if (it != aValues.end())
		{
			// Deleted entry
			int iOldValue = it->second;
			aValues.erase(it);

			updateGlobalStats(-1, iOldValue, 0);
			updateKeyedStats(eKey, -1, iOldValue, 0);
		}
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::changeTurnValue(TKey eKey, int iTurn, int iChange)
{
	if (iChange != 0)
	{
        std::pair<typename data::iterator, bool> it = aValues.insert(std::make_pair(std::make_pair(eKey, iTurn), iChange));

		if (it.second)
		{
			// New entry
			updateGlobalStats(1, iChange);
			updateKeyedStats(eKey, 1, iChange);
		}
		else
		{
			int iOldValue = it.first->second;
			int iNewValue = iOldValue + iChange;

			if (iNewValue != 0)
			{
				// Updated entry
				it.first->second = iNewValue;
				updateGlobalStats(0, iOldValue, iNewValue);
				updateKeyedStats(eKey, 0, iOldValue, iNewValue);
			}
			else
			{
				// Deleted entry
				aValues.erase(it.first);
				updateGlobalStats(-1, iOldValue, 0);
				updateKeyedStats(eKey, -1, iOldValue, 0);
			}
		}
	}
}

template <typename TKey>
inline TurnValueStats TurnValueMap<TKey>::getStats(TKey eKey) const
{
    typename keyed_stats::const_iterator it = aKeyedStats.find(eKey);
	if (it != aKeyedStats.end())
	{
		return it->second;
	}
	else
	{
		return TurnValueStats();
	}
}

template <typename TKey>
inline int TurnValueMap<TKey>::getNumValues(TKey eKey) const
{
	return getStats(eKey).iNumValues;
}

template <typename TKey>
inline int TurnValueMap<TKey>::getMinValue(TKey eKey) const
{
	return getStats(eKey).iMinValue;
}

template <typename TKey>
inline int TurnValueMap<TKey>::getMaxValue(TKey eKey) const
{
	return getStats(eKey).iMaxValue;
}

template <typename TKey>
inline void TurnValueMap<TKey>::clear()
{
	aValues.clear();
	kGlobalStats.clear();
	aKeyedStats.clear();
}

// TurnValueMap Encoding
// ---------------------
//
// NumValues  [ Key    | SeriesLength [ Turn   | Value  | ChunkLength | DiffEncodingSize | Diff          | Diff          | Diff          | Diff          | ... ] ... ]
// 32-bit       32-bit   32-bit         32-bit   32-bit   16-bit        8-bit              [8|16|32]-bit   [8|16|32]-bit   [8|16|32]-bit   [8|16|32]-bit
//
// ---------------------

template <typename TKey>
inline void TurnValueMap<TKey>::read(FDataStreamBase* pStream)
{
	int iNumValues;
	pStream->Read(&iNumValues);
	aValues.clear();
	for (int iI = 0; iI < iNumValues;)
	{
		int iStreamKey;
		int iSeriesLength;

		pStream->Read(&iStreamKey);
		pStream->Read(&iSeriesLength);

		FAssert(iSeriesLength > 0);
		FAssert(iSeriesLength + iI <= iNumValues);

		TKey eStreamKey = (TKey)iStreamKey;
		int iSeriesEnd = iI + iSeriesLength;

		while (iI < iSeriesEnd)
		{
			int iStreamTurn;
			int iStreamValue;
			unsigned short iChunkLength;

			pStream->Read(&iStreamTurn);
			pStream->Read(&iStreamValue);
			pStream->Read(&iChunkLength);

			FAssert(iChunkLength > 0);
			FAssert(iChunkLength + iI <= iNumValues);

			aValues.insert(std::make_pair(std::make_pair(eStreamKey, iStreamTurn), iStreamValue));

			int iChunkEnd = iI + iChunkLength;

			iI++;
			iStreamTurn++;

			if (iChunkLength > 1)
			{
				byte iEncodingSize;
				pStream->Read(&iEncodingSize);
				FAssert(iEncodingSize == 1 || iEncodingSize == 2 || iEncodingSize == 4);

				switch (iEncodingSize)
				{
				case 1:
					for (; iI < iChunkEnd; iI++,iStreamTurn++)
					{
						byte iDiff;
						pStream->Read(&iDiff);
						iStreamValue += (signed char)iDiff;
						aValues.insert(std::make_pair(std::make_pair(eStreamKey, iStreamTurn), iStreamValue));
					}
					break;
				case 2:
					for (; iI < iChunkEnd; iI++,iStreamTurn++)
					{
						short iDiff;
						pStream->Read(&iDiff);
						iStreamValue += iDiff;
						aValues.insert(std::make_pair(std::make_pair(eStreamKey, iStreamTurn), iStreamValue));
					}
					break;
				case 4:
					for (; iI < iChunkEnd; iI++,iStreamTurn++)
					{
						int iDiff;
						pStream->Read(&iDiff);
						iStreamValue += iDiff;
						aValues.insert(std::make_pair(std::make_pair(eStreamKey, iStreamTurn), iStreamValue));
					}
					break;
				}
			}
		}
	}

	updateGlobalStats();
	updateKeyedStats();
}

template <typename TKey>
inline void TurnValueMap<TKey>::write(FDataStreamBase* pStream)
{
	pStream->Write((int)aValues.size());
	if (aValues.size() <= 0)
	{
		return;
	}

    for (typename data::const_iterator it = aValues.begin(); it != aValues.end();)
	{
		TKey eStreamKey = it->first.first;

		// Find the end of the set of values for this key and count how many there are
		int iSeriesLength = 0;
		TurnValueMap<TKey>::data::const_iterator end;
		for (end = it; end != aValues.end(); ++end, ++iSeriesLength)
		{
			if (end->first.first != eStreamKey)
			{
				break;
			}
		}

		pStream->Write(eStreamKey);
		pStream->Write(iSeriesLength);

		// Pack the value set for this key into streams of values for consecutive turns that use the least number of bits per value
		while (it != end)
		{
			int iStartTurn = it->first.second;
			int iStartValue = it->second;
			int iStreamValue = iStartValue; // iStreamValue iterates through previous values so that we can calculate the difference from one value to the next

			unsigned short iChunkLength = 1;
			byte iEncodingSize = 1;

			// Scan the upcoming values in order to determine the most efficient encoding
			TurnValueMap<TKey>::data::const_iterator scan = it;
			for (++scan; scan != end && iChunkLength < MAX_UNSIGNED_SHORT; ++scan, iChunkLength++)
            {
				int iLoopTurn = scan->first.second;
				if (iLoopTurn != iStartTurn + iChunkLength)
				{
					// Break in turn sequence
					break;
				}

				int iLoopAmount = scan->second;
				int iDiff = iLoopAmount - iStreamValue;
				bool bValidByte = iDiff <= MAX_CHAR && iDiff >= MIN_CHAR;
				bool bValidWord = iDiff <= MAX_SHORT && iDiff >= MIN_SHORT;

				iStreamValue = iLoopAmount;

				if (iEncodingSize == 1)
				{
					if (bValidByte)
					{
						// The difference can fit inside of a signed byte, so we continue our scan
						continue;
					}

					if (iChunkLength >= 8)
					{
						// We've scanned at least the minimum count for a stream of 8 bit values
						// End this stream instead of increasing the encoding size of the entire stream
						break;
					}

					// We didn't reach the minimum count for a stream of 8 bit values
					// Incease the encoding size
					iEncodingSize = 2;
				}

				if (iEncodingSize == 2)
				{
					if (bValidByte && iChunkLength >= 4)
					{
						// The difference can fit inside of a smaller encoding
						// End this stream if we have reached our minimum stream count
						break;
					}

					if (bValidWord)
					{
						// The difference can fit inside of a signed word, so we continue our scan
						continue;
					}

					if (iChunkLength >= 6)
					{
						// We've scanned at least the minimum count for a stream of 16 bit values
						// End this stream instead of increasing the encoding size of the entire stream
						break;
					}

					// We didn't reach the minimum count for a stream of 16 bit values
					// Incease the encoding size
					iEncodingSize = 4;
				}

				if ((bValidByte || bValidWord) && iChunkLength >= 3)
				{
					// The difference can fit inside of a smaller encoding
					// End this stream if we have reached our minimum count for a stream of 32 bit values
					break;
				}
			}

			// Write out the header for this value stream
			// Note that this includes a 32 bit start value even if the differences are encoded with fewer bits
			pStream->Write(iStartTurn);
			pStream->Write(iStartValue);
			pStream->Write(iChunkLength);

			it++; // Advance the iterator to reflect the fact that the first value has been written

			if (iChunkLength > 1)
			{
				// If there is more than one value in this stream then record the number of bytes used to encode the differences
				pStream->Write(iEncodingSize);

				// Write out the differences using the appropriate number of bytes
				iStreamValue = iStartValue;

				switch (iEncodingSize)
				{
				case 1:
					for (int iI = 1; iI < iChunkLength; iI++, it++)
					{
						int iDiff = it->second - iStreamValue;
						pStream->Write((signed char)iDiff);
						iStreamValue = it->second;
					}
					break;
				case 2:
					for (int iI = 1; iI < iChunkLength; iI++, it++)
					{
						int iDiff = it->second - iStreamValue;
						pStream->Write((short)iDiff);
						iStreamValue = it->second;
					}
					break;
				default:
					for (int iI = 1; iI < iChunkLength; iI++, it++)
					{
						int iDiff = it->second - iStreamValue;
						pStream->Write(iDiff);
						iStreamValue = it->second;
					}
					break;
				}
			}
		}
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateGlobalStats()
{
    typename data::const_iterator it=aValues.begin();
	if (it != aValues.end())
	{
		kGlobalStats.iNumValues = 0;
		kGlobalStats.iMinValue = it->second;
		kGlobalStats.iMaxValue = it->second;
		for (++it; it != aValues.end(); ++it)
		{
			kGlobalStats.iNumValues++;
			kGlobalStats.iMinValue = std::min(kGlobalStats.iMinValue, it->second);
			kGlobalStats.iMaxValue = std::max(kGlobalStats.iMaxValue, it->second);
		}
	}
	else
	{
		kGlobalStats.iNumValues = 0;
		kGlobalStats.iMinValue = 0;
		kGlobalStats.iMaxValue = 0;
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateGlobalStats(int iCountChange, int iValue)
{
	kGlobalStats.iNumValues += iCountChange;
	
	if (kGlobalStats.iNumValues == 0)
	{
		// No entries
		kGlobalStats.iMinValue = 0;
		kGlobalStats.iMaxValue = 0;
	}
	else if (kGlobalStats.iNumValues == 1)
	{
		// Single entry
		kGlobalStats.iMinValue = iValue;
		kGlobalStats.iMaxValue = iValue;
	}
	else
	{
		kGlobalStats.iMinValue = std::min(kGlobalStats.iMinValue, iValue);
		kGlobalStats.iMaxValue = std::max(kGlobalStats.iMaxValue, iValue);
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateGlobalStats(int iCountChange, int iOldValue, int iValue)
{
	if ((iOldValue == kGlobalStats.iMinValue && iValue > kGlobalStats.iMinValue) || (iOldValue == kGlobalStats.iMaxValue && iValue < kGlobalStats.iMaxValue))
	{
		updateGlobalStats();
	}
	else
	{
		updateGlobalStats(iCountChange, iValue);
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateKeyedStats()
{
	aKeyedStats.clear();

    typename data::const_iterator it=aValues.begin();
	if (it != aValues.end())
	{
		TKey eKey = it->first.first;
		TurnValueStats kStats = TurnValueStats(1, it->second, it->second);

		for (++it; it != aValues.end(); ++it)
		{
			if (it->first.first != eKey)
			{
				aKeyedStats.insert(std::make_pair(eKey, kStats));
				eKey = it->first.first;
				kStats = TurnValueStats(1, it->second, it->second);
			}
			else
			{
				kStats.iNumValues++;
				kStats.iMinValue = std::min(kStats.iMinValue, it->second);
				kStats.iMaxValue = std::max(kStats.iMaxValue, it->second);
			}
		}

		aKeyedStats.insert(std::make_pair(eKey, kStats));
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateKeyedStats(TKey eKey)
{
	TurnValueStats kStats = TurnValueStats();

    for (typename data::const_iterator it=aValues.lower_bound(std::make_pair(eKey, 0)); it != aValues.end(); ++it)
	{
		if (it->first.first != eKey)
		{
			break;
		}

		kStats.iNumValues++;
		if (kStats.iNumValues == 1)
		{
			kStats.iMinValue = it->second;
			kStats.iMaxValue = it->second;
		}
		else
		{
			kStats.iMinValue = std::min(kStats.iMinValue, it->second);
			kStats.iMaxValue = std::max(kStats.iMaxValue, it->second);
		}
	}

	if (kStats.iNumValues > 0)
	{
        std::pair<typename keyed_stats::iterator, bool> it = aKeyedStats.insert(std::make_pair(eKey, kStats));
		if (!it.second)
		{
			it.first->second.iNumValues = kStats.iNumValues;
			it.first->second.iMinValue = kStats.iMinValue;
			it.first->second.iMaxValue = kStats.iMaxValue;
		}
	}
	else
	{
        typename keyed_stats::iterator it = aKeyedStats.find(eKey);
		if (it != aKeyedStats.end())
		{
			aKeyedStats.erase(it);
		}
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateKeyedStats(TKey eKey, int iCountChange, int iValue)
{
	FAssert(iCountChange == 1);

    std::pair<typename keyed_stats::iterator, bool> it = aKeyedStats.insert(std::make_pair(eKey, TurnValueStats(1, iValue, iValue)));
	if (!it.second)
	{
		it.first->second.iNumValues += iCountChange;
		it.first->second.iMinValue = std::min(it.first->second.iMinValue, iValue);
		it.first->second.iMaxValue = std::max(it.first->second.iMaxValue, iValue);
	}
}

template <typename TKey>
inline void TurnValueMap<TKey>::updateKeyedStats(TKey eKey, int iCountChange, int iOldValue, int iValue)
{
	FAssert(iCountChange == -1 || iCountChange == 0);
	FAssert(aKeyedStats.size() > 0);

    typename keyed_stats::iterator it = aKeyedStats.find(eKey);
	if (it != aKeyedStats.end())
    {
		it->second.iNumValues += iCountChange;
		if (it->second.iNumValues == 0)
		{
			aKeyedStats.erase(it);
		}
		else if (it->second.iNumValues == 1)
		{
			it->second.iMinValue = iValue;
			it->second.iMaxValue = iValue;
		}
		else
		{

			if ((iOldValue == it->second.iMinValue && iValue > it->second.iMinValue) || (iOldValue == it->second.iMaxValue && iValue < it->second.iMaxValue))
			{
				updateKeyedStats(eKey);
			}
			else
			{
				it->second.iMinValue = std::min(it->second.iMinValue, iValue);
				it->second.iMaxValue = std::max(it->second.iMaxValue, iValue);
			}
		}
	}
}
// MOD - END - Population Metrics
