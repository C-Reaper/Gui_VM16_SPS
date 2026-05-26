u16 conbelt_get_running(u16 id){
    u16* src = 31232 + id * 16;
    return *src;
}
u16 conbelt_get_speed(u16 id){
    u16* src = 31233 + id * 16;
    return *src;
}
u16 conbelt_get_dir(u16 id){
    u16* src = 31234 + id * 16;
    return *src;
}
void conbelt_set_running(u16 id,u16 running){
    u16* src = 31232 + id * 16;
    *src = running;
}
void conbelt_set_speed(u16 id,u16 speed){
    u16* src = 31233 + id * 16;
    *src = speed;
}
void conbelt_set_dir(u16 id,u16 dir){
    u16* src = 31234 + id * 16;
    *src = dir;
}