#ifndef ISOLVER_GLOBAL_H
#define ISOLVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ISOLVER_LIBRARY)
#  define ISOLVER_EXPORT Q_DECL_EXPORT
#else
#  define ISOLVER_EXPORT Q_DECL_IMPORT
#endif

#endif // ISOLVER_GLOBAL_H
