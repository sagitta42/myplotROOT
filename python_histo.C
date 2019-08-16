#include <string>
#include "TFile.h"
#include "TList.h"
#include "TKey.h"
#include <iostream>
#include "TH1F.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
    /* Save C histograms as txt files */

    // given file
    string filename = argv[1];
    TFile f(filename.c_str());

    // list of histos in given file
	TList* list = f.GetListOfKeys();
	TIter next(list);
	TKey* key;
	
    // loop over the histograms
    cout << "~~~ Reading histograms from file " << filename << endl;

	while( (key = (TKey*)next()) ){
        // get histo
		TH1D* h = (TH1D*) key->ReadObj();

        // file to which to save info
        char outname[30];
        sprintf(outname, "histo_%s.txt", h->GetName());
        cout << outname << endl;
        ofstream out(outname);

        // loop over each bin and save info
        int nbins = h->GetNbinsX();
        for(int b = 0; b < nbins; b++) out << h->GetBinCenter(b) << " " << h->GetBinContent(b) << endl;

        out.close();

    }

    f.Close();

}
