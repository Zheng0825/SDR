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

#include <qa_foo.h>
#include <cppunit/TestAssert.h>

#include <foo.h>

void qa_foo::test1()
{
  // use assertion statements to verify a condition
  Foo *f;
  int x = 0;
  f = new Foo(10);
  if (f) x = f->bar();
  CPPUNIT_ASSERT(x==10);
  if (f) delete f;
}

void qa_foo::test2()
{
  // use CppUnit Macros to check for other conditions
  int x,y;
  int z = 100;
  x = 100; y = 100;
  CPPUNIT_ASSERT_EQUAL(x,y);

  double c,d;
  double w = 12.34;
  double delta = 1.0e-6;
  c = w; d = w;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(c,d,delta);
}
