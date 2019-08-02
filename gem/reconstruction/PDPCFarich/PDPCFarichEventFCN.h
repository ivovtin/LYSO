#ifndef PDPCFarichEventFCN_H
# define PDPCFarichEventFCN_H 1

# include <vector>
# include "TFitterMinuit.h"

class PDPCFarichRecoEvent;

class PDPCFarichEventFCN : public ROOT::Minuit2::FCNGradientBase
{
public:
    PDPCFarichEventFCN() :
	event(0)
    {};
    ~PDPCFarichEventFCN() {};
	
    virtual double Up() const { return 0.5; }

    double operator()(const std::vector<double>& par) const;

    std::vector<double> Gradient(const std::vector<double>& par) const;

    void SetRecoEvent(PDPCFarichRecoEvent* event_) { event = event_; }

    static float GetMinimumRadius() { return excludeRadius; }

private:
    static const float excludeRadius;

    PDPCFarichRecoEvent *event;
};

#endif