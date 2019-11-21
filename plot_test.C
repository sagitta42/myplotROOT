#include "myplot.C"

int main(){

    // create plot, read histos from test.root
    // do not scale them to have the same integral
    // set y axis to log scale
    Myplot* p = new Myplot("test.root", false, true); 

    p->DrawHistos();
    
    p->SetLabels("nhits", "");
    p->SaveCanvas("test_plot.root", true);
}


