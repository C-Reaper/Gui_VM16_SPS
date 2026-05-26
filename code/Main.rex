import Kernel;
import RaySS;
import ConBelt;

u16 main(){
    while(true){
        if rayss_get_state(0) != 0 {
            conbelt_set_speed(7,0);
            conbelt_set_speed(4,10);
        }
        if rayss_get_state(1) != 0 {
            conbelt_set_speed(4,0);
            conbelt_set_speed(5,10);
        }
        if rayss_get_state(2) != 0 {
            conbelt_set_speed(5,0);
            conbelt_set_speed(6,10);
        }
        if rayss_get_state(3) != 0 {
            conbelt_set_speed(6,0);
            conbelt_set_speed(7,10);
        }

        kernel_sleep(10);
    }

    return 0;
}