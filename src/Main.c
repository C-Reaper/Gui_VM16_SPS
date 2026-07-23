#if defined __linux__
#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/VM16.h"
#include "/home/codeleaded/System/Static/Library/VM16_Devices.h"
#include "/home/codeleaded/System/Static/Library/VM16_SPS.h"
#elif defined _WINE
#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/VM16.h"
#include "/home/codeleaded/System/Static/Library/VM16_Devices.h"
#include "/home/codeleaded/System/Static/Library/VM16_SPS.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "F:/home/codeleaded/System/Static/Library/TransformedView.h"
#include "F:/home/codeleaded/System/Static/Library/Random.h"
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

VM16 vm;
Thread vm_th;
Vector objs;
TransformedView tv;

void ConBelt_Render(SPS_ConBelt* cb){
    const Rect sr = TransformedView_WorldScreenRect(&tv,cb->box);
    Rect_Render(WINDOW_STD_ARGS,sr,GRAY);

    const Vec2 dir = Vec2_Dir(cb->d);
    const float dw = 0.2f;
    const unsigned int count = (cb->d % 2U == 0U ? cb->box.d.y : cb->box.d.x) / dw;

    const float rd = 0.5f;
    const float d = cb->d == VM16_CONBELT_DIR_DOWN || cb->d == VM16_CONBELT_DIR_RIGHT ? 1.0f : -1.0f;
    const float offn = cb->time * (cb->v * 0.1f * d) / dw;
    const float off = (offn - (int)offn) * dw;

    for(int i = 0;i<count;i++){
        const float ro = (off + (float)i * dw);

        const Rect rect = Rect_New(
            cb->d % 2U == 0U ? (Vec2){ cb->box.p.x,cb->box.p.y + ro } : (Vec2){ cb->box.p.x + ro,cb->box.p.y },
            cb->d % 2U == 0U ? (Vec2){ cb->box.d.x,dw * rd, } : (Vec2){ dw * rd,cb->box.d.y }
        );

        const Rect sr = TransformedView_WorldScreenRect(&tv,rect);
        Rect_Render(WINDOW_STD_ARGS,sr,DARK_GRAY);
    }
}

void RaySS_Hit(SPS_RaySS* rss,Vector* objs){
    const Vec2 dir = Vec2_OfAngle(rss->a);
    const float m = dir.y / dir.x;
    const float t = rss->p.y - m * rss->p.x;
    rss->t = 1.0f;

    for(int i = 0;i<objs->size;i++){
        Rect* r = (Rect*)Vector_Get(objs,i);
        Vector its = Intersections_Rect_Ray(*r,Ray_New(m,t));
        
        if(its.size > 0){
            const Vec2 pos = *(Vec2*)Vector_Get(&its,0);
            const Vec2 dist = Vec2_Sub(pos,rss->p);
            const float fdist = Vec2_Mag(dist) / rss->len;
            
            if(fdist <= 1.0f){
                rss->t = fdist;
                return;
            }
        }

        Vector_Free(&its);
    }
}
void ConBelt_Transport(SPS_ConBelt* cb,Vector* objs,float t){
    for(int i = 0;i<objs->size;i++){
        Rect* r = (Rect*)Vector_Get(objs,i);
        
        if(Rect_Overlap(*r,cb->box) && cb->v != 0U){
            const Vec2 dir = Vec2_Dir(cb->d);
            const Vec2 sdir = Vec2_Mulf(dir,(float)cb->v * 0.1f);
            r->p = Vec2_Add(r->p,Vec2_Mulf(sdir,t));
        }
    }
}

