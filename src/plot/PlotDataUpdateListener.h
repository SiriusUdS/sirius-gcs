#ifndef PLOTDATAUPDATELISTENER_H
#define PLOTDATAUPDATELISTENER_H

class PlotData;

class PlotDataUpdateListener {
public:
    virtual void onAddData(const PlotData* plotData, float x, float y) = 0;
};

#endif // PLOTDATAUPDATELISTENER_H
