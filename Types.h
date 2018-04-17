#ifndef _TYPES_H_
#define _TYPES_H_

template<typename T, int SIZE> struct Array
{
public:
  T data[SIZE];
};

typedef Array<float,2> float2;
typedef Array<float,3> float3;
typedef Array<float,4> float4;
typedef Array<unsigned char,2> uchar2;
typedef Array<unsigned char,3> uchar3;
typedef Array<unsigned char,4> uchar4;


#endif
