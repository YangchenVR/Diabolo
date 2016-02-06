#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/date_time/time_defs.hpp>
#include <boost/date_time/special_defs.hpp>
#include <boost/date_time/compiler_config.hpp>

namespace VR
{
	namespace App
	{
		namespace OpenGL
		{

			typedef boost::posix_time::ptime vrTicks;
			typedef boost::posix_time::time_duration vrTicksDiff;
			typedef __int64 vrTickType;

			class vrFPS
			{
			public:
				vrFPS()	{}
				~vrFPS(){}
			public:
				enum{ MAXSAMPLES = 64 };
				vrTickType ticklist[MAXSAMPLES];
				int tickindex;
				vrTickType ticksum;
				vrTicks tickStart;
				vrTicks tickEnd;

				void sdkCreateTimer()
				{
					tickindex = ticksum = 0;
					memset(&ticklist[0], '\0', sizeof(ticklist[0])* MAXSAMPLES);
				}

				double sdkGetAverageTimerValue()
				{

					vrTickType newtick = (tickEnd - tickStart).total_microseconds();// .total_milliseconds();
					ticksum -= ticklist[tickindex];  /* subtract value falling off */
					ticksum += newtick;              /* add new value */
					ticklist[tickindex] = newtick;   /* save new value so it can be subtracted later */
					if (++tickindex == MAXSAMPLES)    /* inc buffer index */
						tickindex = 0;

					
					/* return average */
					return (((double)ticksum / MAXSAMPLES));
				}

				//sdkResetTimer

				void sdkStartTimer()
				{
					tickStart = boost::posix_time::microsec_clock::local_time();
				}

				void sdkStopTimer()
				{
					tickEnd = boost::posix_time::microsec_clock::local_time();
				}
			};
		}
	}
}