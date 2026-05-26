u16 rayss_get_running(u16 id){
    u16* src = 31232 + id * 16;
    u16 ret = *src;
    return ret;
}
u16 rayss_get_state(u16 id){
    u16* src = 31233 + id * 16;
    u16 ret = *src;
    return ret;
}
u16 rayss_get_dist(u16 id){
    u16* src = 31234 + id * 16;
    u16 ret = *src;
    return ret;
}
void rayss_set_running(u16 id,u16 running){
    u16* src = 31232 + id * 16;
    *src = running;
}
void rayss_set_state(u16 id,u16 state){
    u16* src = 31233 + id * 16;
    *src = state;
}
void rayss_set_dist(u16 id,u16 dist){
    u16* src = 31234 + id * 16;
    *src = dist;
}