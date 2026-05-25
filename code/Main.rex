import Kernel;
import RaySS;
import ConBelt;

u16 main(){
    while(true){
        if rayss0_get_state() != 0 {
            conbelt_set_speed(10);
            conbelt_set_dir(0);
        }
        if rayss1_get_state() != 0 {
            conbelt_set_speed(10);
            conbelt_set_dir(2);
        }

        kernel_sleep(100);
    }

    return 0;
}