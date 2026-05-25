#if defined(__linux__)
#include "/home/codeleaded/System/Static/Library/VM16.h"
#include "/home/codeleaded/System/Static/Library/VM16_Devices.h"
#include "/home/codeleaded/System/Static/Library/VM16_SPS.h"
#elif defined(_WINE)
#include "/home/codeleaded/System/Static/Library/VM16.h"
#include "/home/codeleaded/System/Static/Library/VM16_Devices.h"
#include "/home/codeleaded/System/Static/Library/VM16_SPS.h"
#elif defined(_WIN32)
#include "F:/home/codeleaded/System/Static/Library/VM16.h"
#include "F:/home/codeleaded/System/Static/Library/VM16_Devices.h"
#include "F:/home/codeleaded/System/Static/Library/VM16_SPS.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif

#define PATH_ASM    "./asm/Main.svm16"
#define PATH_LINK   "./asm/Main.lvm16"
#define PATH_EXE    "./asm/Main.vm16"

int main(int argc,char** argv){

    VM16 vm = VM16_New();
    
    VM16_ConnectDevice(&vm,(VM16_Ram[]){   VM16_Ram_New(VM16_MEMORY_LOW,     VM16_MEMORY_HIGH) });
    VM16_ConnectDevice(&vm,(VM16_Kernel[]){VM16_Kernel_New(VM16_KERNEL_LOW,  VM16_KERNEL_HIGH) });
    VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_New(VM16_RAYSS_LOW,    VM16_RAYSS_HIGH)  });
    VM16_ConnectDevice(&vm,(VM16_Motor[]){ VM16_Motor_New(VM16_MOTOR_LOW,    VM16_MOTOR_HIGH)  });
    
    VM16_Comp(&vm,PATH_ASM,PATH_LINK);
    VM16_Link(&vm,VM16_MEMORY_LOW,"_start",(CStr[]){ PATH_LINK,NULL },PATH_EXE);
    VM16_Load(&vm,VM16_MEMORY_LOW,PATH_EXE);
    VM16_Run(&vm,VM16_MEMORY_LOW);

    printf("Exitcode: %d\n",vm.exitcode);

    VM16_Free(&vm);
    
    return 0;
}