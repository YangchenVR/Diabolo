#ifndef _AbstractApplication_H_
#define _AbstractApplication_H_

namespace VR
{
	namespace App
	{
		template< int dimension >
		class AbstractApplication
		{
		public://configure data
			enum { appDim = dimension };

		public:
			AbstractApplication(){}
			virtual ~AbstractApplication(){}
						
		public://framework interface
			virtual bool init() = 0;
			virtual bool pre_progress() = 0;
			virtual bool run() = 0;
			virtual bool post_progress() = 0;
		
		};
	}//namespace App
}//namespace VR
#endif//_AbstractApplication_H_