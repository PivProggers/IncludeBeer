// Serialization.cpp : Определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <string>
#include "serialization.h"
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "..\..\IncludeBeer\RemoteWorkStation\src\Commands\Command.h"

int main() {
	// create and open a character archive for output
	std::ofstream ofs("filename");

	// create class instance
	string name("run");

	RunAppliation g(name, NULL);

	// save data to archive
	{
		boost::archive::text_oarchive oa(ofs);
		// write class instance to archive
		oa << g;
		// archive and stream closed when destructors are called
	}

	// ... some time later restore the class instance to its orginal state
	gps_position newg;
	{
		// create and open an archive for input
		std::ifstream ifs("filename");
		boost::archive::text_iarchive ia(ifs);
		// read class state from archive
		ia >> newg;
		// archive and stream closed when destructors are called
	}


	namespace boost {
		namespace serialization {

			template<class Archive>
			void serialize(Archive& ar, gps_position& g, const unsigned int version)
			{
				ar& g.degrees;
				ar& g.minutes;
				ar& g.seconds;
			}

		} // namespace serialization
	}

	return 0;
}
