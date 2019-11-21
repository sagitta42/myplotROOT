# myplotROOT

Class: myplot.C

Plotting example: plot_test.C

Compile via 

```console
g++ -Wall -O2 -o ${1%.*} $1 `root-config --cflags --glibs`;
```

Tip: add shortcut to ```.bashrc```

```bash
compileit(){
    g++ -Wall -O2 -o ${1%.*} $1 `root-config --cflags --glibs`;
}
```

And use ```compileit plot_test.C``` to create ```./plot_test```

## Example usage

File ```plot_test.C```:

```c++
int main(){
    Myplot* p = new Myplot("test.root", false, true); 

    p->DrawHistos();
    
    p->SetLabels("nhits", "");
    p->SaveCanvas("test_plot.root", true);
}
```

Go to the bottom for example output of running ```./plot_test```

## Histograms in one ROOT files

Say you collect several histograms that you want to plot in one root file called ```test.root```

```
TFile**         test.root
 TFile*         test.root
  KEY: TH1D     c14_nhits;1     final_nhits_pp
  KEY: TH1D     be7_nhits;1     final_nhits_pp
  KEY: TH1D     po210_nhits;1   final_nhits_pp
```

You simply give this file to Myplot:

```c++
Myplot* p = new Myplot("test.root");
```

Optional arguments are

- **scale** (true|false) scale the histograms to the same area (default false)

- **yscale** (true|false) set Y axis to log scale (default true)

E.g.

```c++
Myplot* p = new Myplot("test.root", false, true);
```

## Histograms in separate files

If you want to read the histograms from different files, you can initiate an empty plot


```c++
Myplot* p = new Myplot();
```

And then add the histograms from the files, for example

```c++
for( < loop over files ){
    TFile f(<name>);
    TH1D* h = (TH1D*) f->Get(<histo name>);
    p->AddHisto(h);
}
```

## Draw

To draw the histograms, do

```c++
p->DrawHistos()
```

While reading the histos, Myplot has figured out the minimum and maximum values for x and y axes, and will create a Frame based on those values so that all the histograms fit in it. In case you want your own X and Y limits, set them **before** calling ```DrawHistos()``` with ```p->SetXlim(x1,x2)``` and/or ```p->SetYlim(y1,y2)```.

When drawing, this function will automatically create a legend with different colours and adjust the width of the legend based on the length of the label. The label of each histogram is its title.

You can set X and Y labels and a title via:

```c++
p->SetLabels("x label", "y label");
p->SetTitle("title");
```

## Save plot

To save the plot, use the function ```p->SaveCanvas()```.

If you drew from one file, and don't pass any arguments to this function, it will automatically save the canvas with the plot to a file called ```test.root.out``` (i.e. your original input file plus ".out").

If you added histos manually from many files, or want a different name, you can pass it to the function. Also, if you want to save a PDF of the plot, you can set the corresponding bool to true (which is false by default):

```c++
p->SaveCanvas("name.root", true);
```

## Drawing lines

If you want to draw lines on the plot, you can do it via:

```c++
TLine* line = new TLine(...);
p->AddLine(line);
p->DrawLines();
```


## Example

1. Compile ```plot_test.C``` as described above

2. Run ```./plot_test```

```console
$ ./plot_test 
~~ Reading histograms from file test.root
c14_nhits
be7_nhits
po210_nhits
~~ Drawing
XYrange
10.5 370.5 0.1 0.0264498 
x1 = 0.68 y1 = 0.71 x2 = 0.9 y2 = 0.89
Frame
Frame done
c14_nhits
be7_nhits
po210_nhits
~~ Saving canvas to file test_plot.root
Info in <TCanvas::Print>: pdf file test_plot.root.pdf has been created
```
