#include "Pythia8/Pythia.h"

#include "TInterpreter.h"
#include "TTree.h"
#include "TFile.h"
#include "Math/Vector4D.h"

#include <vector>

using namespace std;
using namespace Pythia8;
using namespace ROOT::Math;

int main()
{
	// create Pythia instance
	Pythia pythia;
	
	// static settings
	Settings& settings = pythia.settings;
	Info& info = pythia.info;

	// load settings
	pythia.readFile("Bottom.cmnd");

	//  define parameters from runcard
	int maxNumberOfEvents = settings.mode("Main:numberOfEvents");
	int maxErrors = settings.mode("Main:timesAllowErrors");

	// initialize Pythia
	pythia.init();

	// define ROOT TTree
	TTree *tree = new TTree("tree", "tree");

	// define parameters to save
	Double_t event_weight;
	Int_t nChar;
	Int_t Ups;
	Int_t uniqueBot;
	vector<Int_t> p_ID;
	vector<Int_t> m1_ID;
	vector<Int_t> m2_ID;
	vector<Int_t> d1_ID;
	vector<Int_t> d2_ID;
	vector<Int_t> p_Px;
	vector<Int_t> p_Py;
	vector<Int_t> p_Pz;
	vector<Int_t> p_E;

	// create TTree branches
	tree->Branch("event_weight", &event_weight, "event_weight/D");
	tree->Branch("nChar", &nChar, "nChar/I");
	tree->Branch("Ups", &Ups, "Ups/I");
	tree->Branch("uniqueBot", &uniqueBot, "uniqueBot/I");
	tree->Branch("p_ID", "std::vector<Int_t>", &p_ID);
	tree->Branch("m1_ID", "std::vector<Int_t>", &m1_ID);
	tree->Branch("m2_ID", "std::vector<Int_t>", &m2_ID);
	tree->Branch("d1_ID", "std::vector<Int_t>", &d1_ID);
	tree->Branch("d2_ID", "std::vector<Int_t>", &d2_ID);
	tree->Branch("p_Px", "std::vector<Int_t>", &p_Px);
	tree->Branch("p_Py", "std::vector<Int_t>", &p_Py);
	tree->Branch("p_Pz", "std::vector<Int_t>", &p_Pz);
	tree->Branch("p_E", "std::vector<Int_t>", &p_E);

	// list bottomonium state IDs
	vector<Int_t> IDs;
	IDs.push_back(551);	// \eta_b(1S)
	IDs.push_back(100551);	// \eta_b(2S)
	IDs.push_back(200551);	// \eta_b(3S)
	IDs.push_back(10551);	// \chi_b0(1P)
	IDs.push_back(110551);	// \chi_b0(2P)
	IDs.push_back(210551);	// \chi_b0(3P)
	IDs.push_back(553);	// \Upsilon(1S)
	IDs.push_back(100553);	// \Upsilon(2S)
	IDs.push_back(200553);	// \Upsilon(3S)
	IDs.push_back(300553);	// \Upsilon(4S)
	IDs.push_back(9000553);	// \Upsilon(10860)
	IDs.push_back(9010553);	// \Upsilon(11020)
	IDs.push_back(10553);	// \h_b(1P)
	IDs.push_back(110553);	// \h_b(2P)
	IDs.push_back(210553);	// \h_b(3P)
	IDs.push_back(20553);	// \chi_b1(1P)
	IDs.push_back(120553);	// \chi_b1(2P)
	IDs.push_back(220553);	// \chi_b1(3P)
	IDs.push_back(30553);	// \Upsilon_1(1D)
	IDs.push_back(130553);	// \Upsilon_1(2D)
	IDs.push_back(555);	// \chi_b2(1P)
	IDs.push_back(100555);	// \chi_b2(2P)
	IDs.push_back(200555);	// \chi_b2(3P)
	IDs.push_back(10555);	// \eta_b2(1D)
	IDs.push_back(110555);	// \eta_b2(2D)
	IDs.push_back(20555);	// \Upsilon_2(1D)
	IDs.push_back(120555);	// \Upsilon_2(2D)
	IDs.push_back(557);	// \Upsilon_3(1D)
	IDs.push_back(100557);	// \Upsilon_3(2D)

	// Define temporary Particle and her mothers and daughters
	Particle particle, mother1, mother2, daughter1, daughter2;

	// loop over all events (run loop)
	for (int i = 0; i < maxNumberOfEvents; i++)
	{
		if (!pythia.next())
			continue;
		
		// init. parameters for new loop
		event_weight = info.weight();
		nChar = 0;
		Ups = 0;
		uniqueBot = 0;
		p_ID.clear();
		m1_ID.clear();
		m2_ID.clear();
		d1_ID.clear();
		d2_ID.clear();
		p_Px.clear();
		p_Py.clear();
		p_Pz.clear();
		p_E.clear();

		// single event loop (event loop)
		for (int j = 0; j < pythia.event.size(); j++)
		{
			// Set current particle
			particle = pythia.event[j];
			mother1 = pythia.event[particle.mother1()];
			mother2 = pythia.event[particle.mother2()];
			daughter1 = pythia.event[particle.daughter1()];
			daughter2 = pythia.event[particle.daughter2()];

			// multiplicity counter
			if (particle.isFinal() && particle.isCharged())
			{
				if (particle.pT() >= .20 && abs(particle.eta()) < 1.0)
				{
					nChar++;
				}
			}

			// loop over bottomonium IDs
			for (auto & element : IDs)
			{
				if (particle.id() == element && (daughter1.id() != daughter2.id() || daughter1.id() != particle.id()))
				{
					if(particle.id() == 553)
					{
						Ups++;
					}
					p_ID.push_back(particle.id());
					m1_ID.push_back(mother1.id());
					m2_ID.push_back(mother2.id());
					d1_ID.push_back(daughter1.id());
					d2_ID.push_back(daughter2.id());
					p_Px.push_back(particle.px());
					p_Py.push_back(particle.py());
					p_Pz.push_back(particle.pz());
					p_E.push_back(particle.e());
					uniqueBot++;
				}
			}
		}	// end event loop

		tree->Fill();
	}	// end run loop

	// statistics
	pythia.stat();
	
	// select output file
	TFile out("Bottom.root", "recreate");

	// write tree
	tree->Write();

	// done
	return 0;
}
