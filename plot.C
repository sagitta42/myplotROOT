#include "myplot.C"

int main(int argc, char* argv[]){
	// input to the macro: name of the file with histograms to plot
	string fname = argv[1];
	// input: filename, scale histos by num entries or not
	Myplot* p = new Myplot(fname.c_str(), true);
	p->DrawHistos();
	// optional: set x to be what you want; otherwise finds automatic x lim based on min and max of all histos (y is set automatically)
//    p->SetXlim(0, 100);
	// optional: set title to what you want
	p->SetTitle("Title");
	// optional: set x and y labels
	p->SetLabels("X text", "Y text");
	// saves the canvas to filename.out if no output filename is provided; otherwise to the provided file (creates or overwrites existing one)
	p->SaveCanvas();
}
