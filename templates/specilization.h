// 
// File:   specilization.h
// Author: bwang1
//
// Created on October 10, 2007, 1:55 PM
//

#ifndef _SPECILIZATION_H
#define	_SPECILIZATION_H
//template specilization
template <typename T>
int compares(const T &v1, const T &v2);
template <>
int compares<const char*>(const char* const &v1, const char* const &v2);



#endif	/* _SPECILIZATION_H */

