/*
 * tmos++.h
 *
 *  Created on: Mar 18, 2013
 *      Author: miro
 */

#ifndef TMOS___H_
#define TMOS___H_

namespace tmos
{

  typedef unsigned int 		size_t;
  typedef int				ptrdiff_t;

} // namespace tmos

// Iff -fno-exceptions, transform error handling code to work without it.
# define __try      if (true)
# define __catch(X) if (false)
# define __throw_exception_again
# define __throw_bad_alloc() while(0)
# define __throw_out_of_range(x) while(0)
# define __throw_length_error(x) while(0)

#include <tmos_inc.h>

#include <trace.h>
#include <port_cpp.h>

#endif /* TMOS___H_ */
