1 __kernel void helloworld(__global double* in, __global double* out)
2 {
3     int num = get_global_id(0);
4     out[num] = in[num] / 2.4 *(in[num]/6) ;
5 }
