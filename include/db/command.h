#pragma once
// #include "../include/MD5.h"
/*--------------------QxOrm--------------------*/
#include <QxOrm_Impl.h>
#include <QxOrm.h>
#define QxDao qx::dao
#define QxTransaction qx::QxSession

#ifdef _BUILDING_QX_PRO
#define QX_PRO_DLL_EXPORT QX_DLL_EXPORT_HELPER
#else
#define QX_PRO_DLL_EXPORT QX_DLL_IMPORT_HELPER
#endif

#ifdef _BUILDING_QX_PRO
#define QX_REGISTER_HPP_QX_PRO QX_REGISTER_HPP_EXPORT_DLL
#define QX_REGISTER_CPP_QX_PRO QX_REGISTER_CPP_EXPORT_DLL
#else
#define QX_REGISTER_HPP_QX_PRO QX_REGISTER_HPP_IMPORT_DLL
#define QX_REGISTER_CPP_QX_PRO QX_REGISTER_CPP_IMPORT_DLL
#endif
/*--------------------QxOrm--------------------*/

/*--------------------DECLARE_SINGLETON--------------------*/
#include <QMutex>
#include <QScopedPointer>
#define DECLARE_SINGLETON(ClassName)                 \
    Q_DISABLE_COPY(ClassName)                        \
public:                                              \
    static ClassName *instance()                     \
    {                                                \
        static QMutex mutex;                         \
        static QScopedPointer<ClassName> s_instance; \
        if (Q_UNLIKELY(!s_instance))                 \
        {                                            \
            mutex.lock();                            \
            if (!s_instance)                         \
            {                                        \
                s_instance.reset(new ClassName);     \
            }                                        \
            mutex.unlock();                          \
        }                                            \
        return s_instance.data();                    \
    }                                                \
/*--------------------DECLARE_SINGLETON--------------------*/
