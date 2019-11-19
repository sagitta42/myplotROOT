#include <string>
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TList.h"
#include "TKey.h"
#include <iostream>
#include "TLegend.h"
#include "TCollection.h"
#include "TLine.h"

using namespace std;

class Myplot{
	public:
		Myplot(string filename, bool scale=false, bool yscale=true); // read and store histos from given filename
		Myplot(bool scale=false, bool yscale=true);
		void DrawHistos();
        void DrawLines();
		void SaveCanvas(string outn="", bool pdf=false);

		void SetLabels(string xlabel, string ylabel); // custom given labels for x and y axis
		void SetTitle(string title); // custom given title
		void SetXlim(float x1, float x2);

		double* HistoRange(int hindex);  // return min and max for x and y of given histo
		double* CommonHistoRange(); // find common min and max for x and y for all histos to make a nice frame

		TH1D* GetHisto(int i); // return histo
		void AddHisto(TH1D* h); // add histo to our list
        void AddHisto(string fname, string hname, string newname="");
        void AddLine(TLine* line);

	private:
		string fname; // name of the input file
		TFile* f;
		TH1D* histos[20]; // store histos from the file
        TLine* lines[5]; // lines to draw
		int nhistos; // number of histos 
        int nlines; // number of lines
		int legwidth; // legend width: depends on the lengh of the labels (which are histo titles)
		TCanvas* canvas;
		TH1* frame;
		bool Scale;
		float xmin, xmax;
        bool Yscale;

};

		
Myplot::Myplot(bool scale, bool yscale){
	nhistos = 0;
    nlines = 0;
	xmin = -111;
	xmax = -111;
	Scale = scale;
    Yscale = yscale;
	legwidth = 0; // width of the legend
}; // no file provided, will add histos later


Myplot::Myplot(string filename, bool scale, bool yscale){;
//Myplot::Myplot(string filename){;

	// file with the histos to draw
	fname = filename;
	f = TFile::Open(filename.c_str());
	Scale = scale;
    Yscale = yscale;

	// extract histo names
	TList* list = f->GetListOfKeys();
	TIter next(list);
	TKey* key;
	int i = 0;
	legwidth = 0; // width of the legend
	xmin = -111;
	xmax = -111;

	// store histos
	cout << "~~~ Reading histograms from file " << filename << endl;
	while( (key = (TKey*)next()) ){
		histos[i] = (TH1D*) key->ReadObj();
		histos[i]->SetDirectory(0);
		string hname = histos[i]->GetName();
		int hlength = (int) hname.length(); // length of the histo name = label of the histo in the legend
		cout << hname << endl;
		if(hlength > legwidth) legwidth = hlength; // adjust the width according to the longest label
		i++;
	}

	nhistos = i;


}


void Myplot::DrawHistos(){
	// scale histos to have the same number of entries as the first one (if scale is true)
	if(Scale){
		double nentries = histos[0]->Integral();
		for(int i = 1; i < nhistos; i++){
			double hentries = histos[i]->Integral();
			histos[i]->Scale(nentries*1./hentries);
		}
	}
	
	cout << "~~~ Drawing" << endl;

	int ww = 1000; int hh = 700;
//	int ww = 800; int hh = 600;

	TLegend* leg;
	double* xyrange = CommonHistoRange();
	canvas = new TCanvas("c","c", ww, hh);
	// legend width and height depend on the number of histos and the length of their labels
	float xedge = 0.9;
	float yedge = 0.89;
    legwidth = legwidth*0.8; // too long
    cout << "x1 = " << xedge - legwidth*0.02 << " y1 = " << yedge - 0.06*nhistos << " x2 = " << xedge << " y2 = " << yedge << endl;
	leg = new TLegend(xedge - legwidth*0.015, yedge - 0.06*nhistos, xedge, yedge);
	leg->SetBorderSize(0);
	// frame according to the common x and y ranges of all the histos
//	cout << xmin << " " << xmax << endl;
	if(xmin == -111) xmin = xyrange[0]*0.9;
	if(xmax == -111) xmax = xyrange[1]*1.1;
//	cout << xmin << " " << xmax << endl;
	cout << "Frame" << endl;
	frame = canvas->DrawFrame(xmin, xyrange[2], xmax, xyrange[3]*1.2);
	cout << "Frame done" << endl;
//	frame = canvas->DrawFrame(xyrange[0]*0.9, xyrange[2], xyrange[1]*1.1, xyrange[3]*1.2);
	frame->SetTitle("");
	frame->GetXaxis()->SetLabelSize(0.045);
	frame->GetYaxis()->SetLabelSize(0.045);

//	int linestyle = 0; // for changing linestyle

    // colours = choice from the given list
    int colours[] = {kBlack, kBlue, kRed, kGreen+2, kCyan, kViolet+2, kMagenta, kOrange+7};
    int num_cols = 8; // is there a reasonable way to get array length in C++ without vectors?..
    int col_idx = 0;

	for(int i = 0; i < nhistos; i++){
		// draw
		histos[i]->SetLineWidth(3);
		histos[i]->SetStats(0);

        histos[i]->SetLineColor(colours[col_idx % num_cols]); // colours from the list
//		histos[i]->SetLineColor(col_idx + 1); // colours = 1, 2, 3 ...

        col_idx++;
        // i wanna skip 8, cause it's a green too similar to 3
//        if(col_idx == 7) col_idx++;

        histos[i]->SetLineStyle(1); // all solid
//		histos[i]->SetLineStyle(6 * (linestyle % 2) + 1); // becomes 1 (solid) or 7 (dashed)
//		histos[i]->SetLineStyle(linestyle % 2 + 1); // becomes 1 (solid) or 2 (dashed)
//		linestyle++;
		
        histos[i]->Draw("E same"); // error bars
//        histos[i]->Draw("HIST same"); // no error bars, just a "step" histogram
//        histos[i]->Draw("same"); // if there are error bars, they will be visualized by default -> horizontal bars?
		cout << histos[i]->GetName() << endl;

		leg->AddEntry(histos[i], histos[i]->GetTitle());
//        cout << "legend: " << histos[i]->GetTitle() << endl;
	}

	leg->Draw("same");
	if(Yscale) canvas->SetLogy();

}

