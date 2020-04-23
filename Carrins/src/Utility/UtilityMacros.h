#pragma once

#define CONCAT(a,b) THIS_IS_STUPID(a,b)
#define THIS_IS_STUPID(a,b) a ## b
#define APPLINE(x) CONCAT(x,__LINE__)