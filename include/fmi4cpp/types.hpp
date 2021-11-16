
#ifndef FMI4CPP_TYPES_HPP
#define FMI4CPP_TYPES_HPP

namespace fmi4cpp
{

typedef void* fmi4cppComponent; /* Pointer to FMU instance       */
typedef void* fmi4cppFMUstate; /* Pointer to internal FMU state */
typedef unsigned int fmi4cppValueReference;
typedef double fmi4cppReal;
typedef int fmi4cppInteger;
typedef int fmi4cppBoolean;
typedef char fmi4cppChar;
typedef const fmi4cppChar* fmi4cppString;
typedef char fmi4cppByte;

} // namespace fmi4cpp

#endif //FMI4CPP_TYPES_HPP
