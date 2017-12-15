/* Generated by Cython 0.26.1 */

#ifndef __PYX_HAVE__grid
#define __PYX_HAVE__grid


#ifndef __PYX_HAVE_API__grid

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#ifndef DL_IMPORT
  #define DL_IMPORT(_T) _T
#endif

__PYX_EXTERN_C std::vector<std::vector<std::vector<double> > >  pel_grid(std::vector<std::vector<float> > , double, double, double, double);

#endif /* !__PYX_HAVE_API__grid */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initgrid(void);
#else
PyMODINIT_FUNC PyInit_grid(void);
#endif

#endif /* !__PYX_HAVE__grid */