/*
 * TuplePrinter.hpp
 *
 *  Created on: Dec 19, 2017
 *      Author: Antero Nousiainen
 */

#ifndef TUPLEPRINTER_HPP_
#define TUPLEPRINTER_HPP_

#include <ostream>
#include <tuple>

namespace image
{

     template<typename Tuple, std::size_t N>
     struct TuplePrinter
     {
         static void print(const Tuple & t, std::ostream * os)
         {
             TuplePrinter<Tuple, N-1>::print(t, os);
             *os << ", " << std::get<N-1>(t);
         }
     };

     template<typename Tuple>
     struct TuplePrinter<Tuple, 1>
     {
         static void print(const Tuple& t, std::ostream * os)
         {
             *os << std::get<0>(t);
         }
     };

}

#endif
