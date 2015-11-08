/* -*- c++ -*- */
/* 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <qa_sa.h>
#include <cppunit/TestAssert.h>

#include <itpp/itbase.h>
#include <itpp/base/timing.h>
#include "self_adaptation_mimo_ofdm.h"

using namespace itpp;
using std::cout;
using std::endl;

void qa_sa::test1()
{
  /*
  // use assertion statements to verify a condition
  int x = 10;
  CPPUNIT_ASSERT(x==10);
  */
	int data_size=10000;
	int error_size=1000;
	Real_Timer timer;
	timer.start();
	timer.tic();
	Self_Adaptation_MIMO_OFDM sad(2,0,0,0,2,1000,52,64,1);
	std::string fn("temp");
	timer.tic();
	sad.initialize_database(data_size,error_size);
	cout << "time: " << timer.toc() << endl;
	cout << "---> (" << data_size << "," << error_size << ") Database initialized" << endl;
	timer.tic();
	Array<cmat> chanz(56);
	vec featz(56);
	for (int x=0; x<1000; x++)
	{
		cout << "------> Iteration #" << x+1 << endl;
		RNG_randomize();
		for (int xxx=0; xxx<56; xxx++)
		{ 
			chanz(xxx)=randn_c(2,2);
		}
	//	cout << "time: " << timer.toc() << endl;
		int mcs=randi(1,0,7)(0); 
		bool yesorno=randi(1,0,1)(0);
		sad.update_database(chanz,0.1,mcs,yesorno);
		int recmcs=sad.recommend_parameters(chanz,0.1);
	//	cout << "time: " << timer.toc() << endl;
	 //	cout << "Packet Error: " << yesorno << endl;
		//cout << "MCS for Packet: " << mcs << endl;	
		cout << "---> MCS-" << x << " Recommended: " << recmcs << endl;
	}
	/*sad.print_data(std::cout,false);
	sad.save_database(fn);
	cout << "---> Database saved to " << fn << endl;
	cout << "time: " << timer.toc() << endl;
	sad.initialize_database(1,7,1,1,1,512,112,500,100);
	cout << "---> Blank Database intialized" << endl;
	cout << "time: " << timer.toc() << endl;
	//sad.print_data(std::cout);
	sad.load_database(fn);
	cout << "---> Database loaded from " << fn << endl;
	cout << "time: " << timer.toc() << endl;*/
	sad.print_data(std::cout,false);
}
