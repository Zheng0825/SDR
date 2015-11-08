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

#include <cppunit/TextTestRunner.h>
#include <cppunit/TestSuite.h>

#include <qa_wicom.h>

class qa_test_all
{
  public:
    static CppUnit::TestSuite *suite();
};

CppUnit::TestSuite *
qa_test_all::suite()
{
  CppUnit::TestSuite	*s = new CppUnit::TestSuite ("test-all");

  /* Add all unit tests here */
  s->addTest (qa_wicom::suite());

  return s;
}


int
main (int argc, char **argv)
{

  CppUnit::TextTestRunner	runner;

  runner.addTest (qa_test_all::suite() );

  bool was_successful = runner.run ("", false);

  return was_successful ? 0 : 1;
}
