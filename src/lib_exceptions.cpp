/*
 * ts_exceptions.cpp
 *
 *  Created on: Oct 6, 2009
 *      Author: gralha
 */

#include "lib_exceptions.h"

Index_error::Index_error(std::string message) throw()
 {
            __message = message;
 }

Index_error::~Index_error() throw(){}

 const char* Index_error::what() const throw()
 {
            return (__message.c_str());
 }

 Value_error::Value_error(std::string message) throw()
  {
             __message = message;
  }

 Value_error::~Value_error() throw(){}

  const char* Value_error::what() const throw()
  {
             return (__message.c_str());
  }


