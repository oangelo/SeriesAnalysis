/*
 * ts_exceptions.h
 *
 *  Created on: Oct 6, 2009
 *      Author: gralha
 */

#ifndef TS_EXCEPTIONS_H_
#define TS_EXCEPTIONS_H_

#include <iostream>
#include <string>


class Index_error: public std::exception
{
    public:

        Index_error(std::string message) throw() ;
        ~Index_error() throw ();

        virtual const char* what() const throw();

    protected:
        std::string __message;

};

class  Value_error: public std::exception
{
    public:

         Value_error(std::string message) throw() ;
        ~ Value_error() throw ();

        virtual const char* what() const throw();

    protected:
        std::string __message;

};


#endif /* TS_EXCEPTIONS_H_ */
