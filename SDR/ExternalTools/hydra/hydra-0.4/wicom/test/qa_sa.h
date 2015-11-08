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

#ifndef _INCLUDED_QA_SA_H_
#define _INCLUDED_QA_SA_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

class qa_sa : public CppUnit::TestCase{
  private:
    /* Create list of sub-tests */
    CPPUNIT_TEST_SUITE(qa_sa);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST(test2);
    CPPUNIT_TEST_SUITE_END();

  private:
    /* Create declarations for sub-tests */
    void test1();
    void test2();
};

#endif /* _INCLUDED_QA_SA_H_ */

