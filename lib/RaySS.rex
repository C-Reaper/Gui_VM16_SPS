u16 rayss0_get_running(){
    u16 ret;
    asm "st		" ret "  $30736";
    return ret;
}
u16 rayss0_get_state(){
    u16 ret;
    asm "st		" ret "  $30737";
    return ret;
}
u16 rayss0_get_dist(){
    u16 ret;
    asm "st		" ret "  $30738";
    return ret;
}
void rayss0_set_running(u16 running){
    asm "ld		$30736  " running;
}
void rayss0_set_state(u16 state){
    asm "ld		$30737  " state;
}
void rayss0_set_dist(u16 dist){
    asm "ld		$30738  " dist;
}



u16 rayss1_get_running(){
    u16 ret;
    asm "st		" ret "  $30752";
    return ret;
}
u16 rayss1_get_state(){
    u16 ret;
    asm "st		" ret "  $30753";
    return ret;
}
u16 rayss1_get_dist(){
    u16 ret;
    asm "st		" ret "  $30754";
    return ret;
}
void rayss1_set_running(u16 running){
    asm "ld		$30752  " running;
}
void rayss1_set_state(u16 state){
    asm "ld		$30753  " state;
}
void rayss1_set_dist(u16 dist){
    asm "ld		$30754  " dist;
}