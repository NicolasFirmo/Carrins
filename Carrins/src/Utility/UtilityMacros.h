#pragma once
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define NIC_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define NIC_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define NIC_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define NIC_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define NIC_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define NIC_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define NIC_FUNC_SIG __func__
#else
#define NIC_FUNC_SIG "NIC_FUNC_SIG unknown!"
#endif

#define CONCAT(a,b) THIS_IS_STUPID(a,b)
#define THIS_IS_STUPID(a,b) a ## b
#define APPLINE(x) CONCAT(x,__LINE__)