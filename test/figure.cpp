
    #include "figure.h"
    void Figure:: draw(){
        cout<<"Figure is drawing.\n";
    }
    void Figure:: erase(){
        cout<<"Figure is erasing.\n";
    }
    void Figure::center(){
        cout<<"Figure is centering.\n";
        erase();
        draw();
    }
