#include "myplot.C"

int main(int argc, char* argv[]){
    string fname = argv[1];

    // create plot, read histos from test.root
    // do not scale them to have the same integral
    // set y axis to log scale
    Myplot* p = new Myplot(fname, false, true); 

    p->DrawHistos();
    
    p->SetLabels("nhits", "");

    string outname = fname.substr(0, fname.size() - 5) + "_plot.root";
    p->SaveCanvas(outname, true);
}


