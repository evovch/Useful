#ifndef HISTOWIDGET_H
#define HISTOWIDGET_H

#include <QWidget>

#include <Rtypes.h>
#include <memory>

class TH1D;

namespace Ui {
class cls_HistoWidget;
}

class cls_HistoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cls_HistoWidget(QWidget *parent = 0);

    cls_HistoWidget(TH1D* p_sourceHisto, QWidget *parent = 0);

    ~cls_HistoWidget();

    void FillFromData(std::vector<Double_t> p_data);




public slots:
    void FillTest(void);
    void SetLogY(Int_t p_val = 1);
    void ToggleLogY(void);
    void UnZoom(void);
    void RebinBy2(void);

private:
    Ui::cls_HistoWidget *ui;

    std::unique_ptr<TH1D> mHisto;



    static UInt_t mHistoID;
};

#endif // HISTOWIDGET_H
