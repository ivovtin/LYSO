
class basicTreeFormats {
public:
	enum {CHMB=7, QDC=4, CHMBX=4, CHMBY=3};
	enum labels { Ok=0, Bad=1, Calor=2, Sigma=3, Chi=4 };

	static const double badChi, badSigma, badAngle, badPoint;
private:
	struct anglesX {	// it's tangent, actually
		double fit;
		double pairs[(CHMBX*(CHMBX-1))/2];
		double point;
	};
	struct anglesY {
		double fit;
		double pairs[(CHMBY*(CHMBY-1))/2];
		double point;	// starting point for track
	};
	struct labelsX {
		unsigned char global_label;
		unsigned char channels[CHMBX];		// UChar_t /b
	};
	struct labelsY {
		unsigned char global_label;
		unsigned char channels[CHMBY];
	};
public:
	struct chambers {				// raw DC data
		int channels[CHMB];					// Int_t /I
	};
	struct calorimeter {
		unsigned short channels[QDC];		// UShort_t /s
	};
	struct chamberXtimes {		// DCtimes_*.root
		double channels[CHMBX];
		labelsX labels;
	};
	struct chamberYtimes {
		double channels[CHMBY];
		labelsY labels;
	};
	struct chamberXcalculated {		// DCcoord_*.root
		double channels[CHMBX];
		double chi2;
		double sigmas[CHMBX];
		anglesX angles;
		labelsX labels;
	};
	struct chamberYcalculated {		// decreasing order of size for portability
		double channels[CHMBY];
		double chi2;
		double sigmas[CHMBY];
		anglesY angles;
		labelsY labels;
	};
};
