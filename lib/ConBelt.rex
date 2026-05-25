u16 conbelt_get_running(u16 id){
    u16 ret;
    asm "st		" ret "  $30768";
    return ret;
}
u16 conbelt_get_speed(){
    u16 ret;
    asm "st		" ret "  $30769";
    return ret;
}
u16 conbelt_get_dir(){
    u16 ret;
    asm "st		" ret "  $30770";
    return ret;
}

void conbelt_set_running(u16 running){
    asm "ld		$30768  " running;
}
void conbelt_set_speed(u16 speed){
    asm "ld		$30769  " speed;
}
void conbelt_set_dir(u16 dir){
    asm "ld		$30770  " dir;
}