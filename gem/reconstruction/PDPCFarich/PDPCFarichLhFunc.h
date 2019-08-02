#ifndef PDPCFarichLhFunc_H
# define PDPCFarichLhFunc_H 1

# include <vector>
# include "Math/IFunction.h"

class PDPCFarichRecoEvent;
class PDPCFarichGeometry;

class PDPCFarichLhFunc : public ROOT::Math::IGradientFunctionMultiDim
{
public:
    PDPCFarichLhFunc(PDPCFarichGeometry *f, const char* calfn="");
    virtual ~PDPCFarichLhFunc() {};

    void SetRecoEvent(PDPCFarichRecoEvent* event_) { event = event_; }

    IBaseFunctionMultiDim *Clone() const
    {
        return new PDPCFarichLhFunc(*this);
    }
    unsigned int NDim () const
    {
	return nPars;
    }
    void Gradient(const double *xx, double *grad) const
    {
	double f;
	FdF(xx,f,grad);
    }

    void FdF(const double *xx, double &f, double *grad) const;

    static float GetMinimumRadius() { return excludeRadius; }

protected:
    double DoEval(const double *xx) const
    {
	double f;
	FdF(xx,f,0);
	return f;
    }
    double DoDerivative(const double *xx, unsigned int icoord) const
    {
	double grad[nPars];
	Gradient(xx,grad);
	return grad[icoord];
    }

private:
    static const unsigned int nPars;
    static const float excludeRadius;
    static const float defPixelDCprob;

    PDPCFarichGeometry *fGeometry;
    PDPCFarichRecoEvent *event;
    std::vector<float> pixelEfficiency, pixelDCprob;
};

#endif