void Myplot::DrawLines(){
	cout << "~~~ Drawing lines" << endl;
    for(int i = 0; i < nlines; i++) lines[i]->Draw();
}

void Myplot::SaveCanvas(string outn, bool pdf){
	// output file
	string outname = outn.size() != 0 ? outn : fname + ".out";
	cout << "~~~ Saving canvas to file " << outname << endl;
	TFile out(outname.c_str(), "recreate");
	canvas->Write();
	out.Close();

    // save as pdf
    if(pdf){
        outname = outname + ".pdf";
        canvas->SaveAs(outname.c_str());
    }

    delete canvas;
}


double* Myplot::CommonHistoRange(){
	/* Find common x and y axis range for both histos */

	static double xyrange[4];
	
	// initialize xmin, xmax, ymin and ymax as those of the first histo
	double* xyhrange = HistoRange(0);
	for(int i = 0; i < 4; i++) xyrange[i] = xyhrange[i];

	// loop over histos to update the max and min
	for(int i = 1; i < nhistos; i++){
		xyhrange = HistoRange(i);
		if(xyhrange[0] < xyrange[0]) xyrange[0] = xyhrange[0];
		if(xyhrange[1] > xyrange[1]) xyrange[1] = xyhrange[1];
		if(xyhrange[2] < xyrange[2]) xyrange[2] = xyhrange[2];
		if(xyhrange[3] > xyrange[3]) xyrange[3] = xyhrange[3];
	}

    cout << "XYrange" << endl;
	for(int i = 0; i < 4; i++) cout << xyrange[i] << " ";
	cout << endl;
	
	return xyrange;
}

	
double* Myplot::HistoRange(int hindex){
	/* Find xmin, xmax, ymin and ymax of the histo
	 * hindex: index of the current histo in the vector of histos
	 */

	static double xyhrange[4];

	// find first and last non-zero bin x value
	int bmin = histos[hindex]->FindFirstBinAbove(0);
	int bmax = histos[hindex]->FindLastBinAbove(0);
	xyhrange[0] = histos[hindex]->GetBinCenter(bmin);
	xyhrange[1] = histos[hindex]->GetBinCenter(bmax);

	// ymax
	xyhrange[3] = histos[hindex]->GetMaximum();
	
	// find y min
	xyhrange[2] = xyhrange[3]; // initialize to the highest possible value
	for(int i = bmin; i < bmax; i++){
		double hy = histos[hindex]->GetBinContent(i);
		if(hy < xyhrange[2]) xyhrange[2] = hy;
	}

	// if we want log scale
	if(Yscale) if(xyhrange[2] == 0) xyhrange[2] = 1e-1;
//	if(Yscale) if(xyhrange[2] == 0) xyhrange[2] = 1e-10;

//	for(int i = 0; i < 4; i++) cout << xyhrange[i] << " ";
//	cout << endl;

	return xyhrange;
}


void Myplot::SetLabels(string xlabel, string ylabel){
	/* Set x and y labels that were provided as input */
	frame->SetXTitle(xlabel.c_str());
	frame->SetYTitle(ylabel.c_str());
	frame->SetTitleSize(0.045, "X");
	frame->SetTitleSize(0.045, "Y");
	frame->SetTitleOffset(1.1, "X");
	frame->SetTitleOffset(1.15, "Y");
}


void Myplot::SetTitle(string title){
	frame->SetTitle(title.c_str());
	frame->SetTitleSize(0.045);
}


TH1D* Myplot::GetHisto(int i){ return histos[i]; }


void Myplot::AddHisto(TH1D* h){
	histos[nhistos] = (TH1D*) h->Clone();
	histos[nhistos]->SetDirectory(0);
	string hname = h->GetName();
	int hlength = (int) hname.length(); // length of the histo name = label of the histo in the legend
	if(hlength > legwidth) legwidth = hlength; // adjust the width according to the longest label
	nhistos++;
	cout << "added histo: " << hname << endl;
//    cout << "Legend width: " << legwidth << endl;
}

void Myplot::AddHisto(string flname, string hname, string newname){
    TFile* fl = TFile::Open(flname.c_str());
    TH1D* hi = (TH1D*) fl->Get(hname.c_str());
    if(newname.compare("") != 0) hi->SetTitle(newname.c_str());
    AddHisto(hi);
}


void Myplot::SetXlim(float x1, float x2){
	xmin = x1;
	xmax = x2;
}
        
void Myplot::AddLine(TLine* line){
    lines[nlines] = line;
    nlines++;
}
