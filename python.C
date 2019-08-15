int main(){
    /* Save C histograms as txt files */

    // given file
    string filename = argv[1];

    // list of histos in given file
	TList* list = f->GetListOfKeys();
	TIter next(list);
	TKey* key;
	
    // loop over the histograms
    cout << "~~~ Reading histograms from file " << filename << endl;

	while( (key = (TKey*)next()) ){
        // get histo
		TH1F* h = (TH1D*) key->ReadObj();

        // file to which to save info
        char outname[30];
        sprintf(outname, "histo_%s.txt", h->GetName());
        ofstream out(outname);

        // loop over each bin and save info
        int nbins = h->GetNBins();

        for(int b = 0; b < nbins; b++) out << b->GetBinCenter(b) << " " << b->GetBinContent(b) << endl;

    }

}
