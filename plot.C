#include "myplot.C"

int main(int argc, char* argv[]){
	// input to the macro: name of the file with histograms to plot
	string fname = argv[1];
    
	// 1: filename (string)
    // 2: scale histos to have the same integral (bool)
    // 3: y axis log scale (bool)
	Myplot* p = new Myplot(fname, false, true);
	
    // optional: set axes limits tp what you want
    // has to be set before drawing
    p->SetXlim(0, 100);
    p->SetYlim(0, 2);
    
    // draw
	p->DrawHistos();
	
    // optional: set labels
	p->SetTitle("Title");
	p->SetLabels("X text", "Y text");
	
    // save canvas to given file
    // if not given, saves to <input file>.root.out
	p->SaveCanvas("mystuff.root");
}
