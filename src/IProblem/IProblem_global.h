#ifndef IPROBLEM_GLOBAL_H
#define IPROBLEM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IPROBLEM_LIBRARY)
#  define IPROBLEM_EXPORT Q_DECL_EXPORT
#else
#  define IPROBLEM_EXPORT Q_DECL_IMPORT
#endif

#endif // IPROBLEM_GLOBAL_H