void* VM16_Runner(void* arg){
	VM16_Run(&vm,VM16_MEMORY_LOW);
    printf("Exitcode: %d\n",vm.exitcode);
    return NULL;
}

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.1f,0.1f },
		(float)GetWidth() / (float)GetHeight()
	);
    objs = Vector_New(sizeof(Rect));

    vm = VM16_New();
    VM16_ConnectDevice(&vm,(VM16_Ram[]){ VM16_Ram_New(VM16_MEMORY_LOW,VM16_MEMORY_HIGH) });
    VM16_ConnectDevice(&vm,(VM16_Kernel[]){ VM16_Kernel_New(VM16_KERNEL_LOW,VM16_KERNEL_HIGH) });
    
    //VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_Make(VM16_SPS_LOW,VM16_SPS_LOW + 15,(SPS_RaySS){
    //    .p = { -0.5f,-0.125f },
    //    .a = 0.0f,
    //    .len = 3.0f,
    //    .t = 1.0f
    //})});
    //VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_Make(VM16_SPS_LOW + 16,VM16_SPS_LOW + 31,(SPS_RaySS){
    //    .p = { -0.5f,5.125f },
    //    .a = 0.0f,
    //    .len = 3.0f,
    //    .t = 1.0f
    //})});
    //VM16_ConnectDevice(&vm,(VM16_ConBelt[]){ VM16_ConBelt_Make(VM16_SPS_LOW + 32,VM16_SPS_LOW + 47,(SPS_ConBelt){
    //    .box = { 0.0f,0.0f,1.0f,5.0f },
    //    .time = 0.0f,
    //    .v = 0U,
    //    .d = VM16_CONBELT_DIR_DOWN
    //})});
    
    VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_Make(VM16_SPS_LOW + 0x00,VM16_SPS_LOW + 0x0F,(SPS_RaySS){
       .p = { 0.5f,-0.5f },
       .a = F32_PI05,
       .len = 2.0f,
       .t = 1.0f
    })});
    VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_Make(VM16_SPS_LOW + 0x10,VM16_SPS_LOW + 0x1F,(SPS_RaySS){
       .p = { -0.5f,5.5f },
       .a = 0.0f,
       .len = 3.0f,
       .t = 1.0f
    })});
    VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_Make(VM16_SPS_LOW + 0x20,VM16_SPS_LOW + 0x2F,(SPS_RaySS){
       .p = { 5.5f,6.5f },
       .a = F32_PI05 * 3,
       .len = 3.0f,
       .t = 1.0f
    })});
    VM16_ConnectDevice(&vm,(VM16_RaySS[]){ VM16_RaySS_Make(VM16_SPS_LOW + 0x30,VM16_SPS_LOW + 0x3F,(SPS_RaySS){
       .p = { 6.5f,0.5f },
       .a = F32_PI,
       .len = 3.0f,
       .t = 1.0f
    })});

    VM16_ConnectDevice(&vm,(VM16_ConBelt[]){ VM16_ConBelt_Make(VM16_SPS_LOW + 0x40,VM16_SPS_LOW + 0x4F,(SPS_ConBelt){
       .box = { 0.0f,0.0f,1.0f,5.0f },
       .time = 0.0f,
       .v = 0U,
       .d = VM16_CONBELT_DIR_DOWN
    })});
    VM16_ConnectDevice(&vm,(VM16_ConBelt[]){ VM16_ConBelt_Make(VM16_SPS_LOW + 0x50,VM16_SPS_LOW + 0x5F,(SPS_ConBelt){
       .box = { 0.0f,5.0f,5.0f,1.0f },
       .time = 0.0f,
       .v = 0U,
       .d = VM16_CONBELT_DIR_RIGHT
    })});
    VM16_ConnectDevice(&vm,(VM16_ConBelt[]){ VM16_ConBelt_Make(VM16_SPS_LOW + 0x60,VM16_SPS_LOW + 0x6F,(SPS_ConBelt){
       .box = { 5.0f,1.0f,1.0f,5.0f },
       .time = 0.0f,
       .v = 0U,
       .d = VM16_CONBELT_DIR_UP
    })});
    VM16_ConnectDevice(&vm,(VM16_ConBelt[]){ VM16_ConBelt_Make(VM16_SPS_LOW + 0x70,VM16_SPS_LOW + 0x7F,(SPS_ConBelt){
       .box = { 1.0f,0.0f,5.0f,1.0f },
       .time = 0.0f,
       .v = 0U,
       .d = VM16_CONBELT_DIR_LEFT
    })});

    VM16_Comp(&vm,PATH_ASM,PATH_LINK);
    VM16_Link(&vm,VM16_MEMORY_LOW,"_start",(CStr[]){ PATH_LINK,NULL },PATH_EXE);
    VM16_Load(&vm,VM16_MEMORY_LOW,PATH_EXE);

    vm_th = Thread_New(NULL,VM16_Runner,NULL);
    Thread_Start(&vm_th);
}
void Update(AlxWindow* w){
	TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
	tv.AspectRatio = (float)GetWidth() / (float)GetHeight();
	
	TransformedView_HandlePanZoom(&tv,w->Strokes,GetMouse());
	const Vec2 m = TransformedView_ScreenWorldPos(&tv,GetMouse());

    if(Stroke(ALX_MOUSE_L).PRESSED){
        const Vec2 len = { 0.5f,0.5f };
        Vector_Push(&objs,(Rect[]){ Rect_New(Vec2_Sub(m,Vec2_Mulf(len,0.5f)),len) });
    }else if(Stroke(ALX_MOUSE_R).PRESSED){
        for(int i = 0;i<objs.size;i++){
            const Rect r = *(Rect*)Vector_Get(&objs,i);
            
            if(Rect_Point_Overlap(r,m)){
                Vector_Remove(&objs,i);
            }
        }
    }

	SPS_RaySS* const rss0  = (SPS_RaySS*)((VM16_RaySS*)Vector_Get(&vm.devices,2U))->data;
	SPS_RaySS* const rss1  = (SPS_RaySS*)((VM16_RaySS*)Vector_Get(&vm.devices,3U))->data;
	SPS_RaySS* const rss2  = (SPS_RaySS*)((VM16_RaySS*)Vector_Get(&vm.devices,4U))->data;
	SPS_RaySS* const rss3  = (SPS_RaySS*)((VM16_RaySS*)Vector_Get(&vm.devices,5U))->data;

	SPS_ConBelt* const cb0 = (SPS_ConBelt*)((VM16_ConBelt*)Vector_Get(&vm.devices,6U))->data;
	SPS_ConBelt* const cb1 = (SPS_ConBelt*)((VM16_ConBelt*)Vector_Get(&vm.devices,7U))->data;
	SPS_ConBelt* const cb2 = (SPS_ConBelt*)((VM16_ConBelt*)Vector_Get(&vm.devices,8U))->data;
	SPS_ConBelt* const cb3 = (SPS_ConBelt*)((VM16_ConBelt*)Vector_Get(&vm.devices,9U))->data;
    cb0->time += w->ElapsedTime;
    cb1->time += w->ElapsedTime;
    cb2->time += w->ElapsedTime;
    cb3->time += w->ElapsedTime;

    ConBelt_Transport(cb0,&objs,w->ElapsedTime);
    ConBelt_Transport(cb1,&objs,w->ElapsedTime);
    ConBelt_Transport(cb2,&objs,w->ElapsedTime);
    ConBelt_Transport(cb3,&objs,w->ElapsedTime);

    RaySS_Hit(rss0,&objs);
    RaySS_Hit(rss1,&objs);
    RaySS_Hit(rss2,&objs);
    RaySS_Hit(rss3,&objs);
    
	Clear(BLACK);

    ConBelt_Render(cb0);
    ConBelt_Render(cb1);
    ConBelt_Render(cb2);
    ConBelt_Render(cb3);

    Ray_DirRender(
        WINDOW_STD_ARGS,
        TransformedView_WorldScreenPos(&tv,rss0->p),
        TransformedView_WorldScreenLX(&tv,rss0->len * rss0->t),
        rss0->a,
        RED,
        1.0f
    );
    Ray_DirRender(
        WINDOW_STD_ARGS,
        TransformedView_WorldScreenPos(&tv,rss1->p),
        TransformedView_WorldScreenLX(&tv,rss1->len * rss1->t),
        rss1->a,
        RED,
        1.0f
    );
    Ray_DirRender(
        WINDOW_STD_ARGS,
        TransformedView_WorldScreenPos(&tv,rss2->p),
        TransformedView_WorldScreenLX(&tv,rss2->len * rss2->t),
        rss2->a,
        RED,
        1.0f
    );
    Ray_DirRender(
        WINDOW_STD_ARGS,
        TransformedView_WorldScreenPos(&tv,rss3->p),
        TransformedView_WorldScreenLX(&tv,rss3->len * rss3->t),
        rss3->a,
        RED,
        1.0f
    );

    for(int i = 0;i<objs.size;i++){
        const Rect r = *(Rect*)Vector_Get(&objs,i);
        const Rect sr = TransformedView_WorldScreenRect(&tv,r);
        //Rect_RenderWire(WINDOW_STD_ARGS,sr,WHITE,1.0f);
        Rect_Render(WINDOW_STD_ARGS,sr,WHITE);
    }
}
void Delete(AlxWindow* w){
    Thread_Stop(&vm_th);
    VM16_Free(&vm);
    Vector_Free(&objs);
}

int main(){
    if(Create("SPS Automation (VM16)",1600,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}