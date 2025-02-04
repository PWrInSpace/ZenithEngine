#pragma once
/*
* Precompiled headers listed here don't have to be included into .cpp files as they are added via buildsystem.
* Note that they must still be included into header files that require them!
* 
* Before adding new header specify all it's includes recursively in appearing order with following style.
* Also changes to than files should be considered with updating include lists.
* When in following includes appear same header it's content don't have to be listed.
* All listed headers should have global directory specyfication.
* 
* Below standard headers already included by Common project in alphabetic order.
* Keep this list up to date when changing it's PCH. Note that all Common headers are always included.
*/
/* STD HEADERS:
*** algorithm
*** cassert
*** cfloat
*** cmath
*** cstddef
*** cstdint
*** deque
*** exception
*** filesystem
*** functional
*** intrin.h
*** map
*** random
*** sstream
*** string
*** utility
*** vector
*/


// Standard headers, remove if appears above
#include <memory>