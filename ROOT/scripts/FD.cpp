#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"

#include "TCanvas.h"
#include "TH1.h"

#include "Math/Vector4D.h"
#include <vector>

using namespace std;

void setupCanvas()
{
	gStyle->SetCanvasColor(0);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetStatBorderSize(1);
	gStyle->SetFrameFillColor(0);
	gStyle->SetTitleFillColor(0);
	gStyle->SetPalette(1);
	gROOT->ForceStyle();
}

vector<Int_t> generateIDs()
{
	vector<Int_t> IDs;
	IDs.push_back(551);
	IDs.push_back(100551);
	IDs.push_back(200551);
	IDs.push_back(10551);
	IDs.push_back(110551);
	IDs.push_back(210551);
	IDs.push_back(553);
	IDs.push_back(100553);
	IDs.push_back(200553);
	IDs.push_back(300553);
	IDs.push_back(9000553);
	IDs.push_back(9010553);
	IDs.push_back(10553);
	IDs.push_back(110553);
	IDs.push_back(210553);
	IDs.push_back(20553);
	IDs.push_back(120553);
	IDs.push_back(220553);
	IDs.push_back(30553);
	IDs.push_back(130553);
	IDs.push_back(555);
	IDs.push_back(100555);
	IDs.push_back(200555);
	IDs.push_back(10555);
	IDs.push_back(110555);
	IDs.push_back(20555);
	IDs.push_back(120555);
	IDs.push_back(557);
	IDs.push_back(100557);
	return IDs;
}

void FD()
{
	// Setup canvas
	setupCanvas();

	// Open file
	TFile * f1 = TFile::Open("../data/FD.root");

	// Create local TTree
	TTree * t1 = (TTree *) f1->Get("tree");

	// Declare TTree variables
	// Double_t event_weight;
	Int_t nChar;
	Int_t Ups;
	Int_t uniqueBot;
	vector<Int_t> * p_ID = NULL;
	vector<Int_t> * m1_ID = NULL;
	vector<Int_t> * m2_ID = NULL;
	vector<Int_t> * d1_ID = NULL;
	vector<Int_t> * d2_ID = NULL;
	vector<Int_t> * p_Px = NULL;
	vector<Int_t> * p_Py = NULL;
	vector<Int_t> * p_Pz = NULL;
	vector<Int_t> * p_E = NULL;

	// Link TTree Branches to local variables
	t1->SetBranchAddress("nChar", &nChar);
	t1->SetBranchAddress("Ups", &Ups);
	t1->SetBranchAddress("uniqueBot", &uniqueBot);
	t1->SetBranchAddress("p_ID", &p_ID);
	t1->SetBranchAddress("m1_ID", &m1_ID);
	t1->SetBranchAddress("m2_ID", &m2_ID);
	t1->SetBranchAddress("d1_ID", &d1_ID);
	t1->SetBranchAddress("d2_ID", &d2_ID);
	t1->SetBranchAddress("p_Px", &p_Px);
	t1->SetBranchAddress("p_Py", &p_Py);
	t1->SetBranchAddress("p_Pz", &p_Pz);
	t1->SetBranchAddress("p_E", &p_E);

	// Declare ID vector
	IDs = generateIDs();

	// Declare analysis variables
	Int_t totEvt = 0;
	Int_t upsEvt = 0;
	Int_t totUps = 0;
	Int_t directUps = 0;
	Int_t FDUps = 0;
	Int_t otherUps = 0;
	Bool_t FD = 0;
	auto Lorentz = new ROOT::Math::LorentzVector<ROOT::Math::PxPyPzEVector>;

	vector<Int_t> IDcount(IDs.size());
	for (Int_t i = 0; i < IDcount.size(); i++)
		IDcount[i] = 0;

	// Declare TH1Ds
	auto histFDPt = new TH1D("histAllPt", "", 40, 0, 40);	
	auto histFDnChar = new TH1D("histAllnChar", "", 100, 0, 100);	
	auto histDirPt = new TH1D("histDirPt", "", 40, 0, 40);	
	auto histDirnChar = new TH1D("histDirnChar", "", 100, 0, 100);	

	// Run Loop
	for (Int_t i = 0; i < t1->GetEntries(); i++)
	{
		// Load Entry from TTree
		t1->GetEntry(i);

		totEvt++;
		if (Ups != 0)
			upsEvt++;
		totUps += Ups;

		for (Int_t j = 0; j < p_ID->size(); j++)
		{
			if ((*p_ID)[j] == 553 && (*m1_ID)[j] != 553)
			{
				FD = 0;
				Lorentz->SetPx((*p_Px)[j]);
				Lorentz->SetPy((*p_Py)[j]);
				Lorentz->SetPz((*p_Pz)[j]);
				Lorentz->SetE((*p_E)[j]);
				for (Int_t k = 0; k < IDs.size(); k++)
				{

					if ((*m1_ID)[j] == IDs[k])
					{
						FDUps++;
						IDcount[k]++;
						histFDPt->Fill(Lorentz->Pt());
						histFDnChar->Fill(nChar);
						FD = 1;
						break;
					}
				}
				if (FD == 0)
				{
					directUps++;
					histDirPt->Fill(Lorentz->Pt());
					histDirnChar->Fill(nChar);
				}
			}
		}
	}

	// Analysis summary
	cout << "Total events: " << totEvt << "\n";
	cout << "Total Upsilon events: " << upsEvt << "\n";
	cout << "Total Upsilons: " << totUps << "\n";
	cout << "Direct production Ups: " << directUps << "\n";
	cout << "Feed-down Ups: " << FDUps << "\n";
	cout << "Other feed-down Ups: " << otherUps << "\n\n";
	cout << "ID counts: \n";
	for (Int_t i = 0; i < IDs.size(); i++)
		if (IDcount[i] != 0)
			cout << "ID " << IDs[i] << ": " << IDcount[i] << "\n";

	// Declare TCanvas
	auto c1 = new TCanvas("c1", "FD study", 800, 600);
	auto c2 = new TCanvas("c2", "FD study", 800, 600);
	c1->SetLogy();
	c2->SetLogy();

	Double_t norm = totUps*1.0;
	// Normalise all histograms
	histFDPt->Scale(1/norm);
	histFDnChar->Scale(1/norm);
	histDirPt->Scale(1/norm);
	histDirnChar->Scale(1/norm);

	// Format all histograms
	// FD Pt
	histFDPt->SetMarkerStyle(20);
	histFDPt->SetMarkerSize(.75);
	histFDPt->SetMarkerColor(kBlue);
	histFDPt->SetLineColor(kBlue);

	histFDPt->GetYaxis()->SetTitle("Fraction");	
	histFDPt->GetYaxis()->LabelsOption("v");	
	histFDPt->GetYaxis()->SetTitleOffset(1);	
	//histFDPt->GetYaxis()->SetRangeUser(0.0001,0.5);

	histFDPt->GetXaxis()->SetTitle("p_{T}");	
	histFDPt->GetYaxis()->SetTitleOffset(1.25);	

	histFDPt->SetStats(kFALSE);

	// Dir Pt
	histDirPt->SetMarkerStyle(20);
	histDirPt->SetMarkerSize(.75);
	histDirPt->SetMarkerColor(kRed);
	histDirPt->SetLineColor(kRed);

	//histDirPt->GetYaxis()->SetRangeUser(0.0001,0.5);

	histDirPt->SetStats(kFALSE);

	// FD nChar
	histFDnChar->SetMarkerStyle(20);
	histFDnChar->SetMarkerSize(.75);
	histFDnChar->SetMarkerColor(kBlue);
	histFDnChar->SetLineColor(kBlue);

	histFDnChar->GetYaxis()->SetTitle("Fraction");	
	histFDnChar->GetYaxis()->LabelsOption("v");	
	histFDnChar->GetYaxis()->SetTitleOffset(1);	
	//histFDnChar->GetYaxis()->SetRangeUser(0.0001,0.5);

	histFDnChar->GetXaxis()->SetTitle("n_{Ch}");	
	histFDnChar->GetYaxis()->SetTitleOffset(1.25);	

	histFDnChar->SetStats(kFALSE);

	// Dir nChar
	histDirnChar->SetMarkerStyle(20);
	histDirnChar->SetMarkerSize(.75);
	histDirnChar->SetMarkerColor(kRed);
	histDirnChar->SetLineColor(kRed);

	//histDirnChar->GetYaxis()->SetRangeUser(0.0001,0.5);

	histDirnChar->SetStats(kFALSE);

	// Define legends
	auto l1 = new TLegend(.55, .7, .9, .9);
	l1->SetTextSize(0.025);
	l1->SetHeader("p+p@500 GeV, |#eta| < 1, p_{T} > 200 MeV");
	l1->AddEntry(histFDPt, "PYTHIA 8, #Upsilon from feed-down", "p");
	l1->AddEntry(histDirPt, "PYTHIA 8, #Upsilon from direct production", "p");

	auto l2 = new TLegend(.55, .7, .9, .9);
	l2->SetTextSize(0.025);
	l2->SetHeader("p+p@500 GeV, |#eta| < 1, p_{T} > 200 MeV");
	l2->AddEntry(histFDnChar, "PYTHIA 8, #Upsilon from feed-down", "p");
	l2->AddEntry(histDirnChar, "PYTHIA 8, #Upsilon from direct production", "p");

	// Add hists to canvases
	c1->cd();
	histFDPt->Draw("P");
	histDirPt->Draw("P same");
	l1->Draw("same");

	c2->cd();
	histFDnChar->Draw("P");
	histDirnChar->Draw("P same");
	l2->Draw("same");

	// Print canvases
	c1->Print("../output/FD_Pt.png", "png");
	c2->Print("../output/FD_nChar.png", "png");
}
